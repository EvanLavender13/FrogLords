#!/bin/bash

# Run static analysis on the project.
# - cppcheck: static analysis (Windows .exe)
# - lizard: complexity analysis (Python package)

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
    --inline-suppr \
    --check-level=exhaustive

EXIT_CODE=$?
set -e

echo ""
if [ $EXIT_CODE -eq 0 ]; then
    print_success "cppcheck completed successfully"
else
    print_warning "cppcheck found issues (exit code $EXIT_CODE)"
fi

# Store cppcheck result
CPPCHECK_EXIT_CODE=$EXIT_CODE

# Run lizard complexity analysis
print_info "Starting lizard complexity analysis..."
echo "Analyzing: src/"
echo "Thresholds: CCN=15, Length=100, Arguments=5"
echo ""

set +e
python3 -m lizard src/ \
    --CCN 15 \
    --length 100 \
    --arguments 5 \
    --warnings_only

LIZARD_EXIT_CODE=$?
set -e

echo ""
if [ $LIZARD_EXIT_CODE -eq 0 ]; then
    print_success "lizard analysis passed"
else
    print_warning "lizard found complexity issues (exit code $LIZARD_EXIT_CODE)"
fi

# Exit with failure if either tool failed
if [ $CPPCHECK_EXIT_CODE -ne 0 ] || [ $LIZARD_EXIT_CODE -ne 0 ]; then
    print_error "Analysis failed"
    exit 1
fi

print_success "All analysis checks passed"
exit 0
