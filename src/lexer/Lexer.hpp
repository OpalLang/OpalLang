#pragma once

#include "Token.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Opal {

class Lexer {
public:
    Lexer(std::string source);
    std::vector<Token> scanTokens();
    void               printTokens() const;

private:
    const std::string  source;
    std::vector<Token> tokens;
    int                start   = 0;
    int                current = 0;
    int                line    = 1;
    int                column  = 1;

    void scanToken();
    bool match(char expected);
    char peek();
    char peekNext();
    void string();
    void number();
    void identifier();
    void comment();
    void singleLineComment();
    void multiLineComment();
    bool handleOperator(char c);

    bool isAtEnd();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::string_view value);
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);

    static const std::unordered_map<std::string_view, TokenType> keywords;
    static const std::unordered_map<std::string_view, TokenType> operators;
};

}  // namespace Opal