#include "lexer/Lexer.hpp"
#include "repl/Repl.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

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

    try {
        std::string sourceCode = readFile(argv[1]);
        Opal::Lexer lexer(sourceCode);
        auto tokens = lexer.scanTokens();

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
