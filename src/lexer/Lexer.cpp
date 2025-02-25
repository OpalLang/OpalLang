#include "Lexer.hpp"
#include "../error/Error.hpp"

#include <stdexcept>
#include <iostream>

namespace Opal {
    const std::unordered_map<std::string, TokenType> Lexer::keywords = {
        {"class", TokenType::CLASS},
        {"fn", TokenType::FN},
        {"if", TokenType::IF},
        {"elif", TokenType::ELIF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"for", TokenType::FOR},
        {"foreach", TokenType::FOREACH},
        {"in", TokenType::IN},
        {"try", TokenType::TRY},
        {"catch", TokenType::CATCH},
        {"finally", TokenType::FINALLY},
        {"ret", TokenType::RET},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
        {"nil", TokenType::NIL},
        {"and", TokenType::AND},
        {"or", TokenType::OR},
        {"not", TokenType::NOT},
    };

    const std::unordered_map<std::string, TokenType> Lexer::operators = {
        {"(", TokenType::LEFT_PAREN},
        {")", TokenType::RIGHT_PAREN},
        {"{", TokenType::LEFT_BRACE},
        {"}", TokenType::RIGHT_BRACE},
        {"[", TokenType::LEFT_BRACKET},
        {"]", TokenType::RIGHT_BRACKET},
        {",", TokenType::COMMA},
        {".", TokenType::DOT},
        {":", TokenType::COLON},
        {";", TokenType::SEMICOLON},
        {"+", TokenType::PLUS},
        {"-", TokenType::MINUS},
        {"*", TokenType::MULTIPLY},
        {"/", TokenType::DIVIDE},
        {"%", TokenType::MODULO},
        {"=", TokenType::EQUAL},
        {"!", TokenType::NOT},
        {"<", TokenType::LESS},
        {">", TokenType::GREATER},
        {"&&", TokenType::AND},
        {"||", TokenType::OR},
        {"==", TokenType::EQUAL_EQUAL},
        {"!=", TokenType::NOT_EQUAL},
        {"<=", TokenType::LESS_EQUAL},
        {">=", TokenType::GREATER_EQUAL},
    };

    Lexer::Lexer(std::string source) : source(std::move(source)) {}

    std::vector<Token> Lexer::scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }

        tokens.emplace_back(TokenType::EOF_TOKEN, "EOF", line, column);
        return tokens;
    }

    void Lexer::scanToken() {
        char c = advance();
        
        switch (c) {
            case '"':
                string();
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                column = 1;
                break;
            default:
                std::string potentialOp;
                potentialOp += c;

                if (!isAtEnd()) {
                    std::string twoCharOp = potentialOp + peek();
                    auto twoCharMatch = operators.find(twoCharOp);
                    if (twoCharMatch != operators.end()) {
                        advance();
                        addToken(twoCharMatch->second);
                        break;
                    }
                }

                auto oneCharMatch = operators.find(potentialOp);
                if (oneCharMatch != operators.end()) {
                    addToken(oneCharMatch->second);
                    break;
                }

                if (isDigit(c)) {
                    number();
                    break;
                }

                if (isAlpha(c)) {
                    identifier();
                    break;
                }

                Error::lexerError(line, column - 1, "Unexpected character '" + std::string(1, c) + "'");
                exit(1);
        }
    }

    bool Lexer::match(char expected) {
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;
        current++;
        column++;
        return true;
    }

    char Lexer::peek() {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    char Lexer::peekNext() {
        if (current + 1 >= static_cast<int>(source.length())) return '\0';
        return source[current + 1];
    }

    void Lexer::string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') {
                line++;
                column = 1;
            }
            advance();
        }

        if (isAtEnd()) {
            Error::lexerError(line, column, "Unterminated string");
            exit(1);
        }

        advance();
        addToken(TokenType::STRING, std::string_view(source.data() + start + 1, current - start - 2));
    }

    void Lexer::number() {
        while (isDigit(peek())) advance();

        if (peek() == '.' && isDigit(peekNext())) {
            advance();
            while (isDigit(peek())) advance();
        }

        addToken(TokenType::NUMBER, std::string_view(source.data() + start, current - start));
    }

    void Lexer::identifier() {
        while (isAlphaNumeric(peek())) advance();

        std::string_view text(source.data() + start, current - start);
        auto it = keywords.find(std::string(text));
        TokenType type = it != keywords.end() ? it->second : TokenType::IDENTIFIER;
        addToken(type, text);
    }

    void Lexer::comment() {
        int nesting = 1;
        while (nesting > 0 && !isAtEnd()) {
            if (peek() == '/' && peekNext() == '*') {
                advance();
                advance();
                nesting++;
            } else if (peek() == '*' && peekNext() == '/') {
                advance();
                advance();
                nesting--;
            } else {
                if (peek() == '\n') {
                    line++;
                    column = 1;
                }
                advance();
            }
        }

        if (nesting > 0) {
            Error::lexerError(line, column, "Unterminated comment");
            exit(1);
        }
    }

    bool Lexer::isAtEnd() {
        return current >= static_cast<int>(source.length());
    }

    char Lexer::advance() {
        column++;
        return source[current++];
    }

    void Lexer::addToken(TokenType type) {
        addToken(type, std::string_view(source.data() + start, current - start));
    }

    void Lexer::addToken(TokenType type, std::string_view value) {
        tokens.emplace_back(type, value, line, column - (current - start));
    }

    bool Lexer::isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool Lexer::isAlpha(char c) {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               c == '_';
    }

    bool Lexer::isAlphaNumeric(char c) {
        return isAlpha(c) || isDigit(c);
    }

    void Lexer::printTokens() const {
        for (const auto& token : tokens) {
            std::cout << "Type: " << static_cast<int>(token.type) 
                      << " Value: '" << token.value 
                      << "' Line: " << token.line 
                      << " Column: " << token.column << std::endl;
        }
    }
}