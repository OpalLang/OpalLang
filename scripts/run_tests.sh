#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}🧪 Running tests...${NC}"

if [ ! -d "build" ]; then
    echo -e "${RED}❌ Build directory does not exist. Please run ./scripts/build.sh first${NC}"
    exit 1
fi

echo -e "${BLUE}🔨 Building tests...${NC}"
cmake --build build --target tests || {
    echo -e "${RED}❌ Failed to build tests${NC}"
    exit 1
}

echo -e "${BLUE}🚀 Running tests...${NC}"
cd build && ctest --output-on-failure || {
    echo -e "${RED}❌ Some tests failed${NC}"
    exit 1
}

echo -e "${GREEN}✅ All tests passed!${NC}"