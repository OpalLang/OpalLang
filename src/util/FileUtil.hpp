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

#include <string>

namespace opal {

/**
 * @class FileUtil
 * @brief Utility class for file operations
 *
 * Provides static methods for common file operations such as reading, writing,
 * and checking file existence. This class cannot be instantiated.
 */
class FileUtil {
private:
    FileUtil()                           = delete;
    ~FileUtil()                          = delete;
    FileUtil(const FileUtil&)            = delete;
    FileUtil& operator=(const FileUtil&) = delete;

public:
    /**
     * @brief Reads the entire content of a file
     * @param filepath Path to the file to read
     * @return std::string The content of the file
     * @throws std::runtime_error If the file cannot be opened or read
     */
    static std::string readFile(const std::string& filepath);

    /**
     * @brief Writes content to a file
     * @param filepath Path to the file to write
     * @param content The content to write to the file
     * @throws std::runtime_error If the file cannot be opened or written to
     */
    static void writeFile(const std::string& filepath, const std::string& content);

    /**
     * @brief Checks if a file exists
     * @param filepath Path to the file to check
     * @return bool True if the file exists, false otherwise
     */
    static bool fileExists(const std::string& filepath);

    /**
     * @brief Checks if a file has a valid Opal extension
     * @param filepath Path to the file to check
     * @return bool True if the file has a valid extension, false otherwise
     */
    static bool hasGoodExtension(const std::string& filepath);
};

}  // namespace opal
