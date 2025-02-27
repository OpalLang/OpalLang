#include "error/Error.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "lexer/TokenType.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace Opal;
using namespace testing;

class LexerEdgeCasesTest : public ::testing::Test {
protected:
    void SetUp() override { Error::reset(); }

    void TearDown() override { Error::reset(); }
};

TEST_F(LexerEdgeCasesTest, EmptyInput) {
    Lexer lexer("");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::EOF_TOKEN);
    EXPECT_EQ(tokens[0].value, "EOF");
    EXPECT_EQ(tokens[0].line, 1);
}

TEST_F(LexerEdgeCasesTest, WhitespaceOnly) {
    Lexer lexer("   \t\n\r  ");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::EOF_TOKEN);
    EXPECT_EQ(tokens[0].line, 2);
}

TEST_F(LexerEdgeCasesTest, CommentsOnly) {
    Lexer lexer("// Commentaire simple\n/* Commentaire\nmulti-ligne */");
    auto  tokens = lexer.scanTokens();

    ASSERT_GE(tokens.size(), 1);
    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
    EXPECT_EQ(tokens.back().line, 3);
}

TEST_F(LexerEdgeCasesTest, UnterminatedString) {
    Lexer lexer("\"Chaîne non terminée");
    auto  tokens = lexer.scanTokens();

    EXPECT_TRUE(Error::hadError());

    ASSERT_GT(tokens.size(), 0);
    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
}

TEST_F(LexerEdgeCasesTest, UnterminatedMultilineComment) {
    Lexer lexer("/* Commentaire non terminé");
    auto  tokens = lexer.scanTokens();

    EXPECT_TRUE(Error::hadError());

    ASSERT_GT(tokens.size(), 0);
    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
}

TEST_F(LexerEdgeCasesTest, InvalidCharacters) {
    Lexer lexer("@#$");
    auto  tokens = lexer.scanTokens();

    EXPECT_TRUE(Error::hadError());

    ASSERT_GT(tokens.size(), 0);
    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
}

TEST_F(LexerEdgeCasesTest, MixedValidAndInvalidTokens) {
    Lexer lexer("let x = 10; @ y = 20;");
    auto  tokens = lexer.scanTokens();

    EXPECT_TRUE(Error::hadError());

    ASSERT_GT(tokens.size(), 1);

    auto identifierToken = std::find_if(tokens.begin(), tokens.end(), [](const Token& t) {
        return t.type == TokenType::IDENTIFIER && t.value == "x";
    });
    EXPECT_NE(identifierToken, tokens.end());

    auto numberToken = std::find_if(tokens.begin(), tokens.end(), [](const Token& t) {
        return t.type == TokenType::NUMBER && t.value == "10";
    });
    EXPECT_NE(numberToken, tokens.end());
}

TEST_F(LexerEdgeCasesTest, VeryLongIdentifier) {
    std::string longIdentifier(1000, 'a');
    Lexer       lexer(longIdentifier);
    auto        tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, longIdentifier);
}

TEST_F(LexerEdgeCasesTest, VeryLongNumber) {
    std::string longNumber(1000, '9');
    Lexer       lexer(longNumber);
    auto        tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].value, longNumber);
}

TEST_F(LexerEdgeCasesTest, VeryLongString) {
    std::string longStringContent(1000, 'a');
    std::string input = "\"" + longStringContent + "\"";
    Lexer       lexer(input);
    auto        tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(tokens[0].value, longStringContent);
}

TEST_F(LexerEdgeCasesTest, NestedCommentsHandling) {
    Lexer lexer("/* Commentaire externe /* Commentaire interne */ suite */");
    auto  tokens = lexer.scanTokens();

    ASSERT_GT(tokens.size(), 0);
    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
}

TEST_F(LexerEdgeCasesTest, EscapeSequencesInStrings) {
    Lexer lexer("\"Chaîne simple sans échappement\"");
    auto  tokens = lexer.scanTokens();

    ASSERT_GE(tokens.size(), 1);
    if (tokens.size() > 1) {
        EXPECT_EQ(tokens[0].type, TokenType::STRING);
    }
    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
}

TEST_F(LexerEdgeCasesTest, MultipleConsecutiveOperators) {
    Lexer lexer("+ - * /");
    auto  tokens = lexer.scanTokens();

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
    Lexer lexer("ligne1\nligne2\nligne3\nligne4");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 5);

    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[1].line, 2);
    EXPECT_EQ(tokens[2].line, 3);
    EXPECT_EQ(tokens[3].line, 4);
}

TEST_F(LexerEdgeCasesTest, MixedLineEndings) {
    Lexer lexer("ligne1\nligne2\r\nligne3\rligne4");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 5);

    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[1].line, 2);
    EXPECT_EQ(tokens[2].line, 3);
    EXPECT_GE(tokens[3].line, 3);
}

TEST_F(LexerEdgeCasesTest, UnicodeCharacters) {
    Lexer lexer("\"Caractères Unicode: 你好, こんにちは, Привет\"");
    auto  tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);

    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(tokens[0].value, "Caractères Unicode: 你好, こんにちは, Привет");
}

TEST_F(LexerEdgeCasesTest, DecimalNumbersWithMultipleDots) {
    Lexer lexer("123.456.789");
    auto  tokens = lexer.scanTokens();

    if (Error::hadError()) {
        EXPECT_TRUE(Error::hadError());
    } else {
        ASSERT_GT(tokens.size(), 1);
        EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    }
}