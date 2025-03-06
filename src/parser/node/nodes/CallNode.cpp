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
#include "opal/parser/node/nodes/CallNode.hpp"

#include <spdlog/spdlog.h>

#include <iostream>
#include <stdexcept>

namespace opal {

CallNode::CallNode(TokenType tokenType, const std::string& name)
    : NodeBase(tokenType, NodeType::FUNCTION), _name(name) {}

const std::string& CallNode::getName() const {
    return _name;
}

const std::vector<std::unique_ptr<ArgumentNode>>& CallNode::getArgumentNodes() const {
    return _arguments;
}

const ArgumentNode& CallNode::getArgumentAt(size_t index) const {
    if (index >= _arguments.size()) {
        throw std::out_of_range("Argument index out of range");
    }
    return *_arguments[index];
}

size_t CallNode::getArgumentCount() const {
    return _arguments.size();
}

void CallNode::addArgument(const Token& token) {
    _arguments.push_back(std::make_unique<ArgumentNode>(token));
}

void CallNode::addNestedCallArgument(std::unique_ptr<CallNode> nestedCall) {
    _arguments.push_back(std::make_unique<ArgumentNode>(std::move(nestedCall)));
}

void CallNode::print(size_t indent) const {
    std::string indentation(indent, ' ');
    spdlog::info("{}Call(name={})", indentation, _name);

    if (!_arguments.empty()) {
        spdlog::info("{}  Arguments:", indentation);

        for (size_t i = 0; i < _arguments.size(); i++) {
            const auto& arg = _arguments[i];

            if (arg->isToken()) {
                spdlog::info("{}    [{}]: {}", indentation, i, arg->getToken().value);
            } else {
                spdlog::info("{}    [{}]: Nested Call:", indentation, i);
                arg->getCall().print(indent + 6);  // Further indent nested calls
            }
        }
    }
}

}  // namespace opal