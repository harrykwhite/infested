#ifndef SPRITES_H
#define SPRITES_H

#include <zf4.h>

typedef enum {
    PIXEL_SPRITE,
    PLAYER_SPRITE,
    GUN_SPRITE,
    BULLET_SPRITE,
    CURSOR_SPRITE,

    SPRITE_CNT
} SpriteIndex;

void load_sprite(ZF4Sprite* sprite, int index);

#endif
