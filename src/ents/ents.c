#include "ents.h"

#include <stdalign.h>
#include "../component_types.h"
#include "../sprites.h"

void ent_vel_sys(ZF4Scene* scene) {
    ZF4SceneTypeInfo* sceneTypeInfo = zf4_get_scene_type_info(scene->typeIndex);

    for (int i = 0; i < sceneTypeInfo->entLimit; ++i) {
        ZF4EntID entID = {i, scene->entVersions[i]};

        if (!zf4_does_ent_exist(entID, scene)) {
            continue;
        }

        if (!zf4_does_ent_have_component(entID, VELOCITY_COMPONENT, scene)) {
            continue;
        }

        VelocityComponent* velComp = zf4_get_ent_component(entID, VELOCITY_COMPONENT, scene);

        velComp->vel = zf4_lerp_vec_2d(velComp->vel, velComp->velLerpTarg, velComp->velLerpFactor);

        ZF4Ent* ent = zf4_get_ent(entID, scene);
        ent->pos = zf4_calc_vec_2d_sum(ent->pos, velComp->vel);
    }
}

void write_render_data_of_ents(ZF4Scene* scene) {
    ZF4SceneTypeInfo* sceneTypeInfo = zf4_get_scene_type_info(scene->typeIndex);

    for (int i = 0; i < sceneTypeInfo->entLimit; ++i) {
        ZF4EntID entID = {i, scene->entVersions[i]};

        if (!zf4_does_ent_exist(entID, scene)) {
            continue;
        }

        if (!zf4_does_ent_have_component(entID, SPRITE_COMPONENT, scene)) {
            continue;
        }

        SpriteComponent* spriteComp = zf4_get_ent_component(entID, SPRITE_COMPONENT, scene);

        if (spriteComp->prewrite) {
            spriteComp->prewrite(entID, scene);
        }

        ZF4Ent* ent = zf4_get_ent(entID, scene);

        ZF4SpriteBatchWriteInfo sbWriteInfo = {
            .texIndex = zf4_get_sprite(spriteComp->spriteIndex)->texIndex,
            .pos = ent->pos,
            .srcRect = zf4_get_sprite_src_rect(spriteComp->spriteIndex, 0),
            .origin = spriteComp->origin,
            .rot = spriteComp->rot,
            .scale = spriteComp->scale,
            .alpha = spriteComp->alpha
        };

        zf4_write_to_sprite_batch(&scene->renderer, 0, &sbWriteInfo);

        if (spriteComp->postwrite) {
            spriteComp->postwrite(entID, scene);
        }
    }
}

void load_ent_collider(ZF4RectF* const collider, const ZF4EntID entID, const ZF4Scene* const scene) {
    assert(zf4_is_zero(collider, sizeof(*collider)));

    const ZF4Ent* const ent = zf4_get_ent(entID, scene);
    const SpriteComponent* const spriteComp = zf4_get_ent_component(entID, SPRITE_COMPONENT, scene);

    const ZF4Rect srcRect = zf4_get_sprite_src_rect(spriteComp->spriteIndex, 0); // TEMP: Using just the first frame for now.

    collider->width = srcRect.width * spriteComp->scale.x;
    collider->height = srcRect.height * spriteComp->scale.y;

    collider->x = (ent->pos.x - (collider->width * spriteComp->origin.x));
    collider->y = (ent->pos.y - (collider->height * spriteComp->origin.y));
}

