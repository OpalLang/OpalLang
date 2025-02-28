#pragma once

#include "AtomizerBase.hpp"

#include <memory>
#include <vector>

namespace Opal {

class AtomizerFactory {
public:
    static std::vector<std::unique_ptr<AtomizerBase>> createAtomizers(int& current, std::vector<Token>& tokens);
};

}  // namespace Opal