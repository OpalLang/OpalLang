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

#include "opal/util/FileUtil.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

namespace opal {

std::string FileUtil::readFile(const std::string& filepath) {
    if (!std::filesystem::is_regular_file(filepath)) {
        spdlog::error("The specified path is not a regular file: {}", filepath);
        exit(1);
    }

    std::ifstream file(filepath);
    if (!file.is_open()) {
        spdlog::error("Could not open file: {}", filepath);
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void FileUtil::writeFile(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        spdlog::error("Could not open file for writing: {}", filepath);
        exit(1);
    }
    file << content;
}

bool FileUtil::fileExists(const std::string& filepath) {
    return std::filesystem::is_regular_file(filepath);
}

bool FileUtil::hasGoodExtension(const std::string& filepath) {
    std::string extension = filepath.substr(filepath.find_last_of(".") + 1);
    return extension == "opal" || extension == "op";
}

}  // namespace opal