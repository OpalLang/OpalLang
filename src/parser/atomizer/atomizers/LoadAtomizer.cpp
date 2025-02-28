#include "LoadAtomizer.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace Opal {

LoadAtomizer::LoadAtomizer(int& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool LoadAtomizer::canHandle(TokenType type) const {
    return type == TokenType::LOAD;
}

void LoadAtomizer::atomize() {
    if (peekNext().type != TokenType::STRING) {
        throw std::runtime_error("Expected a value after load definition");
    }
    std::cout << "Load path: " << peekNext().value << std::endl;
    advance();
}

}  // namespace Opal