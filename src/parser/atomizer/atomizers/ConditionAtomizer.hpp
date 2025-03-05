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
 * @class ConditionAtomizer
 * @brief Atomizer for handling conditional statements
 *
 * Processes token sequences that represent conditional expressions
 * such as if-else statements and conditional operators in the Opal language.
 */
class ConditionAtomizer : public AtomizerBase {
public:
    /**
     * @brief Constructs a new Condition Atomizer object
     * @param current Reference to the current token index
     * @param tokens Reference to the token collection
     */
    ConditionAtomizer(size_t& current, std::vector<Token>& tokens);

    /**
     * @brief Checks if this atomizer can handle the given token type
     * @param type The token type to check
     * @return bool True if this atomizer can handle the token type, false otherwise
     */
    bool canHandle(TokenType type) const override;

    /**
     * @brief Converts a sequence of tokens into a condition node
     * @return std::unique_ptr<NodeBase> A unique pointer to the created condition node
     */
    std::unique_ptr<NodeBase> atomize() override;
};

}  // namespace opal