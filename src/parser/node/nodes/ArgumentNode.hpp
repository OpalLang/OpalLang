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

#include <memory>
#include <variant>

namespace opal {

// Forward declaration of CallNode to avoid circular dependencies
class CallNode;

/**
 * @class ArgumentNode
 * @brief Represents a function call argument that can be either a token or a nested call
 */
class ArgumentNode {
private:
    std::variant<Token, std::unique_ptr<CallNode>> _value;

public:
    /**
     * @brief Constructs a new Token-based ArgumentNode
     * @param token The token representing a literal or identifier argument
     */
    explicit ArgumentNode(const Token& token);

    /**
     * @brief Constructs a new CallNode-based ArgumentNode
     * @param call A unique pointer to a nested function call
     */
    explicit ArgumentNode(std::unique_ptr<CallNode> call);

    /**
     * @brief Returns whether this argument contains a token
     */
    bool isToken() const;

    /**
     * @brief Returns whether this argument contains a nested call
     */
    bool isCall() const;

    /**
     * @brief Gets the token value if this argument is a token
     * @throws std::bad_variant_access if this argument is not a token
     */
    const Token& getToken() const;

    /**
     * @brief Gets the call node if this argument is a nested call
     * @throws std::bad_variant_access if this argument is not a nested call
     */
    const CallNode& getCall() const;

    /**
     * @brief Gets a mutable reference to the call node if this argument is a nested call
     * @throws std::bad_variant_access if this argument is not a nested call
     */
    CallNode& getCallMutable();

    /**
     * @brief Prints the argument to standard output
     * @param indent The indentation level for pretty printing
     */
    void print(size_t indent = 0) const;
};

}  // namespace opal