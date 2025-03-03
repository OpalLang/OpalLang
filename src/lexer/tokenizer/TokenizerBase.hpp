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

#include "../Token.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace Opal {

class TokenizerBase {
protected:
    const std::string&  source;
    int&                current;
    int&                line;
    int&                column;
    int&                start;
    std::vector<Token>& tokens;

public:
    TokenizerBase(const std::string&  source,
                  int&                current,
                  int&                line,
                  int&                column,
                  int&                start,
                  std::vector<Token>& tokens);
    virtual ~TokenizerBase() = default;

    virtual bool canHandle(char c) const = 0;
    virtual void tokenize()              = 0;

protected:
    bool isAtEnd() const;
    char peek() const;
    char peekNext() const;
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::string_view value);
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
};

}  // namespace Opal