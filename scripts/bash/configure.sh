#!/bin/bash

# Configure the CMake project for Debug build
# This script runs in WSL and calls Windows cmake.exe

set -e

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

REPO_ROOT="$(get_repo_root)"
BUILD_DIR="$REPO_ROOT/build/ninja"

print_info "Configuring CMake (Debug)..."

# Convert paths to Windows for cmake.exe
WIN_REPO_ROOT="$(to_win_path "$REPO_ROOT")"
WIN_BUILD_DIR="$(to_win_path "$BUILD_DIR")"

cd "$REPO_ROOT"

# Forward any additional arguments to cmake (e.g., -DENABLE_QUATERNION_TESTS=ON)
cmake.exe \
    -S "$WIN_REPO_ROOT" \
    -B "$WIN_BUILD_DIR" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    "$@"

EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    print_success "CMake configuration completed successfully"
    echo "Build directory: $BUILD_DIR"
else
    print_error "CMake configuration failed with exit code $EXIT_CODE"
    exit $EXIT_CODE
fi

exit 0
