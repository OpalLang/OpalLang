#include "TokenizerFactory.hpp"

#include "tokenizers/CommentTokenizer.hpp"
#include "tokenizers/IdentifierTokenizer.hpp"
#include "tokenizers/NumberTokenizer.hpp"
#include "tokenizers/OperatorTokenizer.hpp"
#include "tokenizers/StringTokenizer.hpp"

namespace Opal {

std::vector<std::unique_ptr<TokenizerBase>> TokenizerFactory::createTokenizers(const std::string&  source,
                                                                               int&                current,
                                                                               int&                line,
                                                                               int&                column,
                                                                               int&                start,
                                                                               std::vector<Token>& tokens) {
    std::vector<std::unique_ptr<TokenizerBase>> tokenizers;

    tokenizers.push_back(std::make_unique<CommentTokenizer>(source, current, line, column, start, tokens));
    tokenizers.push_back(std::make_unique<StringTokenizer>(source, current, line, column, start, tokens));
    tokenizers.push_back(std::make_unique<NumberTokenizer>(source, current, line, column, start, tokens));
    tokenizers.push_back(std::make_unique<OperatorTokenizer>(source, current, line, column, start, tokens));
    tokenizers.push_back(std::make_unique<IdentifierTokenizer>(source, current, line, column, start, tokens));

    return tokenizers;
}

}  // namespace Opal