#pragma once
#include <string>
#include <vector>
#include <functional>

class CommandManager {
public:
    static inline char prefix = '.';

    static void init();
    static void execute(const std::string& message);
    static void addCommand(const std::string& name, std::function<void(const std::vector<std::string>&)> callback);

private:
    static inline std::vector<std::pair<std::string, std::function<void(const std::vector<std::string>&)>>> commands;
};