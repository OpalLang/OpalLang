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

#include "opal/parser/atomizer/atomizers/CallAtomizer.hpp"

#include "opal/util/ErrorUtil.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace opal;

CallAtomizer::CallAtomizer(size_t& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool CallAtomizer::canHandle(TokenType type) const {
    if (type != TokenType::IDENTIFIER)
        return false;

    size_t nextIndex = this->_current + 1;
    if (nextIndex < this->_tokens.size() && this->_tokens[nextIndex].type == TokenType::LEFT_PAREN) {
        return true;
    }
    return false;
}

std::unique_ptr<NodeBase> CallAtomizer::atomize() {
    if (this->_current + 1 >= this->_tokens.size()) {
        throw std::runtime_error(ErrorUtil::errorMessage("Unexpected end of input while parsing call",
                                                         this->_tokens[this->_current].line,
                                                         this->_tokens[this->_current].column));
    }

    std::string               callName        = std::string(this->_tokens[this->_current].value);
    Token                     identifierToken = this->_tokens[this->_current];
    std::unique_ptr<CallNode> callNode        = NodeFactory::createCallNode(TokenType::IDENTIFIER, callName);

    this->advance();

    if (this->_current >= this->_tokens.size() || this->_tokens[this->_current].type != TokenType::LEFT_PAREN) {
        throw std::runtime_error(ErrorUtil::errorMessage("Expected '(' after identifier name",
                                                         identifierToken.line,
                                                         identifierToken.column + callName.length()));
    }

    Token parenToken = this->_tokens[this->_current];
    processArguments(callNode, parenToken);

    return callNode;
}

void CallAtomizer::processArguments(std::unique_ptr<CallNode>& callNode, const Token& startToken) {
    this->advance();

    if (this->_current < this->_tokens.size() && this->_tokens[this->_current].type == TokenType::RIGHT_PAREN) {
        this->advance();
        return;
    }

    bool needsComma = false;

    while (this->_current < this->_tokens.size()) {
        if (this->_tokens[this->_current].type == TokenType::RIGHT_PAREN) {
            this->advance();
            return;
        }

        if (needsComma) {
            if (this->_tokens[this->_current].type != TokenType::COMMA) {
                throw std::runtime_error(ErrorUtil::errorMessage("Expected comma between arguments",
                                                                 this->_tokens[this->_current].line,
                                                                 this->_tokens[this->_current].column));
            }
            this->advance();
            if (this->_current < this->_tokens.size() && this->_tokens[this->_current].type == TokenType::RIGHT_PAREN) {
                throw std::runtime_error(ErrorUtil::errorMessage("Trailing comma is not allowed in function calls",
                                                                 this->_tokens[this->_current - 1].line,
                                                                 this->_tokens[this->_current - 1].column));
            }
            needsComma = false;
            continue;
        }

        if (this->_tokens[this->_current].type == TokenType::IDENTIFIER && this->_current + 1 < this->_tokens.size()
            && this->_tokens[this->_current + 1].type == TokenType::LEFT_PAREN) {
            size_t savePosition = this->_current;

            CallAtomizer              nestedAtomizer(this->_current, this->_tokens);
            std::unique_ptr<NodeBase> nestedNode = nestedAtomizer.atomize();

            CallNode* nestedCallPtr = dynamic_cast<CallNode*>(nestedNode.get());
            if (nestedCallPtr) {
                std::unique_ptr<CallNode> nestedCall(static_cast<CallNode*>(nestedNode.release()));
                callNode->addNestedCallArgument(std::move(nestedCall));
                needsComma = true;
            } else {
                this->_current = savePosition;
                callNode->addArgument(this->_tokens[this->_current]);
                this->advance();
                needsComma = true;
            }
        } else {
            callNode->addArgument(this->_tokens[this->_current]);
            this->advance();
            needsComma = true;
        }
    }

    throw std::runtime_error(ErrorUtil::errorMessage("Unexpected end of input while parsing function call arguments",
                                                     startToken.line,
                                                     startToken.column));
}