#include "OperationAtomizer.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace Opal {

OperationAtomizer::OperationAtomizer(int& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool OperationAtomizer::canHandle(TokenType type) const {
    return type == TokenType::PLUS || type == TokenType::MINUS || type == TokenType::MULTIPLY
           || type == TokenType::DIVIDE || type == TokenType::MODULO;
}

void OperationAtomizer::atomize() {
    std::string expr = std::string(tokens[current].value);
    advance();

    while (current >= 0 && static_cast<size_t>(current) < tokens.size()) {
        if (!canHandle(tokens[current].type) || tokens[current].type == TokenType::EOF_TOKEN) {
            break;
        }
        auto op = tokens[current];
        advance();
        if (current >= 0 && static_cast<size_t>(current) < tokens.size()
            && (tokens[current].type == TokenType::NUMBER || tokens[current].type == TokenType::IDENTIFIER)) {
            expr += " " + std::string(op.value) + " " + std::string(tokens[current].value);
            advance();
        } else {
            throw std::runtime_error("Expected a number or identifier after operator");
        }
    }
    std::cout << expr << std::endl;
}

}  // namespace Opal