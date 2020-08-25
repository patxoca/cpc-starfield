#include <cpctelera.h>
#include "../managers/entity.h"
#include "render.h"

static const u8 palette[16 + 1] = {
    HW_BLACK, HW_BRIGHT_WHITE, HW_YELLOW,        HW_WHITE,
    HW_WHITE, HW_RED,          HW_RED,           HW_RED,
    HW_RED,   HW_RED,          HW_RED,           HW_RED,
    HW_RED,   HW_RED,          HW_RED,           HW_RED,
    HW_BLACK
};

void sys_render_draw_star(entity_t *e) __z88dk_fastcall {
    u8 *p;

    if (e->ovptr) {
        *(e->ovptr) &= e->mask;
    }
    if (ENTITY_IS_ALIVE(e)) {
        u8 m;

        m = e->color;
        if (e->x & 1) {   // pixel de la dreta
            m >>= 1;
        }
        p = e->vptr + (e->x >> 1);
        *p |= m;
        e->ovptr = p;
        e->mask = ~m;
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
