#pragma once
#include "../base.h"

class Example : public FeatureBase {
public:
    Example();

    void onDLLTick() override;
    void onNormalActorTick(Actor* actor) override;
};