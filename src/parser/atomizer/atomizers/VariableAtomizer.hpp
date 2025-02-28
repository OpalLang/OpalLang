#pragma once

#include "../AtomizerBase.hpp"

#include <vector>

namespace Opal {

class VariableAtomizer : public AtomizerBase {
public:
    VariableAtomizer(int& current, std::vector<Token>& tokens);
    bool canHandle(TokenType type) const override;
    void atomize() override;
};

}  // namespace Opal
