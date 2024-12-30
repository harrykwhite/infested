#include <zf4.h>
#include "sprites.h"
#include "component_types.h"
#include "scenes/scene_types.h"

int main() {
    ZF4UserGameInfo userGameInfo = {
        .windowInitWidth = 1280,
        .windowInitHeight = 720,
        .windowTitle = "Infested",
        .windowResizable = true,
        .windowHideCursor = true,

        .spriteCnt = SPRITE_CNT,
        .spriteLoader = load_sprite,

        .componentTypeCnt = COMPONENT_TYPE_CNT,
        .componentTypeInfoLoader = load_component_type_info,

        .sceneTypeCnt = SCENE_TYPE_CNT,
        .sceneTypeInfoLoader = load_scene_type_info
    };

    zf4_start_game(&userGameInfo);
}
