#include "world.h"

static constexpr float ik_velLerpFactor = 0.25f;

void on_enemy_spawn(Entity* const ent) {
    assert(ent->typeIndex == ENEMY_ENTITY);

    ent->spriteIndex = ENEMY_SPRITE;

    EnemyEntExt* const enemy = &ent->typeData.enemy;
    enemy->hp = 5;
}

void update_enemy(Entity* const enemyEnt, World* const world) {
    assert(enemyEnt->typeIndex == ENEMY_ENTITY);

    EnemyEntExt* const enemyEntExt = &enemyEnt->typeData.enemy;

    enemyEntExt->vel = zf4::lerp(enemyEntExt->vel, {}, ik_velLerpFactor);
    enemyEnt->pos += enemyEntExt->vel;

    //
    // Projectile Collisions
    //
    {
        const zf4::Rect enemyEntCollider = create_ent_collider(enemyEnt);

        for (int i = 0; i < world->ents.get_len(); ++i) { // NOTE: This loop, although it does nothing, causes tremendous slowdown. An optimisation is to batch entities into chunks based on position. You only access entities near you.
            if (!world->ents.is_active(i)) {
                continue;
            }

            const Entity* const ent = &world->ents[i];

            if (ent->typeIndex != PROJECTILE_ENTITY) {
                continue;
            }

            const zf4::Rect entCollider = create_ent_collider(ent);

            if (zf4::do_rects_intersect(enemyEntCollider, entCollider)) {
                const ProjectileEntExt* const proj = &ent->typeData.projectile;
                hurt_enemy(enemyEnt, 1, proj->vel / 2.0f, world);
                destroy_entity(i, world);
            }
        }
    }
}

void hurt_enemy(Entity* const ent, const int dmg, const zf4::Vec2D force, World* const world) {
    assert(ent->typeIndex == ENEMY_ENTITY);

    EnemyEntExt* const enemy = &ent->typeData.enemy;
    enemy->hp -= dmg;
    enemy->vel += force;
}
