#pragma once

#include "Token.hpp"
#include "tokenizer/TokenizerFactory.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Opal {

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> scanTokens();
    void               printTokens() const;

private:
    std::string                                 source;
    std::vector<Token>                          tokens;
    std::vector<std::unique_ptr<TokenizerBase>> tokenizers;
    int                                         start   = 0;
    int                                         current = 0;
    int                                         line    = 1;
    int                                         column  = 1;

    void scanToken();
    bool isAtEnd() const;
};

}  // namespace Opal