#pragma once
#include <vector>
#include <memory>
#include "../features/base.h"

class EventManager {
public:
    static EventManager& get();

    void initialize();
    void onDLLTick();

private:
    EventManager() = default;

    std::vector<std::unique_ptr<FeatureBase>> m_features;
};