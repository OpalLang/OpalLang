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
#include "opal/parser/atomizer/AtomizerBase.hpp"

#include <memory>
#include <vector>

namespace opal {

/**
 * @class Parser
 * @brief Parses tokens into an Abstract Syntax Tree (AST)
 *
 * The parser takes a sequence of tokens from the lexer and constructs
 * a hierarchical representation of the program structure (AST).
 */
class Parser {
private:
    std::vector<Token>                         _tokens;
    std::vector<std::unique_ptr<AtomizerBase>> _atomizers;
    std::vector<std::unique_ptr<NodeBase>>     _nodes;
    size_t                                     _current = 0;

    /**
     * @brief Checks if the parser has reached the end of the token stream
     * @return bool True if at the end of tokens, false otherwise
     */
    bool isAtEnd() const;

    /**
     * @brief Returns the current token without consuming it
     * @return Token The current token
     */
    Token peek() const;

public:
    /**
     * @brief Constructs a new Parser object
     * @param tokens The vector of tokens to parse
     */
    explicit Parser(std::vector<Token> tokens);

    /**
     * @brief Prints the Abstract Syntax Tree to standard output
     *
     * Useful for debugging and visualizing the parsed structure.
     */
    void printAST() const;
};

}  // namespace opal