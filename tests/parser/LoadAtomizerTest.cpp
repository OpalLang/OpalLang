#include <gtest/gtest.h>
#include "../../src/lexer/Token.hpp"
#include "../../src/parser/atomizer/atomizers/LoadAtomizer.hpp"
#include "../../src/parser/node/nodes/LoadNode.hpp"

namespace opal::Test {

class LoadAtomizerTest : public ::testing::Test {
protected:
    std::vector<Token> tokens;
    size_t current = 0;

    void SetUp() override {
        tokens.clear();
        current = 0;
    }
};

TEST_F(LoadAtomizerTest, BasicLoad) {
    // load "script.opal"
    tokens.emplace_back(TokenType::LOAD, "load", 1, 1);
    tokens.emplace_back(TokenType::STRING, "script.opal", 1, 6);

    LoadAtomizer atomizer(current, tokens);
    EXPECT_TRUE(atomizer.canHandle(tokens[current].type));

    std::unique_ptr<NodeBase> node = atomizer.atomize();
    opal::LoadNode* loadNode = dynamic_cast<LoadNode*>(node.get());
    ASSERT_NE(loadNode, nullptr);
    EXPECT_EQ(loadNode->getPath(), "script.opal");
}

TEST_F(LoadAtomizerTest, LoadWithoutString) {
    // load 42
    tokens.emplace_back(TokenType::LOAD, "load", 1, 1);
    tokens.emplace_back(TokenType::NUMBER, "42", 1, 6);

    LoadAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

TEST_F(LoadAtomizerTest, LoadNoTokenAfter) {
    // load
    tokens.emplace_back(TokenType::LOAD, "load", 1, 1);

    LoadAtomizer atomizer(current, tokens);
    EXPECT_THROW(atomizer.atomize(), std::runtime_error);
}

} // namespace Opal::Test
