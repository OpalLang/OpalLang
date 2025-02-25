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