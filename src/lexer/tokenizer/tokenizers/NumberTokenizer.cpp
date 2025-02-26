#include "NumberTokenizer.hpp"

namespace Opal {

bool NumberTokenizer::canHandle(char c) const {
    return isDigit(c);
}

void NumberTokenizer::tokenize() {
    while (isDigit(peek()))
        advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();

        while (isDigit(peek()))
            advance();
    }

    addToken(TokenType::NUMBER);
}

}  // namespace Opal