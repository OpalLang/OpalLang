#pragma once

#include "../AtomizerBase.hpp"
#include "../../../parser/node/NodeFactory.hpp"
#include "../../../parser/node/nodes/VariableNode.hpp"
#include <memory>
#include <vector>

namespace Opal {

class VariableAtomizer : public AtomizerBase {
public:
    VariableAtomizer(int& current, std::vector<Token>& tokens);
    bool canHandle(TokenType type) const override;
    void atomize() override;
};

}  // namespace Opal
