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
    *Ubuntu*|*Debian*|*Linux\ Mint*|*Pop!_OS*|*elementary\ OS*|*Zorin\ OS*|*KDE\ neon*|*Kubuntu*|*Xubuntu*|*Lubuntu*|*Ubuntu\ MATE*|*Ubuntu\ Budgie*|*Ubuntu\ Studio*)
        echo -e "${YELLOW}🔄 Updating packages...${NC}"
        sudo apt-get update
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        sudo apt-get install -y build-essential cmake g++ clang-format libgtest-dev libgmock-dev doxygen inotify-tools bc
        ;;

    *Arch*|*Manjaro*|*EndeavourOS*|*Garuda*|*ArcoLinux*|*Artix*|*BlackArch*|*Chakra*)
        echo -e "${YELLOW}🔄 Updating packages...${NC}"
        sudo pacman -Syu
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        sudo pacman -S --needed base-devel cmake gcc clang gtest gmock doxygen inotify-tools bc
        ;;

    *Fedora*|*Fedora\ Silverblue*|*Fedora\ CoreOS*|*Fedora\ IoT*|*Fedora\ Kinoite*)
        echo -e "${YELLOW}🔄 Updating packages...${NC}"
        sudo dnf update
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        sudo dnf install -y cmake gcc-c++ clang gtest-devel gmock-devel doxygen inotify-tools bc
        ;;

    *RHEL*|*CentOS*|*Rocky*|*AlmaLinux*|*Oracle\ Linux*|*Scientific\ Linux*)
        echo -e "${YELLOW}🔄 Updating packages...${NC}"
        sudo yum update
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        sudo yum install -y cmake gcc-c++ clang doxygen inotify-tools bc
        sudo yum install -y epel-release
        sudo yum install -y gtest-devel gmock-devel
        ;;

    *openSUSE*|*SUSE*|*SLES*)
        echo -e "${YELLOW}🔄 Updating packages...${NC}"
        sudo zypper refresh
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        sudo zypper install -y cmake gcc-c++ clang gtest gmock doxygen inotify-tools bc
        ;;

    *macOS*|*Darwin*)
        if ! command -v brew &> /dev/null; then
            echo -e "${YELLOW}🍺 Installing Homebrew...${NC}"
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        fi
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        brew install cmake llvm googletest doxygen fswatch bc
        ;;

    *FreeBSD*|*OpenBSD*|*NetBSD*|*DragonFly*)
        echo -e "${YELLOW}🔄 Updating packages...${NC}"
        sudo pkg update
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        sudo pkg install -y cmake llvm gtest gmock doxygen inotify-tools bc
        ;;

    *Alpine*)
        echo -e "${YELLOW}🔄 Updating packages...${NC}"
        sudo apk update
        
        echo -e "${YELLOW}📥 Installing dependencies...${NC}"
        sudo apk add cmake g++ clang gtest gmock doxygen inotify-tools bc
        ;;

    *Microsoft*|*Windows*)
        if grep -q Microsoft /proc/version; then
            echo -e "${YELLOW}🔄 Updating packages (WSL)...${NC}"
            sudo apt-get update
            
            echo -e "${YELLOW}📥 Installing dependencies...${NC}"
            sudo apt-get install -y build-essential cmake g++ clang-format libgtest-dev libgmock-dev doxygen inotify-tools bc
        else
            echo -e "${YELLOW}📥 Installing dependencies via Chocolatey...${NC}"
            if ! command -v choco &> /dev/null; then
                echo -e "${YELLOW}🍫 Installing Chocolatey...${NC}"
                powershell -Command "Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))"
            fi
            choco install -y cmake llvm googletest doxygen bc
            choco install -y visualstudio2022buildtools --package-parameters "--add Microsoft.VisualStudio.Workload.NativeDesktop"
        fi
        ;;
        
    *)
        echo -e "${RED}❌ Unsupported operating system: $OS${NC}"
        echo -e "${YELLOW}Please install manually: cmake, C++ compiler (g++ or clang), googletest, doxygen${NC}"
        exit 1
        ;;
esac

echo -e "${GREEN}✅ Dependencies installation complete!${NC}"
echo -e "${YELLOW}🚀 You can now build the project with ./scripts/build.sh${NC}"