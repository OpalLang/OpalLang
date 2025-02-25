#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"

namespace Opal {
    class Lexer {
    public:
        Lexer(std::string source);
        std::vector<Token> scanTokens();
        void printTokens() const;

    private:
        const std::string source;
        std::vector<Token> tokens;
        int start = 0;
        int current = 0;
        int line = 1;
        int column = 1;

        void scanToken();
        bool match(char expected);
        char peek();
        char peekNext();
        void string();
        void number();
        void identifier();
        void skipWhitespace();
        void comment();

        bool isAtEnd();
        char advance();
        void addToken(TokenType type);
        void addToken(TokenType type, std::string value);
        bool isDigit(char c);
        bool isAlpha(char c);
        bool isAlphaNumeric(char c);

        static const std::unordered_map<std::string, TokenType> keywords;
    };
}