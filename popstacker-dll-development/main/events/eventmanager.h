#pragma once
#include <vector>
#include <memory>
#include "../features/base.h"

class EventManager {
public:
    static EventManager& get();

    void initialize();
    void onDLLTick();
    void onNormalLocalPlayerTick(LocalPlayer* localPlayer);
    void onNormalActorTick(Actor* actor);
    void onKeyPress(uint64_t key, bool isDown);
    void onMouseInput(MouseButton button, bool isDown, const Vec2<float>& pos, char MouseButton);
    void onDirect2DRender();

    std::vector<FeatureBase*>& getFeatures();

    template <typename T>
    T* getFeature() {
        for (auto& f : m_features) {
            if (auto casted = dynamic_cast<T*>(f.get()))
                return casted;
        }
        return nullptr;
    }
private:
    EventManager() = default;

    std::vector<std::unique_ptr<FeatureBase>> m_features;
};