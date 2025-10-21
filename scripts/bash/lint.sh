#!/bin/bash

# Run clang-tidy across translation units under the src directory.
# This script runs in WSL and calls Windows clang-tidy.exe

set -e  # Exit on error

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

# Get repository root and directories
REPO_ROOT="$(get_repo_root)"
SRC_DIR="$REPO_ROOT/src"
BUILD_DIR="$REPO_ROOT/build/ninja"
COMPILE_DB="$BUILD_DIR/compile_commands.json"

if [ ! -d "$SRC_DIR" ]; then
    print_error "src directory not found at $SRC_DIR"
    exit 1
fi

if [ ! -f "$COMPILE_DB" ]; then
    print_error "compile_commands.json not found at $COMPILE_DB. Run the CMake configure task first."
    exit 1
fi

# Determine which files to analyze
if [ $# -gt 0 ]; then
    # Use provided arguments as file paths
    CPP_FILES=()
    for arg in "$@"; do
        # Convert to absolute path if relative
        if [[ "$arg" != /* ]]; then
            arg="$REPO_ROOT/$arg"
        fi

        # Validate file exists and is a .cpp file
        if [ ! -f "$arg" ]; then
            print_error "File not found: $arg"
            exit 1
        fi

        if [[ "$arg" != *.cpp ]]; then
            print_warning "Skipping non-.cpp file: $arg"
            continue
        fi

        CPP_FILES+=("$arg")
    done

    if [ ${#CPP_FILES[@]} -eq 0 ]; then
        print_error "No valid .cpp files provided"
        exit 1
    fi
else
    # Find all .cpp files under src/, sorted
    mapfile -t CPP_FILES < <(find "$SRC_DIR" -type f -name "*.cpp" | sort)

    if [ ${#CPP_FILES[@]} -eq 0 ]; then
        echo "No translation units found under $SRC_DIR"
        exit 0
    fi
fi

print_info "Starting clang-tidy..."
echo "Analyzing ${#CPP_FILES[@]} translation unit(s)"
echo ""

EXIT_CODE=0

# Convert build directory to Windows path for -p flag
WIN_BUILD_DIR="$(to_win_path "$BUILD_DIR")"

for file in "${CPP_FILES[@]}"; do
    # Calculate relative path for display
    RELATIVE="${file#$REPO_ROOT/}"
    echo "Analyzing: $RELATIVE"
    
    # Convert file path to Windows path for clang-tidy
    WIN_FILE="$(to_win_path "$file")"
    
    # Run clang-tidy (suppress errexit for this command)
    set +e
    clang-tidy.exe -p "$WIN_BUILD_DIR" "$WIN_FILE"
    CURRENT_EXIT=$?
    set -e
    
    # Track first non-zero exit code
    if [ $CURRENT_EXIT -ne 0 ] && [ $EXIT_CODE -eq 0 ]; then
        EXIT_CODE=$CURRENT_EXIT
    fi
done

echo ""
if [ $EXIT_CODE -eq 0 ]; then
    print_success "clang-tidy completed successfully"
else
    print_warning "clang-tidy found issues (exit code $EXIT_CODE)"
fi

exit $EXIT_CODE
