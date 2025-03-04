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

#include "opal/parser/atomizer/AtomizerBase.hpp"
#include "opal/parser/node/NodeFactory.hpp"
#include "opal/parser/node/nodes/VariableNode.hpp"

#include <memory>
#include <vector>

namespace opal {

    /**
     * @class VariableAtomizer
     * @brief Atomizer for handling variable declarations and assignments
     * 
     * Processes token sequences that represent variable declarations,
     * assignments, and operations involving variables in the Opal language.
     */
    class VariableAtomizer : public AtomizerBase {
        public:
            /**
             * @brief Constructs a new Variable Atomizer object
             * @param current Reference to the current token index
             * @param tokens Reference to the token collection
             */
            VariableAtomizer(size_t& current, std::vector<Token>& tokens);
            
            /**
             * @brief Checks if this atomizer can handle the given token type
             * @param type The token type to check
             * @return bool True if this atomizer can handle the token type, false otherwise
             */
            bool canHandle(TokenType type) const override;
            
            /**
             * @brief Converts a sequence of tokens into a variable node
             * @return std::unique_ptr<NodeBase> A unique pointer to the created variable node
             */
            std::unique_ptr<NodeBase> atomize() override;

        private:
            /**
             * @brief Handles variable assignment operations
             * @param variableNode Reference to the variable node being processed
             * @return std::unique_ptr<NodeBase> The processed node after assignment
             */
            std::unique_ptr<NodeBase> handleAssignment(std::unique_ptr<VariableNode>& variableNode);
            
            /**
             * @brief Handles operations involving variables
             * @param variableNode Reference to the variable node being processed
             * @return std::unique_ptr<NodeBase> The processed node after operation
             */
            std::unique_ptr<NodeBase> handleOperation(std::unique_ptr<VariableNode>& variableNode);
    };

}  // namespace opal
