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