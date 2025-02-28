#pragma once

#include "../lexer/Token.hpp"

#include <vector>

namespace Opal {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

private:
    std::vector<Token> tokens;

    int current = 0;

    bool isAtEnd() const;
};

}  // namespace Opal