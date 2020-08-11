#include <cpctelera.h>
#include "../managers/entity.h"
#include "render.h"

void sys_render_draw_star(entity_t *e) __z88dk_fastcall {
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

void sys_render_draw_stars(void) {
    entity_mgr_foreach(sys_render_draw_star);
}

void sys_render_init(void) {
    cpct_setVideoMode(0);
    cpct_disableFirmware();
}
