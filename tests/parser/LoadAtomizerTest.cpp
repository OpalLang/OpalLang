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
#include "opal/parser/atomizer/atomizers/LoadAtomizer.hpp"
#include "opal/parser/node/nodes/LoadNode.hpp"

#include <gtest/gtest.h>

namespace opal::Test {

class LoadAtomizerTest : public ::testing::Test {
protected:
    std::vector<Token> tokens;
    size_t             current = 0;

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

    std::unique_ptr<NodeBase> node     = atomizer.atomize();
    opal::LoadNode*           loadNode = dynamic_cast<LoadNode*>(node.get());
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

}  // namespace opal::Test
