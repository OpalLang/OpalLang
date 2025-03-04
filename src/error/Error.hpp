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

#include <iostream>
#include <string>
#include <vector>

namespace opal {

class Error {
public:
    struct ErrorInfo {
        std::string message;
        int         line;
        int         column;
        std::string source;
    };

    static void error(int line, int column, const std::string& message);
    static void lexerError(int line, int column, const std::string& message);
    static void report(int line, int column, const std::string& where, const std::string& message);
    static bool hadError() { return errorOccurred; }
    static void reset() {
        errorOccurred = false;
        errors.clear();
    }
    static const std::vector<ErrorInfo>& getErrors() { return errors; }

private:
    static bool                   errorOccurred;
    static std::vector<ErrorInfo> errors;
};

}  // namespace opal