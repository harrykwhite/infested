#include "world.h"

static constexpr float ik_moveSpd = 3.0f;
static constexpr float ik_velLerpFactor = 0.25f;
static constexpr float ik_shootSpd = 16.0f;
static constexpr int ik_shootInterval = 10;
static constexpr float ik_shootPosOffsDist = 12.0f;
static constexpr zf4::Vec2D ik_origin = {0.5f, 0.5f};

bool get_collision_with_solid(const Entity* const ent, World* const world, const zf4::Camera* const cam) {
    
}

void on_player_spawn(Entity* const ent) {
    assert(ent->typeIndex == PLAYER_ENTITY);
    ent->spriteIndex = PLAYER_SPRITE;
}

bool update_player(Entity* const playerEnt, World* const world, const zf4::Camera* const cam) {
    PlayerEntExt* const playerEntExt = &playerEnt->typeData.player;

    //
    // Movement
    //
    {
        const zf4::Vec2D moveAxis = {
            static_cast<float>(zf4::is_key_down(zf4::KEY_D) - zf4::is_key_down(zf4::KEY_A)),
            static_cast<float>(zf4::is_key_down(zf4::KEY_S) - zf4::is_key_down(zf4::KEY_W))
        };
        const zf4::Vec2D velLerpTarg = moveAxis * ik_moveSpd;
        playerEntExt->vel = zf4::lerp(playerEntExt->vel, velLerpTarg, ik_velLerpFactor);
        playerEnt->pos += playerEntExt->vel;
    }

    //
    // Shooting
    //
    {
        const zf4::Vec2D mouseCamPos = zf4::screen_to_camera_pos(zf4::get_mouse_pos(), cam);
        playerEntExt->shootDir = zf4::calc_dir(playerEnt->pos, mouseCamPos);

        if (playerEntExt->shootTime > 0) {
            --playerEntExt->shootTime;
        } else {
            if (zf4::is_mouse_button_down(zf4::MOUSE_BUTTON_LEFT)) {
                const zf4::Vec2D shootPosOffs = zf4::calc_len_dir(ik_shootPosOffsDist, playerEntExt->shootDir);
                const zf4::Vec2D shootPos = playerEnt->pos + shootPosOffs;

                if (spawn_projectile(shootPos, ik_shootSpd, playerEntExt->shootDir, world) == -1) {
                    return false;
                }

                playerEntExt->shootTime = ik_shootInterval;
            }
        }
    }

    return true;
}
