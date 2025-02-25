#!/bin/bash

# Couleurs pour les messages
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}🚀 Starting build...${NC}"

if ! command -v cmake &> /dev/null; then
    echo -e "${RED}❌ CMake is not installed. Please install it before continuing.${NC}"
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
