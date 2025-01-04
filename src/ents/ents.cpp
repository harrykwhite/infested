#include "ents.h"

void translate_ents_by_vel(zf4::EntityManager* const entManager, zf4::Byte* const entsToDestroyBitset, zf4::MemArena* const scratchSpace) {
    // Get the colliders of solid entities; we don't care about their IDs.
    const auto solidEntColliders = scratchSpace->push<zf4::Rect>(entManager->get_ent_limit());
    int solidEntCnt = 0;

    for (int i = 0; i < entManager->get_ent_limit(); ++i) {
        const zf4::EntID entID = entManager->create_ent_id(i);

        if (!entManager->does_ent_exist(entID)) {
            continue;
        }

        if (!(entManager->get_ent_flags(entID) & SOLID_ENT_FLAG)) {
            continue;
        }

        solidEntColliders[solidEntCnt] = create_ent_collider(entID, entManager);
        ++solidEntCnt;
    }

    // Iterate through entities with velocity components.
    for (int i = 0; i < entManager->get_ent_limit(); ++i) {
        const zf4::EntID entID = entManager->create_ent_id(i);

        if (!entManager->does_ent_exist(entID)) {
            continue;
        }

        if (!entManager->does_ent_have_component(entID, VELOCITY_COMPONENT)) {
            continue;
        }

        const auto velComp = reinterpret_cast<VelocityComponent*>(entManager->get_ent_component(entID, VELOCITY_COMPONENT));

        // Interpolate velocity towards target.
        velComp->vel = zf4::lerp(velComp->vel, velComp->velLerpTarg, velComp->velLerpFactor);

        // Check for and handle collisions with solid entities.
        // TODO: Account for case where the entity is solid.

        switch (velComp->solidEntCollisionEvent) {
            case VELOCITY_COMPONENT_SOLID_COLLISION_EVENT_STOP:
                {
                    const zf4::Rect entHorCollider = create_ent_collider(entID, entManager, {velComp->vel.x, 0.0f});

                    for (int j = 0; j < solidEntCnt; ++j) {
                        if (zf4::do_rects_intersect(entHorCollider, solidEntColliders[j])) {
                            velComp->vel.x = 0.0f;
                            break;
                        }
                    }

                    const zf4::Rect entVerCollider = create_ent_collider(entID, entManager, {0.0f, velComp->vel.y});

                    for (int j = 0; j < solidEntCnt; ++j) {
                        if (zf4::do_rects_intersect(entVerCollider, solidEntColliders[j])) {
                            velComp->vel.y = 0.0f;
                            break;
                        }
                    }

                    if (velComp->vel.x != 0.0f && velComp->vel.y != 0.0f) {
                        const zf4::Rect entDiagCollider = create_ent_collider(entID, entManager, velComp->vel);

                        for (int j = 0; j < solidEntCnt; ++j) {
                            if (zf4::do_rects_intersect(entDiagCollider, solidEntColliders[j])) {
                                velComp->vel.x = 0.0f;
                                break;
                            }
                        }
                    }
                }

                break;

            case VELOCITY_COMPONENT_SOLID_COLLISION_EVENT_DESTROY:
                {
                    const zf4::Rect entCollider = create_ent_collider(entID, entManager);

                    for (int j = 0; j < solidEntCnt; ++j) {
                        if (zf4::do_rects_intersect(entCollider, solidEntColliders[j])) {
                            zf4::activate_bit(entsToDestroyBitset, i);
                            break;
                        }
                    }
                }

                break;
        }

        // Translate the entity.
        entManager->get_ent_pos(entID) += velComp->vel;
    }
}

void write_render_data_of_ents(zf4::Scene* const scene) {
    for (int i = 0; i < scene->entManager.get_ent_limit(); ++i) {
        const zf4::EntID entID = scene->entManager.create_ent_id(i);

        if (!scene->entManager.does_ent_exist(entID)) {
            continue;
        }

        if (!scene->entManager.does_ent_have_component(entID, SPRITE_COMPONENT)) {
            continue;
        }

        const auto spriteComp = reinterpret_cast<SpriteComponent*>(scene->entManager.get_ent_component(entID, SPRITE_COMPONENT));

        if (spriteComp->prewrite) {
            spriteComp->prewrite(entID, scene);
        }

        auto& sprite = zf4::get_game_sprites()[spriteComp->spriteIndex];
        const zf4::Vec2D entPos = scene->entManager.get_ent_pos(entID);
        zf4::write_to_sprite_batch(&scene->renderer, 0, sprite.texIndex, entPos, sprite.frames[0], spriteComp->origin, spriteComp->rot, spriteComp->scale, spriteComp->alpha);

        if (spriteComp->postwrite) {
            spriteComp->postwrite(entID, scene);
        }
    }
}

