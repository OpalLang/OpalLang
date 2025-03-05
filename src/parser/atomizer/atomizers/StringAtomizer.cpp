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

#include "opal/parser/atomizer/atomizers/StringAtomizer.hpp"
#include "opal/parser/node/NodeFactory.hpp"
#include "opal/parser/node/nodes/StringNode.hpp"
#include "opal/util/ErrorUtil.hpp"

using namespace opal;

StringAtomizer::StringAtomizer(size_t& current, std::vector<Token>& tokens) : AtomizerBase(current, tokens) {}

bool StringAtomizer::canHandle(TokenType type) const {
    return type == TokenType::STRING;
}

std::unique_ptr<NodeBase> StringAtomizer::atomize() {
    if (this->_current >= this->_tokens.size()) {
        throw std::runtime_error(ErrorUtil::errorMessage("Unexpected end of input while parsing string",
                                                         this->_tokens[this->_current - 1].line,
                                                         this->_tokens[this->_current - 1].column));
    }

    std::unique_ptr<opal::StringNode> stringNode = NodeFactory::createStringNode();
    parseStringContent(stringNode.get(), std::string(this->_tokens[this->_current].value));

    this->advance();
    return stringNode;
}

void StringAtomizer::parseStringContent(StringNode* stringNode, const std::string& content) {
    size_t pos     = 0;
    size_t lastPos = 0;

    while (pos < content.length()) {
        if (isInterpolationStart(content, pos)) {
            if (pos > lastPos) {
                stringNode->addTextSegment(content.substr(lastPos, pos - lastPos));
            }

            size_t endPos = content.find('}', pos + 2);
            if (endPos == std::string::npos) {
                throw std::runtime_error(ErrorUtil::errorMessage("Unterminated string interpolation",
                                                                 this->_tokens[this->_current].line,
                                                                 this->_tokens[this->_current].column));
            }

            std::string varName = content.substr(pos + 2, endPos - (pos + 2));
            stringNode->addVariableSegment(varName);

            pos     = endPos + 1;
            lastPos = pos;
        } else {
            pos++;
        }
    }

    if (lastPos < content.length()) {
        stringNode->addTextSegment(content.substr(lastPos));
    }
}

bool StringAtomizer::isInterpolationStart(const std::string& content, size_t pos) const {
    return pos + 1 < content.length() && content[pos] == '$' && content[pos + 1] == '{';
}