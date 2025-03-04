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
#include "opal/util/ErrorUtil.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace opal;

OperationAtomizer::OperationAtomizer(size_t& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool OperationAtomizer::canHandle(TokenType type) const {
    return type == TokenType::PLUS || type == TokenType::MINUS || type == TokenType::MULTIPLY
           || type == TokenType::DIVIDE || type == TokenType::MODULO || type == TokenType::LEFT_PAREN;
}

bool OperationAtomizer::isOperand(TokenType type) const {
    return type == TokenType::NUMBER || type == TokenType::IDENTIFIER;
}

void OperationAtomizer::handleToken(std::vector<Token>& tokens, const Token& token) {
    tokens.push_back(token);
    this->advance();
}

void OperationAtomizer::handleParenthesizedExpression(std::vector<Token>& operationTokens) {
    handleToken(operationTokens, this->_tokens[this->_current]); // Push LEFT_PAREN

    if (this->_current < this->_tokens.size() && this->_tokens[this->_current].type == TokenType::RIGHT_PAREN) {
        throw std::runtime_error(
            ErrorUtil::errorMessage("Invalid operation: empty parentheses",
                                  this->_tokens[this->_current].line,
                                  this->_tokens[this->_current].column));
    }

    int parenCount = 1;
    while (this->_current < this->_tokens.size() && parenCount > 0) {
        TokenType currentType = this->_tokens[this->_current].type;
        if (currentType == TokenType::LEFT_PAREN) {
            parenCount++;
        } else if (currentType == TokenType::RIGHT_PAREN) {
            parenCount--;
            if (parenCount == 0) {
                handleToken(operationTokens, this->_tokens[this->_current]);
                return;
            }
        }
        handleToken(operationTokens, this->_tokens[this->_current]);
    }

    throw std::runtime_error(
        ErrorUtil::errorMessage("Unmatched left parenthesis",
                              this->_tokens[this->_current - 1].line,
                              this->_tokens[this->_current - 1].column));
}

void OperationAtomizer::handleOperand(std::vector<Token>& operationTokens) {
    if (this->_current >= this->_tokens.size()) {
        throw std::runtime_error(
            ErrorUtil::errorMessage("Expected operand",
                                  this->_tokens[this->_current - 1].line,
                                  this->_tokens[this->_current - 1].column));
    }

    Token currentToken = this->_tokens[this->_current];
    if (currentToken.type == TokenType::LEFT_PAREN) {
        handleParenthesizedExpression(operationTokens);
    } else if (isOperand(currentToken.type)) {
        handleToken(operationTokens, currentToken);
    } else if (currentToken.type == TokenType::RIGHT_PAREN) {
        throw std::runtime_error(
            ErrorUtil::errorMessage("Unmatched right parenthesis",
                                  currentToken.line,
                                  currentToken.column));
    } else {
        throw std::runtime_error(
            ErrorUtil::errorMessage("Invalid operation: expected a number, identifier, or parenthesized expression",
                                  currentToken.line,
                                  currentToken.column));
    }
}

std::unique_ptr<NodeBase> OperationAtomizer::atomize() {
    std::vector<Token> operationTokens;
    handleOperand(operationTokens);

    while (this->_current < this->_tokens.size()) {
        Token currentToken = this->_tokens[this->_current];
        if (!canHandle(currentToken.type)) {
            if (currentToken.type == TokenType::RIGHT_PAREN) {
                throw std::runtime_error(
                    ErrorUtil::errorMessage("Unmatched right parenthesis",
                                          currentToken.line,
                                          currentToken.column));
            }
            break;
        }

        handleToken(operationTokens, currentToken);
        handleOperand(operationTokens);
    }

    return NodeFactory::createOperationNode(operationTokens);
}
