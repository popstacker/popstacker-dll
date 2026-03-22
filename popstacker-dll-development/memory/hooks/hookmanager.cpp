#include "HookManager.h"
#include "../../lib/kiero/kiero.h"
#include "../../lib/minhook/MinHook.h"

#include "hooks-yeah/clientinstanceupdate.h"
#include "hooks-yeah/present.h"
#include "hooks-yeah/normaltick.h"
#include "hooks-yeah/resizebuffers.h"
#include "hooks-yeah/keymap.h"
#include "hooks-yeah/keymouse.h"
#include "hooks-yeah/chatmessage.h"
#include "hooks-yeah/setrot.h"

void HookManager::init() {
    if (MH_Initialize() != MH_OK)
        return;

    RequestHook<ClientInstanceUpdateHook>(MemoryUtil::findSignature("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 0F"));
    RequestHook<KeyMapHook>(MemoryUtil::findSignature("48 ? ? 48 ? ? ? 4C 8D 05 ? ? ? ? 89"));
    RequestHook<KeyMouseHook>(MemoryUtil::findSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9"));
    RequestHook<SendChatMessageHook>(MemoryUtil::getFuncFromCall(MemoryUtil::findSignature("E8 ? ? ? ? 3C 01 75 44 48 8B 8B ? ? ? ? 48 8B")));

    if (auto vtable = (uintptr_t**)MemoryUtil::getVtableFromSig("48 8D 05 ? ? ? ? 48 89 01 B8 ? ? ? ? 8D 50 FA 44 8D 48 ? 44 8D 40 ? 66 89 44 ? ? E8 ? ? ? ? 48 8B 8B"))
        RequestHook<ActorNormalTickHook>(vtable, 26);

    RequestHook<ActorSetRotHook>("4C 8B DC 49 89 73 10 49 89 7B 20 55 49 8D 6B A1 48 81 EC ? ? ? ? 48 8B");

    uintptr_t** methodsTable = nullptr;

    if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success) {
        methodsTable = (uintptr_t**)kiero::getMethodsTable();
        RequestHook<PresentHook>(methodsTable, 140);
        RequestHook<ResizeBuffersHook>(methodsTable, 145);
    }
    else if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
        methodsTable = (uintptr_t**)kiero::getMethodsTable();
        RequestHook<PresentHook>(methodsTable, 8);
        RequestHook<ResizeBuffersHook>(methodsTable, 13);
    }
    else {
        return;
    }

    auto initHook = getHook<ClientInstanceUpdateHook>();
    if (!initHook || !initHook->enableHook())
        return;

    while (!Game::clientInstance)
        Sleep(250);

    for (auto* hook : hooksCache)
        hook->enableHook();

    for (auto* hook : hooksCache)
        delete hook;

    hooksCache.clear();
}

void HookManager::shutdown() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    kiero::shutdown();

    for (auto* hook : hooksCache)
        delete hook;

    hooksCache.clear();
}