#include "killaura.h"

KillAura::KillAura() : FeatureBase("KillAura", FeatureCategory::Combat) {}

void KillAura::onNormalActorTick(Actor* actor) {
	if (!actor) return;
	if (actor == Game::getLocalPlayer()) return;
	if (actor->getActorTypeComponent() && actor->getActorTypeComponent()->id == 319) {
		if (actor->getPos().distanceTo(Game::getLocalPlayer()->getPos()) > 7.1f)
		targetlist.push_back(actor);
	}
}

void KillAura::onNormalLocalPlayerTick(LocalPlayer* localPlayer) {
	if (!localPlayer) return;

	targetlist.clear();

	for (auto actor : Game::getLocalPlayer()->getlevel()->getRuntimeActorList()) {
		onNormalActorTick(actor);
	}

	for (auto actor : targetlist) {
		localPlayer->getgameMode()->attack(actor);
		localPlayer->swing();
		localPlayer->getrotation()->presentRot.y = 0.f;
	}
}