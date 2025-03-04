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
    : _source(source), _current(current), _line(line), _column(column), _start(start), _tokens(tokens) {}

bool TokenizerBase::isAtEnd() const {
    return _current >= static_cast<int>(_source.length());
}

char TokenizerBase::peek() const {
    if (this->isAtEnd())
        return '\0';
    return _source[_current];
}

char TokenizerBase::peekNext() const {
    if (_current + 1 >= static_cast<int>(_source.length()))
        return '\0';
    return _source[_current + 1];
}

char TokenizerBase::advance() {
    _column++;
    return _source[_current++];
}

void TokenizerBase::addToken(TokenType type) {
    this->addToken(type, std::string_view(_source.data() + _start, _current - _start));
}

void TokenizerBase::addToken(TokenType type, std::string_view value) {
    _tokens.emplace_back(type, value, _line, _column - (_current - _start));
}

bool TokenizerBase::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool TokenizerBase::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool TokenizerBase::isAlphaNumeric(char c) const {
    return this->isAlpha(c) || this->isDigit(c);
}

}  // namespace opal