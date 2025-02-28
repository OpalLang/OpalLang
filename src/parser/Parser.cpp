#include "Parser.hpp"

#include "../lexer/Token.hpp"
#include "atomizer/AtomizerBase.hpp"
#include "atomizer/AtomizerFactory.hpp"

#include <vector>

namespace Opal {

Token Parser::peek() const {
    return tokens[current];
}

bool Parser::isAtEnd() const {
    return current >= tokens.size() || tokens[current].type == TokenType::EOF_TOKEN;
}

Parser::Parser(std::vector<Token> tokens) : tokens(tokens) {
    atomizers = AtomizerFactory::createAtomizers(current, this->tokens);

    while (!isAtEnd()) {
        bool handled = false;
        for (const auto& atomizer : atomizers) {
            if (atomizer->canHandle(peek().type)) {
                atomizer->atomize();
                handled = true;
                break;
            }
        }

        if (!handled) {
            current++;
        }
    }
}

}  // namespace Opal