#include "opal/parser/atomizer/atomizers/CallAtomizer.hpp"
#include "opal/parser/node/nodes/CallNode.hpp"

#include <gtest/gtest.h>

using namespace opal;

class CallAtomizerTest : public ::testing::Test {
protected:
    std::vector<Token> tokens;
    size_t             current = 0;

    void SetUp() override {
        tokens.clear();
        current = 0;
    }
};

TEST_F(CallAtomizerTest, SimpleFunctionCall) {
    tokens = {{TokenType::IDENTIFIER, "foo", 1, 1},
              {TokenType::LEFT_PAREN, "(", 1, 4},
              {TokenType::RIGHT_PAREN, ")", 1, 5}};
    CallAtomizer atomizer(current, tokens);

    std::unique_ptr<CallNode> node = std::unique_ptr<CallNode>(dynamic_cast<CallNode*>(atomizer.atomize().release()));

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getName(), "foo");
    EXPECT_EQ(node->getArgumentCount(), 0);
}

TEST_F(CallAtomizerTest, FunctionCallWithArguments) {
    tokens = {{TokenType::IDENTIFIER, "foo", 1, 1},
              {TokenType::LEFT_PAREN, "(", 1, 4},
              {TokenType::NUMBER, "42", 1, 5},
              {TokenType::COMMA, ",", 1, 7},
              {TokenType::STRING, "bar", 1, 9},
              {TokenType::RIGHT_PAREN, ")", 1, 14}};
    CallAtomizer atomizer(current, tokens);

    std::unique_ptr<CallNode> node = std::unique_ptr<CallNode>(dynamic_cast<CallNode*>(atomizer.atomize().release()));

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getName(), "foo");
    ASSERT_EQ(node->getArgumentCount(), 2);

    const ArgumentNode& arg1 = node->getArgumentAt(0);
    const ArgumentNode& arg2 = node->getArgumentAt(1);

    ASSERT_TRUE(arg1.isToken());
    ASSERT_TRUE(arg2.isToken());
    EXPECT_EQ(arg1.getToken().value, "42");
    EXPECT_EQ(arg2.getToken().value, "bar");
}

TEST_F(CallAtomizerTest, NestedFunctionCall) {
    tokens = {{TokenType::IDENTIFIER, "foo", 1, 1},
              {TokenType::LEFT_PAREN, "(", 1, 4},
              {TokenType::IDENTIFIER, "bar", 1, 5},
              {TokenType::LEFT_PAREN, "(", 1, 8},
              {TokenType::NUMBER, "42", 1, 9},
              {TokenType::RIGHT_PAREN, ")", 1, 11},
              {TokenType::RIGHT_PAREN, ")", 1, 12}};
    CallAtomizer atomizer(current, tokens);

    std::unique_ptr<CallNode> node = std::unique_ptr<CallNode>(dynamic_cast<CallNode*>(atomizer.atomize().release()));

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getName(), "foo");
    ASSERT_EQ(node->getArgumentCount(), 1);

    const ArgumentNode& arg = node->getArgumentAt(0);
    ASSERT_TRUE(arg.isCall());
    ASSERT_FALSE(arg.isToken());

    const CallNode& nestedCall = arg.getCall();
    EXPECT_EQ(nestedCall.getName(), "bar");
    ASSERT_EQ(nestedCall.getArgumentCount(), 1);

    const ArgumentNode& nestedArg = nestedCall.getArgumentAt(0);
    ASSERT_TRUE(nestedArg.isToken());
    EXPECT_EQ(nestedArg.getToken().value, "42");
}

// Error Handling Tests

