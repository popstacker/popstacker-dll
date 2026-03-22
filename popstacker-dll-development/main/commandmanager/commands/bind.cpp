#include "bind.h"
#include "../../events/eventmanager.h"
#include <algorithm>
#include <cctype>

BindCommand::BindCommand()
    : CommandBase("bind", "Bind a feature to a key", "<feature> <key>", { "b" }) {
}

bool BindCommand::execute(const std::vector<std::string>& args) {
    if (args.size() < 3) return false;

    std::string featureName = args[1];
    FeatureBase* feature = nullptr;

    for (auto& f : EventManager::get().getFeatures()) {
        std::string name = f->getName();
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name == featureName) {
            feature = f;
            break;
        }
    }

    if (!feature) {
        Game::getLocalPlayer()->displayClientMessage("Feature '" + featureName + "' not found");
        return true;
    }

    std::string keyStr = args[2];
    if (keyStr.size() == 1) {
        int keyCode = static_cast<int>(toupper(keyStr[0]));
        feature->setKeybind(keyCode);
        Game::getLocalPlayer()->displayClientMessage("Feature '" + feature->getName() + "' bound to '" + static_cast<char>(keyCode) + "'");
    }
    else if (keyStr == "none") {
        feature->setKeybind(0);
        Game::getLocalPlayer()->displayClientMessage("Keybind for '" + feature->getName() + "' removed");
    }
    else {
        Game::getLocalPlayer()->displayClientMessage("Invalid key '" + keyStr + "'");
    }

    return true;
}