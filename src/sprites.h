#pragma once

#include <zf4.h>

enum SpriteIndex {
    PIXEL_SPRITE,
    PLAYER_SPRITE,
    ENEMY_SPRITE,
    GUN_SPRITE,
    BULLET_SPRITE,
    HIVE_SPRITE,
    CURSOR_SPRITE,

    SPRITE_CNT
};

bool load_sprites(zf4::Array<zf4::Sprite>* const sprites, zf4::MemArena* const memArena);
