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

#include "SignalHandler.hpp"

#include <atomic>
#include <csignal>

namespace opal {

class ReplSignalManager {
public:
    ReplSignalManager();
    ~ReplSignalManager();

    void setupSignalHandlers();
    bool isInterruptRequested() const;
    void resetInterruptFlag();
    bool shouldExit() const;
    void setExitFlag(bool value);

    void handleInterrupt(int signal);
    void handleTerminate(int signal);

private:
    std::atomic<bool> interruptRequested;
    std::atomic<bool> exitRequested;
};

}  // namespace opal