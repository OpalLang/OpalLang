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

#include "opal/parser/node/nodes/VariableNode.hpp"

#include <spdlog/spdlog.h>

#include <iostream>

using namespace opal;

VariableNode::VariableNode(TokenType          tokenType,
                           const std::string& name,
                           const std::string& value,
                           bool               isConstant,
                           VariableType       type)
    : NodeBase(tokenType), _name(name), _value(value), _isConstant(isConstant), _type(type) {}

void VariableNode::print(size_t indent) const {
    std::string typeStr;
    switch (this->_type) {
        case VariableType::INT:
            typeStr = "INT";
            break;
        case VariableType::STRING:
            typeStr = "STRING";
            break;
        case VariableType::BOOL:
            typeStr = "BOOL";
            break;
        case VariableType::NIL:
            typeStr = "NIL";
            break;
        default:
            typeStr = "UNKNOWN";
            break;
    }

    this->printIndent(indent);
    spdlog::info("Variable(name={})", this->_name);
    if (this->_operation) {
        std::cout << ", operation=";
        this->_operation->print(0);
    } else if (!this->_value.empty()) {
        std::cout << ", value=" << this->_value;
    }

    std::cout << ", type=" << typeStr << ", const=" << (this->_isConstant ? "true" : "false") << ")" << std::endl;
}
