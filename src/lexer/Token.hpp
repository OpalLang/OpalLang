#pragma once
#include <string>
#include "TokenType.hpp"

namespace Opal {
    class Token {
    public:
        TokenType type;
        std::string value;
        int line;
        int column;

        Token(TokenType type, std::string value, int line, int column)
            : type(type), value(std::move(value)), line(line), column(column) {}
    };
}