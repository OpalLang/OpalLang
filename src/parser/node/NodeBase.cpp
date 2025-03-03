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

#include "NodeBase.hpp"

#include "../../lexer/Token.hpp"

#include <iostream>
#include <string>

namespace Opal {

std::string nodeTypeToString(NodeType type) {
    switch (type) {
        case NodeType::BASE:
            return "BASE";
        case NodeType::VARIABLE:
            return "VARIABLE";
        case NodeType::OPERATION:
            return "OPERATION";
        case NodeType::FUNCTION:
            return "FUNCTION";
        case NodeType::CLASS:
            return "CLASS";
        default:
            return "UNKNOWN";
    }
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::STRING:
            return "STRING";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::MULTIPLY:
            return "MULTIPLY";
        case TokenType::DIVIDE:
            return "DIVIDE";
        case TokenType::EQUAL:
            return "EQUAL";
        case TokenType::CONST:
            return "CONST";
        case TokenType::EOF_TOKEN:
            return "EOF";
        default:
            return "OTHER";
    }
}

NodeBase::NodeBase(TokenType tokenType, NodeType nodeType) : nodeType(nodeType), tokenType(tokenType) {}

void NodeBase::printIndent(size_t indent) {
    for (size_t i = 0; i < indent; i++) {
        std::cout << "  ";
    }
}

void NodeBase::print(size_t indent) const {
    printIndent(indent);
    std::cout << "Node(type=" << nodeTypeToString(nodeType) << ", token=" << tokenTypeToString(tokenType) << ")"
              << std::endl;
}

}  // namespace Opal