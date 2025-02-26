#include "ExitCommand.hpp"

#include <iostream>

namespace Opal {

bool ExitCommand::canHandle(const std::string& commandName) const {
    return commandName == "exit";
}

void ExitCommand::execute() {
    std::cout << "Exiting the REPL" << std::endl;
    exit(0);
}

}  // namespace Opal