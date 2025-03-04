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

#include "opal/parser/atomizer/atomizers/LoadAtomizer.hpp"

#include "opal/parser/node/NodeFactory.hpp"

#include <spdlog/spdlog.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace opal {

LoadAtomizer::LoadAtomizer(size_t& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool LoadAtomizer::canHandle(TokenType type) const {
    return type == TokenType::LOAD;
}

std::unique_ptr<NodeBase> LoadAtomizer::atomize() {
    advance();  // consume LOAD token

    if (current >= tokens.size()) {
        throw std::runtime_error("Expected string after load keyword");
    }

    if (tokens[current].type != TokenType::STRING) {
        throw std::runtime_error("Expected string after load keyword");
    }

    std::string_view path = tokens[current].value;
    advance();

    return std::unique_ptr<NodeBase>(NodeFactory::createLoadNode(path).release());
}

}  // namespace opal