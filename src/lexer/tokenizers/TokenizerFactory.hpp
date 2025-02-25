#pragma once

#include "CommentTokenizer.hpp"
#include "IdentifierTokenizer.hpp"
#include "NumberTokenizer.hpp"
#include "OperatorTokenizer.hpp"
#include "StringTokenizer.hpp"
#include "TokenizerBase.hpp"

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