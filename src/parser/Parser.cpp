#include "Parser.hpp"

#include "../lexer/Token.hpp"
#include "atomizer/AtomizerFactory.hpp"

#include <vector>

namespace Opal {

Parser::Parser(std::vector<Token> tokens) : tokens(tokens) {
    auto atomizers = AtomizerFactory::createAtomizers(current, this->tokens);
    for (const auto& atomizer : atomizers) {
        if (atomizer->canHandle(tokens[current].type)) {
            atomizer->atomize();
        }
    }
}

bool Parser::isAtEnd() const {
    return tokens[current].type == TokenType::EOF_TOKEN;
}

}  // namespace Opal