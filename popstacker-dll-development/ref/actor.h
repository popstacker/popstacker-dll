#pragma once
#include "../memory/memoryutil.h"
#include "../lib/entt/entt.hpp"
#include "level.h"
#include "components.h"
#include <concepts>
#include <cstdint>
#include "gamemode.h"

struct EntityId {
    std::uint32_t rawId;

    [[nodiscard]] constexpr bool operator==(const EntityId& other) const = default;

    [[nodiscard]] constexpr operator std::uint32_t() const {
        return this->rawId;
    }
};

struct EntityIdTraits {
    using value_type = EntityId;

    using entity_type = uint32_t;
    using version_type = uint16_t;

    static constexpr entity_type entity_mask = 0x3FFFF;
    static constexpr version_type version_mask = 0x3FFF;
};

template <typename Type>
struct entt::storage_type<Type, EntityId> {
    using type = basic_storage<Type, EntityId>;
};

template<>
struct entt::entt_traits<EntityId> : entt::basic_entt_traits<EntityIdTraits> {
    static constexpr std::size_t page_size = 2048;
};

struct EntityRegistry : std::enable_shared_from_this<EntityRegistry> {
    std::string name;
    entt::basic_registry<EntityId> registry;
    uint32_t id;
};

struct EntityContext {
    EntityRegistry& registry;
    entt::basic_registry<EntityId>& enttRegistry;
    EntityId entity;

    template<typename T>
    [[nodiscard]] T* tryGetComponent() {
        return this->enttRegistry.try_get<T>(this->entity);
    }

    template<typename T>
    [[nodiscard]] const T* tryGetComponent() const {
        return this->enttRegistry.try_get<T>(this->entity);
    }

    template<typename T>
    [[nodiscard]] bool hasComponent() const {
        return this->enttRegistry.all_of<T>(this->entity);
    }

    template<typename T>
    void getOrAddComponent() { //ye lmao
        return this->enttRegistry.get_or_emplace<T>(this->entity);
    }

    template<typename T>
    void removeComponent() {
        this->enttRegistry.remove<T>(this->entity);
    }
};

enum class ActorFlags : int {
    Unknown = -1,
    Onfire = 0x0,
    Sneaking = 0x1,
    Riding = 0x2,
    Sprinting = 0x3,
    Usingitem = 0x4,
    Invisible = 0x5,
    Tempted = 0x6,
    Inlove = 0x7,
    Saddled = 0x8,
    Powered = 0x9,
    Ignited = 0xA,
    Baby = 0xB,
    Converting = 0xC,
    Critical = 0xD,
    CanShowName = 0xE,
    AlwaysShowName = 0xF,
    Noai = 0x10,
    Silent = 0x11,
    Wallclimbing = 0x12,
    Canclimb = 0x13,
    Canswim = 0x14,
    Canfly = 0x15,
    Canwalk = 0x16,
    Resting = 0x17,
    Sitting = 0x18,
    Angry = 0x19,
    Interested = 0x1A,
    Charged = 0x1B,
    Tamed = 0x1C,
    Orphaned = 0x1D,
    Leashed = 0x1E,
    Sheared = 0x1F,
    Gliding = 0x20,
    Elder = 0x21,
    Moving = 0x22,
    Breathing = 0x23,
    Chested = 0x24,
    Stackable = 0x25,
    ShowBottom = 0x26,
    Standing = 0x27,
    Shaking = 0x28,
    Idling = 0x29,
    Casting = 0x2A,
    Charging = 0x2B,
    WasdControlled = 0x2C,
    CanPowerJump = 0x2D,
    CanDash = 0x2E,
    Lingering = 0x2F,
    HasCollision = 0x30,
    HasGravity = 0x31,
    FireImmune = 0x32,
    Dancing = 0x33,
    Enchanted = 0x34,
    Returntrident = 0x35,
    ContainerIsPrivate = 0x36,
    IsTransforming = 0x37,
    Damagenearbymobs = 0x38,
    Swimming = 0x39,
    Bribed = 0x3A,
    IsPregnant = 0x3B,
    LayingEgg = 0x3C,
    PassengerCanPick = 0x3D,
    TransitionSitting = 0x3E,
    Eating = 0x3F,
    LayingDown = 0x40,
    Sneezing = 0x41,
    Trusting = 0x42,
    Rolling = 0x43,
    Scared = 0x44,
    InScaffolding = 0x45,
    OverScaffolding = 0x46,
    DescendThroughBlock = 0x47,
    Blocking = 0x48,
    TransitionBlocking = 0x49,
    BlockedUsingShield = 0x4A,
    BlockedUsingDamagedShield = 0x4B,
    Sleeping = 0x4C,
    WantsToWake = 0x4D,
    TradeInterest = 0x4E,
    DoorBreaker = 0x4F,
    BreakingObstruction = 0x50,
    DoorOpener = 0x51,
    IsIllagerCaptain = 0x52,
    Stunned = 0x53,
    Roaring = 0x54,
    DelayedAttack = 0x55,
    IsAvoidingMobs = 0x56,
    IsAvoidingBlock = 0x57,
    FacingTargetToRangeAttack = 0x58,
    HiddenWhenInvisible = 0x59,
    IsInUi = 0x5A,
    Stalking = 0x5B,
    Emoting = 0x5C,
    Celebrating = 0x5D,
    Admiring = 0x5E,
    CelebratingSpecial = 0x5F,
    OutOfControl = 0x60,
    RamAttack = 0x61,
    PlayingDead = 0x62,
    InAscendableBlock = 0x63,
    OverDescendableBlock = 0x64,
    Croaking = 0x65,
    EatMob = 0x66,
    JumpGoalJump = 0x67,
    Emerging = 0x68,
    Sniffing = 0x69,
    Digging = 0x6A,
    SonicBoom = 0x6B,
    HasDashCooldown = 0x6C,
    PushTowardsClosestSpace = 0x6D,
    Scenting = 0x6E,
    Rising = 0x6F,
    FeelingHappy = 0x70,
    Searching = 0x71,
    Count = 0x72,
};