bool spawn_bullet_ent(ZF4EntID* const entID, const ZF4Vec2D pos, const float spd, const float dir, const int dmg, const ZF4Scene* const scene) {
    if (!zf4_spawn_ent(entID, pos, scene)) {
        return false;
    }

    //
    // Velocity Component
    //
    if (!zf4_add_component_to_ent(VELOCITY_COMPONENT, *entID, scene)) {
        return false;
    }

    VelocityComponent* const velComp = zf4_get_ent_component(*entID, VELOCITY_COMPONENT, scene);
    velComp->vel = zf4_calc_len_dir_vec_2d(spd, dir);

    //
    // Sprite Component
    //
    if (!zf4_add_component_to_ent(SPRITE_COMPONENT, *entID, scene)) {
        return false;
    }

    SpriteComponent* const spriteComp = zf4_get_ent_component(*entID, SPRITE_COMPONENT, scene);
    spriteComp->spriteIndex = BULLET_SPRITE;

    //
    // Damager Component
    //
    if (!zf4_add_component_to_ent(DAMAGER_COMPONENT, *entID, scene)) {
        return false;
    }

    DamagerComponent* const damagerComp = zf4_get_ent_component(*entID, DAMAGER_COMPONENT, scene);
    damagerComp->dmg = dmg;
    damagerComp->force = velComp->vel;
    damagerComp->willCollideWith = is_not_player_ent;

    return true;
}

void proc_actor_damaging(ZF4Scene* scene) {
    const ZF4SceneTypeInfo* const sceneTypeInfo = zf4_get_scene_type_info(scene->typeIndex);

    //
    // Get damager and actor entities.
    //
    ZF4EntID* damagerEntIDs = zf4_push_to_mem_arena(&scene->scratchSpace, sizeof(ZF4EntID) * sceneTypeInfo->entLimit, alignof(ZF4EntID));
    int damagerEntCnt = 0;

    ZF4EntID* actorEntIDs = zf4_push_to_mem_arena(&scene->scratchSpace, sizeof(ZF4EntID) * sceneTypeInfo->entLimit, alignof(ZF4EntID));
    int actorEntCnt = 0;

    for (int i = 0; i < sceneTypeInfo->entLimit; ++i) {
        const ZF4EntID entID = {i, scene->entVersions[i]};

        if (!zf4_does_ent_exist(entID, scene)) {
            continue;
        }

        if (zf4_does_ent_have_component(entID, DAMAGER_COMPONENT, scene)) {
            damagerEntIDs[damagerEntCnt] = entID;
            ++damagerEntCnt;
        }

        if (zf4_does_ent_have_component(entID, ACTOR_COMPONENT, scene)) {
            actorEntIDs[actorEntCnt] = entID;
            ++actorEntCnt;
        }
    }

    //
    // Check for and handle collisions.
    //
    for (int i = 0; i < damagerEntCnt; ++i) {
        ZF4EntID damagerEntID = damagerEntIDs[i];
        DamagerComponent* damagerComp = zf4_get_ent_component(damagerEntID, DAMAGER_COMPONENT, scene);

        ZF4RectF damagerCollider = {0};
        load_ent_collider(&damagerCollider, damagerEntID, scene);

        bool collisionHappened = false;

        for (int j = 0; j < actorEntCnt; ++j) {
            const ZF4EntID actorEntID = actorEntIDs[j];
            
            if (damagerComp->willCollideWith && !damagerComp->willCollideWith(actorEntID, scene)) {
                continue;
            }
            
            ZF4RectF actorCollider = {0};
            load_ent_collider(&actorCollider, actorEntID, scene);

            if (zf4_do_rect_fs_intersect(&damagerCollider, &actorCollider)) {
                collisionHappened = true;

                ActorComponent* const actorComp = zf4_get_ent_component(actorEntID, ACTOR_COMPONENT, scene);
                actorComp->hp -= damagerComp->dmg;
                actorComp->invTime = actorComp->invTimeMax;

                if (zf4_does_ent_have_component(actorEntID, VELOCITY_COMPONENT, scene)) {
                    VelocityComponent* const velComp = zf4_get_ent_component(actorEntID, VELOCITY_COMPONENT, scene);
                    velComp->vel = zf4_calc_vec_2d_sum(velComp->vel, damagerComp->force);
                }
            }
        }

        if (collisionHappened) {
            zf4_destroy_ent(damagerEntID, scene);
        }
    }
}
