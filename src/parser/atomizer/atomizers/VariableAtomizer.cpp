#include "VariableAtomizer.hpp"
#include "OperationAtomizer.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace Opal {

VariableAtomizer::VariableAtomizer(int& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool VariableAtomizer::canHandle(TokenType type) const {
    return type == TokenType::IDENTIFIER;
}

void VariableAtomizer::atomize() {
    std::string variableName = std::string(tokens[current].value);
    advance();
    
    if (current >= 0 && static_cast<size_t>(current) < tokens.size() && 
        tokens[current].type == TokenType::EQUAL) {
        advance();
        
        if (current < 0 || static_cast<size_t>(current) >= tokens.size()) {
            throw std::runtime_error("Expected value after assignment operator");
        }

        std::cout << "Variable: " << variableName << " = ";
        
        if (tokens[current].type == TokenType::NUMBER || 
            tokens[current].type == TokenType::STRING ||
            tokens[current].type == TokenType::TRUE || 
            tokens[current].type == TokenType::FALSE ||
            tokens[current].type == TokenType::NIL || 
            tokens[current].type == TokenType::IDENTIFIER) {
            
            OperationAtomizer operationAtomizer(current, tokens);
            operationAtomizer.atomize();
        } else {
            throw std::runtime_error("Expected a value or identifier after assignment operator");
        }
    } else {
        std::cout << "Variable reference: " << variableName << std::endl;
    }
}

}  // namespace Opal
