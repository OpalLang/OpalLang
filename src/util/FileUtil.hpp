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
