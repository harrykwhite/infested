#include "title_screen.h"

#include "../asset_indexes.h"
#include "scene_types.h"

static void refresh_text_positions(zf4::Renderer* const renderer, const TitleScreen* const ts) {
    zf4::CharBatchDisplayProps* titleTextDisplayProps = zf4::get_char_batch_display_props(renderer, ts->titleTextCBID);
    titleTextDisplayProps->pos = {
        zf4::get_window_size().x / 2.0f,
        (zf4::get_window_size().y / 2.0f) - 40.0f
    };

    zf4::CharBatchDisplayProps* startTextDisplayProps = zf4::get_char_batch_display_props(renderer, ts->startTextCBID);
    startTextDisplayProps->pos = {
        zf4::get_window_size().x / 2.0f,
        (zf4::get_window_size().y / 2.0f) + 32.0f
    };
}

void init_title_screen_render_layer_props(zf4::RenderLayerProps* const props, const int layerIndex) {
    switch (layerIndex) {
        case GENERAL_TITLE_SCREEN_RENDER_LAYER:
            props->spriteBatchCnt = 0;
            props->charBatchCnt = 2;
            break;
    }
}

bool init_title_screen(zf4::Scene* const scene) {
    const auto ts = static_cast<TitleScreen*>(scene->userData);

    ts->titleTextCBID = zf4::activate_any_char_batch(&scene->renderer, GENERAL_TITLE_SCREEN_RENDER_LAYER, 8, EB_GARAMOND_72_FONT);
    zf4::write_to_char_batch(&scene->renderer, ts->titleTextCBID, "Infested", zf4::FONT_HOR_ALIGN_CENTER, zf4::FONT_VER_ALIGN_CENTER);

    ts->startTextCBID = zf4::activate_any_char_batch(&scene->renderer, GENERAL_TITLE_SCREEN_RENDER_LAYER, 32, EB_GARAMOND_18_FONT);
    zf4::write_to_char_batch(&scene->renderer, ts->startTextCBID, "Press Enter to Start", zf4::FONT_HOR_ALIGN_CENTER, zf4::FONT_VER_ALIGN_CENTER);

    refresh_text_positions(&scene->renderer, ts);

    return true;
}

bool title_screen_tick(zf4::Scene* const scene, int* const sceneChangeIndex) {
    const auto ts = static_cast<TitleScreen*>(scene->userData);

    if (zf4::is_key_down(zf4::KEY_ENTER)) {
        *sceneChangeIndex = WORLD_SCENE;
    }

    refresh_text_positions(&scene->renderer, ts);

    return true;
}
