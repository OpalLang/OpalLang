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

#include "opal/parser/node/nodes/StringNode.hpp"

#include <spdlog/spdlog.h>
#include <iostream>

using namespace opal;

StringNode::StringNode(TokenType tokenType) : NodeBase(tokenType, NodeType::STRING) {}

void StringNode::addTextSegment(const std::string& text) {
    _segments.push_back({StringSegmentType::TEXT, text});
}

void StringNode::addVariableSegment(const std::string& variableName) {
    _segments.push_back({StringSegmentType::VARIABLE, variableName});
}

void StringNode::print(size_t indent) const {
    this->printIndent(indent);
    spdlog::info("String(segments=[");
    
    for (const StringSegment& segment : _segments) {
        this->printIndent(indent + 1);
        if (segment.type == StringSegmentType::VARIABLE) {
            spdlog::info("Variable(\"{}\")", segment.content);
        } else {
            spdlog::info("Text(\"{}\")", segment.content);
        }
    }
    
    this->printIndent(indent);
    spdlog::info("])");
}