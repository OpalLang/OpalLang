#include "AtomizerFactory.hpp"

#include "atomizers/VariableAtomizer.hpp"

#include <memory>
#include <vector>

namespace Opal {

std::vector<std::unique_ptr<AtomizerBase>> AtomizerFactory::createAtomizers(int& current, std::vector<Token>& tokens) {
    std::vector<std::unique_ptr<AtomizerBase>> atomizers;

    atomizers.push_back(std::make_unique<VariableAtomizer>(current, tokens));
    return atomizers;
}

}  // namespace Opal