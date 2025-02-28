#include "AtomizerBase.hpp"

#include "../../lexer/Token.hpp"

#include <vector>

namespace Opal {

AtomizerBase::AtomizerBase(int& current, std::vector<Token>& tokens) : current(current), tokens(tokens) {}

Token AtomizerBase::peek() const {
    return tokens[current];
}

Token AtomizerBase::peekNext() const {
    return tokens[current + 1];
}

Token AtomizerBase::advance() {
    return tokens[++current];
}

}  // namespace Opal