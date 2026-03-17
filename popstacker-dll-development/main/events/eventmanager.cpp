#include "EventManager.h"
#include "../features/example.h"

EventManager& EventManager::get() {
    static EventManager instance;
    return instance;
}

void EventManager::initialize() {
    m_features.emplace_back(std::make_unique<Example>());
}

void EventManager::onDLLTick() {
    for (auto& f : m_features)
        f->onDLLTick();
}