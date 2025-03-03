#include "NodeFactory.hpp"
#include "NodeBase.hpp"
#include "nodes/VariableNode.hpp"

namespace Opal {

std::unique_ptr<NodeBase> NodeFactory::createNode(TokenType type) {
    return std::make_unique<NodeBase>(type);
}

std::unique_ptr<VariableNode> NodeFactory::createVariableNode(
    const std::string& name, 
    const std::string& value, 
    bool isConst) {
    
    return std::make_unique<VariableNode>(
        TokenType::IDENTIFIER,
        name,
        value,
        isConst
    );
}

}  // namespace Opal