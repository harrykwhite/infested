#include "world.h"

#include <algorithm>
#include "../asset_indexes.h"
#include "../ents/ents.h"

static constexpr zf4::Vec2DI ik_worldSize = {2560, 1440};
static constexpr zf4::Vec3D ik_bgColor = {0.59f, 0.79f, 0.93f};

static constexpr float ik_camLookDistLimit = 24.0f;
static constexpr float ik_camLookDistScalarDist = (ik_camLookDistLimit * 32.0f);
static constexpr float ik_camPosLerp = 0.25f;
static constexpr float ik_camShakeLerp = 0.1f;

bool init_world(zf4::Scene* const scene) {
    scene->renderer.bgColor = ik_bgColor;
    scene->renderer.cam.scale = 2.0f;

    const auto world = static_cast<World*>(scene->userData);

    if (!spawn_player_ent(&world->playerEntID, static_cast<zf4::Vec2D>(ik_worldSize) / 2.0f, &scene->entManager)) {
        return false;
    }

    world->camPosNoShake = scene->entManager.get_ent_pos(world->playerEntID);

    for (int i = 0; i < 64; ++i) {
        const zf4::Vec2D spawnPos = {
            zf4::gen_rand_float(0.0f, static_cast<float>(ik_worldSize.x)),
            zf4::gen_rand_float(0.0f, static_cast<float>(ik_worldSize.y))
        };

        if (!spawn_enemy_ent(spawnPos, &scene->entManager)) {
            return false;
        }
    }

    if (!spawn_hive_ent(static_cast<zf4::Vec2D>(ik_worldSize) / 4.0f, &scene->entManager)) {
        return false;
    }

    return true;
}

bool world_tick(zf4::Scene* const scene, int* const sceneChangeIndex) {
    const auto world = static_cast<World*>(scene->userData);

    const auto entsToDestroyBitset = scene->scratchSpace.push<zf4::Byte>(zf4::bits_to_bytes(scene->entManager.get_ent_limit()));

    if (!entsToDestroyBitset) {
        return false;
    }

    //
    // Entity Updates
    //
    update_player_vels(&scene->entManager);
    update_gun_ents(scene);
    proc_actor_damaging(scene, entsToDestroyBitset);

    write_render_data_of_ents(scene);

    translate_ents_by_vel(&scene->entManager, entsToDestroyBitset, &scene->scratchSpace);

    //
    // Camera
    //
    {
        // Determine the target position.
        zf4::Vec2D targPos = world->camPosNoShake;

        if (scene->entManager.does_ent_exist(world->playerEntID)) { // TEMP
            const zf4::Vec2D playerPos = scene->entManager.get_ent_pos(world->playerEntID);

            const zf4::Vec2D mouseCamPos = zf4::screen_to_camera_pos(zf4::get_mouse_pos(), &scene->renderer.cam);
            const float playerToMouseCamPosDist = zf4::calc_dist(playerPos, mouseCamPos);
            const zf4::Vec2D playerToMouseCamPosDir = zf4::calc_normal(mouseCamPos - playerPos);

            const float lookDist = ik_camLookDistLimit * std::min(playerToMouseCamPosDist / ik_camLookDistScalarDist, 1.0f);
            const zf4::Vec2D lookOffs = playerToMouseCamPosDir * lookDist;

            targPos = playerPos + lookOffs;
        }

        // Approach the target position.
        world->camPosNoShake.x = zf4::lerp(world->camPosNoShake.x, targPos.x, ik_camPosLerp);
        world->camPosNoShake.y = zf4::lerp(world->camPosNoShake.y, targPos.y, ik_camPosLerp);

        // Determine the shake to apply, then reduce it.
        const zf4::Vec2D shake = {
            zf4::gen_rand_float(-world->camShake, world->camShake),
            zf4::gen_rand_float(-world->camShake, world->camShake)
        };

        world->camShake = zf4::lerp(world->camShake, 0.0f, ik_camShakeLerp);

        // Lock the camera to the world bounds.
        const zf4::Vec2D camSize = zf4::get_camera_size(&scene->renderer.cam);
        world->camPosNoShake.x = std::clamp(world->camPosNoShake.x, camSize.x / 2.0f, ik_worldSize.x - (camSize.x / 2.0f));
        world->camPosNoShake.y = std::clamp(world->camPosNoShake.y, camSize.y / 2.0f, ik_worldSize.y - (camSize.y / 2.0f));

        // Set the actual renderer camera position.
        scene->renderer.cam.pos = world->camPosNoShake + shake;
    }

    //
    // Entity Destruction
    //
    for (int i = 0; i < scene->entManager.get_ent_limit(); ++i) {
        if (!zf4::is_bit_active(entsToDestroyBitset, i)) {
            continue;
        }

        const zf4::EntID entID = scene->entManager.create_ent_id(i);
        scene->entManager.destroy_ent(entID, scene);
    }

    //
    //
    //
    auto& cursorSprite = zf4::get_game_sprites()[CURSOR_SPRITE];
    zf4::write_to_sprite_batch(&scene->renderer, UI_WORLD_RENDER_LAYER, cursorSprite.texIndex, zf4::get_mouse_pos(), cursorSprite.frames[0], {0.5f, 0.5f}, 0.0f, {scene->renderer.cam.scale, scene->renderer.cam.scale});

    return true;
}

void init_world_render_layer_props(zf4::RenderLayerProps* const props, const int layerIndex) {
    switch (layerIndex) {
        case CAM_WORLD_RENDER_LAYER:
            props->spriteBatchCnt = 8;
            props->charBatchCnt = 0;
            break;

        case UI_WORLD_RENDER_LAYER:
            props->spriteBatchCnt = 1;
            props->charBatchCnt = 0;
            break;
    }
}

void load_world_component_type_limit(int* const typeLimit, const int typeIndex) {
}

bool is_in_world(const zf4::Vec2D pos) {
    return pos.x >= 0.0f && pos.y >= 0.0f && pos.x < ik_worldSize.x && pos.y < ik_worldSize.y;
}
