#pragma once

#include "../../lexer/Token.hpp"

namespace Opal {

class NodeBase {
    protected:
        TokenType type;

    public:
        NodeBase(TokenType type);
        virtual ~NodeBase() = default;
};

}  // namespace Opal