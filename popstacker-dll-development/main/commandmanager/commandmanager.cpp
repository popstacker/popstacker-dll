#include "CommandManager.h"
#include "commands/bind.h"

void CommandManager::init() {
    static BindCommand bindCommand;
    addCommand("bind", [](const std::vector<std::string>& args) {
        bindCommand.execute(args);
        });
}

void CommandManager::addCommand(const std::string& name, std::function<void(const std::vector<std::string>&)> callback) {
    commands.emplace_back(name, callback);
}

void CommandManager::execute(const std::string& message) {
    if (message.empty() || message[0] != prefix) return;

    size_t firstSpace = message.find(' ');
    std::string cmdName = message.substr(1, firstSpace - 1);
    std::vector<std::string> args;

    if (firstSpace != std::string::npos) {
        std::string argStr = message.substr(firstSpace + 1);
        size_t pos = 0;
        while ((pos = argStr.find(' ')) != std::string::npos) {
            args.push_back(argStr.substr(0, pos));
            argStr.erase(0, pos + 1);
        }
        if (!argStr.empty()) args.push_back(argStr);
    }

    args.insert(args.begin(), cmdName);

    for (auto& cmd : commands) {
        if (cmd.first == cmdName) {
            cmd.second(args);
            break;
        }
    }
}