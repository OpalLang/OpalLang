#!/bin/bash

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}🧹 Cleaning project...${NC}"

if [ -d "build" ]; then
    echo -e "${YELLOW}📁 Removing build directory...${NC}"
    rm -rf build
fi

if [ -d "bin" ]; then
    echo -e "${YELLOW}📁 Cleaning bin directory...${NC}"
    rm -rf bin
fi

if [ -d "docs/html" ]; then
    echo -e "${YELLOW}📁 Removing documentation...${NC}"
    rm -rf docs/html
fi

if [ -f "Doxyfile" ]; then
    echo -e "${YELLOW}📄 Removing Doxyfile...${NC}"
    rm -f Doxyfile
fi

if [ -d ".clangd" ]; then
    echo -e "${YELLOW}📁 Removing .clangd directory...${NC}"
    rm -rf .clangd
fi

echo -e "${YELLOW}🗑️  Removing temporary files...${NC}"
find . -name "*.o" -type f -delete
find . -name "*.a" -type f -delete
find . -name "*.so" -type f -delete
find . -name "*.out" -type f -delete
find . -name "*.gch" -type f -delete
find . -name "*.pch" -type f -delete

echo -e "${GREEN}✅ Cleaning completed successfully!${NC}"