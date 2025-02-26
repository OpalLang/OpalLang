#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

if ! command -v clang-format &> /dev/null; then
    echo -e "${RED}❌ clang-format is not installed. Please install it before continuing.${NC}"
    echo -e "${YELLOW}You can install it with:${NC}"
    echo -e "${BLUE}  • Ubuntu/Debian: sudo apt install clang-format${NC}"
    echo -e "${BLUE}  • Arch Linux: sudo pacman -S clang${NC}"
    echo -e "${BLUE}  • Fedora: sudo dnf install clang-tools-extra${NC}"
    echo -e "${BLUE}  • macOS: brew install clang-format${NC}"
    exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC_DIR="${ROOT_DIR}/src"
TESTS_DIR="${ROOT_DIR}/tests"

if [ ! -f "${ROOT_DIR}/.clang-format" ]; then
    echo -e "${YELLOW}⚠️ No .clang-format file found. Creating a default file...${NC}"

    cat > "${ROOT_DIR}/.clang-format" << EOF
---
Language: Cpp
BasedOnStyle: Google
IndentWidth: 4
TabWidth: 4
UseTab: Never
ColumnLimit: 120
AccessModifierOffset: -4
AlignAfterOpenBracket: Align
AlignConsecutiveAssignments: Consecutive
AlignConsecutiveDeclarations: Consecutive
AlignConsecutiveMacros: Consecutive
AlignEscapedNewlines: Right
AlignOperands: Align
AlignTrailingComments: true
AllowAllArgumentsOnNextLine: false
AllowAllParametersOfDeclarationOnNextLine: false
AllowShortBlocksOnASingleLine: Empty
AllowShortCaseLabelsOnASingleLine: false
AllowShortFunctionsOnASingleLine: Inline
AllowShortIfStatementsOnASingleLine: Never
AllowShortLoopsOnASingleLine: false
AlwaysBreakAfterReturnType: None
AlwaysBreakBeforeMultilineStrings: false
AlwaysBreakTemplateDeclarations: Yes
BinPackArguments: false
BinPackParameters: false
BreakBeforeBinaryOperators: NonAssignment
BreakBeforeBraces: Attach
BreakBeforeInheritanceComma: false
BreakBeforeTernaryOperators: true
BreakConstructorInitializers: BeforeColon
BreakInheritanceList: BeforeColon
BreakStringLiterals: true
CompactNamespaces: false
ConstructorInitializerAllOnOneLineOrOnePerLine: true
ConstructorInitializerIndentWidth: 4
ContinuationIndentWidth: 4
Cpp11BracedListStyle: true
DerivePointerAlignment: false
DisableFormat: false
ExperimentalAutoDetectBinPacking: false
FixNamespaceComments: true
IncludeBlocks: Regroup
IncludeCategories:
  - Regex: '^".*"'
    Priority: 1
  - Regex: '^<.*\.h>'
    Priority: 2
  - Regex: '^<.*'
    Priority: 3
IndentCaseLabels: true
IndentGotoLabels: true
IndentPPDirectives: BeforeHash
IndentWrappedFunctionNames: false
KeepEmptyLinesAtTheStartOfBlocks: false
MaxEmptyLinesToKeep: 1
NamespaceIndentation: None
PenaltyBreakAssignment: 2
PenaltyBreakBeforeFirstCallParameter: 1
PenaltyBreakComment: 300
PenaltyBreakFirstLessLess: 120
PenaltyBreakString: 1000
PenaltyExcessCharacter: 1000000
PenaltyReturnTypeOnItsOwnLine: 60
PointerAlignment: Left
ReflowComments: true
SortIncludes: true
SortUsingDeclarations: true
SpaceAfterCStyleCast: false
SpaceAfterLogicalNot: false
SpaceAfterTemplateKeyword: true
SpaceBeforeAssignmentOperators: true
SpaceBeforeCpp11BracedList: false
SpaceBeforeCtorInitializerColon: true
SpaceBeforeInheritanceColon: true
SpaceBeforeParens: ControlStatements
SpaceBeforeRangeBasedForLoopColon: true
SpaceInEmptyParentheses: false
SpacesBeforeTrailingComments: 2
SpacesInAngles: false
SpacesInContainerLiterals: false
SpacesInCStyleCastParentheses: false
SpacesInParentheses: false
SpacesInSquareBrackets: false
Standard: c++17
EOF

    echo -e "${GREEN}✅ .clang-format file created successfully.${NC}"
fi

format_files() {
    local dir=$1
    local count=0
    
    if [ ! -d "$dir" ]; then
        return
    }
    
    echo -e "${YELLOW}🔍 Searching for files to format in ${dir}...${NC}"

    while IFS= read -r file; do
        echo -e "${BLUE}📄 Formatting ${file}...${NC}"
        clang-format -i -style=file "$file"
        ((count++))
    done < <(find "$dir" -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" -o -name "*.cc" -o -name "*.cxx" -o -name "*.hxx" \))
    
    echo -e "${GREEN}✅ $count files formatted in ${dir}${NC}"
}

check_format() {
    local dir=$1
    local has_errors=0
    
    if [ ! -d "$dir" ]; then
        return 0
    fi
    
    echo -e "${YELLOW}🔍 Checking formatting in ${dir}...${NC}"

    while IFS= read -r file; do
        if ! clang-format -style=file "$file" | diff -u "$file" - > /dev/null; then
            echo -e "${RED}❌ File ${file} is not properly formatted.${NC}"
            has_errors=1
        fi
    done < <(find "$dir" -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" -o -name "*.cc" -o -name "*.cxx" -o -name "*.hxx" \))
    
    return $has_errors
}

case "$1" in
    format|--format|-f)
        echo -e "${GREEN}🚀 Formatting source code...${NC}"
        format_files "$SRC_DIR"
        format_files "$TESTS_DIR"
        echo -e "${GREEN}✅ Formatting completed successfully!${NC}"
        ;;
    check|--check|-c)
        echo -e "${GREEN}🔍 Checking code formatting...${NC}"
        check_format "$SRC_DIR"
        src_result=$?
        check_format "$TESTS_DIR"
        tests_result=$?
        
        if [ $src_result -eq 0 ] && [ $tests_result -eq 0 ]; then
            echo -e "${GREEN}✅ All files are properly formatted!${NC}"
            exit 0
        else
            echo -e "${RED}❌ Some files are not properly formatted. Use './scripts/format_code.sh format' to fix them.${NC}"
            exit 1
        fi
        ;;
    *)
        echo -e "${BLUE}Usage: $0 [OPTION]${NC}"
        echo -e "${YELLOW}Options:${NC}"
        echo -e "  ${GREEN}format, --format, -f${NC}    Format all source files"
        echo -e "  ${GREEN}check, --check, -c${NC}      Check if files are properly formatted"
        echo -e ""
        echo -e "${YELLOW}Example:${NC}"
        echo -e "  ${BLUE}$0 format${NC}    # Format all source files"
        exit 0
        ;;
esac

exit 0 