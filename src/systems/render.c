#include <cpctelera.h>
#include "../managers/entity.h"
#include "render.h"

static const u8 palette[16 + 1] = {
    HW_BLACK, HW_BRIGHT_WHITE, HW_BRIGHT_CYAN, HW_YELLOW,
    HW_RED,   HW_RED,          HW_RED,           HW_RED,
    HW_RED,   HW_RED,          HW_RED,           HW_RED,
    HW_RED,   HW_RED,          HW_RED,           HW_RED,
    HW_BLACK
};

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
    cpct_disableFirmware();
    cpct_setVideoMode(0);
    cpct_setPalette(palette, sizeof(palette));
}
