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
