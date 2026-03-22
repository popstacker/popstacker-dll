#pragma once
#include "../../features/base.h"
#include "../../../misc/direct2d.h"

class GUI : public FeatureBase {
private:
    struct ClickWindow {
        std::string name;
        FeatureCategory category;
        Vec2<float> pos;
        bool extended = true;
        float selectedAnim = 0.f;
        std::vector<FeatureBase*> featureList;

        ClickWindow(std::string windowName, Vec2<float> startPos, FeatureCategory c);
    };

    struct StarParticle {
        Vec2<float> pos;
        float size;
        float speed;
        float angleOffset;
        float alpha;
        float life;
        float maxLife;
        float fadeIn;
        float fadeOut;
    };

    std::vector<ClickWindow*> windowList;
    float openAnim = 0.0f;
    bool initialized = false;

    Vec2<float> mousePos = Vec2<float>(0.f, 0.f);
    Vec2<float> mouseDelta = Vec2<float>(0.f, 0.f);

    bool isLeftClickDown = false;
    bool isHoldingLeftClick = false;
    bool isRightClickDown = false;
    bool isHoldingRightClick = false;

    ClickWindow* draggingWindowPtr = nullptr;
    FeatureBase* capturingKeybindFeaturePtr = nullptr;

    void updateSelectedAnimRect(const Vec4<float>& rect, float& anim);

    Color mainColor = Color(175, 0, 0, 175);
    float blurStrength = 5.f;
    bool showDescription = true;

public:
    GUI();
    ~GUI();

    void onKeyPress(uint64_t key, bool isDown) override;
    void onMouseInput(MouseButton button, bool isDown, const Vec2<float>& pos, char mouseButton) override;
    void onDirect2DRender() override;
    void onEnable() override;
    void onDisable() override;

    void initialize();
    void renderParticles();
};