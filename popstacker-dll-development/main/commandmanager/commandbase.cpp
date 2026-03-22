#include "CommandBase.h"
#include <utility>

CommandBase::CommandBase(std::string n, std::string desc, std::string u, std::vector<std::string> a)
    : name(std::move(n)), description(std::move(desc)), usage(std::move(u)), aliases(std::move(a)) {
}

CommandBase::~CommandBase() = default;