#include "VariableAtomizer.hpp"

#include "OperationAtomizer.hpp"
#include "../../../parser/node/NodeFactory.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

namespace Opal {

VariableAtomizer::VariableAtomizer(int& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool VariableAtomizer::canHandle(TokenType type) const {
    if (type != TokenType::IDENTIFIER)
        return false;

    size_t nextIndex = current + 1;
    if (nextIndex < tokens.size() && tokens[nextIndex].type == TokenType::LEFT_PAREN) {
        return false;
    }
    return true;
}

void VariableAtomizer::atomize() {
    std::string variableName = std::string(tokens[current].value);
    advance();

    if (current >= 0 && static_cast<size_t>(current) < tokens.size() && tokens[current].type == TokenType::EQUAL) {
        advance();

        if (current < 0 || static_cast<size_t>(current) >= tokens.size()) {
            throw std::runtime_error("Expected value after assignment operator");
        }

        std::string variableValue;
        bool isConst = false;

        if (current >= 3 && tokens[current-3].type == TokenType::CONST) {
            isConst = true;
        }

        if (tokens[current].type == TokenType::NUMBER || tokens[current].type == TokenType::STRING
            || tokens[current].type == TokenType::TRUE || tokens[current].type == TokenType::FALSE
            || tokens[current].type == TokenType::NIL || tokens[current].type == TokenType::IDENTIFIER) {

            variableValue = std::string(tokens[current].value);

            auto variableNode = NodeFactory::createVariableNode(variableName, variableValue, isConst);
            std::cout << "Created " << (isConst ? "const " : "") << "variable: " 
                      << variableNode->getName() << " = " << variableNode->getValue() << std::endl;

            int currentPos = current;
            advance();
        } else {
            throw std::runtime_error("Expected a value or identifier after assignment operator");
        }
    } else {
        auto variableNode = NodeFactory::createVariableNode(variableName, "", false);
        std::cout << "Variable reference: " << variableNode->getName() << std::endl;
    }
}

}  // namespace Opal
