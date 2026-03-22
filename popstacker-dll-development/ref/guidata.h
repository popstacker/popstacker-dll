#pragma once
#include <set>
#include "../misc/math.h"

class GuiData {
private:
    char pad_0x0[0x30]; // 0x0
public:
    Vec2<float> windowSizeReal; // 0x30
    Vec2<float> windowSizeReal2; // 0x38
    Vec2<float> windowSizeScaled; // 0x40
    float mGuiScale;             // 0x0034
    float mInvGuiScale;          // 0x0038
    bool mIsCurrentlyActive;     // 0x003c
    std::set<int> mPostedErrors; // 0x0040
    int16_t mMenuPointer;        // 0x0050
    int16_t mPointerX;           // 0x0052
    int16_t mPointerY;           // 0x0054
    bool mHasShowPreexistingMessages; // 0x0056
    bool mToolbarWasRendered;          // 0x0057
    int32_t mPrevSelectedSlot;         // 0x0058
    int32_t mPrevSelectedInventoryContainer; // 0x005c
    int32_t mNumSlots;                  // 0x0060
    int32_t mFlashSlotId;               // 0x0064
    double mFlashSlotStartTime;         // 0x0068
};