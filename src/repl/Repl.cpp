#include "Repl.hpp"
#include "../lexer/Lexer.hpp"

#include <iostream>
#include <string>

namespace Opal {

void Repl::start() {
    runPrompt();
}

void Repl::run(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.scanTokens();

    std::cout << "Tokenizing source code" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    lexer.printTokens();
    std::cout << "----------------------------------------" << std::endl;
}

void Repl::runPrompt() {
    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        run(line);
    }
}

}