#!/bin/bash

# Format all C/C++ sources under the src directory with clang-format.
# This script runs in WSL and calls Windows clang-format.exe

set -e  # Exit on error

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

# Get repository root
REPO_ROOT="$(get_repo_root)"
SRC_DIR="$REPO_ROOT/src"

if [ ! -d "$SRC_DIR" ]; then
    print_error "src directory not found at $SRC_DIR"
    exit 1
fi

# Find all C/C++ source files under src/
mapfile -t FILES < <(find "$SRC_DIR" -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \))

if [ ${#FILES[@]} -eq 0 ]; then
    echo "No source files found under $SRC_DIR"
    exit 0
fi

print_info "Starting clang-format..."
echo "Formatting ${#FILES[@]} source file(s) in $SRC_DIR"

# Convert WSL paths to Windows paths for the Windows executable
WIN_FILES=($(convert_paths_to_windows "${FILES[@]}"))

# Call Windows clang-format.exe
clang-format.exe -i "${WIN_FILES[@]}"

EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    print_success "clang-format completed successfully"
else
    print_error "clang-format failed with exit code $EXIT_CODE"
    exit $EXIT_CODE
fi

exit 0
