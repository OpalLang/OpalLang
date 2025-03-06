#include "opal/parser/atomizer/atomizers/VariableAtomizer.hpp"
#include "opal/parser/node/nodes/ArgumentNode.hpp"
#include "opal/parser/node/nodes/CallNode.hpp"
#include "opal/parser/node/nodes/VariableNode.hpp"

#include <gtest/gtest.h>

using namespace opal;

class VariableAtomizerWithCallTest : public ::testing::Test {
protected:
    std::vector<Token> tokens;
    size_t             current = 0;
};

TEST_F(VariableAtomizerWithCallTest, SimpleFunctionCallAssignment) {
    tokens = {{TokenType::IDENTIFIER, "result", 1, 1},
              {TokenType::EQUAL, "=", 1, 8},
              {TokenType::IDENTIFIER, "foo", 1, 10},
              {TokenType::LEFT_PAREN, "(", 1, 13},
              {TokenType::RIGHT_PAREN, ")", 1, 14}};
    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node    = atomizer.atomize();
    VariableNode*             varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "result");
    EXPECT_EQ(varNode->getType(), VariableType::CALL);
    CallNode* callNode = varNode->getCallNode();
    ASSERT_NE(callNode, nullptr);
    EXPECT_EQ(callNode->getName(), "foo");
    EXPECT_EQ(callNode->getArgumentCount(), 0);
}

TEST_F(VariableAtomizerWithCallTest, FunctionCallWithArgumentsAssignment) {
    tokens = {{TokenType::IDENTIFIER, "result", 1, 1},
              {TokenType::EQUAL, "=", 1, 8},
              {TokenType::IDENTIFIER, "foo", 1, 10},
              {TokenType::LEFT_PAREN, "(", 1, 13},
              {TokenType::NUMBER, "42", 1, 14},
              {TokenType::COMMA, ",", 1, 16},
              {TokenType::STRING, "bar", 1, 18},
              {TokenType::RIGHT_PAREN, ")", 1, 23}};
    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node    = atomizer.atomize();
    VariableNode*             varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "result");
    EXPECT_EQ(varNode->getType(), VariableType::CALL);
    CallNode* callNode = varNode->getCallNode();
    ASSERT_NE(callNode, nullptr);
    EXPECT_EQ(callNode->getName(), "foo");

    ASSERT_EQ(callNode->getArgumentCount(), 2);

    ASSERT_TRUE(callNode->getArgumentAt(0).isToken());
    EXPECT_EQ(callNode->getArgumentAt(0).getToken().value, "42");

    ASSERT_TRUE(callNode->getArgumentAt(1).isToken());
    EXPECT_EQ(callNode->getArgumentAt(1).getToken().value, "bar");
}

TEST_F(VariableAtomizerWithCallTest, NestedFunctionCallAssignment) {
    tokens = {{TokenType::IDENTIFIER, "result", 1, 1},
              {TokenType::EQUAL, "=", 1, 8},
              {TokenType::IDENTIFIER, "foo", 1, 10},
              {TokenType::LEFT_PAREN, "(", 1, 13},
              {TokenType::IDENTIFIER, "bar", 1, 14},
              {TokenType::LEFT_PAREN, "(", 1, 17},
              {TokenType::NUMBER, "42", 1, 18},
              {TokenType::RIGHT_PAREN, ")", 1, 20},
              {TokenType::RIGHT_PAREN, ")", 1, 21}};
    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node    = atomizer.atomize();
    VariableNode*             varNode = dynamic_cast<VariableNode*>(node.get());
    ASSERT_NE(varNode, nullptr);
    EXPECT_EQ(varNode->getName(), "result");
    EXPECT_EQ(varNode->getType(), VariableType::CALL);

    CallNode* callNode = varNode->getCallNode();
    ASSERT_NE(callNode, nullptr);
    EXPECT_EQ(callNode->getName(), "foo");

    ASSERT_EQ(callNode->getArgumentCount(), 1);

    const ArgumentNode& firstArg = callNode->getArgumentAt(0);
    ASSERT_TRUE(firstArg.isCall());
    ASSERT_FALSE(firstArg.isToken());

    const CallNode& nestedCall = firstArg.getCall();
    EXPECT_EQ(nestedCall.getName(), "bar");

    ASSERT_EQ(nestedCall.getArgumentCount(), 1);
    const ArgumentNode& nestedArg = nestedCall.getArgumentAt(0);
    ASSERT_TRUE(nestedArg.isToken());
    EXPECT_EQ(nestedArg.getToken().value, "42");
}

TEST_F(VariableAtomizerWithCallTest, MultipleNestedFunctionCalls) {
    tokens = {{TokenType::IDENTIFIER, "result", 1, 1},
              {TokenType::EQUAL, "=", 1, 8},
              {TokenType::IDENTIFIER, "foo", 1, 10},
              {TokenType::LEFT_PAREN, "(", 1, 13},
              {TokenType::NUMBER, "10", 1, 14},
              {TokenType::COMMA, ",", 1, 16},
              {TokenType::IDENTIFIER, "bar", 1, 18},
              {TokenType::LEFT_PAREN, "(", 1, 21},
              {TokenType::NUMBER, "42", 1, 22},
              {TokenType::RIGHT_PAREN, ")", 1, 24},
              {TokenType::COMMA, ",", 1, 25},
              {TokenType::STRING, "test", 1, 27},
              {TokenType::RIGHT_PAREN, ")", 1, 33}};

    VariableAtomizer          atomizer(current, tokens);
    std::unique_ptr<NodeBase> node    = atomizer.atomize();
    VariableNode*             varNode = dynamic_cast<VariableNode*>(node.get());

    ASSERT_NE(varNode, nullptr);
    CallNode* callNode = varNode->getCallNode();
    ASSERT_NE(callNode, nullptr);
    EXPECT_EQ(callNode->getName(), "foo");

    ASSERT_EQ(callNode->getArgumentCount(), 3);

    ASSERT_TRUE(callNode->getArgumentAt(0).isToken());
    EXPECT_EQ(callNode->getArgumentAt(0).getToken().value, "10");

    ASSERT_TRUE(callNode->getArgumentAt(1).isCall());
    const CallNode& nestedCall = callNode->getArgumentAt(1).getCall();
    EXPECT_EQ(nestedCall.getName(), "bar");
    ASSERT_EQ(nestedCall.getArgumentCount(), 1);
    EXPECT_EQ(nestedCall.getArgumentAt(0).getToken().value, "42");

    ASSERT_TRUE(callNode->getArgumentAt(2).isToken());
    EXPECT_EQ(callNode->getArgumentAt(2).getToken().value, "test");
}
