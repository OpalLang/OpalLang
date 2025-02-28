#include "AtomizerFactory.hpp"

#include <memory>
#include <vector>

namespace Opal {

static std::vector<std::unique_ptr<AtomizerBase>> createAtomizers(int& current, std::vector<Token>& tokens) {
    std::vector<std::unique_ptr<AtomizerBase>> atomizers;

    return atomizers;
}

}  // namespace Opal