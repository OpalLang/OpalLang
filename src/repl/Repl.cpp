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

#include "Repl.hpp"

#include "../lexer/Lexer.hpp"
#include "../parser/Parser.hpp"
#include "command/CommandFactory.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace Opal {

void Repl::start() {
    runPrompt();
}

void Repl::run(const std::string& source) {
    if (source.empty()) {
        return;
    }

    std::istringstream iss(source);
    std::string        commandName;
    iss >> commandName;

    std::vector<std::string> args;
    std::string              arg;
    while (iss >> arg) {
        args.push_back(arg);
    }

    auto commands = CommandFactory::createCommands();
    for (const auto& command : commands) {
        if (command->canHandle(commandName)) {
            command->setArguments(args);
            command->execute();
            return;
        }
    }

    Lexer lexer(source);
    auto  tokens = lexer.scanTokens();

    std::cout << "Tokenizing source code" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    lexer.printTokens();
    std::cout << "----------------------------------------" << std::endl;
    Opal::Parser parser(tokens);
    parser.printAST();
    std::cout << "----------------------------------------" << std::endl;
}

void Repl::runPrompt() {
    std::string line;
    while (true) {
        std::cout << "Opal > ";
        std::getline(std::cin, line);
        run(line);
    }
}

}  // namespace Opal