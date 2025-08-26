#include <stdio.h>
#include <stdlib.h>
#include <libdragon.h>

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_32_BPP;

int main(void) {
    /* Initialize display, filesystem, and joypad */
    display_init(res, bit, 2, GAMMA_NONE, FILTERS_DISABLED);
    dfs_init(DFS_DEFAULT_LOCATION);
    joypad_init();

    /* Load two sprites using LibDragon's sprite_load */
    sprite_t *bf  = sprite_load("rom://BF.sprite");
    sprite_t *dad = sprite_load("rom://DAD.sprite");

    /* Check if sprites loaded correctly */
    if (!bf)  printf("Error: BF.sprite not found or failed to load!\n");
    if (!dad) printf("Error: DAD.sprite not found or failed to load!\n");

    while (1) {
        static display_context_t disp = 0;
        disp = display_get();

        /* Clear screen */
        graphics_fill_screen(disp, graphics_make_color(0, 0, 0, 255));

        /* Draw sprites on opposite sides safely */
        if (bf)  graphics_draw_sprite(disp, 200, 110, bf);   // left
        if (dad) graphics_draw_sprite(disp, 30, 50, dad); // right

        display_show(disp);

        /* Handle resolution/depth input */
        joypad_poll();
        joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        if (keys.d_up) {
            
        }
        if (keys.d_down) {
            
        }
        if (keys.d_left) {
            
        }
        if (keys.d_right) {
            
        }
    }
}
