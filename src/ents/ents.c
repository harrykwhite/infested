#include "ents.h"

#include "../component_types.h"

void translate_ents_by_vel(ZF4Scene* scene) {
    ZF4SceneTypeInfo* sceneTypeInfo = zf4_get_scene_type_info(scene->typeIndex);

    for (int i = 0; i < sceneTypeInfo->entLimit; ++i) {
        ZF4EntID entID = {i, scene->entVersions[i]};

        if (!zf4_does_ent_exist(entID, scene)) {
            continue;
        }

        if (!zf4_does_ent_have_component(entID, VELOCITY_COMPONENT, scene)) {
            continue;
        }

        VelocityComponent* velComp = zf4_get_ent_component(entID, VELOCITY_COMPONENT, scene);

        ZF4Ent* ent = zf4_get_ent(entID, scene);
        ent->pos = zf4_calc_vec_2d_sum(ent->pos, velComp->vel);
    }
}

void write_render_data_of_ents(ZF4Scene* scene) {
    ZF4SceneTypeInfo* sceneTypeInfo = zf4_get_scene_type_info(scene->typeIndex);

    for (int i = 0; i < sceneTypeInfo->entLimit; ++i) {
        ZF4EntID entID = {i, scene->entVersions[i]};

        if (!zf4_does_ent_exist(entID, scene)) {
            continue;
        }

        if (!zf4_does_ent_have_component(entID, SPRITE_COMPONENT, scene)) {
            continue;
        }

        SpriteComponent* spriteComp = zf4_get_ent_component(entID, SPRITE_COMPONENT, scene);

        if (spriteComp->prewrite) {
            spriteComp->prewrite(entID, scene);
        }

        ZF4Ent* ent = zf4_get_ent(entID, scene);

        ZF4SpriteBatchWriteInfo sbWriteInfo = {
            .texIndex = zf4_get_sprite(spriteComp->spriteIndex)->texIndex,
            .pos = ent->pos,
            .srcRect = zf4_get_sprite_src_rect(spriteComp->spriteIndex, 0),
            .origin = spriteComp->origin,
            .rot = 0.0f,
            .scale = spriteComp->scale,
            .alpha = spriteComp->alpha
        };

        zf4_write_to_sprite_batch(&scene->renderer, 0, &sbWriteInfo);

        if (spriteComp->postwrite) {
            spriteComp->postwrite(entID, scene);
        }
    }
}
