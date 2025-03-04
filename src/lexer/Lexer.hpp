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

#include "opal/lexer/Token.hpp"
#include "opal/lexer/tokenizer/TokenizerFactory.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace opal {

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> scanTokens();
    void               printTokens() const;

private:
    std::string                                 source;
    std::vector<Token>                          tokens;
    std::vector<std::unique_ptr<TokenizerBase>> tokenizers;
    int                                         start   = 0;
    int                                         current = 0;
    int                                         line    = 1;
    int                                         column  = 1;

    void scanToken();
    bool isAtEnd() const;
};

}  // namespace opal