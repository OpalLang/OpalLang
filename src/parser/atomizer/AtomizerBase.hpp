#pragma once

#include "../../lexer/Token.hpp"

#include <vector>

namespace Opal {

class AtomizerBase {
protected:
    int&                current;
    std::vector<Token>& tokens;

public:
    AtomizerBase(int& current, std::vector<Token>& tokens);
    virtual ~AtomizerBase() = default;

    virtual bool canHandle(TokenType) const = 0;
    virtual void atomize()                  = 0;

protected:
    Token peek() const;
    Token peekNext() const;
    Token advance();
};

}  // namespace Opal