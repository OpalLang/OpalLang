#pragma once

#include "TokenizerBase.hpp"

namespace Opal {

class CommentTokenizer : public TokenizerBase {
public:
    using TokenizerBase::TokenizerBase;

    bool canHandle(char c) const override;
    void tokenize() override;

private:
    void handleSingleLineComment();
    void handleMultiLineComment();
};

}  // namespace Opal