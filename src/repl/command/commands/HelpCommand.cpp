/* OpalLang
 * Copyright (C) 2025 OpalLang
 *
 * This software is free software; you can redistribute it and/or modify it under
 * the terms of the CeCILL-C license as published by CEA, CNRS, and Inria,
 * either version 1.0 of the License or (at your option) any later version.
 *
 * This software is distributed "as is," without any warranty of any kind,
 * either express or implied, including but not limited to the warranties of
 * merchantability or fitness for a particular purpose. See the CeCILL-C license
 * for more details.
 *
 * You should have received a copy of the CeCILL-C license along with this
 * program. If not, see https://cecill.info.
 *
 * Opal is a programming language designed with a focus on readability and
 * performance. It combines modern programming concepts with a clean syntax,
 * making it accessible to newcomers while providing the power and flexibility
 * needed for experienced developers.
 */

#include "opal/repl/command/commands/HelpCommand.hpp"

#include <iomanip>
#include <iostream>

namespace opal {

const std::unordered_map<std::string, std::string> HelpCommand::commandDescriptions = {
    {"help", "Display available commands"},
    {"clear", "Clear the screen"},
    {"exit", "Exit the REPL"}};

void HelpCommand::execute() {
    std::cout << "Available commands:\n\n";

    size_t maxLength = 0;
    for (const std::pair<const std::string, std::string>& cmd_desc : commandDescriptions) {
        maxLength = std::max(maxLength, cmd_desc.first.length());
    }

    for (const std::pair<const std::string, std::string>& cmd_desc : commandDescriptions) {
        std::cout << "  " << std::left << std::setw(maxLength + 2) << cmd_desc.first << cmd_desc.second << std::endl;
    }
    std::cout << std::endl;
}

}  // namespace opal