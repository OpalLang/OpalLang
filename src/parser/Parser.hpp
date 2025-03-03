#pragma once

#include "../lexer/Token.hpp"
#include "atomizer/AtomizerBase.hpp"

#include <memory>
#include <vector>

namespace Opal {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

private:
    std::vector<Token>                         tokens;
    std::vector<std::unique_ptr<AtomizerBase>> atomizers;

    int current = 0;

    bool  isAtEnd() const;
    Token peek() const;
};

}  // namespace Opal