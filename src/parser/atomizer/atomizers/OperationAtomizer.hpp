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

#include <memory>
#include <vector>

namespace opal {

    /**
     * @class OperationAtomizer
     * @brief Atomizer for handling mathematical and logical operations
     * 
     * Processes token sequences that represent operations such as arithmetic,
     * logical comparisons, and other expressions in the Opal language.
     */
    class OperationAtomizer : public AtomizerBase {
        private:
            /**
             * @brief Checks if the given token type represents a valid operand
             * @param type The token type to check
             * @return bool True if the token type is a valid operand
             */
            bool isOperand(TokenType type) const;

            /**
             * @brief Processes a parenthesized expression and adds its tokens to the operation
             * @param operationTokens Vector to store the processed tokens
             */
            void handleParenthesizedExpression(std::vector<Token>& operationTokens);

            /**
             * @brief Processes an operand (number, identifier, or parenthesized expression)
             * @param operationTokens Vector to store the processed tokens
             */
            void handleOperand(std::vector<Token>& operationTokens);

            /**
             * @brief Checks for unmatched right parenthesis in the token sequence
             */
            void checkUnmatchedRightParenthesis() const;

            /**
             * @brief Processes a token and adds it to the operation tokens vector
             * @param tokens Vector to store the processed tokens
             * @param token The token to process
             */
            void handleToken(std::vector<Token>& tokens, const Token& token);

        public:
            /**
             * @brief Constructs a new Operation Atomizer object
             * @param current Reference to the current token index
             * @param tokens Reference to the token collection
             */
            OperationAtomizer(size_t& current, std::vector<Token>& tokens);
            
            /**
             * @brief Checks if this atomizer can handle the given token type
             * @param type The token type to check
             * @return bool True if this atomizer can handle the token type, false otherwise
             */
            bool canHandle(TokenType type) const override;
            
            /**
             * @brief Converts a sequence of tokens into an operation node
             * @return std::unique_ptr<NodeBase> A unique pointer to the created operation node
             */
            std::unique_ptr<NodeBase> atomize() override;
    };

}  // namespace opal