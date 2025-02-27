#include "FileUtil.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Opal {

std::string FileUtil::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void FileUtil::writeFile(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filepath);
    }
    file << content;
}

bool FileUtil::fileExists(const std::string& filepath) {
    std::ifstream file(filepath);
    return file.good();
}

} // namespace Opal 