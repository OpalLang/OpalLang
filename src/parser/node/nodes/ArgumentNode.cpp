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

#include "opal/parser/node/nodes/ArgumentNode.hpp"

#include "opal/parser/node/nodes/CallNode.hpp"

#include <iostream>
#include <string>

using namespace opal;

ArgumentNode::ArgumentNode(const Token& token) : _value(token) {}

ArgumentNode::ArgumentNode(std::unique_ptr<CallNode> call) : _value(std::move(call)) {}

bool ArgumentNode::isToken() const {
    return std::holds_alternative<Token>(_value);
}

bool ArgumentNode::isCall() const {
    return std::holds_alternative<std::unique_ptr<CallNode>>(_value);
}

const Token& ArgumentNode::getToken() const {
    return std::get<Token>(_value);
}

const CallNode& ArgumentNode::getCall() const {
    return *std::get<std::unique_ptr<CallNode>>(_value);
}

CallNode& ArgumentNode::getCallMutable() {
    return *std::get<std::unique_ptr<CallNode>>(_value);
}

void ArgumentNode::print(size_t indent) const {
    std::string indentation(indent, ' ');

    if (isToken()) {
        const Token& token = getToken();
        std::cout << indentation << "Value: " << token.value << std::endl;
    } else {
        std::cout << indentation << "Nested call:" << std::endl;
        getCall().print(indent + 2);
    }
}