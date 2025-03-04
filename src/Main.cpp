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

#include "opal/lexer/Lexer.hpp"
#include "opal/parser/Parser.hpp"
#include "opal/repl/Repl.hpp"
#include "opal/util/FileUtil.hpp"

#include <spdlog/spdlog.h>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    spdlog::set_pattern("[%H:%M:%S] [%^%L%$] %v");
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Opal Language");

    if (argc < 2) {
        opal::Repl repl;

        try {
            repl.start();
        } catch (const std::exception& e) {
            spdlog::error("Error: {}", e.what());
            return 1;
        }
    } else {
        if (!opal::FileUtil::fileExists(argv[1])) {
            std::string error = "File does not exist: " + std::string(argv[1]);
            spdlog::error(error);
            return 1;
        }

        if (!opal::FileUtil::hasGoodExtension(argv[1])) {
            std::string error = "File has an invalid extension: " + std::string(argv[1]);
            spdlog::error(error);
            return 1;
        }

        try {
            std::string              sourceCode = opal::FileUtil::readFile(argv[1]);
            opal::Lexer              lexer(sourceCode);
            std::vector<opal::Token> tokens = lexer.scanTokens();

            spdlog::info("Tokenizing file: {}", argv[1]);
            spdlog::info("----------------------------------------");
            lexer.printTokens();
            spdlog::info("----------------------------------------");

            spdlog::info("Generating AST:");
            spdlog::info("----------------------------------------");
            opal::Parser parser(tokens);
            parser.printAST();
            spdlog::info("----------------------------------------");
        } catch (const std::exception& e) {
            spdlog::error("Error: {}", e.what());
            return 1;
        }
    }

    return 0;
}
