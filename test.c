#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libdragon.h>

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_32_BPP;

#define MAX_ARROWS 64
#define ARROW_SPEED 2   // pixels per frame

typedef struct {
    int x, y;
    sprite_t *img;
    int active;
} Arrow;

/* Lane X positions (Left, Down, Up, Right) */
int lane_x[4] = {60, 120, 180, 240};

/* Arrows + count */
Arrow arrows[MAX_ARROWS];

/* Sprites */
sprite_t *bg;
sprite_t *bf;
sprite_t *dad;
sprite_t *arrow_up;
sprite_t *arrow_down;
sprite_t *arrow_left;
sprite_t *arrow_right;

/* Spawn an arrow in a lane */
void spawn_arrow(int lane) {
    for (int i = 0; i < MAX_ARROWS; i++) {
        if (!arrows[i].active) {
            arrows[i].x = lane_x[lane];
            arrows[i].y = 240;   // bottom of screen
            switch (lane) {
                case 0: arrows[i].img = arrow_left; break;
                case 1: arrows[i].img = arrow_down; break;
                case 2: arrows[i].img = arrow_up; break;
                case 3: arrows[i].img = arrow_right; break;
            }
            arrows[i].active = 1;
            break;
        }
    }
}

int main(void) {
    /* Initialize display, filesystem, and joypad */
    display_init(res, bit, 2, GAMMA_NONE, FILTERS_DISABLED);
    dfs_init(DFS_DEFAULT_LOCATION);
    joypad_init();

    /* Load background and character sprites */
    bg  = sprite_load("rom://bg.sprite");
    bf  = sprite_load("rom://BF.sprite");
    dad = sprite_load("rom://DAD.sprite");

    /* Load arrow sprites */
    arrow_up    = sprite_load("rom://arrow_up.sprite");
    arrow_down  = sprite_load("rom://arrow_down.sprite");
    arrow_left  = sprite_load("rom://arrow_left.sprite");
    arrow_right = sprite_load("rom://arrow_right.sprite");

    /* Clear arrow buffer */
    for (int i = 0; i < MAX_ARROWS; i++) arrows[i].active = 0;

    int frame_counter = 0;

    while (1) {
        static display_context_t disp = 0;
        disp = display_get();

        /* Clear screen (black background behind everything) */
        graphics_fill_screen(disp, graphics_make_color(0, 0, 0, 255));

        /* Draw background */
        if (bg) graphics_draw_sprite(disp, 0, 0, bg);

        /* Draw characters */
        if (bf)  graphics_draw_sprite(disp, 200, 110, bf);
        if (dad) graphics_draw_sprite(disp, 30, 50, dad);

        /* Draw receptor arrows (fixed at top) */
        if (arrow_left)  graphics_draw_sprite_trans(disp, lane_x[0], 20, arrow_left);
        if (arrow_down)  graphics_draw_sprite_trans(disp, lane_x[1], 20, arrow_down);
        if (arrow_up)    graphics_draw_sprite_trans(disp, lane_x[2], 20, arrow_up);
        if (arrow_right) graphics_draw_sprite_trans(disp, lane_x[3], 20, arrow_right);

        /* Spawn a test arrow every 90 frames */
        if (frame_counter % 90 == 0) {
            spawn_arrow(rand() % 4);
        }

        /* Update and draw arrows */
        for (int i = 0; i < MAX_ARROWS; i++) {
            if (arrows[i].active) {
                arrows[i].y -= ARROW_SPEED;
                if (arrows[i].y < -32) {
                    arrows[i].active = 0; // deactivate offscreen
                } else {
                    graphics_draw_sprite_trans(disp, arrows[i].x, arrows[i].y, arrows[i].img);
                }
            }
        }

        display_show(disp);

        /* Input (for later StepMania-style scoring) */
        joypad_poll();
        joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        if (keys.d_left)  printf("Left pressed!\n");
        if (keys.d_down)  printf("Down pressed!\n");
        if (keys.d_up)    printf("Up pressed!\n");
        if (keys.d_right) printf("Right pressed!\n");

        frame_counter++;
    }
}
