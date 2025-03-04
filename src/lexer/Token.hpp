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

#include "opal/lexer/TokenType.hpp"

#include <string_view>

namespace opal {

    /**
     * @class Token
     * @brief Represents a lexical token in the Opal language
     * 
     * A token is the smallest unit of meaning in the language, such as keywords,
     * identifiers, operators, and literals. Each token contains information about
     * its type, value, and position in the source code.
     */
    class Token {
        public:
            TokenType        type;
            std::string_view value;
            int              line;
            int              column;

            /**
             * @brief Constructs a new Token object
             * @param type The type of the token
             * @param value The lexeme (actual text) of the token
             * @param line The line number where the token appears
             * @param column The column number where the token starts
             */
            Token(TokenType type, std::string_view value, int line, int column)
                : type(type), value(value), line(line), column(column) {}
    };

}  // namespace opal