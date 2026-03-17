#include <Windows.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include "main/popstacker.h"
#include "main/events/eventmanager.h"

using namespace winrt;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;

static void setWindowTitle(const std::wstring& title) {
    auto window = CoreApplication::MainView().CoreWindow();
    window.Dispatcher().RunAsync(
        CoreDispatcherPriority::Normal,
        [t = title]() {
            ApplicationView::GetForCurrentView().Title(t);
        }
    );
}

static DWORD WINAPI entry(HMODULE module) {
    setWindowTitle(L"");

    popstacker::initialize();

    setWindowTitle(L"popstacker.dll");

    while (popstacker::isinitialized()) {
        EventManager::get().onDLLTick();
        Sleep(50);
    }

    Sleep(25);

    FreeLibraryAndExitThread(module, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH)
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)entry, module, 0, nullptr);

    return TRUE;
}