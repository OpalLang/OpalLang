#include "Lexer.hpp"

#include "../error/Error.hpp"

#include <iostream>

namespace Opal {

Lexer::Lexer(std::string source) : source(std::move(source)) {
    tokenizers = TokenizerFactory::createTokenizers(this->source, current, line, column, start, tokens);
}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::EOF_TOKEN, "EOF", line, column);
    return tokens;
}

void Lexer::scanToken() {
    char c = source[current];

    if (c == ' ' || c == '\r' || c == '\t') {
        current++;
        column++;
        return;
    }

    if (c == '\n') {
        current++;
        line++;
        column = 1;
        return;
    }

    for (const auto& tokenizer : tokenizers) {
        if (tokenizer->canHandle(c)) {
            tokenizer->tokenize();
            return;
        }
    }

    Error::lexerError(line, column, "Unexpected character '" + std::string(1, c) + "'");
    current++;
    column++;
}

bool Lexer::isAtEnd() const {
    return current >= static_cast<int>(source.length());
}

void Lexer::printTokens() const {
    for (const auto& token : tokens) {
        std::cout << "Type: " << static_cast<int>(token.type) << " Value: '" << token.value << "' Line: " << token.line
                  << " Column: " << token.column << std::endl;
    }
}

}  // namespace Opal