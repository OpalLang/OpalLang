#pragma once

#include "../CommandBase.hpp"

namespace Opal {

class ClearCommand : public CommandBase {
public:
    ClearCommand()           = default;
    ~ClearCommand() override = default;

    bool canHandle(const std::string& commandName) const override;
    void execute() override;
};

}  // namespace Opal