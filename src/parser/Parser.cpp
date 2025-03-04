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

#include "opal/parser/Parser.hpp"

#include "opal/lexer/Token.hpp"
#include "opal/parser/atomizer/AtomizerBase.hpp"
#include "opal/parser/atomizer/AtomizerFactory.hpp"

#include <vector>

using namespace opal;

Token Parser::peek() const {
    return _tokens[_current];
}

bool Parser::isAtEnd() const {
    return _current >= _tokens.size() || _tokens[_current].type == TokenType::EOF_TOKEN;
}

Parser::Parser(std::vector<Token> tokens) : _tokens(tokens) {
    _atomizers = AtomizerFactory::createAtomizers(_current, _tokens);

    while (!this->isAtEnd()) {
        bool handled = false;
        for (const std::unique_ptr<AtomizerBase>& atomizer : _atomizers) {
            if (atomizer->canHandle(this->peek().type)) {
                std::unique_ptr<NodeBase> node = atomizer->atomize();
                if (node) {
                    _nodes.push_back(std::move(node));
                }
                handled = true;
                break;
            }
        }

        if (!handled) {
            _current++;
        }
    }
}

void Parser::printAST() const {
    for (const std::unique_ptr<NodeBase>& node : _nodes) {
        node->print();
    }
}
