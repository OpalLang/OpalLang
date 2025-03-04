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

    /**
     * @class VariableNode
     * @brief AST node representing a variable declaration or reference
     * 
     * Represents a variable in Opal, including its name, value, type,
     * and whether it's a constant. Can also store an associated operation
     * for variable initialization.
     */
    class VariableNode : public NodeBase {
        private:
            std::string _name;                        ///< The name of the variable
            std::string _value;                       ///< The value of the variable (as a string)
            bool        _isConstant;                  ///< Whether the variable is constant (cannot be reassigned)
            VariableType _type;                       ///< The data type of the variable
            std::unique_ptr<OperationNode> _operation; ///< Optional operation for variable initialization

        public:
            /**
             * @brief Constructs a new Variable Node object
             * @param tokenType The token type associated with this node
             * @param name The name of the variable
             * @param value The initial value of the variable
             * @param isConstant Whether the variable is constant (cannot be reassigned)
             * @param type The data type of the variable
             */
            VariableNode(TokenType          tokenType,
                        const std::string& name,
                        const std::string& value,
                        bool               isConstant = false,
                        VariableType       type       = VariableType::UNKNOWN);

            /**
             * @brief Sets the operation for variable initialization
             * @param op The operation node
             */
            void setOperation(std::unique_ptr<OperationNode> op) { _operation = std::move(op); }
            
            /**
             * @brief Sets the value of the variable
             * @param newValue The new value
             */
            void setValue(const std::string& newValue) { _value = newValue; }
            
            /**
             * @brief Sets the type of the variable
             * @param newType The new type
             */
            void setType(VariableType newType) { _type = newType; }
            
            /**
             * @brief Gets the operation for variable initialization
             * @return OperationNode* Pointer to the operation node, or nullptr if none
             */
            OperationNode* getOperation() const { return _operation.get(); }
            
            /**
             * @brief Gets the name of the variable
             * @return const std::string& The variable name
             */
            const std::string& getName() const { return _name; }
            
            /**
             * @brief Gets the value of the variable
             * @return const std::string& The variable value
             */
            const std::string& getValue() const { return _value; }
            
            /**
             * @brief Gets the type of the variable
             * @return VariableType The variable type
             */
            VariableType getType() const { return _type; }
            
            /**
             * @brief Checks if the variable is constant
             * @return bool True if the variable is constant, false otherwise
             */
            bool getIsConstant() const { return _isConstant; }

            /**
             * @brief Prints the node to standard output
             * @param indent The indentation level for pretty printing
             */
            void print(size_t indent = 0) const override;
    };

}  // namespace opal