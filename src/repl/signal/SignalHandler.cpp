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

#include "opal/repl/signal/SignalHandler.hpp"

#include <iostream>

using namespace opal;

std::unordered_map<int, SignalHandler::SignalCallback> SignalHandler::_callbacks;

void SignalHandler::initialize() {}

void SignalHandler::registerHandler(int signalType, SignalCallback callback) {
    _callbacks[signalType] = callback;
    std::signal(signalType, handleSignal);
}

void SignalHandler::restoreDefaultHandler(int signalType) {
    std::signal(signalType, SIG_DFL);
    _callbacks.erase(signalType);
}

void SignalHandler::restoreAllDefaults() {
    std::vector<int> signalTypes;
    for (const std::pair<const int, SignalCallback>& pair : _callbacks) {
        signalTypes.push_back(pair.first);
    }

    for (int signalType : signalTypes) {
        restoreDefaultHandler(signalType);
    }
}

void SignalHandler::handleSignal(int signalType) {
    std::unordered_map<int, SignalCallback>::iterator it = _callbacks.find(signalType);

    if (it != _callbacks.end()) {
        it->second(signalType);
    }
}
