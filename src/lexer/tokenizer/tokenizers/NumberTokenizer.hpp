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

namespace opal {

/**
 * @class NumberTokenizer
 * @brief Tokenizer for handling numeric literals
 *
 * Processes character sequences that represent numeric values in the Opal language,
 * including integers, floating-point numbers, and potentially other numeric formats.
 */
class NumberTokenizer : public TokenizerBase {
public:
    /**
     * @brief Inherits constructor from TokenizerBase
     */
    using TokenizerBase::TokenizerBase;

    /**
     * @brief Checks if this tokenizer can handle the given character
     * @param c The character to check
     * @return bool True if this tokenizer can handle the character, false otherwise
     */
    bool canHandle(char c) const override;

    /**
     * @brief Processes a numeric literal and creates a corresponding token
     */
    void tokenize() override;
};

}  // namespace opal