#include <gtest/gtest.h>
#include "../../src/lexer/Token.hpp"
#include "../../src/parser/atomizer/atomizers/VariableAtomizer.hpp"
#include "../../src/parser/atomizer/atomizers/OperationAtomizer.hpp"
#include "../../src/parser/node/nodes/VariableNode.hpp"
#include "../../src/parser/node/nodes/OperationNode.hpp"

namespace Opal::Test {

class AtomizerTest : public ::testing::Test {
protected:
    std::vector<Token> tokens;
    size_t current = 0;

    void SetUp() override {
        tokens.clear();
        current = 0;
    }
};

TEST_F(AtomizerTest, VariableAtomizerSimpleAssignment) {
    // Testing: x = 42
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 3);
    tokens.emplace_back(TokenType::NUMBER, "42", 1, 5);
    
    VariableAtomizer atomizer(current, tokens);
    EXPECT_TRUE(atomizer.canHandle(tokens[current].type));
    
    auto node = atomizer.atomize();
    ASSERT_NE(node, nullptr);
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "x");
    EXPECT_EQ(varNode->getValue(), "42");
    EXPECT_EQ(varNode->getType(), VariableType::INT);
    EXPECT_FALSE(varNode->getIsConstant());
}

TEST_F(AtomizerTest, VariableAtomizerConstAssignment) {
    // Testing: const x = 42
    tokens.emplace_back(TokenType::CONST, "const", 1, 1);
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 7);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 9);
    tokens.emplace_back(TokenType::NUMBER, "42", 1, 11);
    
    VariableAtomizer atomizer(current, tokens);
    EXPECT_TRUE(atomizer.canHandle(tokens[1].type));
    
    current = 1;
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_TRUE(varNode->getIsConstant());
    EXPECT_EQ(varNode->getValue(), "42");
    EXPECT_EQ(varNode->getType(), VariableType::INT);
}

TEST_F(AtomizerTest, VariableAtomizerStringAssignment) {
    // Testing: message = "hello"
    tokens.emplace_back(TokenType::IDENTIFIER, "message", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 9);
    tokens.emplace_back(TokenType::STRING, "hello", 1, 11);
    
    VariableAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getType(), VariableType::STRING);
    EXPECT_EQ(varNode->getValue(), "hello");
}

TEST_F(AtomizerTest, VariableAtomizerBoolAssignment) {
    // Testing: flag = true
    tokens.emplace_back(TokenType::IDENTIFIER, "flag", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 6);
    tokens.emplace_back(TokenType::TRUE, "true", 1, 8);
    
    VariableAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getType(), VariableType::BOOL);
    EXPECT_EQ(varNode->getValue(), "true");
}

TEST_F(AtomizerTest, VariableWithOperation) {
    // Testing: result = 5 + 3
    tokens.emplace_back(TokenType::IDENTIFIER, "result", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 8);
    tokens.emplace_back(TokenType::NUMBER, "5", 1, 10);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 12);
    tokens.emplace_back(TokenType::NUMBER, "3", 1, 14);
    
    VariableAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "result");
    EXPECT_EQ(varNode->getType(), VariableType::INT);
    
    auto* opNode = varNode->getOperation();
    ASSERT_NE(opNode, nullptr);
    
    const auto& opTokens = opNode->getTokens();
    ASSERT_EQ(opTokens.size(), 3);
    EXPECT_EQ(opTokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(opTokens[0].value, "5");
    EXPECT_EQ(opTokens[1].type, TokenType::PLUS);
    EXPECT_EQ(opTokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(opTokens[2].value, "3");
}

TEST_F(AtomizerTest, BasicOperation) {
    // Testing: 2 + 3
    tokens.emplace_back(TokenType::NUMBER, "2", 1, 1);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 3);
    tokens.emplace_back(TokenType::NUMBER, "3", 1, 5);
    
    OperationAtomizer atomizer(current, tokens);
    EXPECT_TRUE(atomizer.canHandle(tokens[1].type));
    
    auto node = atomizer.atomize();
    auto* opNode = dynamic_cast<OperationNode*>(node.get());
    ASSERT_NE(opNode, nullptr);
    
    const auto& opTokens = opNode->getTokens();
    ASSERT_EQ(opTokens.size(), 3);
    EXPECT_EQ(opTokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(opTokens[0].value, "2");
    EXPECT_EQ(opTokens[1].type, TokenType::PLUS);
    EXPECT_EQ(opTokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(opTokens[2].value, "3");
}

TEST_F(AtomizerTest, OperationWithIdentifiers) {
    // Testing: x + y
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 1);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 3);
    tokens.emplace_back(TokenType::IDENTIFIER, "y", 1, 5);
    
    OperationAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* opNode = dynamic_cast<OperationNode*>(node.get());
    ASSERT_NE(opNode, nullptr);
    
    const auto& opTokens = opNode->getTokens();
    ASSERT_EQ(opTokens.size(), 3);
    EXPECT_EQ(opTokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(opTokens[0].value, "x");
    EXPECT_EQ(opTokens[1].type, TokenType::PLUS);
    EXPECT_EQ(opTokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(opTokens[2].value, "y");
}

TEST_F(AtomizerTest, NilAssignment) {
    // Testing: x = nil
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 3);
    tokens.emplace_back(TokenType::NIL, "nil", 1, 5);
    
    VariableAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getType(), VariableType::NIL);
    EXPECT_EQ(varNode->getValue(), "nil");
}

