#include "entity.h"

#define MAX_ENTITIES 50

entity_t entities[MAX_ENTITIES];
u8 num_entities;
entity_t *free_ptr;


//==============================================================================
// Inicialitza el gestor d'entitats.

void entity_mgr_init(void) {
    cpct_memset(entities, 0, sizeof(entities));
    num_entities = 0;
    free_ptr = entities;
}


//==============================================================================
// Obté espai per una entitat.

entity_t *entity_mgr_alloc(void) {
    entity_t *ptr = free_ptr;
    entity_t *tmp = ptr;

    ++tmp;
    free_ptr = tmp;
    ptr->type = 0;
    num_entities++;
    return ptr;
}

//==============================================================================
// Marca una entitat per destruir-la.

void entity_mgr_gc_mark(entity_t *e) __z88dk_fastcall {
    u8 tmp = e->type;

    e->type = (tmp | ENTITY_DEAD_FLAG);
}

//==============================================================================
// Allibera l'espai ocupar per les entitats marcades.

void entity_mgr_gc_sweep(void) {
    entity_t *curr = entities;
    entity_t *end = free_ptr - 1;
    u8 count = num_entities;

    while (count--) {
        if (!ENTITY_IS_ALIVE(curr)) {
            num_entities--;
            if (count) {
                cpct_memcpy(curr, end, sizeof(entity_t));
            }
            end--;
        } else {
            curr++;
        }
    }
    free_ptr = end + 1;
}

//==============================================================================
// Assigna el tipus.
//
// El bit superior del camp "type" s'utilitza internament per
// registrar si l'entitat està viva o morta. Aquesta funció li assigna
// un valor al tipus respectant aquest bit.

void entity_mgr_set_type(entity_t *e, u8 type) {
    u8 tmp = e->type;

    e->type = (tmp & 0x80) | (type & 0x7f);
}

//==============================================================================
// Itera sobre les entitats vives.

void entity_mgr_foreach(void (*f)(entity_t *e) __z88dk_fastcall) __z88dk_fastcall {
    entity_t *p = entities;
    u8 count = num_entities;
    void (*lf)(entity_t *e)  __z88dk_fastcall = f;

    while (count--) {
        lf(p);
        p++;
    }
}


//==============================================================================
// Retorna el nombre d'entitats lliures

u8 entity_mgr_empty_slots_count(void) {
    return MAX_ENTITIES - num_entities;
}
