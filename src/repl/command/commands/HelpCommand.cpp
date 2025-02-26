#include "HelpCommand.hpp"

#include <iomanip>
#include <iostream>

namespace Opal {

const std::unordered_map<std::string, std::string> HelpCommand::commandDescriptions = {
    {"help", "Display available commands"},
    {"clear", "Clear the screen"},
    {"exit", "Exit the REPL"}};

void HelpCommand::execute() {
    std::cout << "Available commands:\n\n";

    size_t maxLength = 0;
    for (const auto& [cmd, _] : commandDescriptions) {
        maxLength = std::max(maxLength, cmd.length());
    }

    for (const auto& [cmd, desc] : commandDescriptions) {
        std::cout << "  " << std::left << std::setw(maxLength + 2) << cmd << desc << std::endl;
    }
    std::cout << std::endl;
}

}  // namespace Opal