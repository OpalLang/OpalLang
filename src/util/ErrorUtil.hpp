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
     * @class ErrorUtil
     * @brief Utility class for error handling and formatting
     * 
     * Provides static methods for creating formatted error messages.
     * This class cannot be instantiated.
     */
    class ErrorUtil {
        private:
            ErrorUtil()                            = delete;
            ~ErrorUtil()                           = delete;
            ErrorUtil(const ErrorUtil&)            = delete;
            ErrorUtil& operator=(const ErrorUtil&) = delete;

        public:
            /**
             * @brief Creates a formatted error message with location information
             * @param message The error message
             * @param line The line number where the error occurred
             * @param column The column number where the error occurred
             * @return std::string A formatted error message with location information
             */
            static std::string errorMessage(const std::string& message, int line, int column) {
                return message + " at line " + std::to_string(line) + ", column " + std::to_string(column);
            }
    };

}  // namespace opal