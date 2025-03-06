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
#include "opal/parser/atomizer/VariableType.hpp"
#include "opal/parser/atomizer/atomizers/VariableAtomizer.hpp"
#include "opal/parser/node/nodes/OperationNode.hpp"
#include "opal/parser/node/nodes/StringNode.hpp"
#include "opal/parser/node/nodes/VariableNode.hpp"

#include <gtest/gtest.h>

namespace opal::Test {

class VariableAtomizerTest : public ::testing::Test {
protected:
    std::vector<Token> tokens;
    size_t             current = 0;

    void SetUp() override {
        tokens.clear();
        current = 0;
    }
};

TEST_F(VariableAtomizerTest, SimpleAssignment) {
    // Testing: x = 42
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 3);
    tokens.emplace_back(TokenType::NUMBER, "42", 1, 5);

    VariableAtomizer atomizer(current, tokens);
    EXPECT_TRUE(atomizer.canHandle(tokens[current].type));

    std::unique_ptr<NodeBase> node = atomizer.atomize();
    ASSERT_NE(node, nullptr);

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "x");
    EXPECT_EQ(varNode->getValue(), "42");
    EXPECT_EQ(varNode->getType(), VariableType::INT);
    EXPECT_FALSE(varNode->getIsConstant());
}

TEST_F(VariableAtomizerTest, ConstAssignment) {
    // Testing: const x = 42
    tokens.emplace_back(TokenType::CONST, "const", 1, 1);
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 7);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 9);
    tokens.emplace_back(TokenType::NUMBER, "42", 1, 11);

    VariableAtomizer atomizer(current, tokens);
    EXPECT_TRUE(atomizer.canHandle(tokens[1].type));

    current                        = 1;
    std::unique_ptr<NodeBase> node = atomizer.atomize();

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_TRUE(varNode->getIsConstant());
    EXPECT_EQ(varNode->getValue(), "42");
    EXPECT_EQ(varNode->getType(), VariableType::INT);
}

TEST_F(VariableAtomizerTest, StringAssignment) {
    // Testing: message = "hello"
    tokens.emplace_back(TokenType::IDENTIFIER, "message", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 9);
    tokens.emplace_back(TokenType::STRING, "hello", 1, 11);

    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node = atomizer.atomize();

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getType(), VariableType::STRING);
    const opal::StringNode* const& stringNode = varNode->getStringNode();
    ASSERT_NE(stringNode, nullptr);
    const std::vector<StringSegment>& segments = stringNode->getSegments();
    ASSERT_EQ(segments.size(), 1);
    EXPECT_EQ(segments[0].type, StringSegmentType::TEXT);
    EXPECT_EQ(segments[0].content, "hello");
}

TEST_F(VariableAtomizerTest, BoolAssignment) {
    // Testing: flag = true
    tokens.emplace_back(TokenType::IDENTIFIER, "flag", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 6);
    tokens.emplace_back(TokenType::TRUE, "true", 1, 8);

    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node = atomizer.atomize();

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getType(), VariableType::BOOL);
    EXPECT_EQ(varNode->getValue(), "true");
}

TEST_F(VariableAtomizerTest, NilAssignment) {
    // Testing: x = nil
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 3);
    tokens.emplace_back(TokenType::NIL, "nil", 1, 5);

    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node = atomizer.atomize();

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getType(), VariableType::NIL);
    EXPECT_EQ(varNode->getValue(), "nil");
}

TEST_F(VariableAtomizerTest, EmptyStringAssignment) {
    // Testing: text = ""
    tokens.emplace_back(TokenType::IDENTIFIER, "text", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 6);
    tokens.emplace_back(TokenType::STRING, "", 1, 8);

    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node = atomizer.atomize();

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getType(), VariableType::STRING);
    const opal::StringNode* const& stringNode = varNode->getStringNode();
    ASSERT_NE(stringNode, nullptr);
    const std::vector<StringSegment>& segments = stringNode->getSegments();
    ASSERT_EQ(segments.size(), 0);
}

