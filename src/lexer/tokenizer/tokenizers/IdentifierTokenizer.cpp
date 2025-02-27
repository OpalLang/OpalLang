#include "IdentifierTokenizer.hpp"

namespace Opal {

const std::unordered_map<std::string_view, TokenType> IdentifierTokenizer::keywords = {
    {"class", TokenType::CLASS},     {"fn", TokenType::FN},           {"if", TokenType::IF},
    {"elif", TokenType::ELIF},       {"else", TokenType::ELSE},       {"while", TokenType::WHILE},
    {"for", TokenType::FOR},         {"foreach", TokenType::FOREACH}, {"in", TokenType::IN},
    {"try", TokenType::TRY},         {"catch", TokenType::CATCH},     {"finally", TokenType::FINALLY},
    {"ret", TokenType::RET},         {"this", TokenType::THIS},       {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},     {"nil", TokenType::NIL},         {"and", TokenType::AND},
    {"or", TokenType::OR},           {"not", TokenType::NOT},         {"const", TokenType::CONST},
    {"enum", TokenType::ENUM},       {"switch", TokenType::SWITCH},   {"case", TokenType::CASE},
    {"default", TokenType::DEFAULT}, {"break", TokenType::BREAK},     {"continue", TokenType::CONTINUE}};

bool IdentifierTokenizer::canHandle(char c) const {
    return isAlpha(c);
}

void IdentifierTokenizer::tokenize() {
    while (isAlphaNumeric(peek())) {
        advance();
    }

    std::string_view text(source.data() + start, current - start);
    auto             it   = keywords.find(text);
    TokenType        type = it != keywords.end() ? it->second : TokenType::IDENTIFIER;
    addToken(type, text);
}

}  // namespace Opal