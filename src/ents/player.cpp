#include "ents.h"

constexpr float ik_moveSpd = 3.0f;
constexpr float ik_velLerp = 0.25f;
constexpr int ik_initHP = 10;
constexpr int ik_invTime = 30;

bool spawn_player_ent(zf4::EntID* const entID, const zf4::Vec2D pos, zf4::EntityManager* const entManager) {
    assert(zf4::is_zero(entID));

    //
    // Player Entity
    //
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
    spriteComp->spriteIndex = PLAYER_SPRITE;

    if (!entManager->add_component_to_ent(ACTOR_COMPONENT, *entID)) {
        return false;
    }

    const auto actorComp = reinterpret_cast<ActorComponent*>(entManager->get_ent_component(*entID, ACTOR_COMPONENT));
    actorComp->hp = ik_initHP;
    actorComp->invTimeMax = ik_invTime;

    entManager->set_ent_tag(*entID, PLAYER_ENT_TAG);

    //
    // Gun Entity
    //
    zf4::EntID gunEntID = {};

    if (!spawn_gun_ent(&gunEntID, *entID, 10, entManager)) {
        return false;
    }

    return true;
}

void update_player_vels(zf4::EntityManager* const entManager) {
    const zf4::Vec2D moveAxis = {
        static_cast<float>(zf4::is_key_down(zf4::KEY_D) - zf4::is_key_down(zf4::KEY_A)),
        static_cast<float>(zf4::is_key_down(zf4::KEY_S) - zf4::is_key_down(zf4::KEY_W))
    };

    const zf4::Vec2D velLerpTarg = moveAxis * ik_moveSpd;

    for (int i = 0; i < entManager->get_ent_limit(); ++i) {
        const zf4::EntID entID = entManager->create_ent_id(i);

        if (!entManager->does_ent_exist(entID)) {
            continue;
        }

        if (!entManager->does_ent_have_tag(entID, PLAYER_ENT_TAG)) {
            continue;
        }

        const auto velComp = reinterpret_cast<VelocityComponent*>(entManager->get_ent_component(entID, VELOCITY_COMPONENT));
        velComp->velLerpTarg = velLerpTarg;
    }
}
