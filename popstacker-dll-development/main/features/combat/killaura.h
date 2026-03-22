#pragma once
#include "../base.h"

class KillAura : public FeatureBase {
public:
	KillAura();

	void onNormalLocalPlayerTick(LocalPlayer* localPlayer) override;
	void onNormalActorTick(Actor* actor) override;

	std::vector<Actor*> targetlist;
};