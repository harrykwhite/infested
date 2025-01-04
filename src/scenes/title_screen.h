#pragma once

#include <zf4.h>

enum TitleScreenRenderLayer {
    GENERAL_TITLE_SCREEN_RENDER_LAYER,

    TITLE_SCREEN_RENDER_LAYER_CNT
};

struct TitleScreen {
    zf4::CharBatchID titleTextCBID;
    zf4::CharBatchID startTextCBID;
};

void init_title_screen_render_layer_props(zf4::RenderLayerProps* const props, const int layerIndex);
bool init_title_screen(zf4::Scene* const scene);
bool title_screen_tick(zf4::Scene* const scene, int* const sceneChangeIndex);
