#pragma once

#include "../../lexer/Token.hpp"
#include "NodeBase.hpp"
#include "nodes/VariableNode.hpp"
#include <memory>
#include <string>

namespace Opal {

class NodeFactory {
public:
    static std::unique_ptr<NodeBase> createNode(TokenType type);
    static std::unique_ptr<VariableNode> createVariableNode(
        const std::string& name, 
        const std::string& value, 
        bool isConstant = false);
};

}  // namespace Opal