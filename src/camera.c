#include "camera.h"

#define LOOK_DIST_LIMIT 24.0f
#define LOOK_DIST_SCALAR_DIST (LOOK_DIST_LIMIT * 32.0f)
#define POS_LERP 0.25f
#define SHAKE_LERP 0.1f

void camera_tick(CameraMeta* const camMeta, ZF4Scene* const scene, const ZF4EntID playerEntID) {
    ZF4Camera* const cam = &scene->renderer.cam;

    // Determine the target position.
    ZF4Vec2D targPos = camMeta->pos;

    if (zf4_does_ent_exist(playerEntID, scene)) {
        const ZF4Ent* const playerEnt = zf4_get_ent(playerEntID, scene);

        const ZF4Vec2D mouseCamPos = zf4_screen_to_camera_pos(zf4_get_mouse_pos(), cam);
        const ZF4Vec2D playerToMouseCamPosDiff = zf4_calc_vec_2d_diff(mouseCamPos, playerEnt->pos);
        const float playerToMouseCamPosDist = zf4_calc_vec_2d_mag(playerToMouseCamPosDiff);
        const ZF4Vec2D playerToMouseCamPosDir = zf4_calc_vec_2d_normal(playerToMouseCamPosDiff);

        const float lookDist = LOOK_DIST_LIMIT * ZF4_MIN(playerToMouseCamPosDist / LOOK_DIST_SCALAR_DIST, 1.0f);
        const ZF4Vec2D lookOffs = zf4_calc_vec_2d_scaled(playerToMouseCamPosDir, lookDist);

        targPos = zf4_calc_vec_2d_sum(playerEnt->pos, lookOffs);
    }

    // Approach the target position.
    camMeta->pos.x = zf4_lerp(cam->pos.x, targPos.x, POS_LERP);
    camMeta->pos.y = zf4_lerp(cam->pos.y, targPos.y, POS_LERP);

    // Determine the shake to apply, then reduce it.
    const ZF4Vec2D shake = {
        zf4_gen_rand_float(-camMeta->shake, camMeta->shake),
        zf4_gen_rand_float(-camMeta->shake, camMeta->shake)
    };

    camMeta->shake = zf4_lerp(camMeta->shake, 0.0f, SHAKE_LERP);

    // Set the actual camera position.
    cam->pos = zf4_calc_vec_2d_sum(camMeta->pos, shake);
}

void shake_camera(CameraMeta* const camMeta, const float amount) {
    camMeta->shake = ZF4_MAX(camMeta->shake, amount);
}
