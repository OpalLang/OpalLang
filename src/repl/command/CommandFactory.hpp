#pragma once

#include "CommandBase.hpp"

#include <memory>
#include <vector>

namespace Opal {

class CommandFactory {
public:
    static std::vector<std::unique_ptr<CommandBase>> createCommands();
};

}  // namespace Opal