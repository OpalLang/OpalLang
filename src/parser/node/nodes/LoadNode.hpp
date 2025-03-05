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

#include <string>

namespace opal {

/**
 * @class LoadNode
 * @brief AST node representing a module/file import statement
 *
 * Represents a 'load' statement in Opal, which imports code from another file.
 */
class LoadNode : public NodeBase {
private:
    std::string_view _path;

public:
    /**
     * @brief Constructs a new Load Node object
     * @param type The token type associated with this node
     * @param path The path to the file being loaded
     */
    LoadNode(TokenType type, const std::string_view& path);

    /**
     * @brief Gets the path of the file being loaded
     * @return const std::string_view& The file path
     */
    const std::string_view& getPath() const { return _path; }

    /**
     * @brief Prints the node to standard output
     * @param indent The indentation level for pretty printing
     */
    void print(size_t indent = 0) const override;
};

}  // namespace opal