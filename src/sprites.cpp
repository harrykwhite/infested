#include "sprites.h"

bool load_sprites(zf4::Array<zf4::Sprite>* const sprites, zf4::MemArena* const memArena) {
    if (!sprites->init(memArena, SPRITE_CNT)) {
        return false;
    }

    for (int i = 0; i < SPRITE_CNT; ++i) {
        if (!(*sprites)[i].frames.init(memArena, 1)) {
            return false;
        }

        switch (i) {
            case PIXEL_SPRITE:
                (*sprites)[i].frames[0] = {0, 0, 1, 1};
                break;

            case PLAYER_SPRITE:
                (*sprites)[i].frames[0] = zf4::shrink_tex_src_rect_to_pixels({8, 0, 24, 40}, 0);
                break;

            case ENEMY_SPRITE:
                (*sprites)[i].frames[0] = zf4::shrink_tex_src_rect_to_pixels({0, 40, 24, 40}, 0);
                break;

            case GUN_SPRITE:
                (*sprites)[i].frames[0] = zf4::shrink_tex_src_rect_to_pixels({32, 0, 24, 8}, 0);
                break;

            case BULLET_SPRITE:
                (*sprites)[i].frames[0] = zf4::shrink_tex_src_rect_to_pixels({32, 8, 8, 8}, 0);
                break;

            case HIVE_SPRITE:
                (*sprites)[i].frames[0] = zf4::shrink_tex_src_rect_to_pixels({0, 80, 88, 88}, 0);
                break;

            case CURSOR_SPRITE:
                (*sprites)[i].frames[0] = zf4::shrink_tex_src_rect_to_pixels({0, 8, 8, 8}, 0);
                break;
        }
    }

    return true;
}
