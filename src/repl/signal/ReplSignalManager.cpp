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

#include "opal/repl/signal/ReplSignalManager.hpp"

#include <iostream>

using namespace opal;

static ReplSignalManager* activeManager = nullptr;

static void staticInterruptHandler(int signal) {
    if (activeManager) {
        activeManager->handleInterrupt(signal);
    }
}

static void staticTerminateHandler(int signal) {
    if (activeManager) {
        activeManager->handleTerminate(signal);
    }
}

ReplSignalManager::ReplSignalManager() : _interruptRequested(false), _exitRequested(false) {
    activeManager = this;
    SignalHandler::initialize();
}

ReplSignalManager::~ReplSignalManager() {
    SignalHandler::restoreAllDefaults();

    if (activeManager == this) {
        activeManager = nullptr;
    }
}

void ReplSignalManager::setupSignalHandlers() {
    SignalHandler::registerHandler(SIGINT, staticInterruptHandler);
    SignalHandler::registerHandler(SIGTERM, staticTerminateHandler);
}

bool ReplSignalManager::isInterruptRequested() const {
    return _interruptRequested.load();
}

void ReplSignalManager::resetInterruptFlag() {
    _interruptRequested.store(false);
}

bool ReplSignalManager::shouldExit() const {
    return _exitRequested.load();
}

void ReplSignalManager::setExitFlag(bool value) {
    _exitRequested.store(value);
}

void ReplSignalManager::handleInterrupt(int /*signal*/) {
    _interruptRequested.store(true);

    std::cout << "\nOpal > ";
    std::cout.flush();
}

void ReplSignalManager::handleTerminate(int /*signal*/) {
    _exitRequested.store(true);
}
