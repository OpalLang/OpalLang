#pragma once

#include "../CommandBase.hpp"

#include <string>
#include <unordered_map>

namespace Opal {

class HelpCommand : public CommandBase {
private:
    static const std::unordered_map<std::string, std::string> commandDescriptions;

public:
    HelpCommand()           = default;
    ~HelpCommand() override = default;

    bool canHandle(const std::string& commandName) const override {
        return commandName == "help" || commandName == "?";
    }

    void execute() override;
};

}  // namespace Opal