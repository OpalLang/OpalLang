#include "StringTokenizer.hpp"

#include "../../../error/Error.hpp"

namespace Opal {

bool StringTokenizer::canHandle(char c) const {
    return c == '"';
}

void StringTokenizer::tokenize() {
    advance();

    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line++;
            column = 1;
        }
        advance();
    }

    if (isAtEnd()) {
        Error::lexerError(line, column, "Unterminated string");
        return;
    }

    advance();

    addToken(TokenType::STRING, std::string_view(source.data() + start + 1, current - start - 2));
}

}  // namespace Opal