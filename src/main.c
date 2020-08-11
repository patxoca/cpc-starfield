
#include <cpctelera.h>
#include "managers/entity.h"

#define NUM_STARS 30

void create_star(void) {
    entity_t *e;

    e = entity_mgr_alloc();
    entity_mgr_set_type(e, 1);
    e->x = 80;
    e->y = cpct_rand8() % 200;  // INEFICIENT
    e->vx = cpct_rand8() & 3;
    if (e->vx == 0) {
        e->vx = 1;
    }
    e->vptr = cpct_getScreenPtr(CPCT_VMEM_START, e->x, e->y);
    e->ovptr = 0;
    e->color = 0x80;
}

void update_star(entity_t *e) __z88dk_fastcall {
    u8 px = e->x;

    e->x -= e->vx;
    if (px < e->x) {
        entity_mgr_gc_mark(e);
    } else {
        e->vptr -= e->vx;
    }
}

void update_stars(void) {
    entity_mgr_foreach(update_star);
}

void draw_star(entity_t *e) __z88dk_fastcall {
    u8 *p;

    if (e->ovptr) {
        *(e->ovptr) = 0;
    }
    if (ENTITY_IS_ALIVE(e)) {
        p = e->vptr;
        *p = e->color;
        e->ovptr = p;
    }
}

void draw_stars(void) {
    entity_mgr_foreach(draw_star);
}

void add_stars(void) {
    if (entity_mgr_empty_slots_count()) {
        create_star();
    }
}

void create_stars(void) {
    int i;

    for (i = 0; i < NUM_STARS; ++i) {
        create_star();
    }
}

void main(void) {
    cpct_setVideoMode(0);
    cpct_disableFirmware();
    entity_mgr_init();
    /* create_stars(); */
    while (1) {
        add_stars();
        update_stars();
        draw_stars();
        entity_mgr_gc_sweep();
        cpct_waitVSYNC();
    }
}
