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

#pragma once

#include "opal/lexer/Token.hpp"
#include "opal/parser/atomizer/VariableType.hpp"
#include "opal/parser/node/NodeBase.hpp"
#include "opal/parser/node/nodes/LoadNode.hpp"
#include "opal/parser/node/nodes/OperationNode.hpp"
#include "opal/parser/node/nodes/StringNode.hpp"
#include "opal/parser/node/nodes/VariableNode.hpp"

#include <memory>
#include <string>
#include <vector>

namespace opal {

/**
 * @class NodeFactory
 * @brief Factory class for creating AST nodes
 *
 * Provides static methods to create different types of AST nodes,
 * encapsulating the creation logic and ensuring proper initialization.
 */
class NodeFactory {
public:
    /**
     * @brief Creates a generic node based on token type
     * @param type The token type to create a node for
     * @return std::unique_ptr<NodeBase> A unique pointer to the created node
     */
    static std::unique_ptr<NodeBase> createNode(TokenType type);

    /**
     * @brief Creates a variable node
     * @param name The name of the variable
     * @param value The initial value of the variable
     * @param isConstant Whether the variable is constant (cannot be reassigned)
     * @param type The data type of the variable
     * @return std::unique_ptr<VariableNode> A unique pointer to the created variable node
     */
    static std::unique_ptr<VariableNode> createVariableNode(const std::string& name,
                                                            const std::string& value,
                                                            bool               isConstant = false,
                                                            VariableType       type       = VariableType::UNKNOWN) {
        return std::make_unique<VariableNode>(TokenType::IDENTIFIER, name, value, isConstant, type);
    }

    /**
     * @brief Creates an operation node from a sequence of tokens
     * @param tokens The tokens representing the operation
     * @return std::unique_ptr<OperationNode> A unique pointer to the created operation node
     */
    static std::unique_ptr<OperationNode> createOperationNode(const std::vector<Token>& tokens);

    /**
     * @brief Creates a load node for importing modules
     * @param path The path to the module to load
     * @return std::unique_ptr<LoadNode> A unique pointer to the created load node
     */
    static std::unique_ptr<LoadNode> createLoadNode(const std::string_view& path);

    /**
     * @brief Creates a string node
     * @param text The initial text content of the string
     * @return std::unique_ptr<StringNode> A unique pointer to the created string node
     */
    static std::unique_ptr<StringNode> createStringNode() { return std::make_unique<StringNode>(TokenType::STRING); }
};

}  // namespace opal