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

namespace opal {

    /**
     * @enum NodeType
     * @brief Enumerates the different types of AST nodes
     */
    enum class NodeType {
        BASE,
        VARIABLE,
        OPERATION,
        FUNCTION,
        CLASS
    };

    /**
     * @class NodeBase
     * @brief Base class for all Abstract Syntax Tree nodes
     * 
     * Provides common functionality and interface for all node types
     * in the Abstract Syntax Tree.
     */
    class NodeBase {
       protected:
            NodeType  _nodeType;
            TokenType _tokenType;

        public:
            /**
             * @brief Constructs a new Node Base object
             * @param tokenType The token type associated with this node
             * @param nodeType The type of node (defaults to BASE)
             */
            NodeBase(TokenType tokenType, NodeType nodeType = NodeType::BASE);
            
            /**
             * @brief Virtual destructor for proper inheritance
             */
            virtual ~NodeBase() = default;

            /**
             * @brief Gets the node type
             * @return NodeType The type of this node
             */
            NodeType getNodeType() const { return _nodeType; }
            
            /**
             * @brief Gets the token type
             * @return TokenType The token type associated with this node
             */
            TokenType getTokenType() const { return _tokenType; }

            /**
             * @brief Prints the node to standard output
             * @param indent The indentation level for pretty printing
             */
            virtual void print(size_t indent = 0) const;
            
            /**
             * @brief Utility function to print indentation
             * @param indent The number of spaces to indent
             */
            static void printIndent(size_t indent);
    };

}  // namespace opal