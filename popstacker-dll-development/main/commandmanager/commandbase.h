#pragma once
#include <string>
#include <vector>

class CommandBase {
public:
    std::string name;
    std::string description;
    std::string usage;
    std::vector<std::string> aliases;

    CommandBase(std::string n, std::string desc, std::string u, std::vector<std::string> a = {});
    virtual ~CommandBase();
    virtual bool execute(const std::vector<std::string>& args) = 0;
};