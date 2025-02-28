#include "OperationAtomizer.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace Opal {

OperationAtomizer::OperationAtomizer(int& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool OperationAtomizer::canHandle(TokenType type) const {
    return type == TokenType::PLUS || type == TokenType::MINUS ||
           type == TokenType::MULTIPLY || type == TokenType::DIVIDE ||
           type == TokenType::MODULO;
}

void OperationAtomizer::atomize() {
    
}

}  // namespace Opal