bool proc_actor_damaging(zf4::Scene* const scene, zf4::Byte* const entsToDestroyBitset) {
    //
    // Get damager and actor entities.
    //
    const auto damagerEntIDs = scene->scratchSpace.push<zf4::EntID>(scene->entManager.get_ent_limit());
    const auto damagerColliders = scene->scratchSpace.push<zf4::Rect>(scene->entManager.get_ent_limit());

    if (!damagerEntIDs || !damagerColliders) {
        return false;
    }

    const auto actorEntIDs = scene->scratchSpace.push<zf4::EntID>(scene->entManager.get_ent_limit());
    const auto actorColliders = scene->scratchSpace.push<zf4::Rect>(scene->entManager.get_ent_limit());

    if (!actorEntIDs || !actorColliders) {
        return false;
    }

    int damagerEntCnt = 0;
    int actorEntCnt = 0;

    for (int i = 0; i < scene->entManager.get_ent_limit(); ++i) {
        const zf4::EntID entID = scene->entManager.create_ent_id(i);

        if (!scene->entManager.does_ent_exist(entID)) {
            continue;
        }

        if (scene->entManager.does_ent_have_component(entID, DAMAGER_COMPONENT)) {
            damagerEntIDs[damagerEntCnt] = entID;
            damagerColliders[damagerEntCnt] = create_ent_collider(entID, &scene->entManager);
            ++damagerEntCnt;
        }

        if (scene->entManager.does_ent_have_component(entID, ACTOR_COMPONENT)) {
            actorEntIDs[actorEntCnt] = entID;
            actorColliders[actorEntCnt] = create_ent_collider(entID, &scene->entManager);
            ++actorEntCnt;
        }
    }

    //
    // Check for and handle collisions.
    //
    for (int i = 0; i < damagerEntCnt; ++i) {
        const zf4::EntID damagerEntID = damagerEntIDs[i];
        const auto damagerComp = reinterpret_cast<DamagerComponent*>(scene->entManager.get_ent_component(damagerEntID, DAMAGER_COMPONENT));

        for (int j = 0; j < actorEntCnt; ++j) {
            const zf4::EntID actorEntID = actorEntIDs[j];

            if (std::memcmp(&damagerEntID, &actorEntID, sizeof(zf4::EntID)) == 0) {
                // The damager and actor are the same entity, so skip.
                continue;
            }

            if (damagerComp->canDmgOtherEnt && !damagerComp->canDmgOtherEnt(actorEntID, &scene->entManager)) {
                continue;
            }

            if (zf4::do_rects_intersect(damagerColliders[i], actorColliders[j])) {
                // Hurt the actor.
                const auto actorComp = reinterpret_cast<ActorComponent*>(scene->entManager.get_ent_component(actorEntID, ACTOR_COMPONENT));
                actorComp->hp -= damagerComp->dmg;
                actorComp->invTime = actorComp->invTimeMax;

                if (actorComp->hp <= 0) {
                    zf4::activate_bit(entsToDestroyBitset, actorEntID.index);
                }

                // Apply knockback if the actor has a velocity component.
                if (scene->entManager.does_ent_have_component(actorEntID, VELOCITY_COMPONENT)) {
                    zf4::Vec2D knockbackDir;

                    switch (damagerComp->knockbackDirType) {
                        case DIR_FROM_OTHER_DAMAGER_COMPONENT_KNOCKBACK_DIR_TYPE:
                            {
                                const zf4::Vec2D actorPos = scene->entManager.get_ent_pos(actorEntID);
                                const zf4::Vec2D damagerPos = scene->entManager.get_ent_pos(damagerEntID);
                                knockbackDir = zf4::calc_normal(actorPos - damagerPos);
                            }

                            break;

                        case OTHER_VEL_DAMAGER_COMPONENT_KNOCKBACK_DIR_TYPE:
                            const auto damagerVelComp = reinterpret_cast<VelocityComponent*>(scene->entManager.get_ent_component(damagerEntID, VELOCITY_COMPONENT));
                            knockbackDir = zf4::calc_normal(damagerVelComp->vel);
                            break;
                    }

                    const auto velComp = reinterpret_cast<VelocityComponent*>(scene->entManager.get_ent_component(actorEntID, VELOCITY_COMPONENT));
                    velComp->vel += knockbackDir * damagerComp->strength;
                }

                if (damagerComp->destroyOnDmg) {
                    zf4::activate_bit(entsToDestroyBitset, damagerEntID.index);
                }
            }
        }
    }

    return true;
}

bool spawn_hive_ent(zf4::EntID* const entID, const zf4::Vec2D pos, zf4::EntityManager* const entManager) {
    assert(zf4::is_zero(entID));

    if (!entManager->spawn_ent(entID, pos)) {
        return false;
    }

    if (!entManager->add_component_to_ent(SPRITE_COMPONENT, *entID)) {
        return false;
    }

    const auto spriteComp = reinterpret_cast<SpriteComponent*>(entManager->get_ent_component(*entID, SPRITE_COMPONENT));
    spriteComp->spriteIndex = HIVE_SPRITE;

    entManager->add_ent_flag(*entID, SOLID_ENT_FLAG);

    return true;
}
