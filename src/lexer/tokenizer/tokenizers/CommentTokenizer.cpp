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

#include "opal/lexer/tokenizer/tokenizers/CommentTokenizer.hpp"

#include "opal/util/FileUtil.hpp"

#include <spdlog/spdlog.h>

#include <stdexcept>

namespace opal {

bool CommentTokenizer::canHandle(char c) const {
    if (c != '/')
        return false;
    char next = peekNext();
    return next == '/' || next == '*';
}

void CommentTokenizer::tokenize() {
    advance();
    if (peek() == '/') {
        advance();
        handleSingleLineComment();
    } else if (peek() == '*') {
        advance();
        handleMultiLineComment();
    }
}

void CommentTokenizer::handleSingleLineComment() {
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }
    addToken(TokenType::COMMENT);
}

void CommentTokenizer::handleMultiLineComment() {
    int nesting   = 1;
    int startLine = line;

    while (nesting > 0 && !isAtEnd()) {
        if (peek() == '*' && peekNext() == '/') {
            advance();
            advance();
            nesting--;
        } else if (peek() == '/' && peekNext() == '*') {
            advance();
            advance();
            nesting++;
        } else {
            if (peek() == '\n') {
                line++;
                column = 1;
            }
            advance();
        }
    }

    if (nesting > 0) {
        throw std::runtime_error(FileUtil::errorMessage("Unterminated multi-line comment", startLine, 1));
    }

    addToken(TokenType::COMMENT);
}

}  // namespace opal