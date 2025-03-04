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
#include "opal/parser/atomizer/atomizers/OperationAtomizer.hpp"
#include "opal/parser/node/NodeFactory.hpp"
#include "opal/util/ErrorUtil.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace opal;

VariableAtomizer::VariableAtomizer(size_t& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool VariableAtomizer::canHandle(TokenType type) const {
    if (type != TokenType::IDENTIFIER)
        return false;

    size_t nextIndex = this->_current + 1;
    if (nextIndex < this->_tokens.size() && this->_tokens[nextIndex].type == TokenType::LEFT_PAREN) {
        return false;
    }
    return true;
}

std::unique_ptr<NodeBase> VariableAtomizer::atomize() {
    std::string variableName = std::string(_tokens[_current].value);
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
        std::unique_ptr<VariableNode> variableNode =
            NodeFactory::createVariableNode(variableName, "", false, VariableType::UNKNOWN);
        return std::unique_ptr<NodeBase>(variableNode.release());
    }
}

std::unique_ptr<NodeBase> VariableAtomizer::handleAssignment(std::unique_ptr<VariableNode>& variableNode) {
    std::string variableValue;

    OperationAtomizer opAtomizer(this->_current, this->_tokens);
    bool              hasOperator =
        this->_current + 1 < this->_tokens.size() && opAtomizer.canHandle(this->_tokens[this->_current + 1].type);

    if (this->_tokens[this->_current].type == TokenType::NUMBER) {
        if (hasOperator) {
            return std::unique_ptr<NodeBase>(this->handleOperation(variableNode).release());
        } else {
            variableValue = std::string(this->_tokens[this->_current].value);
            variableNode->setValue(variableValue);
            variableNode->setType(VariableType::INT);
        }
    } else if (this->_tokens[this->_current].type == TokenType::IDENTIFIER && hasOperator) {
        return std::unique_ptr<NodeBase>(this->handleOperation(variableNode).release());
    } else if (this->_tokens[this->_current].type == TokenType::STRING) {
        variableValue = std::string(this->_tokens[this->_current].value);
        variableNode->setValue(variableValue);
        variableNode->setType(VariableType::STRING);
    } else if (this->_tokens[this->_current].type == TokenType::TRUE
               || this->_tokens[this->_current].type == TokenType::FALSE) {
        variableValue = std::string(this->_tokens[this->_current].value);
        variableNode->setValue(variableValue);
        variableNode->setType(VariableType::BOOL);
    } else if (this->_tokens[this->_current].type == TokenType::NIL) {
        variableValue = "nil";
        variableNode->setValue(variableValue);
        variableNode->setType(VariableType::NIL);
    } else if (this->_tokens[this->_current].type == TokenType::IDENTIFIER) {
        variableValue = std::string(this->_tokens[this->_current].value);
        variableNode->setValue(variableValue);
        variableNode->setType(VariableType::UNKNOWN);
    } else {
        throw std::runtime_error(ErrorUtil::errorMessage("Expected a value or identifier after assignment operator",
                                                         this->_tokens[this->_current].line,
                                                         this->_tokens[this->_current].column));
    }
    this->advance();
    return std::unique_ptr<NodeBase>(variableNode.release());
}

std::unique_ptr<NodeBase> VariableAtomizer::handleOperation(std::unique_ptr<VariableNode>& variableNode) {
    size_t nextIndex = this->_current + 1;
    if (nextIndex < this->_tokens.size()) {
        OperationAtomizer opAtomizer(this->_current, this->_tokens);
        if (this->_tokens[this->_current].type == TokenType::NUMBER
            || (nextIndex < this->_tokens.size() && opAtomizer.canHandle(this->_tokens[nextIndex].type))) {
            std::unique_ptr<OperationNode> opNode =
                std::unique_ptr<OperationNode>(dynamic_cast<OperationNode*>(opAtomizer.atomize().release()));
            if (opNode) {
                variableNode->setOperation(std::move(opNode));
                variableNode->setType(VariableType::INT);
                return std::unique_ptr<NodeBase>(variableNode.release());
            }
        }
    }

    std::string variableValue = std::string(this->_tokens[this->_current].value);
    variableNode->setValue(variableValue);
    variableNode->setType(this->_tokens[this->_current].type == TokenType::NUMBER ? VariableType::INT
                                                                                  : VariableType::UNKNOWN);
    this->advance();
    return std::unique_ptr<NodeBase>(variableNode.release());
}
