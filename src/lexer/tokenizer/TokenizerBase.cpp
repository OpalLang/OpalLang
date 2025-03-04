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

#include "opal/lexer/tokenizer/TokenizerBase.hpp"

namespace opal {

TokenizerBase::TokenizerBase(const std::string&  source,
                             int&                current,
                             int&                line,
                             int&                column,
                             int&                start,
                             std::vector<Token>& tokens)
    : source(source), current(current), line(line), column(column), start(start), tokens(tokens) {}

bool TokenizerBase::isAtEnd() const {
    return current >= static_cast<int>(source.length());
}

char TokenizerBase::peek() const {
    if (isAtEnd())
        return '\0';
    return source[current];
}

char TokenizerBase::peekNext() const {
    if (current + 1 >= static_cast<int>(source.length()))
        return '\0';
    return source[current + 1];
}

char TokenizerBase::advance() {
    column++;
    return source[current++];
}

void TokenizerBase::addToken(TokenType type) {
    addToken(type, std::string_view(source.data() + start, current - start));
}

void TokenizerBase::addToken(TokenType type, std::string_view value) {
    tokens.emplace_back(type, value, line, column - (current - start));
}

bool TokenizerBase::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool TokenizerBase::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool TokenizerBase::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

}  // namespace opal