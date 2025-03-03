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

#include "Parser.hpp"

#include "../lexer/Token.hpp"
#include "atomizer/AtomizerBase.hpp"
#include "atomizer/AtomizerFactory.hpp"

#include <vector>

namespace Opal {

Token Parser::peek() const {
    return tokens[current];
}

bool Parser::isAtEnd() const {
    return current >= tokens.size() || tokens[current].type == TokenType::EOF_TOKEN;
}

Parser::Parser(std::vector<Token> tokens) : tokens(tokens) {
    atomizers = AtomizerFactory::createAtomizers(current, this->tokens);

    while (!isAtEnd()) {
        bool handled = false;
        for (const auto& atomizer : atomizers) {
            if (atomizer->canHandle(peek().type)) {
                auto node = atomizer->atomize();
                if (node) {
                    nodes.push_back(std::move(node));
                }
                handled = true;
                break;
            }
        }

        if (!handled) {
            current++;
        }
    }
}

void Parser::printAST() const {
    for (const auto& node : nodes) {
        node->print();
    }
}

}  // namespace Opal