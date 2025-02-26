#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace Opal {

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
    static void reset() { errorOccurred = false; }
    static const std::vector<ErrorInfo>& getErrors() { return errors; }

private:
    static bool                   errorOccurred;
    static std::vector<ErrorInfo> errors;
};

}  // namespace Opal