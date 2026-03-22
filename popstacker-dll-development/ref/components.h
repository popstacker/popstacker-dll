#pragma once
#include "../misc/math.h"
#include <unordered_map>
#include <string>

class ActorRuntimeID {
public:
    uint64_t id;
};

struct RuntimeIDComponent {
    ActorRuntimeID runtimeId;
};

struct ActorHeadRotationComponent {
    float headYaw;
    float headYawPrev;
};

struct IEntityComponent {};

class ActorOwnerComponent : IEntityComponent {
public:
    class Actor* actor;
};

struct ActorRotationComponent {
    Vec2<float> presentRot;
    Vec2<float> prevRot;
};

struct ActorTypeComponent {
    uint32_t id;
};

struct AnimationComponent {
};

struct DimensionTypeComponent {
    int type;
};

template<typename T>
class FlagComponent : IEntityComponent {};

// struct OnGroundFlag; doesnt exist in 1.21.2
struct OnGroundFlagComponent : IEntityComponent {};

struct JumpFromGroundRequestFlag {};
struct PlayerComponentFlag {};
struct HorizontalCollisionFlag {};
struct InWaterFlag {};
struct InLavaFlag {};

struct MobBodyRotationComponent {
    float bodyYaw;
    float bodyYawPrev;
};

struct MoveInputComponent {
    bool sneakingPrev;

private:
    char pad0x1[0x5];

public:
    bool jumpingPrev;
    bool sprintingPrev;

private:
    char pad0x8[0x2];

public:
    bool forwardPrev;
    bool backwardPrev;
    bool leftPrev;
    bool rightPrev;

private:
    char pad0xE[0x12];

public:
    bool sneaking;

private:
    char pad0x21[0x5];

public:
    bool jumping;
    bool sprinting;

private:
    char pad0x28[0x2];

public:
    bool forward;
    bool backward;
    bool left;
    bool right;

private:
    char pad0x2E[0x1A];

public:
    float sideMovement;
    float forwardMovement;

private:
    char pad0x50[0x38];

public:
    bool isPressed() const {
        return forward || backward || left || right;
    };
};

struct RenderPositionComponent {
    Vec3<float> eyePos;
};

struct StateVectorComponent {
    Vec3<float> pos;
    Vec3<float> oldPos;
    Vec3<float> velocity;
};