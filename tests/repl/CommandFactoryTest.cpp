#include "repl/command/CommandFactory.hpp"
#include "repl/command/commands/ClearCommand.hpp"
#include "repl/command/commands/ExitCommand.hpp"
#include "repl/command/commands/HelpCommand.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace Opal;
using namespace testing;

class CommandFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CommandFactoryTest, CreateAllCommands) {
    auto commands = CommandFactory::createCommands();

    ASSERT_FALSE(commands.empty());

    bool hasHelp  = false;
    bool hasClear = false;
    bool hasExit  = false;

    for (const auto& cmd : commands) {
        if (cmd->canHandle("help"))
            hasHelp = true;
        if (cmd->canHandle("clear"))
            hasClear = true;
        if (cmd->canHandle("exit"))
            hasExit = true;
    }

    EXPECT_TRUE(hasHelp) << "Help command not found";
    EXPECT_TRUE(hasClear) << "Clear command not found";
    EXPECT_TRUE(hasExit) << "Exit command not found";
}

TEST_F(CommandFactoryTest, CommandUniqueness) {
    auto commands = CommandFactory::createCommands();

    std::set<std::string> handledCommands;
    for (const auto& cmd : commands) {
        std::vector<std::string> testCommands = {"help", "clear", "exit"};
        for (const auto& testCmd : testCommands) {
            if (cmd->canHandle(testCmd)) {
                EXPECT_TRUE(handledCommands.insert(testCmd).second)
                    << "Duplicate command handler found for: " << testCmd;
            }
        }
    }
}

TEST_F(CommandFactoryTest, CommandInstantiation) {
    auto commands = CommandFactory::createCommands();

    for (const auto& cmd : commands) {
        EXPECT_NE(cmd.get(), nullptr) << "Null command instance found";
    }
}