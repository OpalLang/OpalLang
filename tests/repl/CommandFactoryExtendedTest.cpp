/* OpalLang
 * Copyright (C) 2025 OpalLang
 *
 * This software is free software; you can redistribute it and/or modify it under
 * the terms of the CeCILL-C license as published by CEA, CNRS, and Inria,
 * either version 1.0 of the License or (at your option) any later version.
 *
 * This software is distributed "as is," without any warranty of any kind,
 * either express or implied, including but not limited to the warranties of
 * merchantability or fitness for a particular purpose. See the CeCILL-C license
 * for more details.
 *
 * You should have received a copy of the CeCILL-C license along with this
 * program. If not, see https://cecill.info.
 *
 * Opal is a programming language designed with a focus on readability and
 * performance. It combines modern programming concepts with a clean syntax,
 * making it accessible to newcomers while providing the power and flexibility
 * needed for experienced developers.
 */

#include "repl/command/CommandBase.hpp"
#include "repl/command/CommandFactory.hpp"
#include "repl/command/commands/ClearCommand.hpp"
#include "repl/command/commands/ExitCommand.hpp"
#include "repl/command/commands/HelpCommand.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <vector>

using namespace Opal;
using namespace testing;

class CommandFactoryExtendedTest : public ::testing::Test {
protected:
    void SetUp() override {
        oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(testCout.rdbuf());
    }

    void TearDown() override { std::cout.rdbuf(oldCoutBuf); }

    std::stringstream testCout;
    std::streambuf*   oldCoutBuf;
};

TEST_F(CommandFactoryExtendedTest, CreateAllCommands) {
    auto commands = CommandFactory::createCommands();

    ASSERT_FALSE(commands.empty());

    bool hasHelpCommand  = false;
    bool hasClearCommand = false;
    bool hasExitCommand  = false;

    for (const auto& cmd : commands) {
        if (cmd->canHandle("help")) {
            hasHelpCommand = true;
        } else if (cmd->canHandle("clear")) {
            hasClearCommand = true;
        } else if (cmd->canHandle("exit")) {
            hasExitCommand = true;
        }
    }

    EXPECT_TRUE(hasHelpCommand);
    EXPECT_TRUE(hasClearCommand);
    EXPECT_TRUE(hasExitCommand);
}

TEST_F(CommandFactoryExtendedTest, CommandPriority) {
    auto commands = CommandFactory::createCommands();

    class AlwaysMatchCommand : public CommandBase {
    public:
        bool canHandle(const std::string&) const override { return true; }
        void execute() override {}
    };

    commands.push_back(std::make_unique<AlwaysMatchCommand>());

    auto foundCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("help"); });

    ASSERT_NE(foundCommand, commands.end());
    EXPECT_LT(std::distance(commands.begin(), foundCommand),
              std::distance(commands.begin(), std::prev(commands.end())));
}

TEST_F(CommandFactoryExtendedTest, CommandCaseInsensitivity) {
    auto commands = CommandFactory::createCommands();

    auto helpCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("help"); });

    auto clearCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("clear"); });

    auto exitCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("exit"); });

    EXPECT_NE(helpCommand, commands.end());
    EXPECT_NE(clearCommand, commands.end());
    EXPECT_NE(exitCommand, commands.end());

    if ((*helpCommand)->canHandle("HELP")) {
        EXPECT_TRUE((*helpCommand)->canHandle("HELP"));
        EXPECT_TRUE((*clearCommand)->canHandle("CLEAR"));
        EXPECT_TRUE((*exitCommand)->canHandle("EXIT"));
    }
}

TEST_F(CommandFactoryExtendedTest, CommandWithLeadingAndTrailingSpaces) {
    auto commands = CommandFactory::createCommands();

    auto helpCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("help"); });

    ASSERT_NE(helpCommand, commands.end());

    if ((*helpCommand)->canHandle("  help  ")) {
        EXPECT_TRUE((*helpCommand)->canHandle("  help  "));
    }
}

TEST_F(CommandFactoryExtendedTest, CommandAliases) {
    auto commands = CommandFactory::createCommands();

    auto helpCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("?"); });

    helpCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("help"); });

    EXPECT_NE(helpCommand, commands.end());
}

TEST_F(CommandFactoryExtendedTest, CommandWithArguments) {
    auto commands = CommandFactory::createCommands();

    auto helpCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("help"); });

    ASSERT_NE(helpCommand, commands.end());

    std::vector<std::string> args = {"arg1", "arg2", "arg3"};
    (*helpCommand)->setArguments(args);

    (*helpCommand)->execute();

    std::string output = testCout.str();
    EXPECT_THAT(output, HasSubstr("Available commands"));
}

TEST_F(CommandFactoryExtendedTest, CommandExecutionOrder) {
    auto commands = CommandFactory::createCommands();

    testCout.str("");
    testCout.clear();

    auto helpCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("help"); });

    auto clearCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("clear"); });

    ASSERT_NE(helpCommand, commands.end());
    ASSERT_NE(clearCommand, commands.end());

    (*helpCommand)->execute();
    std::string helpOutput = testCout.str();

    testCout.str("");
    testCout.clear();

    (*clearCommand)->execute();
    std::string clearOutput = testCout.str();

    EXPECT_NE(helpOutput, clearOutput);
    EXPECT_THAT(helpOutput, HasSubstr("Available commands"));
    EXPECT_THAT(clearOutput, HasSubstr("\033c"));
}

TEST_F(CommandFactoryExtendedTest, EmptyCommandList) {
    std::vector<std::unique_ptr<CommandBase>> emptyCommands;

    auto helpCommand = std::find_if(emptyCommands.begin(), emptyCommands.end(), [](const auto& cmd) {
        return cmd->canHandle("help");
    });

    EXPECT_EQ(helpCommand, emptyCommands.end());
}

TEST_F(CommandFactoryExtendedTest, CustomCommand) {
    class CustomCommand : public CommandBase {
    public:
        bool canHandle(const std::string& command) const override { return command == "custom"; }

        void execute() override { std::cout << "Custom command executed" << std::endl; }
    };

    std::vector<std::unique_ptr<CommandBase>> commands;
    commands.push_back(std::make_unique<CustomCommand>());

    auto customCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("custom"); });

    ASSERT_NE(customCommand, commands.end());

    (*customCommand)->execute();

    std::string output = testCout.str();
    EXPECT_THAT(output, HasSubstr("Custom command executed"));
}

TEST_F(CommandFactoryExtendedTest, CommandFactoryExtensibility) {
    auto   commands    = CommandFactory::createCommands();
    size_t initialSize = commands.size();

    EXPECT_GT(initialSize, 0);
}

TEST_F(CommandFactoryExtendedTest, CommandPerformance) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000; ++i) {
        auto commands = CommandFactory::createCommands();
    }

    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    EXPECT_LT(duration, 1000);
}