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