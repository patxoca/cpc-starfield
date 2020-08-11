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


void entity_mgr_init(void);
entity_t *entity_mgr_alloc(void);
void entity_mgr_gc_mark(entity_t *e) __z88dk_fastcall;
void entity_mgr_gc_sweep(void);
void entity_mgr_set_type(entity_t *e, u8 type);
void entity_mgr_foreach(void (*f)(entity_t *e) __z88dk_fastcall) __z88dk_fastcall;
u8 entity_mgr_empty_slots_count(void);
