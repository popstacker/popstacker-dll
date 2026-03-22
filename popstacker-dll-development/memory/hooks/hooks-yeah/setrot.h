#pragma once
#include "../funchook.h"
#include "../../../main/features/combat/killaura.h"

class ActorSetRotHook : public FuncHook {
private:
    using func_t = float* (__fastcall*)(__int64, __int64, Actor*);
    static inline func_t oFunc = nullptr;

    static float* __fastcall ActorSetRotCallback(__int64 a1, __int64 a2, Actor* actor) {
        if (!actor || !oFunc) return nullptr;

        LocalPlayer* localPlayer = Game::getLocalPlayer();
        float* result = oFunc(a1, a2, actor);

        if (localPlayer && actor == localPlayer) {
            if (EventManager::get().getFeature<KillAura>()->isEnabled()) {
                // for when we have rotations
            }
        }

        return result;
    }

public:
    ActorSetRotHook() noexcept {
        OriginFunc = reinterpret_cast<void*>(&oFunc);
        func = reinterpret_cast<void*>(&ActorSetRotCallback);
    }
};
