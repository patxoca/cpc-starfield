#pragma once

#include <cpctelera.h>

#define ENTITY_DEAD_FLAG 0x80
#define ENTITY_IS_ALIVE(e) (((e)->type & ENTITY_DEAD_FLAG) == 0)

typedef struct {
    u8 type;
    u8 x, y;
    i8 vx;
    u8 color;
    u8 *vptr;    // punter a la memòria de vídeo on pintar
    u8 *ovptr;   // punter a la memoria de vídeo on esborrar
} entity_t;


//==============================================================================
//   Inicialitza el gestor d'entitats.
//

void entity_mgr_init(void);


//==============================================================================
//   Obté espai per una entitat.
//

entity_t *entity_mgr_alloc(void);


//==============================================================================
//   Marca una entitat per destruir-la.
//
void entity_mgr_gc_mark(entity_t *e) __z88dk_fastcall;


//==============================================================================
//   Allibera l'espai ocupat per les entitats marcades.
//

void entity_mgr_gc_sweep(void);


//==============================================================================
//   Assigna el tipus.
//
//   El bit superior del camp "type" s'utilitza internament per
//   registrar si l'entitat està viva o morta. Aquesta funció li
//   assigna un valor al tipus respectant aquest bit.

void entity_mgr_set_type(entity_t *e, u8 type);


//==============================================================================
//   Itera sobre les entitats.

void entity_mgr_foreach(void (*f)(entity_t *e) __z88dk_fastcall) __z88dk_fastcall;


//==============================================================================
//   Retorna el nombre d'entitats lliures

u8 entity_mgr_empty_slots_count(void);
