#include "util/FileUtil.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <stdexcept>

using namespace Opal;
using namespace testing;

class FileUtilTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDir = "test_files";
        if (system(("mkdir -p " + testDir).c_str()) != 0) {
            throw std::runtime_error("Failed to create test directory: " + testDir);
        }
    }

    void TearDown() override {
        if (system(("rm -rf " + testDir).c_str()) != 0) {
            // Just log the error since this is in teardown
            std::cerr << "Warning: Failed to remove test directory: " << testDir << std::endl;
        }
    }

    std::string testDir;
};

TEST_F(FileUtilTest, ReadFile) {
    std::string testFilePath = testDir + "/test_read.txt";
    std::string testContent = "Test content\nSecond line\nThird line";
    
    std::ofstream outFile(testFilePath);
    outFile << testContent;
    outFile.close();
    
    std::string readContent = FileUtil::readFile(testFilePath);
    EXPECT_EQ(readContent, testContent);
}

TEST_F(FileUtilTest, ReadNonExistentFile) {
    std::string nonExistentFile = testDir + "/non_existent.txt";
    
    EXPECT_THROW(FileUtil::readFile(nonExistentFile), std::runtime_error);
}

TEST_F(FileUtilTest, WriteFile) {
    std::string testFilePath = testDir + "/test_write.txt";
    std::string testContent = "Content to write\nSecond line";
    
    FileUtil::writeFile(testFilePath, testContent);
    
    std::ifstream inFile(testFilePath);
    std::string line;
    std::string readContent;
    
    while (std::getline(inFile, line)) {
        readContent += line;
        if (!inFile.eof()) {
            readContent += "\n";
        }
    }
    
    EXPECT_EQ(readContent, testContent);
}

TEST_F(FileUtilTest, WriteToExistingFile) {
    std::string testFilePath = testDir + "/test_overwrite.txt";
    std::string initialContent = "Initial content";
    std::string newContent = "New content";
    
    std::ofstream outFile(testFilePath);
    outFile << initialContent;
    outFile.close();
    
    FileUtil::writeFile(testFilePath, newContent);
    
    std::string readContent = FileUtil::readFile(testFilePath);
    EXPECT_EQ(readContent, newContent);
}

TEST_F(FileUtilTest, FileExists) {
    std::string testFilePath = testDir + "/test_exists.txt";
    std::ofstream outFile(testFilePath);
    outFile << "Test";
    outFile.close();
    
    EXPECT_TRUE(FileUtil::fileExists(testFilePath));
    
    std::string nonExistentFile = testDir + "/non_existent.txt";
    EXPECT_FALSE(FileUtil::fileExists(nonExistentFile));
}

TEST_F(FileUtilTest, WriteToDirectoryWithoutPermission) {
    std::string restrictedDir = testDir + "/restricted";
    
    if (system(("mkdir -p " + restrictedDir).c_str()) != 0) {
        FAIL() << "Failed to create restricted directory for test";
    }
    
    if (system(("chmod 000 " + restrictedDir).c_str()) != 0) {
        FAIL() << "Failed to change permissions on restricted directory";
    }
    
    std::string testFilePath = restrictedDir + "/test.txt";
    std::string testContent = "Test";
    
    EXPECT_THROW(FileUtil::writeFile(testFilePath, testContent), std::runtime_error);
    
    if (system(("chmod 755 " + restrictedDir).c_str()) != 0) {
        std::cerr << "Warning: Failed to restore permissions on " << restrictedDir << std::endl;
    }
}

TEST_F(FileUtilTest, ReadEmptyFile) {
    std::string testFilePath = testDir + "/empty.txt";
    std::ofstream outFile(testFilePath);
    outFile.close();
    
    std::string content = FileUtil::readFile(testFilePath);
    EXPECT_EQ(content, "");
}

TEST_F(FileUtilTest, WriteLargeFile) {
    std::string testFilePath = testDir + "/large.txt";
    std::string largeContent;
    
    for (int i = 0; i < 1024 * 1024 / 10; ++i) {
        largeContent += "0123456789";
    }
    
    FileUtil::writeFile(testFilePath, largeContent);
    
    std::string readContent = FileUtil::readFile(testFilePath);
    EXPECT_EQ(readContent.size(), largeContent.size());
    EXPECT_EQ(readContent, largeContent);
}

TEST_F(FileUtilTest, FileExistsWithSpecialCharacters) {
    std::string testFilePath = testDir + "/special@#$%^&.txt";
    std::ofstream outFile(testFilePath);
    outFile << "Test";
    outFile.close();
    
    EXPECT_TRUE(FileUtil::fileExists(testFilePath));
}