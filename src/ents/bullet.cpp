#include "ents.h"

bool spawn_bullet_ent(zf4::EntID* const entID, const zf4::Vec2D pos, const float spd, const float dir, const int dmg, zf4::EntityManager* const entManager) {
    assert(zf4::is_zero(entID));
    assert(spd > 0);
    assert(dmg > 0);

    if (!entManager->spawn_ent(entID, pos)) {
        return false;
    }

    if (!entManager->add_component_to_ent(VELOCITY_COMPONENT, *entID)) {
        return false;
    }

    const auto velComp = reinterpret_cast<VelocityComponent*>(entManager->get_ent_component(*entID, VELOCITY_COMPONENT));
    velComp->vel = zf4::calc_len_dir(spd, dir);
    velComp->solidEntCollisionEvent = VELOCITY_COMPONENT_SOLID_COLLISION_EVENT_DESTROY;

    if (!entManager->add_component_to_ent(SPRITE_COMPONENT, *entID)) {
        return false;
    }

    const auto spriteComp = reinterpret_cast<SpriteComponent*>(entManager->get_ent_component(*entID, SPRITE_COMPONENT));
    spriteComp->spriteIndex = BULLET_SPRITE;

    if (!entManager->add_component_to_ent(DAMAGER_COMPONENT, *entID)) {
        return false;
    }

    const auto damagerComp = reinterpret_cast<DamagerComponent*>(entManager->get_ent_component(*entID, DAMAGER_COMPONENT));
    damagerComp->dmg = dmg;
    damagerComp->strength = spd;
    damagerComp->canDmgOtherEnt = [](const zf4::EntID otherEntID, const zf4::EntityManager* const entManager) {
        return !entManager->does_ent_have_tag(otherEntID, PLAYER_ENT_TAG);
    };
    damagerComp->destroyOnDmg = true;
    damagerComp->knockbackDirType = OTHER_VEL_DAMAGER_COMPONENT_KNOCKBACK_DIR_TYPE;

    return true;
}
