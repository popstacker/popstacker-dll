#pragma once
#include <string>
#include "../../ref/game.h"

enum class FeatureCategory {
    Client,
    Render,
    World,
    Combat,
    Movement,
    Misc,
    Visual
};

class FeatureBase {
public:
    FeatureBase(std::string name, FeatureCategory category, uint64_t keybind = 0x0) noexcept;
    virtual ~FeatureBase();

    const std::string& getName() const noexcept;

    virtual void onDLLTick();
    virtual void onNormalLocalPlayerTick(LocalPlayer* localPlayer);
    virtual void onNormalActorTick(Actor* actor);
    virtual void onKeyPress(uint64_t key, bool isDown);
    virtual void onMouseInput(MouseButton button, bool isDown, const Vec2<float>& pos, char MouseButton);
    virtual void onDirect2DRender();
    virtual void onEnable();
    virtual void onDisable();

    bool isEnabled() const noexcept;
    void setEnabled(bool enabled) noexcept;
    void toggle() noexcept;

    uint64_t getKeybind() const noexcept;
    void setKeybind(uint64_t key) noexcept;
    FeatureCategory getCategory() const noexcept { return m_category; }
private:
    std::string m_name;
    FeatureCategory m_category;
    bool m_enabled = false;
    uint64_t m_keybind = 0;
public: // gui shi
    float selectedAnim = 0.f;
};