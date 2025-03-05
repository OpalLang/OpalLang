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

#include "opal/lexer/Token.hpp"
#include "opal/parser/atomizer/atomizers/StringAtomizer.hpp"
#include "opal/parser/node/nodes/StringNode.hpp"

#include <gtest/gtest.h>

using namespace opal;

class StringAtomizerTest : public ::testing::Test {
protected:
    std::vector<Token> tokens;
    size_t             current = 0;
};

TEST_F(StringAtomizerTest, SimpleString) {
    tokens = {{TokenType::STRING, "Hello World", 1, 1}};
    StringAtomizer atomizer(current, tokens);

    std::unique_ptr<StringNode> node = std::unique_ptr<StringNode>(dynamic_cast<StringNode*>(atomizer.atomize().release()));

    ASSERT_NE(node, nullptr);
    const std::vector<StringSegment>& segments = node->getSegments();
    ASSERT_EQ(segments.size(), 1);
    ASSERT_EQ(segments[0].type, StringSegmentType::TEXT);
    ASSERT_EQ(segments[0].content, "Hello World");
}

TEST_F(StringAtomizerTest, StringWithInterpolation) {
    tokens = {{TokenType::STRING, "Hello ${name}!", 1, 1}};
    StringAtomizer atomizer(current, tokens);

    std::unique_ptr<StringNode> node = std::unique_ptr<StringNode>(dynamic_cast<StringNode*>(atomizer.atomize().release()));

    ASSERT_NE(node, nullptr);
    const std::vector<StringSegment>& segments = node->getSegments();
    ASSERT_EQ(segments.size(), 3);
    ASSERT_EQ(segments[0].type, StringSegmentType::TEXT);
    ASSERT_EQ(segments[0].content, "Hello ");
    ASSERT_EQ(segments[1].type, StringSegmentType::VARIABLE);
    ASSERT_EQ(segments[1].content, "name");
    ASSERT_EQ(segments[2].type, StringSegmentType::TEXT);
    ASSERT_EQ(segments[2].content, "!");
}

TEST_F(StringAtomizerTest, MultipleInterpolations) {
    tokens = {{TokenType::STRING, "${greeting} ${name}! How are ${state}?", 1, 1}};
    StringAtomizer atomizer(current, tokens);

    std::unique_ptr<StringNode> node = std::unique_ptr<StringNode>(dynamic_cast<StringNode*>(atomizer.atomize().release()));

    ASSERT_NE(node, nullptr);
    const std::vector<StringSegment>& segments = node->getSegments();
    ASSERT_EQ(segments.size(), 6);
    ASSERT_EQ(segments[0].type, StringSegmentType::VARIABLE);
    ASSERT_EQ(segments[0].content, "greeting");
    ASSERT_EQ(segments[1].type, StringSegmentType::TEXT);
    ASSERT_EQ(segments[1].content, " ");
    ASSERT_EQ(segments[2].type, StringSegmentType::VARIABLE);
    ASSERT_EQ(segments[2].content, "name");
    ASSERT_EQ(segments[3].type, StringSegmentType::TEXT);
    ASSERT_EQ(segments[3].content, "! How are ");
    ASSERT_EQ(segments[4].type, StringSegmentType::VARIABLE);
    ASSERT_EQ(segments[4].content, "state");
    ASSERT_EQ(segments[5].type, StringSegmentType::TEXT);
    ASSERT_EQ(segments[5].content, "?");
}

TEST_F(StringAtomizerTest, UnterminatedInterpolation) {
    tokens = {{TokenType::STRING, "Hello ${name", 1, 1}};
    StringAtomizer atomizer(current, tokens);

    ASSERT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(StringAtomizerTest, EmptyInterpolation) {
    tokens = {{TokenType::STRING, "Hello ${}", 1, 1}};
    StringAtomizer atomizer(current, tokens);

    std::unique_ptr<StringNode> node = std::unique_ptr<StringNode>(dynamic_cast<StringNode*>(atomizer.atomize().release()));

    ASSERT_NE(node, nullptr);
    const std::vector<StringSegment>& segments = node->getSegments();
    ASSERT_EQ(segments.size(), 2);
    ASSERT_EQ(segments[0].type, StringSegmentType::TEXT);
    ASSERT_EQ(segments[0].content, "Hello ");
    ASSERT_EQ(segments[1].type, StringSegmentType::VARIABLE);
    ASSERT_EQ(segments[1].content, "");
}