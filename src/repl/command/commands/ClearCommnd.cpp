#include "ClearCommand.hpp"

#include <iostream>

namespace Opal {

bool ClearCommand::canHandle(const std::string& commandName) const {
    return commandName == "clear";
}

void ClearCommand::execute() {
    std::cout << "\033c";
}

}  // namespace Opal