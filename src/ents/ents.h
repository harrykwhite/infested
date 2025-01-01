#ifndef ENT_H
#define ENT_H

#include <zf4.h>
#include "../camera.h"

typedef enum {
    PLAYER_ENT_TAG
} EntTag;

void ent_vel_sys(ZF4Scene* scene);
void write_render_data_of_ents(ZF4Scene* scene);
void load_ent_collider(ZF4RectF* const collider, const ZF4EntID entID, const ZF4Scene* const scene);
bool spawn_bullet_ent(ZF4EntID* const entID, const ZF4Vec2D pos, const float spd, const float dir, const int dmg, const ZF4Scene* const scene);
void proc_actor_damaging(ZF4Scene* scene);

bool spawn_player_ent(ZF4EntID* const entID, const ZF4Vec2D pos, const ZF4Scene* const scene);
void update_player_ent_vel_comps(ZF4Scene* scene);

bool spawn_enemy_ent(ZF4EntID* const entID, const ZF4Vec2D pos, ZF4Scene* const scene);

bool spawn_gun_ent(ZF4EntID* const entID, const ZF4EntID ownerEntID, const int shootInterval, const ZF4Scene* const scene);
bool update_gun_ents(const ZF4Scene* const scene, CameraMeta* const camMeta);

bool is_player_ent(const ZF4EntID entID, const ZF4Scene* const scene);
bool is_not_player_ent(const ZF4EntID entID, const ZF4Scene* const scene);

inline bool spawn_bullet_ent_noref(const ZF4Vec2D pos, const float spd, const float dir, const int dmg, const ZF4Scene* const scene) {
    ZF4EntID entID = {0};
    return spawn_bullet_ent(&entID, pos, spd, dir, dmg, scene);
}

inline bool spawn_player_ent_noref(const ZF4Vec2D pos, const ZF4Scene* const scene) {
    ZF4EntID entID = {0};
    return spawn_player_ent(&entID, pos, scene);
}

inline bool spawn_enemy_ent_noref(const ZF4Vec2D pos, ZF4Scene* const scene) {
    ZF4EntID entID = {0};
    return spawn_enemy_ent(&entID, pos, scene);
}

inline bool spawn_gun_ent_noref(const ZF4EntID ownerEntID, const int shootInterval, const ZF4Scene* const scene) {
    ZF4EntID entID = {0};
    return spawn_gun_ent(&entID, ownerEntID, shootInterval, scene);
}

#endif
