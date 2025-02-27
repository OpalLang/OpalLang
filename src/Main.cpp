#include "lexer/Lexer.hpp"
#include "repl/Repl.hpp"
#include "util/FileUtil.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::cout << "Opal Language" << std::endl;

    if (argc < 2) {
        Opal::Repl repl;

        try {
            repl.start();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

    if (!Opal::FileUtil::fileExists(argv[1])) {
        std::cerr << "File does not exist: " << argv[1] << std::endl;
        return 1;
    }

    try {
        std::string sourceCode = Opal::FileUtil::readFile(argv[1]);
        Opal::Lexer lexer(sourceCode);
        auto        tokens = lexer.scanTokens();

        std::cout << "Tokenizing file: " << argv[1] << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        lexer.printTokens();
        std::cout << "----------------------------------------" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