TEST_F(CallAtomizerTest, MissingLeftParenthesis) {
    tokens = {{TokenType::IDENTIFIER, "foo", 1, 1},
              {TokenType::NUMBER, "42", 1, 5},
              {TokenType::RIGHT_PAREN, ")", 1, 7}};
    CallAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(CallAtomizerTest, MissingRightParenthesis) {
    tokens = {{TokenType::IDENTIFIER, "foo", 1, 1},
              {TokenType::LEFT_PAREN, "(", 1, 4},
              {TokenType::NUMBER, "42", 1, 5}};
    CallAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(CallAtomizerTest, MissingComma) {
    tokens = {{TokenType::IDENTIFIER, "foo", 1, 1},
              {TokenType::LEFT_PAREN, "(", 1, 4},
              {TokenType::NUMBER, "42", 1, 5},
              {TokenType::STRING, "test", 1, 8},
              {TokenType::RIGHT_PAREN, ")", 1, 14}};
    CallAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(CallAtomizerTest, TrailingComma) {
    tokens = {{TokenType::IDENTIFIER, "foo", 1, 1},
              {TokenType::LEFT_PAREN, "(", 1, 4},
              {TokenType::NUMBER, "42", 1, 5},
              {TokenType::COMMA, ",", 1, 7},
              {TokenType::RIGHT_PAREN, ")", 1, 9}};
    CallAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(CallAtomizerTest, EmptyArgumentBetweenCommas) {
    tokens = {{TokenType::IDENTIFIER, "foo", 1, 1},
              {TokenType::LEFT_PAREN, "(", 1, 4},
              {TokenType::NUMBER, "42", 1, 5},
              {TokenType::COMMA, ",", 1, 7},
              {TokenType::COMMA, ",", 1, 8},
              {TokenType::STRING, "test", 1, 10},
              {TokenType::RIGHT_PAREN, ")", 1, 16}};
    CallAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(CallAtomizerTest, MultipleNestedCalls) {
    tokens = {{TokenType::IDENTIFIER, "foo", 1, 1},
              {TokenType::LEFT_PAREN, "(", 1, 4},
              {TokenType::IDENTIFIER, "bar", 1, 5},
              {TokenType::LEFT_PAREN, "(", 1, 8},
              {TokenType::IDENTIFIER, "baz", 1, 9},
              {TokenType::LEFT_PAREN, "(", 1, 12},
              {TokenType::NUMBER, "42", 1, 13},
              {TokenType::RIGHT_PAREN, ")", 1, 15},
              {TokenType::RIGHT_PAREN, ")", 1, 16},
              {TokenType::RIGHT_PAREN, ")", 1, 17}};
    CallAtomizer              atomizer(current, tokens);
    std::unique_ptr<CallNode> node = std::unique_ptr<CallNode>(dynamic_cast<CallNode*>(atomizer.atomize().release()));

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getName(), "foo");
    ASSERT_EQ(node->getArgumentCount(), 1);

    const ArgumentNode& arg1 = node->getArgumentAt(0);
    ASSERT_TRUE(arg1.isCall());

    const CallNode& nestedCall1 = arg1.getCall();
    EXPECT_EQ(nestedCall1.getName(), "bar");
    ASSERT_EQ(nestedCall1.getArgumentCount(), 1);

    const ArgumentNode& arg2 = nestedCall1.getArgumentAt(0);
    ASSERT_TRUE(arg2.isCall());

    const CallNode& nestedCall2 = arg2.getCall();
    EXPECT_EQ(nestedCall2.getName(), "baz");
    ASSERT_EQ(nestedCall2.getArgumentCount(), 1);

    const ArgumentNode& arg3 = nestedCall2.getArgumentAt(0);
    ASSERT_TRUE(arg3.isToken());
    EXPECT_EQ(arg3.getToken().value, "42");
}

TEST_F(CallAtomizerTest, MismatchedParenthesesInNestedCall) {
    tokens = {{TokenType::IDENTIFIER, "foo", 1, 1},
              {TokenType::LEFT_PAREN, "(", 1, 4},
              {TokenType::IDENTIFIER, "bar", 1, 5},
              {TokenType::LEFT_PAREN, "(", 1, 8},
              {TokenType::NUMBER, "42", 1, 9},
              {TokenType::RIGHT_PAREN, ")", 1, 11}};  // Missing closing parenthesis for foo
    CallAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}
