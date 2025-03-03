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

#include "../../../lexer/Token.hpp"
#include "../../atomizer/VariableType.hpp"
#include "../NodeBase.hpp"
#include "OperationNode.hpp"

#include <memory>
#include <string>

namespace opal {

class VariableNode : public NodeBase {
private:
    std::string                    name;
    std::string                    value;
    VariableType                   type;
    bool                           isConst;
    std::unique_ptr<OperationNode> operation;

public:
    VariableNode(TokenType          tokenType,
                 const std::string& name,
                 const std::string& value,
                 bool               isConst = false,
                 VariableType       type    = VariableType::UNKNOWN);

    void               setOperation(std::unique_ptr<OperationNode> op) { operation = std::move(op); }
    void               setValue(const std::string& newValue) { value = newValue; }
    void               setType(VariableType newType) { type = newType; }
    OperationNode*     getOperation() const { return operation.get(); }
    const std::string& getName() const { return name; }
    const std::string& getValue() const { return value; }
    VariableType       getType() const { return type; }
    bool               getIsConst() const { return isConst; }

    void print(size_t indent = 0) const override;
};

}  // namespace opal