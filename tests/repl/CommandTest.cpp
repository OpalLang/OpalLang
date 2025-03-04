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

#include <sstream>

using namespace opal;
using namespace testing;

class CommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(testCout.rdbuf());
    }

    void TearDown() override { std::cout.rdbuf(oldCoutBuf); }

    std::stringstream testCout;
    std::streambuf*   oldCoutBuf;
};

TEST_F(CommandTest, HelpCommand) {
    auto commands = CommandFactory::createCommands();
    auto helpCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("help"); });

    ASSERT_NE(helpCommand, commands.end());
    (*helpCommand)->execute();

    std::string output = testCout.str();
    EXPECT_THAT(output, HasSubstr("Available commands"));
    EXPECT_THAT(output, HasSubstr("help"));
    EXPECT_THAT(output, HasSubstr("clear"));
    EXPECT_THAT(output, HasSubstr("exit"));
}

TEST_F(CommandTest, ClearCommand) {
    auto commands = CommandFactory::createCommands();
    auto clearCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("clear"); });

    ASSERT_NE(clearCommand, commands.end());
    (*clearCommand)->execute();

    std::string output = testCout.str();
    EXPECT_THAT(output, HasSubstr("\033c"));
}

TEST_F(CommandTest, ExitCommand) {
    auto commands = CommandFactory::createCommands();
    auto exitCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("exit"); });

    ASSERT_NE(exitCommand, commands.end());

    testing::internal::CaptureStdout();
    EXPECT_EXIT((*exitCommand)->execute(), ::testing::ExitedWithCode(0), "");
}

TEST_F(CommandTest, UnknownCommand) {
    auto commands       = CommandFactory::createCommands();
    auto unknownCommand = std::find_if(commands.begin(), commands.end(), [](const auto& cmd) {
        return cmd->canHandle("unknown_command");
    });

    EXPECT_EQ(unknownCommand, commands.end());
}

TEST_F(CommandTest, CommandWithArguments) {
    std::vector<std::string> args     = {"arg1", "arg2"};
    auto                     commands = CommandFactory::createCommands();
    auto                     helpCommand =
        std::find_if(commands.begin(), commands.end(), [](const auto& cmd) { return cmd->canHandle("help"); });

    ASSERT_NE(helpCommand, commands.end());
    (*helpCommand)->setArguments(args);
    (*helpCommand)->execute();

    std::string output = testCout.str();
    EXPECT_THAT(output, HasSubstr("Available commands"));
}