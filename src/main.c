
#include <cpctelera.h>
#include "managers/entity.h"
#include "systems/physic.h"
#include "systems/render.h"


void main(void) {
    sys_render_init();
    entity_mgr_init();
    while (1) {
        sys_physics_add_star();
        sys_physics_update_stars();
        cpct_waitVSYNC();
        sys_render_draw_stars();
        entity_mgr_gc();
    }
}