class Actor {
public:
    CLASS_MEMBER(Level*, level, 0x250);
    CLASS_MEMBER(StateVectorComponent*, stateVector, 0x290);
    CLASS_MEMBER(ActorRotationComponent*, rotation, 0x2A0);
public:
    EntityContext entityContext;
public:
    RenderPositionComponent* getRenderPositionComponent() {
        using func_t = RenderPositionComponent * (__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(MemoryUtil::getFuncFromCall(MemoryUtil::findSignature("E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? F3 0F 10 85 ? ? ? ? F3 0F 11 00 F3")));
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    ActorTypeComponent* getActorTypeComponent() {
        using func_t = ActorTypeComponent * (__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(MemoryUtil::getFuncFromCall(MemoryUtil::findSignature("E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 8B 10 45")));
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    ActorHeadRotationComponent* getActorHeadRotationComponent() {
        using func_t = ActorHeadRotationComponent * (__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(MemoryUtil::getFuncFromCall(MemoryUtil::findSignature("E8 ? ? ? ? 48 85 C0 74 57 F3 0F")));
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));

        ActorHeadRotationComponent* comp = Func(a1, a2);
        if (!comp) {
            static ActorHeadRotationComponent fallbackComp;
            return &fallbackComp;
        }
        return comp;
    }

    MobBodyRotationComponent* getMobBodyRotationComponent() {
        using func_t = MobBodyRotationComponent * (__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(MemoryUtil::getFuncFromCall(MemoryUtil::findSignature("E8 ? ? ? ? 48 85 C0 74 2E F3 0F 10 10 49")));
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    Vec3<float> getPos() {
        if (stateVector) {
            return stateVector->pos;
        }
        return Vec3<float>{0.0f, 0.0f, 0.0f};
    }

    Vec3<float> getEyePos() {
        auto renderPosComp = getRenderPositionComponent();
        if (renderPosComp) {
            return renderPosComp->eyePos;
        }
        return Vec3<float>{0.0f, 0.0f, 0.0f};
    }

    void swing() {
        MemoryUtil::CallVFunc<117, void>(this);
    }
public:
    virtual bool getStatusFlag(ActorFlags flag);
    virtual void setStatusFlag(ActorFlags flag, bool value);
};

class Mob : public Actor {
public:
    void setSprinting(bool shouldSprint) {
        MemoryUtil::CallVFunc<153, void, bool>(this, shouldSprint);
    }
};

class Player : public Mob {
public:
    CLASS_MEMBER(GameMode*, gameMode, 0xEC8);
public:
    int getItemUseDuration() {
        return MemoryUtil::CallVFunc<162, int>(this);
    }
};

class LocalPlayer : public Player {
public:
    void displayClientMessage(const std::string& message) {
        MemoryUtil::CallVFunc<213, void, const std::string&>(this, message);
    }
};
