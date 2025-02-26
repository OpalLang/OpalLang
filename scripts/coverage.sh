#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." &> /dev/null && pwd)"
CSS_FILE="${PROJECT_ROOT}/docs/css/gcov.css"

if [ ! -f "${CSS_FILE}" ]; then
    echo -e "${RED}❌ CSS file not found at ${CSS_FILE}${NC}"
    exit 1
fi

if ! command -v lcov &> /dev/null; then
    echo -e "${RED}❌ lcov is not installed. Please install it before continuing.${NC}"
    echo -e "${YELLOW}You can install it with:${NC}"
    echo -e "${BLUE}  • Ubuntu/Debian: sudo apt install lcov${NC}"
    echo -e "${BLUE}  • Arch Linux: sudo pacman -S lcov${NC}"
    echo -e "${BLUE}  • Fedora: sudo dnf install lcov${NC}"
    echo -e "${BLUE}  • macOS: brew install lcov${NC}"
    exit 1
fi

echo -e "${GREEN}🔍 Running Opal code coverage analysis...${NC}\n"

# Create and enter build directory
mkdir -p "${PROJECT_ROOT}/build"
cd "${PROJECT_ROOT}/build" || exit 1

echo -e "${GREEN}📦 Configuring CMake...${NC}"
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage" .. || {
    echo -e "${RED}❌ CMake configuration failed${NC}"
    exit 1
}

echo -e "\n${GREEN}🔨 Building project...${NC}"
cmake --build . -j || {
    echo -e "${RED}❌ Build failed${NC}"
    exit 1
}

echo -e "\n${GREEN}🧪 Running tests...${NC}"
ctest --output-on-failure || {
    echo -e "${RED}❌ Some tests failed${NC}"
    exit 1
}

mkdir -p coverage

echo -e "\n${GREEN}📊 Generating coverage report...${NC}"
lcov --capture --directory . \
     --output-file coverage/coverage.info \
     --ignore-errors unused,inconsistent \
     --rc lcov_branch_coverage=1 \
     --exclude '/usr/*' \
     --exclude '*/tests/*' \
     --exclude '*/build/_deps/googletest-src/googlemock/include/gmock/*' \
     --exclude '*/build/_deps/googletest-src/googletest/src/*' \
     --exclude '*/include/gtest/*' \
     --exclude '*/include/gtest/internal/*' || {
    echo -e "${RED}❌ Failed to generate lcov report${NC}"
    exit 1
}

echo -e "\n${GREEN}📈 Generating HTML report...${NC}"
genhtml coverage/coverage.info \
    --output-directory coverage \
    --css-file "${CSS_FILE}" \
    --demangle-cpp \
    --legend \
    --function-coverage \
    --branch-coverage || {
    echo -e "${RED}❌ Failed to generate HTML report${NC}"
    exit 1
}

COVERAGE_FILE="${PROJECT_ROOT}/build/coverage/index.html"
if [ -f "${COVERAGE_FILE}" ]; then
    echo -e "\n${GREEN}✅ Coverage report generated successfully!${NC}"
    echo -e "Open ${GREEN}${COVERAGE_FILE}${NC} in your browser to view the results."
else
    echo -e "\n${RED}❌ Coverage report file not found at expected location${NC}"
    exit 1
fi