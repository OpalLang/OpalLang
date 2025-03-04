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

#include "error/Error.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>

using namespace opal;
using namespace testing;

class ErrorTest : public ::testing::Test {
protected:
    void SetUp() override {
        oldCerrBuf = std::cerr.rdbuf();
        std::cerr.rdbuf(testCerr.rdbuf());
        Error::reset();
    }

    void TearDown() override { std::cerr.rdbuf(oldCerrBuf); }

    std::string getOutput() {
        std::string output = testCerr.str();
        testCerr.str("");
        testCerr.clear();
        return output;
    }

    std::stringstream testCerr;
    std::streambuf*   oldCerrBuf;
};

TEST_F(ErrorTest, ErrorReporting) {
    Error::error(1, 5, "Message d'erreur de test");

    std::string output = getOutput();
    EXPECT_THAT(output, HasSubstr("[line 1, column 5]"));
    EXPECT_THAT(output, HasSubstr("Message d'erreur de test"));
    EXPECT_TRUE(Error::hadError());
}

TEST_F(ErrorTest, LexerErrorReporting) {
    Error::lexerError(2, 10, "Erreur de lexer de test");

    std::string output = getOutput();
    EXPECT_THAT(output, HasSubstr("[line 2, column 10]"));
    EXPECT_THAT(output, HasSubstr("Lexical error"));
    EXPECT_THAT(output, HasSubstr("Erreur de lexer de test"));
    EXPECT_TRUE(Error::hadError());
}

TEST_F(ErrorTest, CustomErrorReporting) {
    Error::report(3, 15, "Contexte personnalisé", "Message d'erreur personnalisé");

    std::string output = getOutput();
    EXPECT_THAT(output, HasSubstr("[line 3, column 15]"));
    EXPECT_THAT(output, HasSubstr("Contexte personnalisé"));
    EXPECT_THAT(output, HasSubstr("Message d'erreur personnalisé"));
}

TEST_F(ErrorTest, MultipleErrors) {
    Error::error(1, 5, "Première erreur");
    std::string output1 = getOutput();

    Error::error(2, 10, "Deuxième erreur");
    std::string output2 = getOutput();

    Error::lexerError(3, 15, "Troisième erreur");
    std::string output3 = getOutput();

    EXPECT_THAT(output1, HasSubstr("Première erreur"));
    EXPECT_THAT(output2, HasSubstr("Deuxième erreur"));
    EXPECT_THAT(output3, HasSubstr("Troisième erreur"));
    EXPECT_TRUE(Error::hadError());
}

TEST_F(ErrorTest, ErrorReset) {
    Error::error(1, 5, "Erreur de test");
    getOutput();

    EXPECT_TRUE(Error::hadError());

    Error::reset();
    EXPECT_FALSE(Error::hadError());
}

TEST_F(ErrorTest, GetErrors) {
    Error::error(1, 5, "Première erreur");
    getOutput();

    Error::lexerError(2, 10, "Deuxième erreur");
    getOutput();

    const std::vector<Error::ErrorInfo>& errors = Error::getErrors();
    ASSERT_EQ(errors.size(), 2);

    EXPECT_EQ(errors[0].message, "Première erreur");
    EXPECT_EQ(errors[0].line, 1);
    EXPECT_EQ(errors[0].column, 5);

    EXPECT_EQ(errors[1].message, "Deuxième erreur");
    EXPECT_EQ(errors[1].line, 2);
    EXPECT_EQ(errors[1].column, 10);
}

TEST_F(ErrorTest, ErrorInfoStructure) {
    Error::error(1, 5, "Message de test");
    getOutput();

    const std::vector<Error::ErrorInfo>& errors = Error::getErrors();
    ASSERT_EQ(errors.size(), 1);

    const Error::ErrorInfo& info = errors[0];
    EXPECT_EQ(info.message, "Message de test");
    EXPECT_EQ(info.line, 1);
    EXPECT_EQ(info.column, 5);
}

TEST_F(ErrorTest, ZeroLineAndColumn) {
    Error::error(0, 0, "Erreur sans position");

    std::string output = getOutput();
    EXPECT_THAT(output, HasSubstr("[line 0, column 0]"));
    EXPECT_THAT(output, HasSubstr("Erreur sans position"));
    EXPECT_TRUE(Error::hadError());
}

TEST_F(ErrorTest, EmptyErrorMessage) {
    Error::error(1, 5, "");

    std::string output = getOutput();
    EXPECT_THAT(output, HasSubstr("[line 1, column 5]"));
    EXPECT_TRUE(Error::hadError());
}

TEST_F(ErrorTest, LargeLineAndColumnValues) {
    Error::error(999999, 999999, "Erreur avec grandes valeurs");

    std::string output = getOutput();
    EXPECT_THAT(output, HasSubstr("[line 999999, column 999999]"));
    EXPECT_THAT(output, HasSubstr("Erreur avec grandes valeurs"));
    EXPECT_TRUE(Error::hadError());
}

TEST_F(ErrorTest, ErrorsAfterReset) {
    Error::error(1, 5, "Première erreur");
    getOutput();

    Error::reset();

    // Vérifier que le vecteur d'erreurs est vide après reset
    EXPECT_EQ(Error::getErrors().size(), 0);

    Error::error(2, 10, "Deuxième erreur");
    getOutput();

    const std::vector<Error::ErrorInfo>& errors = Error::getErrors();
    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].message, "Deuxième erreur");
    EXPECT_TRUE(Error::hadError());
}