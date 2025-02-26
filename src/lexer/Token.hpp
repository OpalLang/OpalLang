#pragma once

#include "TokenType.hpp"

#include <string_view>

namespace Opal {

class Token {
public:
    TokenType        type;
    std::string_view value;
    int              line;
    int              column;

    Token(TokenType type, std::string_view value, int line, int column)
        : type(type), value(value), line(line), column(column) {}
};

}  // namespace Opal