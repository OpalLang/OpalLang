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
        STAR,
        SLASH,
        PERCENT,
        CARET,
        EQUAL,
        EQUAL_EQUAL,
        BANG,
        BANG_EQUAL,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,
        AND,
        OR,

        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACE,
        RIGHT_BRACE,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        COMMA,
        DOT,
        COLON,

        COMMENT,
        EOF_TOKEN,
        ERROR
    };
}