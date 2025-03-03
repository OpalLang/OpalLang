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

#include "Lexer.hpp"

#include "../error/Error.hpp"

#include <iostream>

namespace opal {

Lexer::Lexer(std::string source) : source(std::move(source)) {
    tokenizers = TokenizerFactory::createTokenizers(this->source, current, line, column, start, tokens);
}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::EOF_TOKEN, "EOF", line, column);
    return tokens;
}

void Lexer::scanToken() {
    char c = source[current];

    if (c == ' ' || c == '\r' || c == '\t') {
        current++;
        column++;
        return;
    }

    if (c == '\n') {
        current++;
        line++;
        column = 1;
        return;
    }

    for (const auto& tokenizer : tokenizers) {
        if (tokenizer->canHandle(c)) {
            tokenizer->tokenize();
            return;
        }
    }

    Error::lexerError(line, column, "Unexpected character '" + std::string(1, c) + "'");
    current++;
    column++;
}

bool Lexer::isAtEnd() const {
    return current >= static_cast<int>(source.length());
}

void Lexer::printTokens() const {
    for (const auto& token : tokens) {
        std::cout << "Type: " << static_cast<int>(token.type) << " Value: '" << token.value << "' Line: " << token.line
                  << " Column: " << token.column << std::endl;
    }
}

}  // namespace opal