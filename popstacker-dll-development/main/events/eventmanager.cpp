#include "EventManager.h"
#include "../features/misc/example.h"
#include "../features/visual/gui.h"
#include "../features/combat/killaura.h"

EventManager& EventManager::get() {
    static EventManager instance;
    return instance;
}

void EventManager::initialize() {
    m_features.emplace_back(std::make_unique<Example>());
    m_features.emplace_back(std::make_unique<KillAura>());

    auto clickGUI = std::make_unique<GUI>();
    clickGUI->initialize();
    m_features.emplace_back(std::move(clickGUI));
}

void EventManager::onDLLTick() {
    for (auto& f : m_features)
        if (f->isEnabled())
            f->onDLLTick();
}

void EventManager::onNormalLocalPlayerTick(LocalPlayer* localPlayer) {
    for (auto& f : m_features)
        if (f->isEnabled())
            f->onNormalLocalPlayerTick(localPlayer);
}

void EventManager::onNormalActorTick(Actor* actor) {
    for (auto& f : m_features)
        if (f->isEnabled())
            f->onNormalActorTick(actor);
}

void EventManager::onKeyPress(uint64_t key, bool isDown) {
    for (auto& f : m_features)
        if (f->isEnabled())
            f->onKeyPress(key, isDown);
}

void EventManager::onMouseInput(MouseButton button, bool isDown, const Vec2<float>& pos, char MouseButton) {
    for (auto& f : m_features)
        if (f->isEnabled())
            f->onMouseInput(button, isDown, pos, MouseButton);
}

void EventManager::onDirect2DRender() {
    for (auto& f : m_features)
        if (f->isEnabled())
            f->onDirect2DRender();
}

std::vector<FeatureBase*>& EventManager::getFeatures() {
    static std::vector<FeatureBase*> featurePtrs;
    featurePtrs.clear();
    for (auto& f : m_features)
        featurePtrs.push_back(f.get());
    return featurePtrs;
}