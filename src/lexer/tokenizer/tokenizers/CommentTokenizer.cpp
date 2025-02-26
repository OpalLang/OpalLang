#include "CommentTokenizer.hpp"

#include "../../../error/Error.hpp"

namespace Opal {

bool CommentTokenizer::canHandle(char c) const {
    if (c != '/')
        return false;
    char next = peekNext();
    return next == '/' || next == '*';
}

void CommentTokenizer::tokenize() {
    advance();
    if (peek() == '/') {
        advance();
        handleSingleLineComment();
    } else if (peek() == '*') {
        advance();
        handleMultiLineComment();
    }
}

void CommentTokenizer::handleSingleLineComment() {
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }
    addToken(TokenType::COMMENT);
}

void CommentTokenizer::handleMultiLineComment() {
    int nesting = 1;

    while (nesting > 0 && !isAtEnd()) {
        if (peek() == '*' && peekNext() == '/') {
            advance();
            advance();
            nesting--;
        } else if (peek() == '/' && peekNext() == '*') {
            advance();
            advance();
            nesting++;
        } else {
            if (peek() == '\n') {
                line++;
                column = 1;
            }
            advance();
        }
    }

    if (nesting > 0) {
        Error::lexerError(line, column, "Unterminated multi-line comment");
        return;
    }

    addToken(TokenType::COMMENT);
}

}  // namespace Opal