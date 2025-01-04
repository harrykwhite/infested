#pragma once

#include <zf4.h>
#include "../sprites.h"

enum WorldRenderLayer {
    CAM_WORLD_RENDER_LAYER,
    UI_WORLD_RENDER_LAYER,

    WORLD_RENDER_LAYER_CNT
};

struct World {
    zf4::EntID playerEntID;

    zf4::Vec2D camPosNoShake;
    float camShake;
};

bool init_world(zf4::Scene* const scene);
bool world_tick(zf4::Scene* const scene, int* const sceneChangeIndex);
void init_world_render_layer_props(zf4::RenderLayerProps* const props, const int layerIndex);
void load_world_component_type_limit(int* const typeLimit, const int typeIndex);
bool is_in_world(const zf4::Vec2D pos);
