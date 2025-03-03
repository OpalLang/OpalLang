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

#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "lexer/TokenType.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace Opal;
using namespace testing;

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(LexerTest, ScanNumbers) {
    Lexer lexer("123 45.67");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 3);

    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].value, "123");
    EXPECT_EQ(tokens[0].line, 1);

    EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[1].value, "45.67");
    EXPECT_EQ(tokens[1].line, 1);

    EXPECT_EQ(tokens[2].type, TokenType::EOF_TOKEN);
    EXPECT_EQ(tokens[2].value, "EOF");
}

TEST_F(LexerTest, ScanStrings) {
    Lexer lexer("\"Hello, World!\" \"Test\"");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 3);

    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(tokens[0].value, "Hello, World!");
    EXPECT_EQ(tokens[0].line, 1);

    EXPECT_EQ(tokens[1].type, TokenType::STRING);
    EXPECT_EQ(tokens[1].value, "Test");
    EXPECT_EQ(tokens[1].line, 1);

    EXPECT_EQ(tokens[2].type, TokenType::EOF_TOKEN);
    EXPECT_EQ(tokens[2].value, "EOF");
}

TEST_F(LexerTest, ScanOperators) {
    Lexer lexer("+ - * / % == != > >= < <=");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 12);

    EXPECT_EQ(tokens[0].type, TokenType::PLUS);
    EXPECT_EQ(tokens[1].type, TokenType::MINUS);
    EXPECT_EQ(tokens[2].type, TokenType::MULTIPLY);
    EXPECT_EQ(tokens[3].type, TokenType::DIVIDE);
    EXPECT_EQ(tokens[4].type, TokenType::MODULO);
    EXPECT_EQ(tokens[5].type, TokenType::EQUAL_EQUAL);
    EXPECT_EQ(tokens[6].type, TokenType::NOT_EQUAL);
    EXPECT_EQ(tokens[7].type, TokenType::GREATER);
    EXPECT_EQ(tokens[8].type, TokenType::GREATER_EQUAL);
    EXPECT_EQ(tokens[9].type, TokenType::LESS);
    EXPECT_EQ(tokens[10].type, TokenType::LESS_EQUAL);

    EXPECT_EQ(tokens[11].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanKeywords) {
    Lexer lexer("if elif else while for class fn ret");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 9);

    EXPECT_EQ(tokens[0].type, TokenType::IF);
    EXPECT_EQ(tokens[1].type, TokenType::ELIF);
    EXPECT_EQ(tokens[2].type, TokenType::ELSE);
    EXPECT_EQ(tokens[3].type, TokenType::WHILE);
    EXPECT_EQ(tokens[4].type, TokenType::FOR);
    EXPECT_EQ(tokens[5].type, TokenType::CLASS);
    EXPECT_EQ(tokens[6].type, TokenType::FN);
    EXPECT_EQ(tokens[7].type, TokenType::RET);

    EXPECT_EQ(tokens[8].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanIdentifiers) {
    Lexer lexer("variable x y123 _test");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 5);

    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "variable");

    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].value, "x");

    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].value, "y123");

    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].value, "_test");

    EXPECT_EQ(tokens[4].type, TokenType::EOF_TOKEN);
    EXPECT_EQ(tokens[4].value, "EOF");
}

