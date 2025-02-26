#pragma once

namespace Opal {

enum class TokenType {
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

    NUMBER,
    STRING,
    TRUE,
    FALSE,
    NIL,

    IDENTIFIER,

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

    // Binary operators
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,
    BITWISE_NOT,
    SHIFT_LEFT,
    SHIFT_RIGHT,

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

    COMMENT,
    EOF_TOKEN,
    ERROR
};

}