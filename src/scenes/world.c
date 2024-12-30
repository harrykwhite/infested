#include "world.h"

#include <stdalign.h>
#include "../asset_indexes.h"
#include "../sprites.h"
#include "../component_types.h"
#include "../ents/ents.h"

#define WORLD_SIZE ((ZF4Vec2D) {2560, 1440}) // TEMP

#define BG_COLOR ((ZF4Vec3D) {0.59f, 0.79f, 0.93f})

#define CAM_SCALE 2.0f
#define CAM_LOOK_DIST_LIMIT 24.0f
#define CAM_LOOK_DIST_SCALAR_DIST (CAM_LOOK_DIST_LIMIT * 32.0f)
#define CAM_POS_LERP 0.25f

void init_world_render_layer_props(ZF4RenderLayerProps* props, int layerIndex) {
    switch (layerIndex) {
        case CAM_WORLD_RENDER_LAYER:
            props->spriteBatchCnt = 1;
            props->charBatchCnt = 0;
            break;

        case UI_WORLD_RENDER_LAYER:
            props->spriteBatchCnt = 1;
            props->charBatchCnt = 64; // TEMP
            break;
    }
}

void load_world_component_type_limit(int* typeLimit, int typeIndex) {
}

bool init_world(ZF4Scene* scene) {
    World* world = scene->userData;

    scene->renderer.cam.scale = CAM_SCALE;
    scene->renderer.bgColor = BG_COLOR;

    world->playerEntID = spawn_player_ent((ZF4Vec2D) { 0.0f, 0.0f }, scene);
    world->camMeta.pos = zf4_get_ent(world->playerEntID, scene)->pos;

    return true;
}

bool world_tick(ZF4Scene* scene, int* sceneChangeIndex) {
    World* world = scene->userData;

    update_player_vels(world->playerEntID, scene);
    translate_ents_by_vel(scene);
    write_render_data_of_ents(scene);

    camera_tick(&world->camMeta, scene, world->playerEntID);

    if (zf4_is_key_pressed(ZF4_KEY_SPACE)) {
        shake_camera(&world->camMeta, 4.0f);
    }

    //
    // Cursor
    //
    {
        ZF4SpriteBatchWriteInfo sbWriteInfo = {
            .texIndex = zf4_get_sprite(CURSOR_SPRITE)->texIndex,
            .pos = zf4_get_mouse_pos(),
            .srcRect = zf4_get_sprite_src_rect(CURSOR_SPRITE, 0),
            .origin = {0.5f, 0.5f},
            .rot = 0.0f,
            .scale = {scene->renderer.cam.scale, scene->renderer.cam.scale},
            .alpha = 1.0f
        };

        zf4_write_to_sprite_batch(&scene->renderer, UI_WORLD_RENDER_LAYER, &sbWriteInfo);
    }

    return true;
}
