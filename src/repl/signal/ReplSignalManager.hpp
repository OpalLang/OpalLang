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

#include "opal/repl/signal/SignalHandler.hpp"

#include <atomic>
#include <csignal>

namespace opal {

/**
 * @class ReplSignalManager
 * @brief Manages signal handling for the REPL environment
 *
 * Handles system signals such as SIGINT (Ctrl+C) and SIGTERM to provide
 * graceful interruption and termination of the REPL.
 */
class ReplSignalManager {
private:
    std::atomic<bool> _interruptRequested;
    std::atomic<bool> _exitRequested;

public:
    /**
     * @brief Constructs a new ReplSignalManager object
     */
    ReplSignalManager();

    /**
     * @brief Destroys the ReplSignalManager object
     */
    ~ReplSignalManager();

    /**
     * @brief Sets up the signal handlers for the REPL
     */
    void setupSignalHandlers();

    /**
     * @brief Checks if an interrupt was requested
     * @return bool True if an interrupt was requested, false otherwise
     */
    bool isInterruptRequested() const;

    /**
     * @brief Resets the interrupt flag
     */
    void resetInterruptFlag();

    /**
     * @brief Checks if the REPL should exit
     * @return bool True if the REPL should exit, false otherwise
     */
    bool shouldExit() const;

    /**
     * @brief Sets the exit flag
     * @param value The value to set the exit flag to
     */
    void setExitFlag(bool value);

    /**
     * @brief Handles interrupt signals (SIGINT)
     * @param signal The signal number
     */
    void handleInterrupt(int signal);

    /**
     * @brief Handles termination signals (SIGTERM)
     * @param signal The signal number
     */
    void handleTerminate(int signal);
};

}  // namespace opal