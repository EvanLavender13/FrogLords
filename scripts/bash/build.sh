#!/bin/bash

# Build the project using CMake
# This script runs in WSL and calls Windows cmake.exe and ninja.exe

set -e

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

REPO_ROOT="$(get_repo_root)"
BUILD_DIR="$REPO_ROOT/build/ninja"

if [ ! -f "$BUILD_DIR/build.ninja" ]; then
    print_error "Build directory not configured. Run configure.sh first."
    exit 1
fi

print_info "Building project..."

# Convert build dir to Windows path
WIN_BUILD_DIR="$(to_win_path "$BUILD_DIR")"

cmake.exe --build "$WIN_BUILD_DIR"

EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    print_success "Build completed successfully"
    echo "Executable: $BUILD_DIR/FrogLords.exe"
else
    print_error "Build failed with exit code $EXIT_CODE"
    exit $EXIT_CODE
fi

exit 0
