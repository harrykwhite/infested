#ifndef CAMERA_H
#define CAMERA_H

#include <zf4.h>

typedef struct {
    ZF4Vec2D pos;
    float shake;
} CameraMeta;

void camera_tick(CameraMeta* const camMeta, ZF4Scene* const scene, const ZF4EntID playerEntID);
void shake_camera(CameraMeta* const camMeta, const float amount);

#endif
