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

#include "IdentifierTokenizer.hpp"

namespace Opal {

const std::unordered_map<std::string_view, TokenType> IdentifierTokenizer::keywords = {
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
    return isAlpha(c);
}

void IdentifierTokenizer::tokenize() {
    while (isAlphaNumeric(peek())) {
        advance();
    }

    std::string_view text(source.data() + start, current - start);
    auto             it   = keywords.find(text);
    TokenType        type = it != keywords.end() ? it->second : TokenType::IDENTIFIER;
    addToken(type, text);
}

}  // namespace Opal