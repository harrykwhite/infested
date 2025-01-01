#include "ents.h"

#include <zf4.h>
#include "../component_types.h"
#include "../sprites.h"

#define BULLET_SPD 13.0f

bool spawn_gun_ent(ZF4EntID* const entID, const ZF4EntID ownerEntID, const int shootInterval, const ZF4Scene* const scene) {
    assert(zf4_is_zero(entID, sizeof(*entID)));
    assert(zf4_does_ent_exist(ownerEntID, scene));
    assert(shootInterval > 0);

    const ZF4Ent* const ownerEnt = zf4_get_ent(ownerEntID, scene);

    if (!zf4_spawn_ent(entID, ownerEnt->pos, scene)) {
        return false;
    }

    if (!zf4_add_component_to_ent(SPRITE_COMPONENT, *entID, scene)) {
        return false;
    }

    SpriteComponent* spriteComp = zf4_get_ent_component(*entID, SPRITE_COMPONENT, scene);
    spriteComp->spriteIndex = GUN_SPRITE;
    spriteComp->origin = (ZF4Vec2D) {0.0f, 0.5f};

    if (!zf4_add_component_to_ent(GUN_COMPONENT, *entID, scene)) {
        return false;
    }

    GunComponent* gunComp = zf4_get_ent_component(*entID, GUN_COMPONENT, scene);
    gunComp->ownerEntID = ownerEntID;
    gunComp->shootInterval = shootInterval;

    return true;
}

bool update_gun_ents(const ZF4Scene* const scene, CameraMeta* const camMeta) {
    const ZF4SceneTypeInfo* const sceneTypeInfo = zf4_get_scene_type_info(scene->typeIndex);
    const ZF4Vec2D mouseCamPos = zf4_screen_to_camera_pos(zf4_get_mouse_pos(), &scene->renderer.cam);

    for (int i = 0; i < sceneTypeInfo->entLimit; ++i) {
        const ZF4EntID entID = {i, scene->entVersions[i]};

        if (!zf4_does_ent_exist(entID, scene)) {
            continue;
        }

        if (!zf4_does_ent_have_component(entID, SPRITE_COMPONENT, scene)) {
            continue;
        }

        if (!zf4_does_ent_have_component(entID, GUN_COMPONENT, scene)) {
            continue;
        }

        ZF4Ent* const ent = zf4_get_ent(entID, scene);
        SpriteComponent* const spriteComp = zf4_get_ent_component(entID, SPRITE_COMPONENT, scene);
        GunComponent* const gunComp = zf4_get_ent_component(entID, GUN_COMPONENT, scene);

        if (!zf4_does_ent_exist(gunComp->ownerEntID, scene)) {
            zf4_destroy_ent(entID, scene);
            continue;
        }

        const float mouseDir = zf4_calc_vec_2d_dir_rads(ent->pos, mouseCamPos);
        spriteComp->rot = mouseDir; // TEMP

        const ZF4Ent* const ownerEnt = zf4_get_ent(gunComp->ownerEntID, scene);
        ent->pos = ownerEnt->pos;

        if (gunComp->shootTime > 0) {
            gunComp->shootTime--;
        } else {
            // TEMP: Check a function pointer.
            if (zf4_is_mouse_button_down(ZF4_MOUSE_BUTTON_LEFT)) {
                const float bulletPosOffsDist = (float)zf4_get_sprite_src_rect(spriteComp->spriteIndex, 0).width;
                const ZF4Vec2D bulletPosOffs = zf4_calc_len_dir_vec_2d(bulletPosOffsDist, spriteComp->rot);
                const ZF4Vec2D bulletPos = zf4_calc_vec_2d_sum(ownerEnt->pos, bulletPosOffs);
                
                if (!spawn_bullet_ent_noref(bulletPos, BULLET_SPD, spriteComp->rot, 1, scene)) {
                    return false;
                }

                shake_camera(camMeta, 1.0f);

                gunComp->shootTime = gunComp->shootInterval;
            }
        }
    }

    return true;
}
