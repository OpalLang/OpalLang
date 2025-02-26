#include "OperatorTokenizer.hpp"

namespace Opal {

const std::unordered_map<std::string_view, TokenType> OperatorTokenizer::operators = {
    {"(", TokenType::LEFT_PAREN},     {")", TokenType::RIGHT_PAREN},  {"{", TokenType::LEFT_BRACE},
    {"}", TokenType::RIGHT_BRACE},    {"[", TokenType::LEFT_BRACKET}, {"]", TokenType::RIGHT_BRACKET},
    {",", TokenType::COMMA},          {".", TokenType::DOT},          {":", TokenType::COLON},
    {";", TokenType::SEMICOLON},      {"+", TokenType::PLUS},         {"-", TokenType::MINUS},
    {"*", TokenType::MULTIPLY},       {"/", TokenType::DIVIDE},       {"%", TokenType::MODULO},
    {"=", TokenType::EQUAL},          {"!", TokenType::NOT},          {"<", TokenType::LESS},
    {">", TokenType::GREATER},        {"&&", TokenType::AND},         {"||", TokenType::OR},
    {"==", TokenType::EQUAL_EQUAL},   {"!=", TokenType::NOT_EQUAL},   {"<=", TokenType::LESS_EQUAL},
    {">=", TokenType::GREATER_EQUAL}, {"^", TokenType::POWER},
};

bool OperatorTokenizer::canHandle(char c) const {
    std::string str(1, c);
    std::string potential = str + peek();
    return operators.find(str) != operators.end() || operators.find(potential) != operators.end();
}

void OperatorTokenizer::tokenize() {
    std::string first(1, advance());

    if (!isAtEnd()) {
        std::string potential = first + peek();
        auto        it        = operators.find(potential);
        if (it != operators.end()) {
            advance();
            addToken(it->second);
            return;
        }
    }

    auto it = operators.find(first);
    if (it != operators.end()) {
        addToken(it->second);
    }
}

}  // namespace Opal