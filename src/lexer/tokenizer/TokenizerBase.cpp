#include "TokenizerBase.hpp"

namespace Opal {

TokenizerBase::TokenizerBase(const std::string&  source,
                             int&                current,
                             int&                line,
                             int&                column,
                             int&                start,
                             std::vector<Token>& tokens)
    : source(source), current(current), line(line), column(column), start(start), tokens(tokens) {}

bool TokenizerBase::isAtEnd() const {
    return current >= static_cast<int>(source.length());
}

char TokenizerBase::peek() const {
    if (isAtEnd())
        return '\0';
    return source[current];
}

char TokenizerBase::peekNext() const {
    if (current + 1 >= static_cast<int>(source.length()))
        return '\0';
    return source[current + 1];
}

char TokenizerBase::advance() {
    column++;
    return source[current++];
}

void TokenizerBase::addToken(TokenType type) {
    addToken(type, std::string_view(source.data() + start, current - start));
}

void TokenizerBase::addToken(TokenType type, std::string_view value) {
    tokens.emplace_back(type, value, line, column - (current - start));
}

bool TokenizerBase::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool TokenizerBase::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool TokenizerBase::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

}  // namespace Opal