#pragma once

#include "../AtomizerBase.hpp"

#include <vector>

namespace Opal {

class OperationAtomizer : public AtomizerBase {
public:
    OperationAtomizer(int& current, std::vector<Token>& tokens);
    bool canHandle(TokenType type) const override;
    void atomize() override;
};

}  // namespace Opal