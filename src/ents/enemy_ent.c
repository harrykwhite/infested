#include "ents.h"

#include "../sprites.h"
#include "../component_types.h"

#define VEL_LERP 0.25f
#define INIT_HP 30

bool spawn_enemy_ent(ZF4EntID* const entID, const ZF4Vec2D pos, ZF4Scene* const scene) {
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

    SpriteComponent* const spriteComp = zf4_get_ent_component(*entID, SPRITE_COMPONENT, scene);
    spriteComp->spriteIndex = ENEMY_SPRITE;

    if (!zf4_add_component_to_ent(ACTOR_COMPONENT, *entID, scene)) {
        return false;
    }

    ActorComponent* const actorComp = zf4_get_ent_component(*entID, ACTOR_COMPONENT, scene);
    actorComp->hp = INIT_HP;
    actorComp->invTimeMax = 0; // TEMP?

    return true;
}
