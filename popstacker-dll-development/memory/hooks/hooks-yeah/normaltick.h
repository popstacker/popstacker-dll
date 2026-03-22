#pragma once
#include "../funchook.h"

class ActorNormalTickHook : public FuncHook {
protected:
    static inline bool once = false;

private:
    using Func = void(__thiscall*)(Actor*);
    static inline Func original = nullptr;

    static void callback(Actor* self) {
        EventManager::get().onNormalActorTick(self);

        if (auto* local = Game::getLocalPlayer(); local == self)
            EventManager::get().onNormalLocalPlayerTick(local);

        original(self);
    }

public:
    ActorNormalTickHook() {
        OriginFunc = reinterpret_cast<void*>(&original);
        func = reinterpret_cast<void*>(&callback);
    }

    ~ActorNormalTickHook() {
        once = false;
    }
};