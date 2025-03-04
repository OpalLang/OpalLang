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

#include "opal/lexer/tokenizer/tokenizers/OperatorTokenizer.hpp"

using namespace opal;

const std::unordered_map<std::string_view, TokenType> OperatorTokenizer::_operators = {
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
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::MULTIPLY},
    {"/", TokenType::DIVIDE},
    {"%", TokenType::MODULO},
    {"^", TokenType::POWER},
    {"=", TokenType::EQUAL},
    {"!", TokenType::NOT},
    {"<", TokenType::LESS},
    {">", TokenType::GREATER},
    {"==", TokenType::EQUAL_EQUAL},
    {"!=", TokenType::NOT_EQUAL},
    {"<=", TokenType::LESS_EQUAL},
    {">=", TokenType::GREATER_EQUAL},
    {"&&", TokenType::AND},
    {"||", TokenType::OR},
    {"++", TokenType::INCREMENT},
    {"--", TokenType::DECREMENT},
    {"..", TokenType::RANGE},
    {"&", TokenType::BITWISE_AND},
    {"|", TokenType::BITWISE_OR},
    {"~", TokenType::BITWISE_NOT},
    {"#", TokenType::BITWISE_XOR},
    {"<<", TokenType::SHIFT_LEFT},
    {">>", TokenType::SHIFT_RIGHT},
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
    std::string potential2 = str + this->peek();
    std::string potential3 = potential2 + this->peekNext();
    return this->_operators.find(str) != this->_operators.end()
           || this->_operators.find(potential2) != this->_operators.end()
           || this->_operators.find(potential3) != this->_operators.end();
}

void OperatorTokenizer::tokenize() {
    std::string first(1, this->advance());

    if (!this->isAtEnd()) {
        std::string potential3                                              = first + this->peek() + this->peekNext();
        std::unordered_map<std::string_view, TokenType>::const_iterator it3 = this->_operators.find(potential3);
        if (it3 != this->_operators.end()) {
            this->advance();
            this->advance();
            std::string_view text(this->_source.data() + this->_start, this->_current - this->_start);
            this->addToken(it3->second, text);
            return;
        }

        std::string                                                     potential2 = first + this->peek();
        std::unordered_map<std::string_view, TokenType>::const_iterator it2        = this->_operators.find(potential2);
        if (it2 != this->_operators.end()) {
            this->advance();
            std::string_view text(this->_source.data() + this->_start, this->_current - this->_start);
            this->addToken(it2->second, text);
            return;
        }
    }

    std::unordered_map<std::string_view, TokenType>::const_iterator it = this->_operators.find(first);
    if (it != this->_operators.end()) {
        std::string_view text(this->_source.data() + this->_start, this->_current - this->_start);

        this->addToken(it->second, text);
    }
}
