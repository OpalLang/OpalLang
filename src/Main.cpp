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

#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "repl/Repl.hpp"
#include "util/FileUtil.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::cout << "Opal Language" << std::endl;

    if (argc < 2) {
        opal::Repl repl;

        try {
            repl.start();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    } else {
        if (!opal::FileUtil::fileExists(argv[1])) {
            std::cerr << "File does not exist: " << argv[1] << std::endl;
            return 1;
        }

        if (!opal::FileUtil::hasGoodExtension(argv[1])) {
            std::cerr << "File has an invalid extension: " << argv[1] << std::endl;
            return 1;
        }

        try {
            std::string sourceCode = opal::FileUtil::readFile(argv[1]);
            opal::Lexer lexer(sourceCode);
            auto        tokens = lexer.scanTokens();

            std::cout << "Tokenizing file: " << argv[1] << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            lexer.printTokens();
            std::cout << "----------------------------------------" << std::endl;

            std::cout << "Generating AST:" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            opal::Parser parser(tokens);
            parser.printAST();
            std::cout << "----------------------------------------" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

    return 0;
}
