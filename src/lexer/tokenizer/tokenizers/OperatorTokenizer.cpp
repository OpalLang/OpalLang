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

#include "OperatorTokenizer.hpp"

namespace opal {

const std::unordered_map<std::string_view, TokenType> OperatorTokenizer::operators = {
    // Delimiters
    {"(", TokenType::LEFT_PAREN},
    {")", TokenType::RIGHT_PAREN},
    {"{", TokenType::LEFT_BRACE},
    {"}", TokenType::RIGHT_BRACE},
    {"[", TokenType::LEFT_BRACKET},
    {"]", TokenType::RIGHT_BRACKET},
    {",", TokenType::COMMA},
    {".", TokenType::DOT},
    {":", TokenType::COLON},
    {";", TokenType::SEMICOLON},

    // Arithmetic Operators
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::MULTIPLY},
    {"/", TokenType::DIVIDE},
    {"%", TokenType::MODULO},
    {"^", TokenType::POWER},

    // Comparison Operators
    {"=", TokenType::EQUAL},
    {"!", TokenType::NOT},
    {"<", TokenType::LESS},
    {">", TokenType::GREATER},
    {"==", TokenType::EQUAL_EQUAL},
    {"!=", TokenType::NOT_EQUAL},
    {"<=", TokenType::LESS_EQUAL},
    {">=", TokenType::GREATER_EQUAL},

    // Logical Operators
    {"&&", TokenType::AND},
    {"||", TokenType::OR},

    // Increment/Decrement
    {"++", TokenType::INCREMENT},
    {"--", TokenType::DECREMENT},

    // Range Operator
    {"..", TokenType::RANGE},

    // Bitwise Operators
    {"&", TokenType::BITWISE_AND},
    {"|", TokenType::BITWISE_OR},
    {"~", TokenType::BITWISE_NOT},
    {"#", TokenType::BITWISE_XOR},
    {"<<", TokenType::SHIFT_LEFT},
    {">>", TokenType::SHIFT_RIGHT},

    // Compound Assignment Operators
    {"+=", TokenType::PLUS_EQUAL},
    {"-=", TokenType::MINUS_EQUAL},
    {"*=", TokenType::MULTIPLY_EQUAL},
    {"/=", TokenType::DIVIDE_EQUAL},
    {"%=", TokenType::MODULO_EQUAL},
    {"^=", TokenType::POWER_EQUAL},
    {"&=", TokenType::AND_EQUAL},
    {"|=", TokenType::OR_EQUAL},
    {"#=", TokenType::XOR_EQUAL},
    {"<<=", TokenType::SHIFT_LEFT_EQUAL},
    {">>=", TokenType::SHIFT_RIGHT_EQUAL},
};

bool OperatorTokenizer::canHandle(char c) const {
    std::string str(1, c);
    std::string potential2 = str + peek();
    std::string potential3 = potential2 + peekNext();
    return operators.find(str) != operators.end() || operators.find(potential2) != operators.end()
           || operators.find(potential3) != operators.end();
}

void OperatorTokenizer::tokenize() {
    std::string first(1, advance());

    if (!isAtEnd()) {
        // 3 char op
        std::string potential3 = first + peek() + peekNext();
        auto        it3        = operators.find(potential3);
        if (it3 != operators.end()) {
            advance();
            advance();
            std::string_view text(source.data() + start, current - start);
            addToken(it3->second, text);
            return;
        }

        // 2 char op
        std::string potential2 = first + peek();
        auto        it2        = operators.find(potential2);
        if (it2 != operators.end()) {
            advance();
            std::string_view text(source.data() + start, current - start);
            addToken(it2->second, text);
            return;
        }
    }

    // 1 char op
    auto it = operators.find(first);
    if (it != operators.end()) {
        std::string_view text(source.data() + start, current - start);
        addToken(it->second, text);
    }
}

}  // namespace opal