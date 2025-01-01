#include "ents.h"

#include "../component_types.h"
#include "../sprites.h"

#define MOVE_SPD 3.0f
#define VEL_LERP 0.25f
#define INIT_HP 100
#define INV_TIME 30

bool spawn_player_ent(ZF4EntID* const entID, const ZF4Vec2D pos, const ZF4Scene* const scene) {
    assert(zf4_is_zero(entID, sizeof(*entID)));

    if (!zf4_spawn_ent(entID, pos, scene)) {
        return false;
    }

    if (!zf4_add_component_to_ent(VELOCITY_COMPONENT, *entID, scene)) {
        return false;
    }

    VelocityComponent* const velComp = zf4_get_ent_component(*entID, VELOCITY_COMPONENT, scene);
    velComp->velLerpFactor = VEL_LERP;

    if (!zf4_add_component_to_ent(SPRITE_COMPONENT, *entID, scene)) {
        return false;
    }

    SpriteComponent* const spriteComp = (SpriteComponent*)zf4_get_ent_component(*entID, SPRITE_COMPONENT, scene);
    spriteComp->spriteIndex = PLAYER_SPRITE;

    if (!zf4_add_component_to_ent(ACTOR_COMPONENT, *entID, scene)) {
        return false;
    }

    ActorComponent* const actorComp = (ActorComponent*)zf4_get_ent_component(*entID, ACTOR_COMPONENT, scene);
    actorComp->hp = INIT_HP;
    actorComp->invTimeMax = INV_TIME;

    ZF4Ent* const ent = zf4_get_ent(*entID, scene);
    ent->tag = PLAYER_ENT_TAG;

    return true;
}

void update_player_ent_vel_comps(ZF4Scene* scene) {
    const ZF4Vec2D moveAxis = {
        (float)(zf4_is_key_down(ZF4_KEY_D) - zf4_is_key_down(ZF4_KEY_A)),
        (float)(zf4_is_key_down(ZF4_KEY_S) - zf4_is_key_down(ZF4_KEY_W))
    };

    const ZF4Vec2D velLerpTarg = zf4_calc_vec_2d_scaled(moveAxis, MOVE_SPD);

    const ZF4SceneTypeInfo* const sceneTypeInfo = zf4_get_scene_type_info(scene->typeIndex);

    for (int i = 0; i < sceneTypeInfo->entLimit; ++i) {
        const ZF4EntID entID = {i, scene->entVersions[i]};

        if (!zf4_does_ent_exist(entID, scene)) {
            continue;
        }

        if (!zf4_does_ent_have_tag(entID, PLAYER_ENT_TAG, scene)) {
            continue;
        }

        VelocityComponent* const velComp = zf4_get_ent_component(entID, VELOCITY_COMPONENT, scene);
        velComp->velLerpTarg = velLerpTarg;
    }
}
