#pragma once

#include <zf4.h>

enum SceneType {
    TITLE_SCREEN_SCENE,
    WORLD_SCENE,

    SCENE_TYPE_CNT
};

void load_scene_type_info(zf4::SceneTypeInfo* const typeInfo, const int typeIndex);
