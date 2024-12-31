#ifndef ENT_H
#define ENT_H

#include <zf4.h>
#include "../camera.h"

typedef enum {
    PLAYER_ENT_TAG
} EntTag;

void translate_ents_by_vel(ZF4Scene* scene);
void write_render_data_of_ents(ZF4Scene* scene);
ZF4EntID spawn_bullet_ent(const ZF4Vec2D pos, const float spd, const float dir, const ZF4Scene* const scene);

ZF4EntID spawn_player_ent(ZF4Vec2D pos, ZF4Scene* scene);
void update_player_ent_vels(ZF4Scene* scene);

ZF4EntID spawn_gun_ent(const ZF4EntID ownerEntID, const int shootInterval, const ZF4Scene* const scene);
void update_gun_ents(const ZF4Scene* const scene, CameraMeta* const camMeta);

#endif
