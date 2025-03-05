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

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace opal {

/**
 * @class CommandBase
 * @brief Base class for all REPL commands
 *
 * Provides a common interface for all commands that can be executed
 * in the REPL environment, such as 'exit', 'help', 'clear', etc.
 */
class CommandBase {
protected:
    std::vector<std::string> _args;

public:
    /**
     * @brief Default constructor
     */
    CommandBase() = default;

    /**
     * @brief Virtual destructor for proper inheritance
     */
    virtual ~CommandBase() = default;

    /**
     * @brief Checks if this command can handle the given command name
     * @param commandName The name of the command to check
     * @return bool True if this command can handle the given name, false otherwise
     */
    virtual bool canHandle(const std::string& commandName) const = 0;

    /**
     * @brief Executes the command with the current arguments
     */
    virtual void execute() = 0;

    /**
     * @brief Sets the arguments for this command
     * @param arguments The arguments to set
     */
    void setArguments(const std::vector<std::string>& arguments) { _args = arguments; }
};

}  // namespace opal