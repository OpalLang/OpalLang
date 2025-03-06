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
#include "opal/parser/node/NodeBase.hpp"
#include "opal/parser/node/nodes/ArgumentNode.hpp"

#include <memory>
#include <string>
#include <vector>

namespace opal {

/**
 * @class CallNode
 * @brief Node representing a function or class call
 */
class CallNode : public NodeBase {
private:
    std::string                                _name;
    std::vector<std::unique_ptr<ArgumentNode>> _arguments;

public:
    /**
     * @brief Constructs a new Call Node
     */
    CallNode(TokenType tokenType, const std::string& name);

    /**
     * @brief Gets the name of the call
     */
    const std::string& getName() const;

    /**
     * @brief Gets the arguments of the call
     * @return A vector of ArgumentNode pointers
     */
    const std::vector<std::unique_ptr<ArgumentNode>>& getArgumentNodes() const;

    /**
     * @brief Gets the argument at a specific position
     * @param index The position of the argument to retrieve
     * @return Reference to the argument node at the specified position
     * @throws std::out_of_range if index is out of range
     */
    const ArgumentNode& getArgumentAt(size_t index) const;

    /**
     * @brief Gets the number of arguments
     */
    size_t getArgumentCount() const;

    /**
     * @brief Adds a token as an argument to the call
     */
    void addArgument(const Token& token);

    /**
     * @brief Adds a nested call as an argument
     */
    void addNestedCallArgument(std::unique_ptr<CallNode> nestedCall);

    /**
     * @brief Prints the node to standard output
     * @param indent The indentation level for pretty printing
     */
    void print(size_t indent = 0) const override;
};

}  // namespace opal