#include "base.h"
#include <utility>

FeatureBase::FeatureBase(std::string name, FeatureCategory category, uint64_t keybind) noexcept
    : m_name(std::move(name)), m_category(category), m_keybind(keybind) {
}

FeatureBase::~FeatureBase() = default;

const std::string& FeatureBase::getName() const noexcept {
    return m_name;
}

void FeatureBase::onDLLTick() {}
void FeatureBase::onNormalLocalPlayerTick(LocalPlayer* localPlayer) {}
void FeatureBase::onNormalActorTick(Actor* actor) {}
void FeatureBase::onKeyPress(uint64_t key, bool isDown) {}
void FeatureBase::onMouseInput(MouseButton button, bool isDown, const Vec2<float>& pos, char MouseButton) {}
void FeatureBase::onDirect2DRender() {}
void FeatureBase::onEnable() {}
void FeatureBase::onDisable() {}

bool FeatureBase::isEnabled() const noexcept {
    return m_enabled;
}

void FeatureBase::setEnabled(bool enabled) noexcept {
    if (m_enabled == enabled) return;

    m_enabled = enabled;

    if (m_enabled) {
        onEnable();
    }
    else {
        onDisable(); 
    }
}

void FeatureBase::toggle() noexcept {
    setEnabled(!m_enabled);
}

uint64_t FeatureBase::getKeybind() const noexcept {
    return m_keybind;
}

void FeatureBase::setKeybind(uint64_t key) noexcept {
    m_keybind = key;
}