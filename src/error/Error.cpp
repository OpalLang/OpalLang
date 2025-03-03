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

#include "Error.hpp"

namespace Opal {

bool                          Error::errorOccurred = false;
std::vector<Error::ErrorInfo> Error::errors;

void Error::error(int line, int column, const std::string& message) {
    report(line, column, "", message);
}

void Error::lexerError(int line, int column, const std::string& message) {
    report(line, column, "Lexical error", message);
}

void Error::report(int line, int column, const std::string& where, const std::string& message) {
    errorOccurred = true;

    ErrorInfo error;
    error.line    = line;
    error.column  = column;
    error.message = message;
    error.source  = where;
    errors.push_back(error);

    std::cerr << "[line " << line << ", column " << column << "] ";
    if (!where.empty()) {
        std::cerr << where << ": ";
    }
    std::cerr << message << std::endl;
}

}  // namespace Opal