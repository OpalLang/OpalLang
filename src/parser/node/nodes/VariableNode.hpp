/* OpalLang
 * Copyright (C) 2025 OpalLang
 *
 * This software is free software; you can redistribute it and/or modify it under
 * the terms of the CeCILL-C license as published by CEA, CNRS, and Inria,
 * either version 1.0 of the License or (at your option) any later version.
 *
 * This software is distributed "as is," without any warranty of any kind,
 * either express or implied, including but not limited to the warranties of
 * merchantability or fitness for a particular purpose. See the CeCILL-C license
 * for more details.
 *
 * You should have received a copy of the CeCILL-C license along with this
 * program. If not, see https://cecill.info.
 *
 * Opal is a programming language designed with a focus on readability and
 * performance. It combines modern programming concepts with a clean syntax,
 * making it accessible to newcomers while providing the power and flexibility
 * needed for experienced developers.
 */

#pragma once

#include "opal/lexer/Token.hpp"
#include "opal/parser/atomizer/VariableType.hpp"
#include "opal/parser/node/NodeBase.hpp"
#include "opal/parser/node/nodes/CallNode.hpp"
#include "opal/parser/node/nodes/OperationNode.hpp"
#include "opal/parser/node/nodes/StringNode.hpp"

#include <memory>
#include <string>
#include <variant>

namespace opal {

/**
 * @class VariableNode
 * @brief AST node representing a variable declaration or reference
 *
 * Represents a variable in Opal, including its name, value, type,
 * and whether it's a constant. Uses a variant to store different value types.
 */
class VariableNode : public NodeBase {
private:
    std::string  _name;
    bool         _isConstant;
    VariableType _type;

    std::variant<std::string, std::unique_ptr<OperationNode>, std::unique_ptr<StringNode>, std::unique_ptr<CallNode>>
        _value;

    template <typename T>
    void setValueAndType(T&& value, VariableType type) {
        _value = std::forward<T>(value);
        _type  = type;
    }

public:
    VariableNode(TokenType tokenType, const std::string& name, bool isConstant = false)
        : NodeBase(tokenType), _name(name), _isConstant(isConstant), _type(VariableType::UNKNOWN) {}

    VariableNode(TokenType          tokenType,
                 const std::string& name,
                 const std::string& value,
                 bool               isConstant = false,
                 VariableType       type       = VariableType::UNKNOWN)
        : VariableNode(tokenType, name, isConstant) {
        setValueAndType(value, type);
    }

    VariableNode(TokenType                      tokenType,
                 const std::string&             name,
                 std::unique_ptr<OperationNode> operation,
                 bool                           isConstant = false)
        : VariableNode(tokenType, name, isConstant) {
        setValueAndType(std::move(operation), VariableType::INT);
    }

    VariableNode(TokenType                   tokenType,
                 const std::string&          name,
                 std::unique_ptr<StringNode> stringNode,
                 bool                        isConstant = false)
        : VariableNode(tokenType, name, isConstant) {
        setValueAndType(std::move(stringNode), VariableType::STRING);
    }

    VariableNode(TokenType                 tokenType,
                 const std::string&        name,
                 std::unique_ptr<CallNode> callNode,
                 bool                      isConstant = false)
        : VariableNode(tokenType, name, isConstant) {
        setValueAndType(std::move(callNode), VariableType::CALL);
    }

    bool isSimpleValue() const { return std::holds_alternative<std::string>(_value); }
    bool isOperation() const { return std::holds_alternative<std::unique_ptr<OperationNode>>(_value); }
    bool isStringNode() const { return std::holds_alternative<std::unique_ptr<StringNode>>(_value); }
    bool isCallNode() const { return std::holds_alternative<std::unique_ptr<CallNode>>(_value); }

    void setValue(const std::string& value) { setValueAndType(value, VariableType::UNKNOWN); }
    void setOperation(std::unique_ptr<OperationNode> op) { setValueAndType(std::move(op), VariableType::INT); }
    void setStringNode(std::unique_ptr<StringNode> node) { setValueAndType(std::move(node), VariableType::STRING); }
    void setCallNode(std::unique_ptr<CallNode> node) { setValueAndType(std::move(node), VariableType::CALL); }

    const std::string& getValue() const { return std::get<std::string>(_value); }
    OperationNode*     getOperation() const {
            return isOperation() ? std::get<std::unique_ptr<OperationNode>>(_value).get() : nullptr;
    }
    StringNode* getStringNode() const {
        return isStringNode() ? std::get<std::unique_ptr<StringNode>>(_value).get() : nullptr;
    }
    CallNode* getCallNode() const { return isCallNode() ? std::get<std::unique_ptr<CallNode>>(_value).get() : nullptr; }

    const std::string& getName() const { return _name; }
    VariableType       getType() const { return _type; }
    bool               getIsConstant() const { return _isConstant; }

    void setType(VariableType newType) { _type = newType; }

    void print(size_t indent = 0) const override;
};

}  // namespace opal