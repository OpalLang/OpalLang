#pragma once

namespace Opal {

enum class TokenType {
    // Keywords
    CLASS,
    FN,
    IF,
    ELIF,
    ELSE,
    WHILE,
    FOR,
    FOREACH,
    IN,
    TRY,
    CATCH,
    FINALLY,
    RET,
    THIS,

    // Literals
    NUMBER,
    STRING,
    TRUE,
    FALSE,
    NIL,

    // Identifiers
    IDENTIFIER,

    // Basic Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    POWER,
    EQUAL,
    EQUAL_EQUAL,
    NOT,
    NOT_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    AND,
    OR,
    INCREMENT,
    DECREMENT,
    RANGE,

    // Assignment Operators
    PLUS_EQUAL,
    MINUS_EQUAL,
    MULTIPLY_EQUAL,
    DIVIDE_EQUAL,
    MODULO_EQUAL,
    POWER_EQUAL,
    AND_EQUAL,
    OR_EQUAL,
    XOR_EQUAL,
    SHIFT_LEFT_EQUAL,
    SHIFT_RIGHT_EQUAL,

    // Bitwise Operators
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,
    BITWISE_NOT,
    SHIFT_LEFT,
    SHIFT_RIGHT,

    // Delimiters
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COMMA,
    DOT,
    COLON,
    SEMICOLON,

    // Special Tokens
    COMMENT,
    EOF_TOKEN,
    ERROR
};

}