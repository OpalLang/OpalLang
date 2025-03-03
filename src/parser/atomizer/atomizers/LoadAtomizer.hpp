#pragma once

#include "../AtomizerBase.hpp"

#include <vector>

namespace Opal {

class LoadAtomizer : public AtomizerBase {
public:
    LoadAtomizer(int& current, std::vector<Token>& tokens);
    bool canHandle(TokenType type) const override;
    void atomize() override;
};

}  // namespace Opal