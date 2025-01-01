#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

#include <zf4.h>

typedef enum {
    VELOCITY_COMPONENT,
    SPRITE_COMPONENT,
    ACTOR_COMPONENT,
    GUN_COMPONENT,
    DAMAGER_COMPONENT,
    HITBOX_COMPONENT,

    COMPONENT_TYPE_CNT
} ComponentType;

typedef struct {
    ZF4Vec2D vel;
    ZF4Vec2D velLerpTarg;
    float velLerpFactor;
} VelocityComponent;

typedef struct {
    int spriteIndex;
    ZF4Vec2D origin;
    float rot;
    ZF4Vec2D scale;
    float alpha;

    void (*prewrite)(ZF4EntID entID, ZF4Scene* scene);
    void (*postwrite)(ZF4EntID entID, ZF4Scene* scene);
} SpriteComponent;

typedef struct {
    int hp;
    int invTime;
    int invTimeMax;
} ActorComponent;

typedef struct {
    ZF4EntID ownerEntID;

    int shootTime;
    int shootInterval;
} GunComponent;

typedef struct {
    int dmg;
    ZF4Vec2D force;
    bool (*willCollideWith)(const ZF4EntID entID, const ZF4Scene* const scene);
} DamagerComponent;

typedef struct {
    int dmg;
    ZF4Vec2D force; // NOTE: Could be made into strength, if we want direction to be calculated based on collision relative to the hitbox.
    ZF4RectF collider;
} HitboxComponent;

void load_component_type_info(ZF4ComponentTypeInfo* typeInfo, int typeIndex);

#endif
