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

#include <string>
#include <string_view>
#include <vector>

namespace opal {

/**
 * @class TokenizerBase
 * @brief Base class for all tokenizers in the Opal lexer
 *
 * Provides common functionality for tokenizing different types of tokens
 * such as identifiers, numbers, strings, operators, etc.
 */
class TokenizerBase {
protected:
    const std::string&  _source;
    int&                _current;
    int&                _line;
    int&                _column;
    int&                _start;
    std::vector<Token>& _tokens;

public:
    /**
     * @brief Constructs a new Tokenizer Base object
     * @param source Reference to the source code
     * @param current Reference to the current position in the source
     * @param line Reference to the current line number
     * @param column Reference to the current column number
     * @param start Reference to the start position of the current token
     * @param tokens Reference to the token collection
     */
    TokenizerBase(const std::string&  source,
                  int&                current,
                  int&                line,
                  int&                column,
                  int&                start,
                  std::vector<Token>& tokens);

    /**
     * @brief Virtual destructor for proper inheritance
     */
    virtual ~TokenizerBase() = default;

    /**
     * @brief Checks if this tokenizer can handle the given character
     * @param c The character to check
     * @return bool True if this tokenizer can handle the character, false otherwise
     */
    virtual bool canHandle(char c) const = 0;

    /**
     * @brief Tokenizes the current lexeme and adds the resulting token to the collection
     */
    virtual void tokenize() = 0;

protected:
    /**
     * @brief Checks if the end of the source has been reached
     * @return bool True if at the end of source, false otherwise
     */
    bool isAtEnd() const;

    /**
     * @brief Returns the current character without consuming it
     * @return char The current character
     */
    char peek() const;

    /**
     * @brief Returns the next character without consuming it
     * @return char The next character, or '\0' if at the end of source
     */
    char peekNext() const;

    /**
     * @brief Consumes and returns the current character
     * @return char The current character
     */
    char advance();

    /**
     * @brief Adds a token with the given type to the collection
     * @param type The type of the token
     */
    void addToken(TokenType type);

    /**
     * @brief Adds a token with the given type and value to the collection
     * @param type The type of the token
     * @param value The value of the token
     */
    void addToken(TokenType type, std::string_view value);

    /**
     * @brief Checks if a character is a digit (0-9)
     * @param c The character to check
     * @return bool True if the character is a digit, false otherwise
     */
    bool isDigit(char c) const;

    /**
     * @brief Checks if a character is alphabetic (a-z, A-Z, _)
     * @param c The character to check
     * @return bool True if the character is alphabetic, false otherwise
     */
    bool isAlpha(char c) const;

    /**
     * @brief Checks if a character is alphanumeric (a-z, A-Z, 0-9, _)
     * @param c The character to check
     * @return bool True if the character is alphanumeric, false otherwise
     */
    bool isAlphaNumeric(char c) const;
};

}  // namespace opal