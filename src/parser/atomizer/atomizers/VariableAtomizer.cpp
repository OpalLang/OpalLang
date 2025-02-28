#include "VariableAtomizer.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace Opal {

VariableAtomizer::VariableAtomizer(int& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool VariableAtomizer::canHandle(TokenType type) const {
    return type == TokenType::IDENTIFIER;
}

void VariableAtomizer::atomize() {
    if (peekNext().type == TokenType::EQUAL) {
        advance();
        if (peekNext().type == TokenType::NUMBER || peekNext().type == TokenType::STRING
            || peekNext().type == TokenType::TRUE || peekNext().type == TokenType::FALSE
            || peekNext().type == TokenType::NIL) {
            auto assignment = advance();
            std::cout << "Variable: " << tokens[current - 2].value << " = " << assignment.value << std::endl;
        } else {
            throw std::runtime_error("Expected a value after the assignment operator");
        }
    } else {
        advance();
    }
}

}  // namespace Opal
