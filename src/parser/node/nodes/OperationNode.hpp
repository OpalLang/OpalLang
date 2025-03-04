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
#include "opal/parser/node/NodeBase.hpp"

#include <vector>

namespace opal {

    /**
     * @class OperationNode
     * @brief AST node representing an operation (expression)
     * 
     * Represents an operation or expression in Opal, such as arithmetic
     * operations, function calls, or any other expression.
     */
    class OperationNode : public NodeBase {
        private:
            std::vector<Token> _tokens; ///< The tokens that make up this operation

        public:
            /**
             * @brief Constructs a new Operation Node object
             * @param tokenType The token type associated with this node
             * @param tokens The tokens that make up this operation
             */
            OperationNode(TokenType tokenType, const std::vector<Token>& tokens);

            /**
             * @brief Gets the tokens that make up this operation
             * @return const std::vector<Token>& The tokens
             */
            const std::vector<Token>& getTokens() const { return _tokens; }
            
            /**
             * @brief Prints the node to standard output
             * @param indent The indentation level for pretty printing
             */
            void print(size_t indent = 0) const override;
    };

}  // namespace opal