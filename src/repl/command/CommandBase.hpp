#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Opal {

class CommandBase {
protected:
    std::vector<std::string> args;

public:
    CommandBase()          = default;
    virtual ~CommandBase() = default;

    virtual bool canHandle(const std::string& commandName) const = 0;
    virtual void execute()                                       = 0;

    void setArguments(const std::vector<std::string>& arguments) { args = arguments; }
};

}  // namespace Opal