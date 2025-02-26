#pragma once

#include "TokenizerBase.hpp"

namespace Opal {

class StringTokenizer : public TokenizerBase {
public:
    using TokenizerBase::TokenizerBase;

    bool canHandle(char c) const override;
    void tokenize() override;
};

}  // namespace Opal