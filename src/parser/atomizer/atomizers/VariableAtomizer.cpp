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

#include "opal/parser/atomizer/atomizers/VariableAtomizer.hpp"

#include "opal/parser/atomizer/VariableType.hpp"
#include "opal/parser/atomizer/atomizers/CallAtomizer.hpp"
#include "opal/parser/atomizer/atomizers/OperationAtomizer.hpp"
#include "opal/parser/atomizer/atomizers/StringAtomizer.hpp"
#include "opal/parser/node/NodeFactory.hpp"
#include "opal/util/ErrorUtil.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace opal;

VariableAtomizer::VariableAtomizer(size_t& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool VariableAtomizer::canHandle(TokenType type) const {
    if (type != TokenType::IDENTIFIER && type != TokenType::CONST)
        return false;

    if (type == TokenType::CONST)
        return this->_current + 1 < this->_tokens.size();

    size_t nextIndex = this->_current + 1;
    if (nextIndex < this->_tokens.size() && this->_tokens[nextIndex].type == TokenType::LEFT_PAREN) {
        return false;
    }
    return true;
}

std::unique_ptr<NodeBase> VariableAtomizer::atomize() {
    if (this->_tokens[this->_current].type == TokenType::CONST) {
        if (this->_current + 1 >= this->_tokens.size()
            || this->_tokens[this->_current + 1].type != TokenType::IDENTIFIER) {
            throw std::runtime_error(ErrorUtil::errorMessage("Expected identifier after 'const' keyword",
                                                             this->_tokens[this->_current].line,
                                                             this->_tokens[this->_current].column));
        }
        this->advance();
    }

    std::string variableName = std::string(this->_tokens[this->_current].value);
    this->advance();

    if (this->_current < this->_tokens.size() && this->_tokens[this->_current].type == TokenType::EQUAL) {
        Token equalToken = this->_tokens[this->_current];
        this->advance();

        if (this->_current >= this->_tokens.size()) {
            throw std::runtime_error(ErrorUtil::errorMessage("No value provided after assignment operator",
                                                             equalToken.line,
                                                             equalToken.column));
        }

        bool isConst = (this->_current >= 3 && this->_tokens[this->_current - 3].type == TokenType::CONST);
        std::unique_ptr<VariableNode> variableNode =
            NodeFactory::createVariableNode(variableName, "", isConst, VariableType::UNKNOWN);

        return std::unique_ptr<NodeBase>(this->handleAssignment(variableNode).release());
    } else {
        if (this->_current > 0 && this->_tokens[this->_current - 2].type == TokenType::CONST) {
            throw std::runtime_error(ErrorUtil::errorMessage("const declarations must include an initializer",
                                                             this->_tokens[this->_current - 2].line,
                                                             this->_tokens[this->_current - 2].column));
        }

        std::unique_ptr<VariableNode> variableNode =
            NodeFactory::createVariableNode(variableName, "", false, VariableType::UNKNOWN);
        return std::unique_ptr<NodeBase>(variableNode.release());
    }
}

std::unique_ptr<NodeBase> VariableAtomizer::handleAssignment(std::unique_ptr<VariableNode>& variableNode) {
    TokenType currentType = this->_tokens[this->_current].type;

    if (isCallPattern()) {
        return handleCall(variableNode);
    }

    if (shouldHandleAsOperation(currentType)) {
        return handleOperation(variableNode);
    }

    setVariableValueAndType(variableNode, currentType);
    this->advance();
    return std::unique_ptr<NodeBase>(variableNode.release());
}

void VariableAtomizer::setVariableValueAndType(std::unique_ptr<VariableNode>& variableNode, TokenType type) {
    switch (type) {
        case TokenType::STRING: {
            StringAtomizer              stringAtomizer(this->_current, this->_tokens);
            std::unique_ptr<StringNode> stringNode =
                std::unique_ptr<StringNode>(dynamic_cast<StringNode*>(stringAtomizer.atomize().release()));
            if (stringNode) {
                variableNode->setStringNode(std::move(stringNode));
            }
            break;
        }
        case TokenType::TRUE:
        case TokenType::FALSE: {
            std::string value = std::string(this->_tokens[this->_current].value);
            variableNode->setValue(value);
            variableNode->setType(VariableType::BOOL);
            break;
        }
        case TokenType::NIL: {
            variableNode->setValue("nil");
            variableNode->setType(VariableType::NIL);
            break;
        }
        case TokenType::NUMBER: {
            std::string value = std::string(this->_tokens[this->_current].value);
            variableNode->setValue(value);
            variableNode->setType(VariableType::INT);
            break;
        }
        case TokenType::IDENTIFIER: {
            std::string value = std::string(this->_tokens[this->_current].value);
            variableNode->setValue(value);
            variableNode->setType(VariableType::UNKNOWN);
            break;
        }
        default:
            throw std::runtime_error(ErrorUtil::errorMessage("Expected a value or identifier after assignment operator",
                                                             this->_tokens[this->_current].line,
                                                             this->_tokens[this->_current].column));
    }
}

bool VariableAtomizer::isCallPattern() const {
    if (this->_current >= this->_tokens.size() || this->_tokens[this->_current].type != TokenType::IDENTIFIER)
        return false;

    size_t nextIndex = this->_current + 1;
    return nextIndex < this->_tokens.size() && this->_tokens[nextIndex].type == TokenType::LEFT_PAREN;
}

std::unique_ptr<NodeBase> VariableAtomizer::handleCall(std::unique_ptr<VariableNode>& variableNode) {
    CallAtomizer callAtomizer(this->_current, this->_tokens);
    if (callAtomizer.canHandle(this->_tokens[this->_current].type)) {
        std::unique_ptr<CallNode> callNode =
            std::unique_ptr<CallNode>(dynamic_cast<CallNode*>(callAtomizer.atomize().release()));
        if (callNode) {
            variableNode->setCallNode(std::move(callNode));
            return std::unique_ptr<NodeBase>(variableNode.release());
        }
    }
    return handleAsSimpleValue(variableNode);
}

bool VariableAtomizer::shouldHandleAsOperation(TokenType currentType) {
    OperationAtomizer opAtomizer(this->_current, this->_tokens);
    bool              hasOperator =
        this->_current + 1 < this->_tokens.size()
        && (opAtomizer.canHandle(this->_tokens[this->_current + 1].type) || currentType == TokenType::LEFT_PAREN);

    return ((currentType == TokenType::LEFT_PAREN || currentType == TokenType::NUMBER) && hasOperator)
           || (currentType == TokenType::IDENTIFIER && hasOperator);
}

std::unique_ptr<NodeBase> VariableAtomizer::handleOperation(std::unique_ptr<VariableNode>& variableNode) {
    OperationAtomizer opAtomizer(this->_current, this->_tokens);
    if (canParseAsOperation(opAtomizer)) {
        std::unique_ptr<OperationNode> opNode = parseOperation(opAtomizer);
        if (opNode) {
            variableNode->setOperation(std::move(opNode));
            return std::unique_ptr<NodeBase>(variableNode.release());
        }
    }
    return handleAsSimpleValue(variableNode);
}

bool VariableAtomizer::canParseAsOperation(const OperationAtomizer& opAtomizer) const {
    return this->_tokens[this->_current].type == TokenType::LEFT_PAREN
           || this->_tokens[this->_current].type == TokenType::NUMBER
           || (this->_current + 1 < this->_tokens.size()
               && opAtomizer.canHandle(this->_tokens[this->_current + 1].type));
}

std::unique_ptr<OperationNode> VariableAtomizer::parseOperation(OperationAtomizer& opAtomizer) {
    return std::unique_ptr<OperationNode>(dynamic_cast<OperationNode*>(opAtomizer.atomize().release()));
}

std::unique_ptr<NodeBase> VariableAtomizer::handleAsSimpleValue(std::unique_ptr<VariableNode>& variableNode) {
    std::string variableValue = std::string(this->_tokens[this->_current].value);
    variableNode->setValue(variableValue);
    this->advance();
    return std::unique_ptr<NodeBase>(variableNode.release());
}
