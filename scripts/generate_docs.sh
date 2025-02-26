#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}📚 Generating documentation with Doxygen...${NC}"

if ! command -v doxygen &> /dev/null; then
    echo -e "${RED}❌ Doxygen is not installed. Please install it before continuing.${NC}"
    echo -e "${YELLOW}You can install it with:${NC}"
    echo -e "${BLUE}  • Ubuntu/Debian: sudo apt install doxygen${NC}"
    echo -e "${BLUE}  • Arch Linux: sudo pacman -S doxygen${NC}"
    echo -e "${BLUE}  • Fedora: sudo dnf install doxygen${NC}"
    echo -e "${BLUE}  • macOS: brew install doxygen${NC}"
    exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DOCS_DIR="${ROOT_DIR}/docs"

if [ ! -d "$DOCS_DIR" ]; then
    echo -e "${YELLOW}📁 Creating docs directory...${NC}"
    mkdir -p "$DOCS_DIR"
fi

if [ ! -f "${ROOT_DIR}/Doxyfile" ]; then
    echo -e "${YELLOW}⚠️ No Doxyfile found. Creating a default configuration...${NC}"
    
    doxygen -g "${ROOT_DIR}/Doxyfile" > /dev/null

    sed -i.bak 's/PROJECT_NAME.*=.*/PROJECT_NAME           = "Opal"/' "${ROOT_DIR}/Doxyfile"
    sed -i.bak 's/OUTPUT_DIRECTORY.*=.*/OUTPUT_DIRECTORY       = docs/' "${ROOT_DIR}/Doxyfile"
    sed -i.bak 's/EXTRACT_ALL.*=.*/EXTRACT_ALL            = YES/' "${ROOT_DIR}/Doxyfile"
    sed -i.bak 's/EXTRACT_PRIVATE.*=.*/EXTRACT_PRIVATE        = YES/' "${ROOT_DIR}/Doxyfile"
    sed -i.bak 's/EXTRACT_STATIC.*=.*/EXTRACT_STATIC         = YES/' "${ROOT_DIR}/Doxyfile"
    sed -i.bak 's/INPUT.*=.*/INPUT                  = src include/' "${ROOT_DIR}/Doxyfile"
    sed -i.bak 's/RECURSIVE.*=.*/RECURSIVE              = YES/' "${ROOT_DIR}/Doxyfile"
    sed -i.bak 's/GENERATE_HTML.*=.*/GENERATE_HTML          = YES/' "${ROOT_DIR}/Doxyfile"
    sed -i.bak 's/GENERATE_LATEX.*=.*/GENERATE_LATEX         = NO/' "${ROOT_DIR}/Doxyfile"
    sed -i.bak 's/USE_MDFILE_AS_MAINPAGE.*=.*/USE_MDFILE_AS_MAINPAGE = README.md/' "${ROOT_DIR}/Doxyfile"

    rm -f "${ROOT_DIR}/Doxyfile.bak"
    
    echo -e "${GREEN}✅ Doxyfile created successfully.${NC}"
fi

echo -e "${YELLOW}🔨 Generating documentation...${NC}"
cd "${ROOT_DIR}" && doxygen Doxyfile

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Documentation generated successfully!${NC}"
    echo -e "${BLUE}📂 Documentation is available at: ${DOCS_DIR}/html/index.html${NC}"
else
    echo -e "${RED}❌ Failed to generate documentation${NC}"
    exit 1
fi

case "$1" in
    open|--open|-o)
        echo -e "${YELLOW}🌐 Opening documentation in browser...${NC}"
        if [ "$(uname)" == "Darwin" ]; then
            open "${DOCS_DIR}/html/index.html"
        elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
            if command -v xdg-open &> /dev/null; then
                xdg-open "${DOCS_DIR}/html/index.html"
            else
                echo -e "${YELLOW}⚠️ Could not open browser automatically. Please open ${DOCS_DIR}/html/index.html manually.${NC}"
            fi
        elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ] || [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
            start "${DOCS_DIR}/html/index.html"
        else
            echo -e "${YELLOW}⚠️ Could not open browser automatically. Please open ${DOCS_DIR}/html/index.html manually.${NC}"
        fi
        ;;
esac

exit 0 