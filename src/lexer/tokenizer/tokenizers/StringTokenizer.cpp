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

#include "opal/util/ErrorUtil.hpp"

#include <stdexcept>

using namespace opal;

bool StringTokenizer::canHandle(char c) const {
    return c == '"';
}

void StringTokenizer::tokenize() {
    this->advance();

    while (this->peek() != '"' && !this->isAtEnd()) {
        if (this->peek() == '\n') {
            this->_line++;
            this->_column = 1;
        }
        this->advance();
    }

    if (this->isAtEnd()) {
        throw std::runtime_error(ErrorUtil::errorMessage("Unterminated string", this->_line, 1));
    }

    this->advance();
    std::string_view text(this->_source.data() + this->_start + 1, this->_current - this->_start - 2);

    this->addToken(TokenType::STRING, text);
}
