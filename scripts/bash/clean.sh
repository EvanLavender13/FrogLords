#!/bin/bash

# Clean the build directory
# This script runs in WSL

set -e

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

REPO_ROOT="$(get_repo_root)"
BUILD_DIR="$REPO_ROOT/build/ninja"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory does not exist: $BUILD_DIR"
    echo "Nothing to clean."
    exit 0
fi

print_info "Cleaning build directory..."

rm -rf "$BUILD_DIR"

print_success "Build directory cleaned: $BUILD_DIR"
exit 0