TEST_F(VariableAtomizerTest, VariableWithSimpleOperation) {
    // Testing: result = 5 + 3
    tokens.emplace_back(TokenType::IDENTIFIER, "result", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 8);
    tokens.emplace_back(TokenType::NUMBER, "5", 1, 10);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 12);
    tokens.emplace_back(TokenType::NUMBER, "3", 1, 14);

    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node = atomizer.atomize();

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "result");
    EXPECT_EQ(varNode->getType(), VariableType::INT);

    opal::OperationNode* opNode = varNode->getOperation();
    ASSERT_NE(opNode, nullptr);
    const std::vector<Token>& opTokens = opNode->getTokens();
    ASSERT_EQ(opTokens.size(), 3);
    EXPECT_EQ(opTokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(opTokens[0].value, "5");
    EXPECT_EQ(opTokens[1].type, TokenType::PLUS);
    EXPECT_EQ(opTokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(opTokens[2].value, "3");
}

TEST_F(VariableAtomizerTest, VariableWithComplexOperation) {
    // Testing: result = (2 + 2) * 3
    tokens.emplace_back(TokenType::IDENTIFIER, "result", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 8);
    tokens.emplace_back(TokenType::LEFT_PAREN, "(", 1, 10);
    tokens.emplace_back(TokenType::NUMBER, "2", 1, 11);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 13);
    tokens.emplace_back(TokenType::NUMBER, "2", 1, 15);
    tokens.emplace_back(TokenType::RIGHT_PAREN, ")", 1, 16);
    tokens.emplace_back(TokenType::MULTIPLY, "*", 1, 18);
    tokens.emplace_back(TokenType::NUMBER, "3", 1, 20);

    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node = atomizer.atomize();

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "result");
    EXPECT_EQ(varNode->getType(), VariableType::INT);

    opal::OperationNode* opNode = varNode->getOperation();
    ASSERT_NE(opNode, nullptr);
    const std::vector<Token>& opTokens = opNode->getTokens();
    ASSERT_EQ(opTokens.size(), 7);
    EXPECT_EQ(opTokens[0].type, TokenType::LEFT_PAREN);
    EXPECT_EQ(opTokens[1].value, "2");
    EXPECT_EQ(opTokens[2].type, TokenType::PLUS);
    EXPECT_EQ(opTokens[3].value, "2");
    EXPECT_EQ(opTokens[4].type, TokenType::RIGHT_PAREN);
    EXPECT_EQ(opTokens[5].type, TokenType::MULTIPLY);
    EXPECT_EQ(opTokens[6].value, "3");
}

TEST_F(VariableAtomizerTest, VariableReassignment) {
    // Testing: x = y
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 3);
    tokens.emplace_back(TokenType::IDENTIFIER, "y", 1, 5);

    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node = atomizer.atomize();

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "x");
    EXPECT_EQ(varNode->getValue(), "y");
    EXPECT_EQ(varNode->getType(), VariableType::UNKNOWN);
    EXPECT_FALSE(varNode->getIsConstant());
}

TEST_F(VariableAtomizerTest, InvalidVariableAssignment) {
    // Testing missing value after assignment
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 3);

    VariableAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(VariableAtomizerTest, ConstStringWithInterpolation) {
    // Testing: const greeting = "Hello ${name}!"
    tokens.emplace_back(TokenType::CONST, "const", 1, 1);
    tokens.emplace_back(TokenType::IDENTIFIER, "greeting", 1, 7);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 16);
    tokens.emplace_back(TokenType::STRING, "Hello ${name}!", 1, 18);

    VariableAtomizer atomizer(current, tokens);
    EXPECT_TRUE(atomizer.canHandle(tokens[1].type));

    current                        = 1;
    std::unique_ptr<NodeBase> node = atomizer.atomize();

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_TRUE(varNode->getIsConstant());
    EXPECT_EQ(varNode->getType(), VariableType::STRING);
    const opal::StringNode* const& stringNode = varNode->getStringNode();
    ASSERT_NE(stringNode, nullptr);
    const std::vector<StringSegment>& segments = stringNode->getSegments();
    ASSERT_EQ(segments.size(), 3);
    EXPECT_EQ(segments[0].type, StringSegmentType::TEXT);
    EXPECT_EQ(segments[0].content, "Hello ");
    EXPECT_EQ(segments[1].type, StringSegmentType::VARIABLE);
    EXPECT_EQ(segments[1].content, "name");
    EXPECT_EQ(segments[2].type, StringSegmentType::TEXT);
    EXPECT_EQ(segments[2].content, "!");
}

TEST_F(VariableAtomizerTest, MissingIdentifierAfterConst) {
    // Testing: const = 42
    tokens.emplace_back(TokenType::CONST, "const", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 7);
    tokens.emplace_back(TokenType::NUMBER, "42", 1, 9);

    VariableAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(VariableAtomizerTest, MultipleStringInterpolations) {
    // Testing: msg = "${greeting}, ${name}!"
    tokens.emplace_back(TokenType::IDENTIFIER, "msg", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 5);
    tokens.emplace_back(TokenType::STRING, "${greeting}, ${name}!", 1, 7);

    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node = atomizer.atomize();

    opal::VariableNode* varNode = dynamic_cast<opal::VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getType(), VariableType::STRING);
    const opal::StringNode* const& stringNode = varNode->getStringNode();
    ASSERT_NE(stringNode, nullptr);
    const std::vector<StringSegment>& segments = stringNode->getSegments();
    ASSERT_EQ(segments.size(), 4);
    EXPECT_EQ(segments[0].type, StringSegmentType::VARIABLE);
    EXPECT_EQ(segments[0].content, "greeting");
    EXPECT_EQ(segments[1].type, StringSegmentType::TEXT);
    EXPECT_EQ(segments[1].content, ", ");
    EXPECT_EQ(segments[2].type, StringSegmentType::VARIABLE);
    EXPECT_EQ(segments[2].content, "name");
    EXPECT_EQ(segments[3].type, StringSegmentType::TEXT);
    EXPECT_EQ(segments[3].content, "!");
}
}  // namespace opal::Test