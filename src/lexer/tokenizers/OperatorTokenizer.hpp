#pragma once

#include "TokenizerBase.hpp"

#include <string_view>
#include <unordered_map>

namespace Opal {

class OperatorTokenizer : public TokenizerBase {
public:
    using TokenizerBase::TokenizerBase;

    bool canHandle(char c) const override;
    void tokenize() override;

private:
    static const std::unordered_map<std::string_view, TokenType> operators;
};

}  // namespace Opal