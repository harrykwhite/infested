#include "component_types.h"

#include <stdalign.h>

static void load_sprite_component_defaults(void* comp) {
    SpriteComponent* spriteComp = comp;
    spriteComp->origin = (ZF4Vec2D) {0.5f, 0.5f};
    spriteComp->scale = (ZF4Vec2D) {1.0f, 1.0f};
    spriteComp->alpha = 1.0f;
}

void load_component_type_info(ZF4ComponentTypeInfo* typeInfo, int typeIndex) {
    switch (typeIndex) {
        case VELOCITY_COMPONENT:
            typeInfo->size = sizeof(VelocityComponent);
            typeInfo->alignment = alignof(VelocityComponent);
            break;

        case SPRITE_COMPONENT:
            typeInfo->size = sizeof(SpriteComponent);
            typeInfo->alignment = alignof(SpriteComponent);
            typeInfo->defaultsLoader = load_sprite_component_defaults;
            break;

        case ACTOR_COMPONENT:
            typeInfo->size = sizeof(ActorComponent);
            typeInfo->alignment = alignof(ActorComponent);
            break;

        case GUN_COMPONENT:
            typeInfo->size = sizeof(GunComponent);
            typeInfo->alignment = alignof(GunComponent);
            break;

        case HITBOX_COMPONENT:
            typeInfo->size = sizeof(HitboxComponent);
            typeInfo->alignment = alignof(HitboxComponent);
            break;

        case DMG_TEXT_COMPONENT:
            typeInfo->size = sizeof(DmgTextComponent);
            typeInfo->alignment = alignof(DmgTextComponent);
            break;
    }
}
