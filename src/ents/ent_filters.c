#include "ents.h"

bool is_player_ent(const ZF4EntID entID, const ZF4Scene* const scene) {
    return zf4_does_ent_have_tag(entID, PLAYER_ENT_TAG, scene); // TEMP: This is just lame!
}

bool is_not_player_ent(const ZF4EntID entID, const ZF4Scene* const scene) {
    return !is_player_ent(entID, scene);
}
