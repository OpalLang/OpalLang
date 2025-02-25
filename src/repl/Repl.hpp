#pragma once

#include <string>

namespace Opal {

class Repl {
public:
    void start();
private:
    void run(const std::string& source);
    void runPrompt();
    void runFile(const std::string& path);
};

}