#include "entity.h"

#define MAX_ENTITIES 50

entity_t entities[MAX_ENTITIES];
u8 num_entities;
entity_t *free_ptr;


void entity_mgr_init(void) {
    cpct_memset(entities, 0, sizeof(entities));
    num_entities = 0;
    free_ptr = entities;
}


entity_t *entity_mgr_alloc(void) {
    entity_t *ptr = free_ptr;
    entity_t *tmp = ptr;
    u8 ne;

    ++tmp;
    free_ptr = tmp;
    ptr->type = 0;
    ne = num_entities;
    ne++;
    num_entities = ne;
    return ptr;
}


void entity_mgr_release(entity_t *e) __z88dk_fastcall {
    e->type |= ENTITY_DEAD_FLAG;
}


void entity_mgr_set_type(entity_t *e, u8 type) {
    e->type = (e->type & 0x80) | (type & 0x7f);
}


u8 entity_mgr_empty_slots_count(void) {
    return MAX_ENTITIES - num_entities;
}
