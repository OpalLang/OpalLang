#pragma once

#include "../Token.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace Opal {

class TokenizerBase {
protected:
    const std::string&  source;
    int&                current;
    int&                line;
    int&                column;
    int&                start;
    std::vector<Token>& tokens;

public:
    TokenizerBase(const std::string&  source,
                  int&                current,
                  int&                line,
                  int&                column,
                  int&                start,
                  std::vector<Token>& tokens);
    virtual ~TokenizerBase() = default;

    virtual bool canHandle(char c) const = 0;
    virtual void tokenize()              = 0;

protected:
    bool isAtEnd() const;
    char peek() const;
    char peekNext() const;
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::string_view value);
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
};

}  // namespace Opal