TEST_F(LexerTest, ScanSimpleProgram) {
    Lexer lexer(R"(
        fn main() {
            x = 10;
            y = 20;
            if (x < y) {
                ret x + y;
            }
        }
    )");

    auto tokens = lexer.scanTokens();

    ASSERT_GT(tokens.size(), 1);

    auto fnToken = std::find_if(tokens.begin(), tokens.end(), [](const Token& t) { return t.type == TokenType::FN; });
    EXPECT_NE(fnToken, tokens.end());

    auto ifToken = std::find_if(tokens.begin(), tokens.end(), [](const Token& t) { return t.type == TokenType::IF; });
    EXPECT_NE(ifToken, tokens.end());

    auto returnToken =
        std::find_if(tokens.begin(), tokens.end(), [](const Token& t) { return t.type == TokenType::RET; });
    EXPECT_NE(returnToken, tokens.end());

    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanSingleLineComment) {
    Lexer lexer("x = 10; // This is a comment\ny = 20;");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 10);

    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "x");
    EXPECT_EQ(tokens[1].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[2].value, "10");
    EXPECT_EQ(tokens[3].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[4].type, TokenType::COMMENT);
    EXPECT_EQ(tokens[4].value, "// This is a comment");
    EXPECT_EQ(tokens[4].line, 1);

    EXPECT_EQ(tokens[5].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[5].value, "y");
    EXPECT_EQ(tokens[5].line, 2);
    EXPECT_EQ(tokens[6].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[7].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[7].value, "20");
    EXPECT_EQ(tokens[8].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[9].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanMultiLineComment) {
    Lexer lexer("start /* This is a\nmulti-line\ncomment */ end");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 4);

    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "start");
    EXPECT_EQ(tokens[0].line, 1);

    EXPECT_EQ(tokens[1].type, TokenType::COMMENT);
    EXPECT_EQ(tokens[1].value, "/* This is a\nmulti-line\ncomment */");

    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].value, "end");
    EXPECT_EQ(tokens[2].line, 3);

    EXPECT_EQ(tokens[3].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanNestedMultiLineComment) {
    Lexer lexer("before /* outer comment /* nested comment */ still in comment */ after");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 4);

    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "before");

    EXPECT_EQ(tokens[1].type, TokenType::COMMENT);
    EXPECT_EQ(tokens[1].value, "/* outer comment /* nested comment */ still in comment */");

    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].value, "after");

    EXPECT_EQ(tokens[3].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanIncrementDecrementOperators) {
    Lexer lexer("x++ y-- ++a --b");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 9);

    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "x");
    EXPECT_EQ(tokens[1].type, TokenType::INCREMENT);
    EXPECT_EQ(tokens[1].value, "++");

    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].value, "y");
    EXPECT_EQ(tokens[3].type, TokenType::DECREMENT);
    EXPECT_EQ(tokens[3].value, "--");

    EXPECT_EQ(tokens[4].type, TokenType::INCREMENT);
    EXPECT_EQ(tokens[4].value, "++");
    EXPECT_EQ(tokens[5].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[5].value, "a");

    EXPECT_EQ(tokens[6].type, TokenType::DECREMENT);
    EXPECT_EQ(tokens[6].value, "--");
    EXPECT_EQ(tokens[7].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[7].value, "b");

    EXPECT_EQ(tokens[8].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanRangeOperator) {
    Lexer lexer("1..10");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 4);

    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].value, "1");

    EXPECT_EQ(tokens[1].type, TokenType::RANGE);
    EXPECT_EQ(tokens[1].value, "..");

    EXPECT_EQ(tokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[2].value, "10");

    EXPECT_EQ(tokens[3].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanBinaryOperators) {
    Lexer lexer("x & y | z # w ~ k << m >> n");
    auto  tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 14);

    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "x");
    EXPECT_EQ(tokens[1].type, TokenType::BITWISE_AND);
    EXPECT_EQ(tokens[1].value, "&");
    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].value, "y");

    EXPECT_EQ(tokens[3].type, TokenType::BITWISE_OR);
    EXPECT_EQ(tokens[3].value, "|");
    EXPECT_EQ(tokens[4].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[4].value, "z");

    EXPECT_EQ(tokens[5].type, TokenType::BITWISE_XOR);
    EXPECT_EQ(tokens[5].value, "#");
    EXPECT_EQ(tokens[6].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[6].value, "w");

    EXPECT_EQ(tokens[7].type, TokenType::BITWISE_NOT);
    EXPECT_EQ(tokens[7].value, "~");
    EXPECT_EQ(tokens[8].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[8].value, "k");

    EXPECT_EQ(tokens[9].type, TokenType::SHIFT_LEFT);
    EXPECT_EQ(tokens[9].value, "<<");
    EXPECT_EQ(tokens[10].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[10].value, "m");

    EXPECT_EQ(tokens[11].type, TokenType::SHIFT_RIGHT);
    EXPECT_EQ(tokens[11].value, ">>");
    EXPECT_EQ(tokens[12].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[12].value, "n");

    EXPECT_EQ(tokens[13].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanCompoundBinaryOperations) {
    Lexer lexer("(a & b) | (c << 2) >> 1");
    auto  tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 14);

    EXPECT_EQ(tokens[0].type, TokenType::LEFT_PAREN);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].value, "a");
    EXPECT_EQ(tokens[2].type, TokenType::BITWISE_AND);
    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].value, "b");
    EXPECT_EQ(tokens[4].type, TokenType::RIGHT_PAREN);

    EXPECT_EQ(tokens[5].type, TokenType::BITWISE_OR);
    EXPECT_EQ(tokens[6].type, TokenType::LEFT_PAREN);
    EXPECT_EQ(tokens[7].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[7].value, "c");
    EXPECT_EQ(tokens[8].type, TokenType::SHIFT_LEFT);
    EXPECT_EQ(tokens[9].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[9].value, "2");
    EXPECT_EQ(tokens[10].type, TokenType::RIGHT_PAREN);

    EXPECT_EQ(tokens[11].type, TokenType::SHIFT_RIGHT);
    EXPECT_EQ(tokens[12].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[12].value, "1");

    EXPECT_EQ(tokens[13].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanCompoundAssignmentOperators) {
    Lexer lexer("x += 1; y -= 2; z *= 3; w /= 4; a %= 5; b ^= 6; c &= 7; d |= 8; e #= 9; f <<= 10; g >>= 11");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 44);

    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "x");
    EXPECT_EQ(tokens[1].type, TokenType::PLUS_EQUAL);
    EXPECT_EQ(tokens[2].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[2].value, "1");
    EXPECT_EQ(tokens[3].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[4].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[4].value, "y");
    EXPECT_EQ(tokens[5].type, TokenType::MINUS_EQUAL);
    EXPECT_EQ(tokens[6].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[6].value, "2");
    EXPECT_EQ(tokens[7].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[8].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[8].value, "z");
    EXPECT_EQ(tokens[9].type, TokenType::MULTIPLY_EQUAL);
    EXPECT_EQ(tokens[10].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[10].value, "3");
    EXPECT_EQ(tokens[11].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[12].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[12].value, "w");
    EXPECT_EQ(tokens[13].type, TokenType::DIVIDE_EQUAL);
    EXPECT_EQ(tokens[14].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[14].value, "4");
    EXPECT_EQ(tokens[15].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[16].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[16].value, "a");
    EXPECT_EQ(tokens[17].type, TokenType::MODULO_EQUAL);
    EXPECT_EQ(tokens[18].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[18].value, "5");
    EXPECT_EQ(tokens[19].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[20].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[20].value, "b");
    EXPECT_EQ(tokens[21].type, TokenType::POWER_EQUAL);
    EXPECT_EQ(tokens[22].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[22].value, "6");
    EXPECT_EQ(tokens[23].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[24].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[24].value, "c");
    EXPECT_EQ(tokens[25].type, TokenType::AND_EQUAL);
    EXPECT_EQ(tokens[26].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[26].value, "7");
    EXPECT_EQ(tokens[27].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[28].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[28].value, "d");
    EXPECT_EQ(tokens[29].type, TokenType::OR_EQUAL);
    EXPECT_EQ(tokens[30].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[30].value, "8");
    EXPECT_EQ(tokens[31].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[32].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[32].value, "e");
    EXPECT_EQ(tokens[33].type, TokenType::XOR_EQUAL);
    EXPECT_EQ(tokens[34].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[34].value, "9");
    EXPECT_EQ(tokens[35].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[36].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[36].value, "f");
    EXPECT_EQ(tokens[37].type, TokenType::SHIFT_LEFT_EQUAL);
    EXPECT_EQ(tokens[38].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[38].value, "10");
    EXPECT_EQ(tokens[39].type, TokenType::SEMICOLON);

    EXPECT_EQ(tokens[40].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[40].value, "g");
    EXPECT_EQ(tokens[41].type, TokenType::SHIFT_RIGHT_EQUAL);
    EXPECT_EQ(tokens[42].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[42].value, "11");

    EXPECT_EQ(tokens[43].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanNewKeywords) {
    Lexer lexer("const enum switch case default break continue");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 8);

    EXPECT_EQ(tokens[0].type, TokenType::CONST);
    EXPECT_EQ(tokens[0].value, "const");

    EXPECT_EQ(tokens[1].type, TokenType::ENUM);
    EXPECT_EQ(tokens[1].value, "enum");

    EXPECT_EQ(tokens[2].type, TokenType::SWITCH);
    EXPECT_EQ(tokens[2].value, "switch");

    EXPECT_EQ(tokens[3].type, TokenType::CASE);
    EXPECT_EQ(tokens[3].value, "case");

    EXPECT_EQ(tokens[4].type, TokenType::DEFAULT);
    EXPECT_EQ(tokens[4].value, "default");

    EXPECT_EQ(tokens[5].type, TokenType::BREAK);
    EXPECT_EQ(tokens[5].value, "break");

    EXPECT_EQ(tokens[6].type, TokenType::CONTINUE);
    EXPECT_EQ(tokens[6].value, "continue");

    EXPECT_EQ(tokens[7].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanSwitchStatement) {
    Lexer lexer(R"(
        switch (value) {
            case 1:
                ret "one";
            case 2:
                ret "two";
            default:
                ret "other";
        }
    )");
    auto  tokens = lexer.scanTokens();

    auto switchToken =
        std::find_if(tokens.begin(), tokens.end(), [](const Token& t) { return t.type == TokenType::SWITCH; });
    EXPECT_NE(switchToken, tokens.end());

    auto caseTokens =
        std::count_if(tokens.begin(), tokens.end(), [](const Token& t) { return t.type == TokenType::CASE; });
    EXPECT_EQ(caseTokens, 2);

    auto defaultToken =
        std::find_if(tokens.begin(), tokens.end(), [](const Token& t) { return t.type == TokenType::DEFAULT; });
    EXPECT_NE(defaultToken, tokens.end());

    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanEnumDeclaration) {
    Lexer lexer(R"(
        enum Color {
            RED,
            GREEN,
            BLUE
        }
    )");
    auto  tokens = lexer.scanTokens();

    EXPECT_EQ(tokens[0].type, TokenType::ENUM);
    EXPECT_EQ(tokens[0].value, "enum");

    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].value, "Color");

    EXPECT_EQ(tokens[2].type, TokenType::LEFT_BRACE);

    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].value, "RED");
    EXPECT_EQ(tokens[4].type, TokenType::COMMA);

    EXPECT_EQ(tokens[5].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[5].value, "GREEN");
    EXPECT_EQ(tokens[6].type, TokenType::COMMA);

    EXPECT_EQ(tokens[7].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[7].value, "BLUE");

    EXPECT_EQ(tokens[8].type, TokenType::RIGHT_BRACE);
    EXPECT_EQ(tokens[9].type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, ScanLoadStatements) {
    Lexer lexer("load \"my.h\" load \"math\"");
    auto  tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].type, TokenType::LOAD);
    EXPECT_EQ(tokens[0].value, "load");
    EXPECT_EQ(tokens[1].type, TokenType::STRING);
    EXPECT_EQ(tokens[1].value, "my.h");
    EXPECT_EQ(tokens[2].type, TokenType::LOAD);
    EXPECT_EQ(tokens[2].value, "load");
    EXPECT_EQ(tokens[3].type, TokenType::STRING);
    EXPECT_EQ(tokens[3].value, "math");
    EXPECT_EQ(tokens[4].type, TokenType::EOF_TOKEN);
}
