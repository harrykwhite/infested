#include "ents.h"

#include "../component_types.h"
#include "../sprites.h"

#define INIT_HP 100
#define INV_TIME 30
#define VEL_LERP 0.25f
#define MOVE_SPD 3.0f

ZF4EntID spawn_player_ent(ZF4Vec2D pos, ZF4Scene* scene) {
    ZF4EntID entID = zf4_spawn_ent(pos, scene);

    zf4_add_component_to_ent(VELOCITY_COMPONENT, entID, scene);

    SpriteComponent* spriteComp = zf4_add_component_to_ent(SPRITE_COMPONENT, entID, scene);
    spriteComp->spriteIndex = PLAYER_SPRITE;

    ActorComponent* actorComp = zf4_add_component_to_ent(ACTOR_COMPONENT, entID, scene);
    actorComp->hp = INIT_HP;
    actorComp->invTimeMax = INV_TIME;

    return entID;
}

void update_player_vels(ZF4EntID entID, ZF4Scene* scene) {
    const ZF4Vec2D moveAxis = {
        (float)(zf4_is_key_down(ZF4_KEY_D) - zf4_is_key_down(ZF4_KEY_A)),
        (float)(zf4_is_key_down(ZF4_KEY_S) - zf4_is_key_down(ZF4_KEY_W))
    };

    ZF4Vec2D velTarg = zf4_calc_vec_2d_scaled(moveAxis, MOVE_SPD);

    VelocityComponent* velComp = zf4_get_ent_component(entID, VELOCITY_COMPONENT, scene);

    velComp->vel.x = zf4_lerp(velComp->vel.x, velTarg.x, VEL_LERP);
    velComp->vel.y = zf4_lerp(velComp->vel.y, velTarg.y, VEL_LERP);
}
