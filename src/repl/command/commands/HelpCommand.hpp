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

#include "opal/repl/command/CommandBase.hpp"

#include <string>
#include <unordered_map>

namespace opal {

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

}  // namespace opal