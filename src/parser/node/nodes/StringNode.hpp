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
#include <string>
#include <vector>

namespace opal {

enum class StringSegmentType { TEXT, VARIABLE };

struct StringSegment {
    StringSegmentType type;
    std::string       content;
};

/**
 * @class StringNode
 * @brief AST node representing a string literal, which may contain interpolation
 *
 * Represents a string in Opal, which can be either a simple string or
 * an interpolated string containing variables or expressions.
 */
class StringNode : public NodeBase {
public:
    explicit StringNode(TokenType tokenType = TokenType::STRING);
    void                              addTextSegment(const std::string& text);
    void                              addVariableSegment(const std::string& variableName);
    void                              print(size_t indent) const override;
    const std::vector<StringSegment>& getSegments() const { return _segments; }

private:
    std::vector<StringSegment> _segments;
};

}  // namespace opal