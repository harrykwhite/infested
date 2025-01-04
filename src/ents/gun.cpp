#include "ents.h"

static constexpr float ik_bulletSpd = 13.0f;
static constexpr float ik_shootCamShake = 1.0f;

bool spawn_gun_ent(zf4::EntID* const entID, const zf4::EntID ownerEntID, const int shootInterval, zf4::EntityManager* const entManager) {
    assert(zf4::is_zero(entID));
    assert(entManager->does_ent_exist(ownerEntID));
    assert(shootInterval > 0);

    if (!entManager->spawn_ent(entID, entManager->get_ent_pos(ownerEntID))) {
        return false;
    }

    if (!entManager->add_component_to_ent(SPRITE_COMPONENT, *entID)) {
        return false;
    }

    const auto spriteComp = reinterpret_cast<SpriteComponent*>(entManager->get_ent_component(*entID, SPRITE_COMPONENT));
    spriteComp->spriteIndex = GUN_SPRITE;
    spriteComp->origin = {0.0f, 0.5f};

    if (!entManager->add_component_to_ent(GUN_COMPONENT, *entID)) {
        return false;
    }

    const auto gunComp = reinterpret_cast<GunComponent*>(entManager->get_ent_component(*entID, GUN_COMPONENT));
    gunComp->ownerEntID = ownerEntID;
    gunComp->shootInterval = shootInterval;

    return true;
}

bool update_gun_ents(zf4::Scene* const scene) {
    const zf4::Vec2D mouseCamPos = zf4::screen_to_camera_pos(zf4::get_mouse_pos(), &scene->renderer.cam);

    for (int i = 0; i < scene->entManager.get_ent_limit(); ++i) {
        const zf4::EntID entID = scene->entManager.create_ent_id(i);

        if (!scene->entManager.does_ent_exist(entID)) {
            continue;
        }

        if (!scene->entManager.does_ent_have_component(entID, SPRITE_COMPONENT)) {
            continue;
        }

        if (!scene->entManager.does_ent_have_component(entID, GUN_COMPONENT)) {
            continue;
        }

        const auto spriteComp = reinterpret_cast<SpriteComponent*>(scene->entManager.get_ent_component(entID, SPRITE_COMPONENT));
        const auto gunComp = reinterpret_cast<GunComponent*>(scene->entManager.get_ent_component(entID, GUN_COMPONENT));

        if (!scene->entManager.does_ent_exist(gunComp->ownerEntID)) {
            scene->entManager.destroy_ent(entID, scene);
            continue;
        }

        zf4::Vec2D& entPos = scene->entManager.get_ent_pos(entID);

        entPos = scene->entManager.get_ent_pos(gunComp->ownerEntID);

        const float mouseDir = zf4::calc_dir(entPos, mouseCamPos);
        spriteComp->rot = mouseDir; // TEMP?

        if (gunComp->shootTime > 0) {
            gunComp->shootTime--;
        } else {
            // TEMP: Check a function pointer.
            if (zf4::is_mouse_button_down(zf4::MOUSE_BUTTON_LEFT)) {
                const float bulletPosOffsDist = static_cast<float>(zf4::get_game_sprites()[spriteComp->spriteIndex].frames[0].width);
                const zf4::Vec2D bulletPosOffs = zf4::calc_len_dir(bulletPosOffsDist, spriteComp->rot);
                const zf4::Vec2D bulletPos = entPos + bulletPosOffs;

                if (!spawn_bullet_ent(bulletPos, ik_bulletSpd, spriteComp->rot, 1, &scene->entManager)) {
                    return false;
                }

                gunComp->shootTime = gunComp->shootInterval;
            }
        }
    }

    return true;
}
