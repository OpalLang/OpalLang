#include "Repl.hpp"

#include "../lexer/Lexer.hpp"
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