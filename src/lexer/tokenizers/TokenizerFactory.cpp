#include "TokenizerFactory.hpp"

#include "CommentTokenizer.hpp"
#include "IdentifierTokenizer.hpp"
#include "NumberTokenizer.hpp"
#include "OperatorTokenizer.hpp"
#include "StringTokenizer.hpp"

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