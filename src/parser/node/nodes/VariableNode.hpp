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
#include "opal/parser/node/nodes/OperationNode.hpp"

#include <memory>
#include <string>

namespace opal {

    class VariableNode : public NodeBase {
        private:
            std::string _name;
            std::string _value;
            bool        _isConstant;
            VariableType _type;
            std::unique_ptr<OperationNode> _operation;

        public:
            VariableNode(TokenType          tokenType,
                        const std::string& name,
                        const std::string& value,
                        bool               isConstant = false,
                        VariableType       type       = VariableType::UNKNOWN);

            void               setOperation(std::unique_ptr<OperationNode> op) { _operation = std::move(op); }
            void               setValue(const std::string& newValue) { _value = newValue; }
            void               setType(VariableType newType) { _type = newType; }
            OperationNode*     getOperation() const { return _operation.get(); }
            const std::string& getName() const { return _name; }
            const std::string& getValue() const { return _value; }
            VariableType       getType() const { return _type; }
            bool               getIsConstant() const { return _isConstant; }

            void print(size_t indent = 0) const override;
    };

}  // namespace opal