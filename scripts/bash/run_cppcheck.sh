#!/bin/bash

# Run cppcheck on the project.
# This script runs in WSL and calls Windows cppcheck.exe

set -e

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

REPO_ROOT="$(get_repo_root)"
BUILD_DIR="$REPO_ROOT/build/ninja"
COMPILE_DB="$BUILD_DIR/compile_commands.json"
CPPCHECK_BUILD_DIR="$REPO_ROOT/build/cppcheck"

if [ ! -f "$COMPILE_DB" ]; then
    print_error "compile_commands.json not found at $COMPILE_DB. Run the CMake configure task first."
    exit 1
fi

# Create cppcheck build dir if not exists
if [ ! -d "$CPPCHECK_BUILD_DIR" ]; then
    mkdir -p "$CPPCHECK_BUILD_DIR"
fi

print_info "Starting cppcheck..."
echo "Analyzing project with compile database: $COMPILE_DB"
echo "Filter: src/**"
echo ""

# Convert paths to Windows format for cppcheck.exe
WIN_COMPILE_DB="$(to_win_path "$COMPILE_DB")"
WIN_CPPCHECK_BUILD_DIR="$(to_win_path "$CPPCHECK_BUILD_DIR")"

# Run cppcheck (suppress errexit for this command)
set +e
cppcheck.exe \
    "--project=$WIN_COMPILE_DB" \
    "--cppcheck-build-dir=$WIN_CPPCHECK_BUILD_DIR" \
    --enable=all \
    --std=c++20 \
    '--file-filter=src/**' \
    --suppress=missingIncludeSystem \
    '--suppress=*:external/**' \
    '--suppress=*:generated/**' \
    --check-level=exhaustive

EXIT_CODE=$?
set -e

echo ""
if [ $EXIT_CODE -eq 0 ]; then
    print_success "cppcheck completed successfully"
else
    print_warning "cppcheck found issues (exit code $EXIT_CODE)"
fi

exit $EXIT_CODE
