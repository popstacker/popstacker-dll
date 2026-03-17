#pragma once
#include <string>

enum class FeatureCategory {
    Client
};

class FeatureBase {
public:
    FeatureBase(std::string name, FeatureCategory category) noexcept;
    virtual ~FeatureBase();

    const std::string& getName() const noexcept;

    virtual void onDLLTick();

private:
    std::string m_name;
    FeatureCategory m_category;
};