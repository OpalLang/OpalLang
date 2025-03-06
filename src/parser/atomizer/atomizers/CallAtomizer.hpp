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
 */

#pragma once

#include "opal/parser/atomizer/AtomizerBase.hpp"
#include "opal/parser/node/NodeFactory.hpp"
#include "opal/parser/node/nodes/CallNode.hpp"

#include <memory>
#include <vector>

namespace opal {

/**
 * @class CallAtomizer
 * @brief Atomizer for handling function and class calls
 */
class CallAtomizer : public AtomizerBase {
public:
    CallAtomizer(size_t& current, std::vector<Token>& tokens);
    bool                      canHandle(TokenType type) const override;
    std::unique_ptr<NodeBase> atomize() override;

private:
    void processArguments(std::unique_ptr<CallNode>& callNode, const Token& startToken);
};

}  // namespace opal