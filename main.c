#include <libdragon.h>
#include <stdlib.h>

/* Characters */
static sprite_t *background_sprite;
static sprite_t *sprite1, *sprite2;
static rspq_block_t* background_block;

typedef struct {
    float x, y, scale_factor;
} object_t;

#define NUM_SPRITES 2
static object_t sprites[NUM_SPRITES];

/* Camera position */
static float camera_x = 0, camera_y = 0;
/* Target camera position for smooth panning */
static float target_x = 0, target_y = 0;

static void render(int cur_frame) {
    surface_t *disp = display_get();
    rdpq_attach_clear(disp, NULL);

    if (!background_block){
        rspq_block_begin();
            rdpq_set_mode_copy(false);
            rdpq_sprite_blit(background_sprite, 0, 0, NULL);
            rdpq_set_mode_standard();
        background_block = rspq_block_end();
    }
    rspq_block_run(background_block);

    // --- Enable transparency for all sprites ---
    rdpq_mode_alphacompare(1);                // skip fully transparent pixels
    rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY); // blend semi-transparent pixels

    // Characters (adjusted for camera offset)
    rdpq_sprite_blit(sprite1, (int32_t)(sprites[0].x - camera_x), (int32_t)(sprites[0].y - camera_y),
        &(rdpq_blitparms_t){ .scale_x=sprites[0].scale_factor, .scale_y=sprites[0].scale_factor });
    rdpq_sprite_blit(sprite2, (int32_t)(sprites[1].x - camera_x), (int32_t)(sprites[1].y - camera_y),
        &(rdpq_blitparms_t){ .scale_x=sprites[1].scale_factor, .scale_y=sprites[1].scale_factor });

    rdpq_detach_show();
}

int main(void) {
    debug_init_isviewer();
    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_DEDITHER);
    dfs_init(DFS_DEFAULT_LOCATION);
    rdpq_init();
    joypad_init();

    // Load assets
    background_sprite = sprite_load("rom:/images/bg.sprite");
    sprite1 = sprite_load("rom:/images/Characters/BF.sprite");
    sprite2 = sprite_load("rom:/images/Characters/DAD.sprite");

    // Characters
    uint32_t screen_w = display_get_width();
    uint32_t screen_h = display_get_height();

    sprites[0].x = 3*screen_w/4.f - sprite1->width/2;
    sprites[0].y = screen_h/1.6f - sprite1->height/2;
    sprites[0].scale_factor = 1.0f;

    sprites[1].x = screen_w/3.0f - sprite2->width/2;
    sprites[1].y = screen_h/2.0f - sprite2->height/2;
    sprites[1].scale_factor = 1.0f;

    int cur_frame = 0;
    while (1) {
        /* --- Input --- */
        joypad_poll();
        joypad_buttons_t btn = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        if (btn.a) {
            // Center camera on sprite1
            target_x = sprites[0].x - (screen_w / 2 - sprite1->width / 2);
            target_y = sprites[0].y - (screen_h / 2 - sprite1->height / 2);
        }
        if (btn.b) {
            // Center camera on sprite2
            target_x = sprites[1].x - (screen_w / 2 - sprite2->width / 2);
            target_y = sprites[1].y - (screen_h / 2 - sprite2->height / 2);
        }

        /* --- Smooth camera movement --- */
        camera_x += (target_x - camera_x) * 0.1f;
        camera_y += (target_y - camera_y) * 0.1f;

        /* Render everything */
        render(cur_frame);
        cur_frame++;
    }
}
