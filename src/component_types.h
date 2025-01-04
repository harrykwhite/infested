#pragma once

#include <zf4.h>

enum ComponentType {
    VELOCITY_COMPONENT,
    SPRITE_COMPONENT,
    ACTOR_COMPONENT,
    GUN_COMPONENT,
    DAMAGER_COMPONENT,

    COMPONENT_TYPE_CNT
};

enum VelocityComponentSolidEntCollisionEvent {
    VELOCITY_COMPONENT_SOLID_COLLISION_EVENT_STOP,
    VELOCITY_COMPONENT_SOLID_COLLISION_EVENT_DESTROY
};

struct VelocityComponent {
    zf4::Vec2D vel;
    zf4::Vec2D velLerpTarg;
    float velLerpFactor;
    VelocityComponentSolidEntCollisionEvent solidEntCollisionEvent;
};

struct SpriteComponent {
    int spriteIndex;
    zf4::Vec2D origin;
    float rot;
    zf4::Vec2D scale;
    float alpha;

    void (*prewrite)(zf4::EntID entID, zf4::Scene* scene);
    void (*postwrite)(zf4::EntID entID, zf4::Scene* scene);
};

struct ActorComponent {
    int hp;
    int invTime;
    int invTimeMax; // NOTE: Wasting space in the cache line?
};

struct GunComponent {
    zf4::EntID ownerEntID;

    int shootTime;
    int shootInterval;
};

enum DamagerComponentKnockbackDirType {
    DIR_FROM_OTHER_DAMAGER_COMPONENT_KNOCKBACK_DIR_TYPE,
    OTHER_VEL_DAMAGER_COMPONENT_KNOCKBACK_DIR_TYPE
};

struct DamagerComponent {
    int dmg;
    float strength;
    bool (*canDmgOtherEnt)(const zf4::EntID otherEntID, const zf4::EntityManager* const entManager);
    bool destroyOnDmg;
    DamagerComponentKnockbackDirType knockbackDirType;
};

void load_component_type_info(zf4::ComponentTypeInfo* typeInfo, int typeIndex);
