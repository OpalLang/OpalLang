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

#include "opal/lexer/tokenizer/tokenizers/IdentifierTokenizer.hpp"

using namespace opal;

const std::unordered_map<std::string_view, TokenType> IdentifierTokenizer::_keywords = {
    {"class", TokenType::CLASS},     {"fn", TokenType::FN},           {"if", TokenType::IF},
    {"elif", TokenType::ELIF},       {"else", TokenType::ELSE},       {"while", TokenType::WHILE},
    {"for", TokenType::FOR},         {"foreach", TokenType::FOREACH}, {"in", TokenType::IN},
    {"try", TokenType::TRY},         {"catch", TokenType::CATCH},     {"finally", TokenType::FINALLY},
    {"ret", TokenType::RET},         {"this", TokenType::THIS},       {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},     {"nil", TokenType::NIL},         {"and", TokenType::AND},
    {"or", TokenType::OR},           {"not", TokenType::NOT},         {"const", TokenType::CONST},
    {"enum", TokenType::ENUM},       {"switch", TokenType::SWITCH},   {"case", TokenType::CASE},
    {"default", TokenType::DEFAULT}, {"break", TokenType::BREAK},     {"continue", TokenType::CONTINUE},
    {"load", TokenType::LOAD}};

bool IdentifierTokenizer::canHandle(char c) const {
    return this->isAlpha(c);
}

void IdentifierTokenizer::tokenize() {
    while (this->isAlphaNumeric(this->peek())) {
        this->advance();
    }

    std::string_view text(this->_source.data() + this->_start, this->_current - this->_start);
    std::unordered_map<std::string_view, TokenType>::const_iterator it = this->_keywords.find(text);
    TokenType type = it != this->_keywords.end() ? it->second : TokenType::IDENTIFIER;
    this->addToken(type, text);
}
