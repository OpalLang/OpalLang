#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${GREEN}🚀 Starting build...${NC}"

if ! command -v cmake &> /dev/null; then
    echo -e "${RED}❌ CMake is not installed. Please install it before continuing.${NC}"
    echo -e "${YELLOW}You can install it with:${NC}"
    echo -e "${BLUE}  • Ubuntu/Debian: sudo apt install cmake${NC}"
    echo -e "${BLUE}  • Arch Linux: sudo pacman -S cmake${NC}"
    echo -e "${BLUE}  • Fedora: sudo dnf install cmake${NC}"
    echo -e "${BLUE}  • macOS: brew install cmake${NC}"
    exit 1
fi

echo -e "${GREEN}📦 Configuring CMake...${NC}"
cmake -B build || {
    echo -e "${RED}❌ CMake configuration failed${NC}"
    exit 1
}

echo -e "${GREEN}🔨 Building project...${NC}"
cmake --build build || {
    echo -e "${RED}❌ Build failed${NC}"
    exit 1
}

echo -e "${GREEN}✅ Build completed successfully!${NC}"
