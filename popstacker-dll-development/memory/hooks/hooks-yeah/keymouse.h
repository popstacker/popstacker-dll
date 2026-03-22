#pragma once
#include "../FuncHook.h"

class KeyMouseHook : public FuncHook {
private:
    using Func = void(__fastcall*)(__int64, char, char, __int16, __int16, __int16, __int16, char);
    static inline Func original = nullptr;

    static void callback(__int64 a1, char button, char isDown, __int16 x, __int16 y, __int16 relX, __int16 relY, char a8) {
        Game::keyMousePtr = reinterpret_cast<void*>(a1 + 0x10);

        EventManager::get().onMouseInput(
            static_cast<MouseButton>(button),
            static_cast<bool>(isDown),
            Vec2<float>(static_cast<float>(x), static_cast<float>(y)), button
        );

        original(a1, button, isDown, x, y, relX, relY, a8);
    }

public:
    static void sendClick(MouseButton button, MouseAction action, const Vec2<float>& pos = { 0, 0 }) {
        original(0,
            static_cast<char>(button),
            static_cast<char>(action),
            static_cast<__int16>(pos.x),
            static_cast<__int16>(pos.y),
            0, 0, 0
        );
    }

    KeyMouseHook() {
        OriginFunc = reinterpret_cast<void*>(&original);
        func = reinterpret_cast<void*>(&callback);
    }
};