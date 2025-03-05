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

#include <csignal>
#include <functional>
#include <unordered_map>

namespace opal {

/**
 * @class SignalHandler
 * @brief Static utility class for handling system signals
 *
 * Provides a centralized mechanism for registering and managing
 * signal handlers throughout the application.
 */
class SignalHandler {
public:
    /**
     * @typedef SignalCallback
     * @brief Function type for signal handler callbacks
     */
    using SignalCallback = std::function<void(int)>;

    /**
     * @brief Initializes the signal handling system
     */
    static void initialize();

    /**
     * @brief Registers a handler for a specific signal
     * @param signalType The signal type (e.g., SIGINT, SIGTERM)
     * @param callback The function to call when the signal is received
     */
    static void registerHandler(int signalType, SignalCallback callback);

    /**
     * @brief Restores the default handler for a specific signal
     * @param signalType The signal type to restore
     */
    static void restoreDefaultHandler(int signalType);

    /**
     * @brief Restores all signal handlers to their default behavior
     */
    static void restoreAllDefaults();

    /**
     * @brief Internal function called when a signal is received
     * @param signalType The type of signal that was received
     */
    static void handleSignal(int signalType);

private:
    /**
     * @brief Map of signal types to their handler callbacks
     */
    static std::unordered_map<int, SignalCallback> _callbacks;

    /**
     * @brief Private constructor to prevent instantiation
     */
    SignalHandler() = delete;
};

}  // namespace opal