TEST_F(AtomizerTest, InvalidOperation) {
    // Testing invalid operation: missing second operand
    tokens.emplace_back(TokenType::NUMBER, "5", 1, 1);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 3);
    
    OperationAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(AtomizerTest, InvalidVariableAssignment) {
    // Testing missing value after assignment
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 3);
    
    VariableAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(AtomizerTest, MultipleArithmeticOperations) {
    // Testing: result = 5 + 3 - 2
    tokens.emplace_back(TokenType::IDENTIFIER, "result", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 8);
    tokens.emplace_back(TokenType::NUMBER, "5", 1, 10);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 12);
    tokens.emplace_back(TokenType::NUMBER, "3", 1, 14);
    tokens.emplace_back(TokenType::MINUS, "-", 1, 16);
    tokens.emplace_back(TokenType::NUMBER, "2", 1, 18);
    
    VariableAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "result");
    EXPECT_EQ(varNode->getType(), VariableType::INT);
    
    auto* opNode = varNode->getOperation();
    ASSERT_NE(opNode, nullptr);
    
    const auto& opTokens = opNode->getTokens();
    ASSERT_EQ(opTokens.size(), 5);
    EXPECT_EQ(opTokens[0].value, "5");
    EXPECT_EQ(opTokens[1].type, TokenType::PLUS);
    EXPECT_EQ(opTokens[2].value, "3");
    EXPECT_EQ(opTokens[3].type, TokenType::MINUS);
    EXPECT_EQ(opTokens[4].value, "2");
}

TEST_F(AtomizerTest, VariableReassignment) {
    // Testing: x = y
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 3);
    tokens.emplace_back(TokenType::IDENTIFIER, "y", 1, 5);
    
    VariableAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "x");
    EXPECT_EQ(varNode->getValue(), "y");
    EXPECT_EQ(varNode->getType(), VariableType::UNKNOWN);
    EXPECT_FALSE(varNode->getIsConstant());
}

TEST_F(AtomizerTest, MultipleOperationsWithIdentifiers) {
    // Testing: x + y + z
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 1);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 3);
    tokens.emplace_back(TokenType::IDENTIFIER, "y", 1, 5);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 7);
    tokens.emplace_back(TokenType::IDENTIFIER, "z", 1, 9);
    
    OperationAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* opNode = dynamic_cast<OperationNode*>(node.get());
    ASSERT_NE(opNode, nullptr);
    
    const auto& opTokens = opNode->getTokens();
    ASSERT_EQ(opTokens.size(), 5);
    EXPECT_EQ(opTokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(opTokens[0].value, "x");
    EXPECT_EQ(opTokens[1].type, TokenType::PLUS);
    EXPECT_EQ(opTokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(opTokens[2].value, "y");
    EXPECT_EQ(opTokens[3].type, TokenType::PLUS);
    EXPECT_EQ(opTokens[4].type, TokenType::IDENTIFIER);
    EXPECT_EQ(opTokens[4].value, "z");
}

TEST_F(AtomizerTest, MixedOperations) {
    // Testing: result = 42 * x + y
    tokens.emplace_back(TokenType::IDENTIFIER, "result", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 8);
    tokens.emplace_back(TokenType::NUMBER, "42", 1, 10);
    tokens.emplace_back(TokenType::MULTIPLY, "*", 1, 13);
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 15);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 17);
    tokens.emplace_back(TokenType::IDENTIFIER, "y", 1, 19);
    
    VariableAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "result");
    EXPECT_EQ(varNode->getType(), VariableType::INT);
    
    auto* opNode = varNode->getOperation();
    ASSERT_NE(opNode, nullptr);
    
    const auto& opTokens = opNode->getTokens();
    ASSERT_EQ(opTokens.size(), 5);
    EXPECT_EQ(opTokens[0].value, "42");
    EXPECT_EQ(opTokens[1].type, TokenType::MULTIPLY);
    EXPECT_EQ(opTokens[2].value, "x");
    EXPECT_EQ(opTokens[3].type, TokenType::PLUS);
    EXPECT_EQ(opTokens[4].value, "y");
}

