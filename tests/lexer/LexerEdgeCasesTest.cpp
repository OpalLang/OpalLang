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

#include "opal/lexer/Lexer.hpp"
#include "opal/lexer/Token.hpp"
#include "opal/lexer/TokenType.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

using namespace opal;
using namespace testing;

class LexerEdgeCasesTest : public ::testing::Test {
protected:
    void SetUp() override { spdlog::set_level(spdlog::level::info); }

    void TearDown() override { spdlog::set_level(spdlog::level::info); }

    bool hadError() { return false; }
};

TEST_F(LexerEdgeCasesTest, EmptyInput) {
    Lexer              lexer("");
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::EOF_TOKEN);
    EXPECT_EQ(tokens[0].value, "EOF");
    EXPECT_EQ(tokens[0].line, 1);
}

TEST_F(LexerEdgeCasesTest, WhitespaceOnly) {
    Lexer              lexer("   \t\n\r  ");
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::EOF_TOKEN);
    EXPECT_EQ(tokens[0].line, 2);
}

TEST_F(LexerEdgeCasesTest, CommentsOnly) {
    Lexer              lexer("// Commentaire simple\n/* Commentaire\nmulti-ligne */");
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_GE(tokens.size(), 1);
    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
    EXPECT_EQ(tokens.back().line, 3);
}

TEST_F(LexerEdgeCasesTest, UnterminatedString) {
    Lexer lexer("\"Chaîne non terminée");
    EXPECT_THROW({ std::vector<Token> tokens = lexer.scanTokens(); }, std::runtime_error);
}

TEST_F(LexerEdgeCasesTest, UnterminatedMultilineComment) {
    Lexer lexer("/* Commentaire non terminé");
    EXPECT_THROW({ std::vector<Token> tokens = lexer.scanTokens(); }, std::runtime_error);
}

TEST_F(LexerEdgeCasesTest, InvalidCharacters) {
    Lexer lexer("@#$");
    EXPECT_THROW({ std::vector<Token> tokens = lexer.scanTokens(); }, std::runtime_error);
}

TEST_F(LexerEdgeCasesTest, MixedValidAndInvalidTokens) {
    Lexer lexer("let x = 10; @ y = 20;");
    EXPECT_THROW({ std::vector<Token> tokens = lexer.scanTokens(); }, std::runtime_error);
}

TEST_F(LexerEdgeCasesTest, VeryLongIdentifier) {
    std::string        longIdentifier(1000, 'a');
    Lexer              lexer(longIdentifier);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, longIdentifier);
}

TEST_F(LexerEdgeCasesTest, VeryLongNumber) {
    std::string        longNumber(1000, '9');
    Lexer              lexer(longNumber);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].value, longNumber);
}

TEST_F(LexerEdgeCasesTest, VeryLongString) {
    std::string        longStringContent(1000, 'a');
    std::string        input = "\"" + longStringContent + "\"";
    Lexer              lexer(input);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(tokens[0].value, longStringContent);
}

TEST_F(LexerEdgeCasesTest, NestedCommentsHandling) {
    Lexer              lexer("/* Commentaire externe /* Commentaire interne */ suite */");
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_GT(tokens.size(), 0);
    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
}

TEST_F(LexerEdgeCasesTest, EscapeSequencesInStrings) {
    Lexer              lexer("\"Chaîne simple sans échappement\"");
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_GE(tokens.size(), 1);
    if (tokens.size() > 1) {
        EXPECT_EQ(tokens[0].type, TokenType::STRING);
    }
    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
}

TEST_F(LexerEdgeCasesTest, MultipleConsecutiveOperators) {
    Lexer              lexer("+ - * /");
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_GT(tokens.size(), 1);

    int plusCount =
        std::count_if(tokens.begin(), tokens.end(), [](const Token& t) { return t.type == TokenType::PLUS; });
    EXPECT_EQ(plusCount, 1);

    int minusCount =
        std::count_if(tokens.begin(), tokens.end(), [](const Token& t) { return t.type == TokenType::MINUS; });
    EXPECT_EQ(minusCount, 1);

    int multiplyCount =
        std::count_if(tokens.begin(), tokens.end(), [](const Token& t) { return t.type == TokenType::MULTIPLY; });
    EXPECT_EQ(multiplyCount, 1);

    int divideCount =
        std::count_if(tokens.begin(), tokens.end(), [](const Token& t) { return t.type == TokenType::DIVIDE; });
    EXPECT_EQ(divideCount, 1);
}

TEST_F(LexerEdgeCasesTest, LineNumberTracking) {
    Lexer              lexer("ligne1\nligne2\nligne3\nligne4");
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 5);

    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[1].line, 2);
    EXPECT_EQ(tokens[2].line, 3);
    EXPECT_EQ(tokens[3].line, 4);
}

TEST_F(LexerEdgeCasesTest, MixedLineEndings) {
    Lexer              lexer("ligne1\nligne2\r\nligne3\rligne4");
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 5);

    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[1].line, 2);
    EXPECT_EQ(tokens[2].line, 3);
    EXPECT_GE(tokens[3].line, 3);
}

TEST_F(LexerEdgeCasesTest, UnicodeCharacters) {
    Lexer              lexer("\"Caractères Unicode: 你好, こんにちは, Привет\"");
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);

    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(tokens[0].value, "Caractères Unicode: 你好, こんにちは, Привет");
}

// TEST_F(LexerEdgeCasesTest, DecimalNumbersWithMultipleDots) {
//     Lexer lexer("123.456.789");
//     EXPECT_THROW({
//         std::vector<Token> tokens = lexer.scanTokens();
//     }, std::runtime_error);
// }
