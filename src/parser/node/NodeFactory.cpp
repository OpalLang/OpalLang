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

#include "opal/parser/node/NodeFactory.hpp"

#include "opal/parser/atomizer/VariableType.hpp"
#include "opal/parser/node/NodeBase.hpp"
#include "opal/parser/node/nodes/LoadNode.hpp"
#include "opal/parser/node/nodes/OperationNode.hpp"
#include "opal/parser/node/nodes/VariableNode.hpp"

namespace opal {

std::unique_ptr<NodeBase> NodeFactory::createNode(TokenType tokenType) {
    return std::make_unique<NodeBase>(tokenType);
}

std::unique_ptr<OperationNode> NodeFactory::createOperationNode(const std::vector<Token>& tokens) {
    TokenType operationType = tokens.empty() ? TokenType::PLUS : tokens[0].type;

    return std::make_unique<OperationNode>(operationType, tokens);
}

std::unique_ptr<LoadNode> NodeFactory::createLoadNode(const std::string_view& path) {
    return std::make_unique<LoadNode>(TokenType::LOAD, path);
}

}  // namespace opal