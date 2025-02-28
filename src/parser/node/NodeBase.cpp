#include "NodeBase.hpp"

#include "../../lexer/Token.hpp"

namespace Opal {

NodeBase::NodeBase(TokenType type) : type(type) {}

}  // namespace Opal