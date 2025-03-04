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

#include "opal/lexer/Lexer.hpp"

#include "opal/util/ErrorUtil.hpp"

#include <spdlog/spdlog.h>

#include <iostream>
#include <stdexcept>

using namespace opal;

Lexer::Lexer(std::string source) : source(std::move(source)) {
    tokenizers = TokenizerFactory::createTokenizers(this->source,
                                                    this->current,
                                                    this->line,
                                                    this->column,
                                                    this->start,
                                                    this->tokens);
}

std::vector<Token> Lexer::scanTokens() {
    while (!this->isAtEnd()) {
        this->start = this->current;
        this->scanToken();
    }

    this->tokens.emplace_back(TokenType::EOF_TOKEN, "EOF", this->line, this->column);
    return this->tokens;
}

void Lexer::scanToken() {
    char c = this->source[this->current];

    if (c == ' ' || c == '\r' || c == '\t') {
        this->current++;
        this->column++;
        return;
    }

    if (c == '\n') {
        this->current++;
        this->line++;
        this->column = 1;
        return;
    }

    for (const std::unique_ptr<TokenizerBase>& tokenizer : this->tokenizers) {
        if (tokenizer->canHandle(c)) {
            tokenizer->tokenize();
            return;
        }
    }

    throw std::runtime_error(
        ErrorUtil::errorMessage("Invalid character '" + std::string(1, c) + "'", this->line, this->column));
}

bool Lexer::isAtEnd() const {
    return this->current >= static_cast<int>(this->source.length());
}

void Lexer::printTokens() const {
    for (const Token& token : this->tokens) {
        spdlog::info("Type: {}, Value: '{}', Line: {}, Column: {}",
                     static_cast<int>(token.type),
                     token.value,
                     token.line,
                     token.column);
    }
}
