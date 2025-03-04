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

    /**
     * @class Lexer
     * @brief Tokenizes Opal source code into a sequence of tokens
     * 
     * The lexer is responsible for breaking down source code into meaningful tokens
     * that can be processed by the parser.
     */
    class Lexer {
        public:
            /**
             * @brief Constructs a new Lexer object
             * @param source The source code to tokenize
             */
            explicit Lexer(std::string source);
            
            /**
             * @brief Scans the source code and produces a vector of tokens
             * @return std::vector<Token> The tokens extracted from the source
             */
            std::vector<Token> scanTokens();
            
            /**
             * @brief Prints all tokens to standard output
             * 
             * Useful for debugging and visualizing the lexical analysis results.
             */
            void printTokens() const;

        private:
            std::string                                 _source;
            std::vector<Token>                          _tokens;
            std::vector<std::unique_ptr<TokenizerBase>> _tokenizers;
            int                                         _start   = 0;
            int                                         _current = 0;
            int                                         _line    = 1;
            int                                         _column  = 1;

            /**
             * @brief Scans a single token from the current position
             * 
             * Advances the current position and adds the found token to the token list.
             */
            void scanToken();
            
            /**
             * @brief Checks if the lexer has reached the end of the source
             * @return bool True if at the end of source, false otherwise
             */
            bool isAtEnd() const;
    };

}  // namespace opal