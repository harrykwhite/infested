#include "ents.h"

static constexpr float ik_velLerp = 0.25f;
static constexpr int ik_initHP = 5;

bool spawn_enemy_ent(zf4::EntID* const entID, const zf4::Vec2D pos, zf4::EntityManager* const entManager) {
    if (!entManager->spawn_ent(entID, pos)) {
        return false;
    }

    if (!entManager->add_component_to_ent(VELOCITY_COMPONENT, *entID)) {
        return false;
    }

    const auto velComp = reinterpret_cast<VelocityComponent*>(entManager->get_ent_component(*entID, VELOCITY_COMPONENT));
    velComp->velLerpFactor = ik_velLerp;

    if (!entManager->add_component_to_ent(SPRITE_COMPONENT, *entID)) {
        return false;
    }

    const auto spriteComp = reinterpret_cast<SpriteComponent*>(entManager->get_ent_component(*entID, SPRITE_COMPONENT));
    spriteComp->spriteIndex = ENEMY_SPRITE;

    if (!entManager->add_component_to_ent(ACTOR_COMPONENT, *entID)) {
        return false;
    }

    const auto actorComp = reinterpret_cast<ActorComponent*>(entManager->get_ent_component(*entID, ACTOR_COMPONENT));
    actorComp->hp = ik_initHP;
    actorComp->invTimeMax = 0; // TEMP?

    if (!entManager->add_component_to_ent(DAMAGER_COMPONENT, *entID)) {
        return false;
    }

    const auto damagerComp = reinterpret_cast<DamagerComponent*>(entManager->get_ent_component(*entID, DAMAGER_COMPONENT));
    damagerComp->strength = 12.0f;
    damagerComp->dmg = 1;
    damagerComp->canDmgOtherEnt = [](const zf4::EntID otherEntID, const zf4::EntityManager* const entManager) {
        return entManager->does_ent_have_tag(otherEntID, PLAYER_ENT_TAG);
    };

    return true;
}
