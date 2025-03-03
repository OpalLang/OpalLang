#pragma once

#include "../../../lexer/Token.hpp"
#include "../NodeBase.hpp"
#include <string>

namespace Opal {

class VariableNode : public NodeBase {
private:
    std::string name;
    std::string value;
    bool isConstant;

public:
    VariableNode(TokenType type, const std::string& name, const std::string& value, bool isConstant = false);
    
    const std::string& getName() const { return name; }
    const std::string& getValue() const { return value; }
    bool getIsConst() const { return isConstant; }
};

}  // namespace Opal