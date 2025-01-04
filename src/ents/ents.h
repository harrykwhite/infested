#pragma once

#include <zf4.h>
#include "../component_types.h"
#include "../sprites.h"

enum EntTag {
    PLAYER_ENT_TAG
};

enum EntFlag : zf4::Byte {
    SOLID_ENT_FLAG = 1 << 0
};

void translate_ents_by_vel(zf4::EntityManager* const entManager, zf4::Byte* const entsToDestroyBitset, zf4::MemArena* const scratchSpace);
void write_render_data_of_ents(zf4::Scene* const scene);
bool proc_actor_damaging(zf4::Scene* const scene, zf4::Byte* const entsToDestroyBitset);
bool spawn_hive_ent(zf4::EntID* const entID, const zf4::Vec2D pos, zf4::EntityManager* const entManager);

bool spawn_player_ent(zf4::EntID* const entID, const zf4::Vec2D pos, zf4::EntityManager* const entManager);
void update_player_vels(zf4::EntityManager* const entManager);

bool spawn_enemy_ent(zf4::EntID* const entID, const zf4::Vec2D pos, zf4::EntityManager* const entManager);

bool spawn_gun_ent(zf4::EntID* const entID, const zf4::EntID ownerEntID, const int shootInterval, zf4::EntityManager* const entManager);
bool update_gun_ents(zf4::Scene* const scene);

bool spawn_bullet_ent(zf4::EntID* const entID, const zf4::Vec2D pos, const float spd, const float dir, const int dmg, zf4::EntityManager* const entManager);

inline zf4::Rect create_ent_collider(const zf4::EntID entID, const zf4::EntityManager* const entManager, const zf4::Vec2D offs = {}) {
    const auto spriteComp = reinterpret_cast<const SpriteComponent*>(entManager->get_ent_component(entID, SPRITE_COMPONENT));
    return entManager->create_ent_collider(entID, spriteComp->spriteIndex, spriteComp->origin, spriteComp->scale, offs);
}

inline bool spawn_hive_ent(const zf4::Vec2D pos, zf4::EntityManager* const entManager) {
    zf4::EntID entID = {};
    return spawn_hive_ent(&entID, pos, entManager);
}

inline bool spawn_enemy_ent(const zf4::Vec2D pos, zf4::EntityManager* const entManager) {
    zf4::EntID entID = {};
    return spawn_enemy_ent(&entID, pos, entManager);
}

inline bool spawn_gun_ent(const zf4::EntID ownerEntID, const int shootInterval, zf4::EntityManager* const entManager) {
    zf4::EntID entID = {};
    return spawn_gun_ent(&entID, ownerEntID, shootInterval, entManager);
}

inline bool spawn_bullet_ent(const zf4::Vec2D pos, const float spd, const float dir, const int dmg, zf4::EntityManager* const entManager) {
    zf4::EntID entID = {};
    return spawn_bullet_ent(&entID, pos, spd, dir, dmg, entManager);
}
