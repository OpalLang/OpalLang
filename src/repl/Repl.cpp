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

#include "opal/repl/Repl.hpp"

#include "opal/lexer/Lexer.hpp"
#include "opal/parser/Parser.hpp"
#include "opal/repl/command/CommandFactory.hpp"

#include <spdlog/spdlog.h>

#include <iostream>
#include <sstream>
#include <string>

namespace opal {

void Repl::start() {
    signalManager.setupSignalHandlers();
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

    std::vector<std::unique_ptr<CommandBase>> commands = CommandFactory::createCommands();
    for (const std::unique_ptr<CommandBase>& command : commands) {
        if (command->canHandle(commandName)) {
            command->setArguments(args);
            command->execute();
            return;
        }
    }

    Lexer              lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    spdlog::info("Tokenizing source code");
    spdlog::info("----------------------------------------");
    lexer.printTokens();
    spdlog::info("----------------------------------------");
    Parser parser(tokens);
    parser.printAST();
    spdlog::info("----------------------------------------");
}

void Repl::runPrompt() {
    std::string line;

    while (!signalManager.shouldExit()) {
        std::cout << "Opal > ";

        if (signalManager.isInterruptRequested()) {
            signalManager.resetInterruptFlag();
            continue;
        }

        std::getline(std::cin, line);

        if (std::cin.eof()) {
            spdlog::info("Exiting REPL");
            break;
        }

        if (std::cin.fail()) {
            std::cin.clear();
            spdlog::info("Input error. Please try again.");
            continue;
        }

        run(line);
    }
}

}  // namespace opal