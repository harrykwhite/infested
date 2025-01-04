#include <zf4.h>
#include "sprites.h"
#include "component_types.h"
#include "scenes/scene_types.h"

int main() {
    const zf4::UserGameInfo userGameInfo = {
        .windowInitWidth = 1280,
        .windowInitHeight = 720,
        .windowTitle = "Infested",
        .windowResizable = true,
        .windowHideCursor = true,

        .spritesLoader = load_sprites,

        .componentTypeCnt = COMPONENT_TYPE_CNT,
        .componentTypeInfoLoader = load_component_type_info,

        .sceneTypeCnt = SCENE_TYPE_CNT,
        .sceneTypeInfoLoader = load_scene_type_info
    };

    zf4::start_game(&userGameInfo);
}
