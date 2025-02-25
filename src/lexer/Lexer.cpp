#include "Lexer.hpp"
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
        {"nil", TokenType::NIL}
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
            case '(': addToken(TokenType::LEFT_PAREN); break;
            case ')': addToken(TokenType::RIGHT_PAREN); break;
            case '{': addToken(TokenType::LEFT_BRACE); break;
            case '}': addToken(TokenType::RIGHT_BRACE); break;
            case '[': addToken(TokenType::LEFT_BRACKET); break;
            case ']': addToken(TokenType::RIGHT_BRACKET); break;
            case ',': addToken(TokenType::COMMA); break;
            case '.': addToken(TokenType::DOT); break;
            case ':': addToken(TokenType::COLON); break;
            case '+': addToken(TokenType::PLUS); break;
            case '-': addToken(TokenType::MINUS); break;
            case '*': addToken(TokenType::STAR); break;
            case '/': 
                if (match('/')) {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else if (match('*')) {
                    comment();
                } else {
                    addToken(TokenType::SLASH);
                }
                break;
            case '%': addToken(TokenType::PERCENT); break;
            case '^': addToken(TokenType::CARET); break;
            case '=':
                addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
                break;
            case '!':
                addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
                break;
            case '<':
                addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
                break;
            case '>':
                addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
                break;
            case '&':
                if (match('&')) addToken(TokenType::AND);
                break;
            case '|':
                if (match('|')) addToken(TokenType::OR);
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                column = 1;
                break;
            case '"': string(); break;
            default:
                if (isDigit(c)) {
                    number();
                } else if (isAlpha(c)) {
                    identifier();
                } else {
                    throw std::runtime_error("Unexpected character");
                }
                break;
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
        if (current + 1 >= source.length()) return '\0';
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
            throw std::runtime_error("Unterminated string");
        }

        advance();
        std::string value = source.substr(start + 1, current - start - 2);
        addToken(TokenType::STRING, value);
    }

    void Lexer::number() {
        while (isDigit(peek())) advance();

        if (peek() == '.' && isDigit(peekNext())) {
            advance();
            while (isDigit(peek())) advance();
        }

        addToken(TokenType::NUMBER,
                source.substr(start, current - start));
    }

    void Lexer::identifier() {
        while (isAlphaNumeric(peek())) advance();

        std::string text = source.substr(start, current - start);
        auto it = keywords.find(text);
        TokenType type = it != keywords.end() ? it->second : TokenType::IDENTIFIER;
        addToken(type);
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
            throw std::runtime_error("Unterminated comment");
        }
    }

    bool Lexer::isAtEnd() {
        return current >= source.length();
    }

    char Lexer::advance() {
        column++;
        return source[current++];
    }

    void Lexer::addToken(TokenType type) {
        addToken(type, source.substr(start, current - start));
    }

    void Lexer::addToken(TokenType type, std::string value) {
        tokens.emplace_back(type, std::move(value), line, column - (current - start));
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