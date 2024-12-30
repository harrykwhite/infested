#ifndef WORLD_H
#define WORLD_H

#include <zf4.h>
#include "../camera.h"

typedef enum {
    CAM_WORLD_RENDER_LAYER,
    UI_WORLD_RENDER_LAYER,

    WORLD_RENDER_LAYER_CNT
} WorldRenderLayer;

typedef struct {
    CameraMeta camMeta;
    ZF4EntID playerEntID;
} World;

void init_world_render_layer_props(ZF4RenderLayerProps* props, int layerIndex);
void load_world_component_type_limit(int* typeLimit, int typeIndex);
bool init_world(ZF4Scene* scene);
bool world_tick(ZF4Scene* scene, int* sceneChangeIndex);

#endif
