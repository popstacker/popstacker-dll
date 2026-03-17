#include "base.h"
#include <utility>

FeatureBase::FeatureBase(std::string name, FeatureCategory category) noexcept
    : m_name(std::move(name)), m_category(category) {
}

FeatureBase::~FeatureBase() = default;

const std::string& FeatureBase::getName() const noexcept {
    return m_name;
}

void FeatureBase::onDLLTick() {}