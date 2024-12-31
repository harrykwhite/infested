#include "sprites.h"

#include "asset_indexes.h"

static void load_pixel_sprite_src_rect(ZF4Rect* srcRect, int frameIndex) {
    switch (frameIndex) {
        case 0:
            srcRect->x = 0;
            srcRect->y = 0;
            srcRect->width = 1;
            srcRect->height = 1;
            break;
    }
}

static void load_player_sprite_src_rect(ZF4Rect* srcRect, int frameIndex) {
    switch (frameIndex) {
        case 0:
            srcRect->x = 8;
            srcRect->y = 0;
            srcRect->width = 24;
            srcRect->height = 40;
            break;
    }
}

static void load_gun_sprite_src_rect(ZF4Rect* srcRect, int frameIndex) {
    switch (frameIndex) {
        case 0:
            srcRect->x = 32;
            srcRect->y = 0;
            srcRect->width = 22;
            srcRect->height = 6;
            break;
    }
}

static void load_bullet_sprite_src_rect(ZF4Rect* srcRect, int frameIndex) {
    switch (frameIndex) {
        case 0:
            srcRect->x = 32;
            srcRect->y = 8;
            srcRect->width = 3;
            srcRect->height = 3;
            break;
    }
}

static void load_cursor_sprite_src_rect(ZF4Rect* srcRect, int frameIndex) {
    switch (frameIndex) {
        case 0:
            srcRect->x = 0;
            srcRect->y = 8;
            srcRect->width = 4;
            srcRect->height = 4;
            break;
    }
}

void load_sprite(ZF4Sprite* sprite, int index) {
    switch (index) {
        case PIXEL_SPRITE:
            *sprite = (ZF4Sprite) {
                .texIndex = 0,
                .frameCnt = 1,
                .srcRectLoader = load_pixel_sprite_src_rect
            };

            break;

        case PLAYER_SPRITE:
            *sprite = (ZF4Sprite) {
                .texIndex = 0,
                .frameCnt = 1,
                .srcRectLoader = load_player_sprite_src_rect
            };

            break;

        case GUN_SPRITE:
            *sprite = (ZF4Sprite) {
                .texIndex = 0,
                .frameCnt = 1,
                .srcRectLoader = load_gun_sprite_src_rect
            };

            break;

        case BULLET_SPRITE:
            *sprite = (ZF4Sprite) {
                .texIndex = 0,
                .frameCnt = 1,
                .srcRectLoader = load_bullet_sprite_src_rect
            };

            break;

        case CURSOR_SPRITE:
            *sprite = (ZF4Sprite) {
                .texIndex = 0,
                .frameCnt = 1,
                .srcRectLoader = load_cursor_sprite_src_rect
            };

            break;
    }
}
