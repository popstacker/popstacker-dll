#include "Example.h"
#include <Windows.h>

Example::Example()
    : FeatureBase("example", FeatureCategory::Misc, VK_SPACE) {
}

void Example::onDLLTick() {
    OutputDebugStringA("example tick\n");
}

void Example::onNormalActorTick(Actor* actor) {
    if (actor == Game::getLocalPlayer()) {
        actor->swing();
    }
}