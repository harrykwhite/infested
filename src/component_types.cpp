#include "component_types.h"

static void load_sprite_component_defaults(zf4::Byte* const comp) {
    const auto spriteComp = reinterpret_cast<SpriteComponent*>(comp);
    spriteComp->origin = {0.5f, 0.5f};
    spriteComp->scale = {1.0f, 1.0f};
    spriteComp->alpha = 1.0f;
}

void load_component_type_info(zf4::ComponentTypeInfo* typeInfo, int typeIndex) {
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

        case DAMAGER_COMPONENT:
            typeInfo->size = sizeof(DamagerComponent);
            typeInfo->alignment = alignof(DamagerComponent);
            break;
    }
}
