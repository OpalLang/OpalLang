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
#include "opal/parser/node/nodes/StringNode.hpp"
#include "opal/parser/node/NodeBase.hpp"

#include <memory>
#include <vector>

namespace opal {

/**
 * @class StringAtomizer
 * @brief Handles atomization of string literals, including interpolation. Empty strings will have no segments.
 */
class StringAtomizer : public AtomizerBase {
public:
    /**
     * @brief Construct a new String Atomizer
     * @param current Reference to the current token position
     * @param tokens Vector of tokens being processed
     */
    StringAtomizer(size_t& current, std::vector<Token>& tokens);

    /**
     * @brief Check if this atomizer can handle the given token type
     * @param type Token type to check
     * @return true if this atomizer can handle the token type
     */
    bool canHandle(TokenType type) const override;

    /**
     * @brief Process a string token and create a StringNode
     * @return std::unique_ptr<NodeBase> The created node
     * @throws std::runtime_error if unexpected end of input or unterminated interpolation
     */
    std::unique_ptr<NodeBase> atomize() override;

private:
    /**
     * @brief Parse string content for interpolation markers
     * @param stringNode The node to populate with segments
     * @param content The string content to parse
     * @note For empty strings, no segments will be added
     */
    void parseStringContent(StringNode* stringNode, const std::string& content);

    /**
     * @brief Check if a character at position is the start of interpolation
     * @param content String content being parsed
     * @param pos Current position in the string
     * @return true if interpolation marker found
     */
    bool isInterpolationStart(const std::string& content, size_t pos) const;
};

}  // namespace opal