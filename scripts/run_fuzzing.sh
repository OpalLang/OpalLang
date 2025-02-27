#!/bin/bash
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'
echo -e "${YELLOW}🔍 Checking if Python 3 is installed...${NC}"
if command -v python3 &>/dev/null; then
    echo -e "${GREEN}✅ Python 3 is already installed!${NC}"
else
    echo -e "${YELLOW}📦 Python 3 is not installed. Attempting to install...${NC}"
    
    if command -v apt-get &>/dev/null; then
        sudo apt-get update
        sudo apt-get install -y python3
    elif command -v dnf &>/dev/null; then
        sudo dnf install -y python3
    elif command -v yum &>/dev/null; then
        sudo yum install -y python3
    elif command -v pacman &>/dev/null; then
        sudo pacman -S --noconfirm python
    elif command -v zypper &>/dev/null; then
        sudo zypper install -y python3
    elif command -v brew &>/dev/null; then
        brew install python
    else
        echo -e "${RED}❌ Could not determine package manager. Please install Python 3 manually.${NC}"
        exit 1
    fi
    
    if command -v python3 &>/dev/null; then
        echo -e "${GREEN}✅ Python 3 has been successfully installed!${NC}"
    else
        echo -e "${RED}❌ Failed to install Python 3. Please install it manually.${NC}"
        exit 1
    fi
fi

PYTHON_VERSION=$(python3 --version)
echo -e "${GREEN}ℹ️ $PYTHON_VERSION detected${NC}"

echo -e "${YELLOW}🚀 Running fuzzing tests...${NC}"
python3 $(dirname "$0")/fuzzing_test.py "$@"

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Fuzzing tests completed successfully!${NC}"
else
    echo -e "${RED}❌ Fuzzing tests failed with error code $?${NC}"
    exit 1
fi