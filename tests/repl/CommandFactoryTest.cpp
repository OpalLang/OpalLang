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

#include "opal/repl/command/CommandFactory.hpp"
#include "opal/repl/command/commands/ClearCommand.hpp"
#include "opal/repl/command/commands/ExitCommand.hpp"
#include "opal/repl/command/commands/HelpCommand.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace opal;
using namespace testing;

class CommandFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CommandFactoryTest, CreateAllCommands) {
    std::vector<std::unique_ptr<CommandBase>> commands = CommandFactory::createCommands();

    ASSERT_FALSE(commands.empty());

    bool hasHelp  = false;
    bool hasClear = false;
    bool hasExit  = false;

    for (const std::unique_ptr<CommandBase>& cmd : commands) {
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
    std::vector<std::unique_ptr<CommandBase>> commands = CommandFactory::createCommands();

    std::set<std::string> handledCommands;
    for (const std::unique_ptr<CommandBase>& cmd : commands) {
        std::vector<std::string> testCommands = {"help", "clear", "exit"};
        for (const std::string& testCmd : testCommands) {
            if (cmd->canHandle(testCmd)) {
                EXPECT_TRUE(handledCommands.insert(testCmd).second)
                    << "Duplicate command handler found for: " << testCmd;
            }
        }
    }
}

TEST_F(CommandFactoryTest, CommandInstantiation) {
    std::vector<std::unique_ptr<CommandBase>> commands = CommandFactory::createCommands();

    for (const std::unique_ptr<CommandBase>& cmd : commands) {
        EXPECT_NE(cmd.get(), nullptr) << "Null command instance found";
    }
}