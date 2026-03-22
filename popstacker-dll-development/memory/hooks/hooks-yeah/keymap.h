#pragma once
#include "../FuncHook.h"
#include "../../../main/features/visual/gui.h"

class KeyMapHook : public FuncHook {
private:
    using Func = __int64(__fastcall*)(uint64_t, bool);
    static inline Func original = nullptr;

    static __int64 callback(uint64_t key, bool isDown) {
        if (!isDown)
            return original(key, isDown);

        auto gui = EventManager::get().getFeature<GUI>();
        if (gui && key == gui->getKeybind()) {
            gui->toggle();
        }

        for (auto& f : EventManager::get().getFeatures()) {
            if (f == gui)
                continue;

            if (key == f->getKeybind() && Game::canMove()) {
                f->toggle();
            }
        }

        EventManager::get().onKeyPress(key, isDown);

        return original(key, isDown);
    }

public:
    static void createKeyPress(uint64_t key, bool down) {
        original(key, down);
    }

    static inline void sendKeyDown(uint64_t key) {
        original(key, true);
    }

    static inline void sendKeyUp(uint64_t key) {
        original(key, false);
    }

    static inline void sendKeyCode(uint64_t key) {
        original(key, true);
        original(key, false);
    }

    KeyMapHook() {
        OriginFunc = reinterpret_cast<void*>(&original);
        func = reinterpret_cast<void*>(&callback);
    }

    void onHookRequest() override {
        uintptr_t sigOffset = address + 7;
        int offset = *reinterpret_cast<int*>(sigOffset + 3);
        Game::keyMapPtr = reinterpret_cast<void*>(sigOffset + offset + 7);
    }
};