#include <cpctelera.h>
#include "../managers/entity.h"
#include "physic.h"

void sys_physics_create_star(void) {
    entity_t *e;

    e = entity_mgr_alloc();
    entity_mgr_set_type(e, 1);
    e->x = 159;
    e->y = cpct_rand8() % 200;  // INEFICIENT
    e->vx = 1 + (cpct_rand8() & 3);
    e->vptr = cpct_getScreenPtr(CPCT_VMEM_START, 0, e->y);
    e->ovptr = 0;
    e->mask = 0;
    switch (e->vx) {
    case 1:
        e->color = 0x20;  // colors [4, 0]
        break;
    case 2:
        e->color = 0x88;  // colors [3, 0]
        break;
    case 3:
        e->color = 0x08;  // colors [2, 0]
        break;
    case 4:
        e->color = 0x80;  // colors [1, 0]
        break;
    }
}

void sys_physics_update_star(entity_t *e) __z88dk_fastcall {
    u8 px = e->x;

    e->x -= e->vx;
    if (px < e->x) {
        entity_mgr_release(e);
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