TEST_F(AtomizerTest, ConstStringAssignment) {
    // Testing: const message = "hello world"
    tokens.emplace_back(TokenType::CONST, "const", 1, 1);
    tokens.emplace_back(TokenType::IDENTIFIER, "message", 1, 7);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 15);
    tokens.emplace_back(TokenType::STRING, "hello world", 1, 17);
    
    VariableAtomizer atomizer(current, tokens);
    EXPECT_TRUE(atomizer.canHandle(tokens[1].type));
    
    current = 1;
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_TRUE(varNode->getIsConstant());
    EXPECT_EQ(varNode->getValue(), "hello world");
    EXPECT_EQ(varNode->getType(), VariableType::STRING);
}

TEST_F(AtomizerTest, EmptyString) {
    // Testing: text = ""
    tokens.emplace_back(TokenType::IDENTIFIER, "text", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 6);
    tokens.emplace_back(TokenType::STRING, "", 1, 8);
    
    VariableAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getValue(), "");
    EXPECT_EQ(varNode->getType(), VariableType::STRING);
}

TEST_F(AtomizerTest, DivisionOperation) {
    // Testing: result = x / 2
    tokens.emplace_back(TokenType::IDENTIFIER, "result", 1, 1);
    tokens.emplace_back(TokenType::EQUAL, "=", 1, 8);
    tokens.emplace_back(TokenType::IDENTIFIER, "x", 1, 10);
    tokens.emplace_back(TokenType::DIVIDE, "/", 1, 12);
    tokens.emplace_back(TokenType::NUMBER, "2", 1, 14);
    
    VariableAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    
    auto* varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    
    auto* opNode = varNode->getOperation();
    ASSERT_NE(opNode, nullptr);
    
    const auto& opTokens = opNode->getTokens();
    ASSERT_EQ(opTokens.size(), 3);
    EXPECT_EQ(opTokens[0].value, "x");
    EXPECT_EQ(opTokens[1].type, TokenType::DIVIDE);
    EXPECT_EQ(opTokens[2].value, "2");
}

TEST_F(AtomizerTest, OperationAtomizerComplex) {
    // Testing: 2 * 3 + 5
    tokens.emplace_back(TokenType::NUMBER, "2", 1, 1);
    tokens.emplace_back(TokenType::MULTIPLY, "*", 1, 3);
    tokens.emplace_back(TokenType::NUMBER, "3", 1, 5);
    tokens.emplace_back(TokenType::PLUS, "+", 1, 7);
    tokens.emplace_back(TokenType::NUMBER, "5", 1, 9);

    OperationAtomizer atomizer(current, tokens);
    auto node = atomizer.atomize();
    auto* opNode = dynamic_cast<OperationNode*>(node.get());
    ASSERT_NE(opNode, nullptr);

    const auto& opTokens = opNode->getTokens();
    ASSERT_EQ(opTokens.size(), 5);
    EXPECT_EQ(opTokens[0].value, "2");
    EXPECT_EQ(opTokens[1].type, TokenType::MULTIPLY);
    EXPECT_EQ(opTokens[2].value, "3");
    EXPECT_EQ(opTokens[3].type, TokenType::PLUS);
    EXPECT_EQ(opTokens[4].value, "5");
}

} // namespace Opal::Test