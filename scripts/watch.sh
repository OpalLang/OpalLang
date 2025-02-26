#!/bin/bash

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

if ! command -v inotifywait &> /dev/null; then
    echo -e "${RED}❌ inotify-tools is not installed. Required installation:${NC}"
    echo -e "  Ubuntu/Debian: ${YELLOW}sudo apt-get install inotify-tools${NC}"
    echo -e "  Arch Linux: ${YELLOW}sudo pacman -S inotify-tools${NC}"
    echo -e "  Fedora: ${YELLOW}sudo dnf install inotify-tools${NC}"
    exit 1
fi

cleanup() {
    echo -e "\n${BLUE}🧹 Cleaning up...${NC}"
    kill $(jobs -p) &> /dev/null
    exit 0
}

trap cleanup SIGINT SIGTERM

build_project() {
    echo -e "\n${GREEN}🔄 Rebuilding project...${NC}"
    ./scripts/build.sh
}

build_project

echo -e "${BLUE}👀 Watching for changes...${NC}"
echo -e "${YELLOW}Watching directories: src/, tests/${NC}"
echo -e "${YELLOW}Press Ctrl+C to stop${NC}\n"

while true; do
    inotifywait -r -e modify,create,delete,move src/ tests/ 2>/dev/null

    sleep 0.5

    if ! read -t 0.5; then
        build_project
    fi
done 