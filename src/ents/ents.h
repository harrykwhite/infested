#ifndef ENT_H
#define ENT_H

#include <zf4.h>

void translate_ents_by_vel(ZF4Scene* scene);
void write_render_data_of_ents(ZF4Scene* scene);

ZF4EntID spawn_player_ent(ZF4Vec2D pos, ZF4Scene* scene);
void update_player_vels(ZF4EntID entID, ZF4Scene* scene);

#endif
