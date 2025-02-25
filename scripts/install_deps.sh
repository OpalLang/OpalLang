#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}📦 Installing dependencies for Opal...${NC}"

if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$NAME
elif type lsb_release >/dev/null 2>&1; then
    OS=$(lsb_release -si)
else
    OS=$(uname -s)
fi

echo -e "${YELLOW}💻 Detected system: $OS${NC}"

case "$OS" in
    *Ubuntu*|*Debian*)
        echo -e "${YELLOW}🔄 Updating packages...${NC}"
        sudo apt-get update
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        sudo apt-get install -y build-essential cmake g++ clang-format libgtest-dev libgmock-dev
        ;;
        
    *Arch*|*Manjaro*)
        echo -e "${YELLOW}🔄 Updating packages...${NC}"
        sudo pacman -Syu
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        sudo pacman -S --needed base-devel cmake gcc clang gtest gmock
        ;;
        
    *Fedora*)
        echo -e "${YELLOW}🔄 Updating packages...${NC}"
        sudo dnf update
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        sudo dnf install -y cmake gcc-c++ clang gtest-devel gmock-devel
        ;;
        
    *macOS*|*Darwin*)
        if ! command -v brew &> /dev/null; then
            echo -e "${YELLOW}🍺 Installing Homebrew...${NC}"
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        fi
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        brew install cmake llvm googletest
        ;;
        
    *)
        echo -e "${RED}❌ Unsupported operating system: $OS${NC}"
        echo -e "${YELLOW}Please install manually: cmake, C++ compiler (g++ or clang), googletest${NC}"
        exit 1
        ;;
esac

echo -e "${GREEN}✅ Dependencies installation complete!${NC}"
echo -e "${YELLOW}🚀 You can now build the project with ./scripts/build.sh${NC}"