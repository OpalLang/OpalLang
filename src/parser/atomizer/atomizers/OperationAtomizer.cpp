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

#include "opal/parser/atomizer/atomizers/OperationAtomizer.hpp"
#include "opal/parser/node/NodeFactory.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <spdlog/spdlog.h>

namespace opal {

OperationAtomizer::OperationAtomizer(size_t& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool OperationAtomizer::canHandle(TokenType type) const {
    return type == TokenType::PLUS || type == TokenType::MINUS || type == TokenType::MULTIPLY
           || type == TokenType::DIVIDE || type == TokenType::MODULO;
}

bool isnumber(TokenType type) {
    return type == TokenType::NUMBER;
}

std::unique_ptr<NodeBase> OperationAtomizer::atomize() {
    std::vector<Token> operationTokens;

    operationTokens.push_back(tokens[current]);
    advance();

    while (current < tokens.size()) {
        if (!canHandle(tokens[current].type)) {
            break;
        }

        operationTokens.push_back(tokens[current]);
        advance();

        if (current < tokens.size()
            && (tokens[current].type == TokenType::NUMBER || tokens[current].type == TokenType::IDENTIFIER)) {
            operationTokens.push_back(tokens[current]);
            advance();
        } else {
            spdlog::error("Expected a number or identifier after operator");
            exit(1);
        }
    }
    return NodeFactory::createOperationNode(operationTokens);
}

}  // namespace opal