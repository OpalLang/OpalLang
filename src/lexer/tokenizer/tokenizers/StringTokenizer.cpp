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

#include "opal/lexer/tokenizer/tokenizers/StringTokenizer.hpp"

#include <spdlog/spdlog.h>

namespace opal {

bool StringTokenizer::canHandle(char c) const {
    return c == '"';
}

void StringTokenizer::tokenize() {
    advance();

    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line++;
            column = 1;
        }
        advance();
    }

    if (isAtEnd()) {
        spdlog::error("Unterminated string");
        exit(1);
    }

    advance();
    addToken(TokenType::STRING, std::string_view(source.data() + start + 1, current - start - 2));
}

}  // namespace opal