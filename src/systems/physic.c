#include <cpctelera.h>
#include "../managers/entity.h"
#include "physic.h"

void sys_physics_create_star(void) {
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

void sys_physics_update_star(entity_t *e) __z88dk_fastcall {
    u8 px = e->x;

    e->x -= e->vx;
    if (px < e->x) {
        entity_mgr_gc_mark(e);
    } else {
        e->vptr -= e->vx;
    }
}

void sys_physics_add_star(void) {
    if (entity_mgr_empty_slots_count()) {
        sys_physics_create_star();
    }
}

void sys_physics_update_stars(void) {
    entity_mgr_foreach(sys_physics_update_star);
}
