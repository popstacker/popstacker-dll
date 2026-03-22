#pragma once
#include <cstdint>
#include <cstdarg>
#include <string>
#include "../memory/memoryutil.h"
#include "../misc/math.h"
#include "clientinstance.h"
#include "actor.h"

namespace Game {
    inline ClientInstance* clientInstance = nullptr;
    inline void* keyMapPtr = nullptr;
    inline void* keyMousePtr = nullptr;
    inline int viewPerspectiveMode = 0;

    inline ClientInstance* getClientInstance() {
        return clientInstance;
    }

    inline bool isPressingKey(uint32_t key) {
        if (!keyMapPtr) return false;
        return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(keyMapPtr) + key * 0x4);
    }

    inline bool isLeftClicking() {
        if (!keyMousePtr) return false;
        return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(keyMousePtr) + 1);
    }

    inline bool isRightClicking() {
        if (!keyMousePtr) return false;
        return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(keyMousePtr) + 2);
    }

    inline bool canMove() {
        return clientInstance ? clientInstance->mcGame->mouseGrabbed : false;
    }

    inline LocalPlayer* getLocalPlayer() {
        return clientInstance->getLocalPlayer();
    }
}