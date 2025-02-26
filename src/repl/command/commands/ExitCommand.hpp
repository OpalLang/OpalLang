#pragma once

#include "../CommandBase.hpp"

namespace Opal {

class ExitCommand : public CommandBase {
public:
    ExitCommand()           = default;
    ~ExitCommand() override = default;

    bool canHandle(const std::string& commandName) const override;
    void execute() override;
};

}  // namespace Opal