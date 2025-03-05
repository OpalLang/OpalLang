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
#include <vector>

namespace opal {

/**
 * @class AtomizerBase
 * @brief Base class for all atomizers in the Opal parser
 *
 * Atomizers are responsible for converting sequences of tokens into AST nodes.
 * Each atomizer specializes in handling a specific language construct.
 */
class AtomizerBase {
protected:
    size_t&             _current;
    std::vector<Token>& _tokens;

public:
    /**
     * @brief Constructs a new Atomizer Base object
     * @param current Reference to the current token index
     * @param tokens Reference to the token collection
     */
    AtomizerBase(size_t& current, std::vector<Token>& tokens);

    /**
     * @brief Virtual destructor for proper inheritance
     */
    virtual ~AtomizerBase() = default;

    /**
     * @brief Checks if this atomizer can handle the given token type
     * @param type The token type to check
     * @return bool True if this atomizer can handle the token type, false otherwise
     */
    virtual bool canHandle(TokenType type) const = 0;

    /**
     * @brief Converts a sequence of tokens into an AST node
     * @return std::unique_ptr<NodeBase> A unique pointer to the created AST node
     */
    virtual std::unique_ptr<NodeBase> atomize() = 0;

protected:
    /**
     * @brief Returns the current token without consuming it
     * @return Token The current token
     */
    Token peek() const;

    /**
     * @brief Returns the next token without consuming it
     * @return Token The next token
     */
    Token peekNext() const;

    /**
     * @brief Consumes and returns the current token
     * @return Token The current token
     */
    Token advance();
};

}  // namespace opal