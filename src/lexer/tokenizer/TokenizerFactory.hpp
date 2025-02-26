#pragma once

#include "TokenizerBase.hpp"
#include "tokenizers/CommentTokenizer.hpp"
#include "tokenizers/IdentifierTokenizer.hpp"
#include "tokenizers/NumberTokenizer.hpp"
#include "tokenizers/OperatorTokenizer.hpp"
#include "tokenizers/StringTokenizer.hpp"

#include <memory>
#include <vector>

namespace Opal {

class TokenizerFactory {
public:
    static std::vector<std::unique_ptr<TokenizerBase>> createTokenizers(const std::string&  source,
                                                                        int&                current,
                                                                        int&                line,
                                                                        int&                column,
                                                                        int&                start,
                                                                        std::vector<Token>& tokens);
};

}  // namespace Opal