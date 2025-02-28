#include "Parser.hpp"

#include "../lexer/Token.hpp"

#include <vector>

namespace Opal {

Parser::Parser(std::vector<Token> tokens) : tokens(tokens) {}

bool Parser::isAtEnd() const {
    return tokens[current].type == TokenType::EOF_TOKEN;
}

}  // namespace Opal