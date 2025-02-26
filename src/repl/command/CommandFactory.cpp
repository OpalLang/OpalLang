#include "CommandFactory.hpp"

#include "commands/ClearCommand.hpp"
#include "commands/ExitCommand.hpp"
#include "commands/HelpCommand.hpp"
namespace Opal {

std::vector<std::unique_ptr<CommandBase>> CommandFactory::createCommands() {
    std::vector<std::unique_ptr<CommandBase>> commands;

    commands.push_back(std::make_unique<HelpCommand>());
    commands.push_back(std::make_unique<ExitCommand>());
    commands.push_back(std::make_unique<ClearCommand>());

    return commands;
}

}  // namespace Opal