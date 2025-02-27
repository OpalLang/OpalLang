#pragma once

#include <string>

namespace Opal {

class FileUtil {
public:
    static std::string readFile(const std::string& filepath);
    static void        writeFile(const std::string& filepath, const std::string& content);
    static bool        fileExists(const std::string& filepath);

private:
    FileUtil()                           = delete;
    ~FileUtil()                          = delete;
    FileUtil(const FileUtil&)            = delete;
    FileUtil& operator=(const FileUtil&) = delete;
};

}  // namespace Opal
