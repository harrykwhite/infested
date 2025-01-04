#include "world.h"

int spawn_projectile(const zf4::Vec2D pos, const float spd, const float dir, World* const world) {
    const int entIndex = spawn_entity(pos, PROJECTILE_ENTITY, world);

    if (entIndex != -1) {
        ProjectileEntExt* const entExt = &world->ents[entIndex].typeData.projectile;
        entExt->vel = zf4::calc_len_dir(spd, dir);
    }

    return entIndex;
}

/*int spawn_projectile(const zf4::Vec2D pos, const float spd, const float dir, World* const world) {
    const int projIndex = world->projectiles.activate_first_inactive();

    if (projIndex != -1) {
        world->projectiles[projIndex].pos = pos;
        world->projectiles[projIndex].vel = zf4::calc_len_dir(spd, dir);
    }

    return projIndex;
}

void destroy_projectile(const int index, World* const world) {
    // NOTE: Should we do asserts explicitly here? They exist anyway.
    world->projectiles.deactivate(index);
}

zf4::Rect create_projectile_collider(const Projectile* const proj) {
    return create_collider(proj->pos, BULLET_SPRITE, ik_origin);
}*/
