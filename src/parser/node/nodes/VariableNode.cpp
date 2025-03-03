#include "VariableNode.hpp"

namespace Opal {

VariableNode::VariableNode(TokenType type, const std::string& name, const std::string& value, bool isConstant)
    : NodeBase(type), name(name), value(value), isConstant(isConstant) {}

}  // namespace Opal