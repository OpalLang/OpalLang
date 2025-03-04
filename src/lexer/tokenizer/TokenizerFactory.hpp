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

#include "opal/lexer/tokenizer/TokenizerBase.hpp"

#include <memory>
#include <vector>

namespace opal {

    /**
     * @class TokenizerFactory
     * @brief Factory class for creating tokenizer instances
     * 
     * Creates and initializes all the different tokenizers needed by the lexer
     * to recognize various token types in the Opal language.
     */
    class TokenizerFactory {
        public:
            /**
             * @brief Creates a collection of tokenizers for lexical analysis
             * @param source Reference to the source code
             * @param current Reference to the current position in the source
             * @param line Reference to the current line number
             * @param column Reference to the current column number
             * @param start Reference to the start position of the current token
             * @param tokens Reference to the token collection
             * @return std::vector<std::unique_ptr<TokenizerBase>> A collection of initialized tokenizers
             */
            static std::vector<std::unique_ptr<TokenizerBase>> createTokenizers(const std::string&  source,
                                                                                int&                current,
                                                                                int&                line,
                                                                                int&                column,
                                                                                int&                start,
                                                                                std::vector<Token>& tokens);
    };

}  // namespace opal