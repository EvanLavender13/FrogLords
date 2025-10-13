#!/bin/bash

# Run the built executable
# This script runs in WSL and calls Windows FrogLords.exe

set -e

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

REPO_ROOT="$(get_repo_root)"
EXECUTABLE="$REPO_ROOT/build/ninja/FrogLords.exe"

if [ ! -f "$EXECUTABLE" ]; then
    print_error "Executable not found at $EXECUTABLE"
    echo "Run build.sh first to compile the project."
    exit 1
fi

print_info "Running FrogLords..."
echo ""

# WSL can execute .exe files directly using the WSL path
cd "$REPO_ROOT"
"$EXECUTABLE" "$@"

EXIT_CODE=$?
echo ""
if [ $EXIT_CODE -eq 0 ]; then
    print_success "Program exited successfully"
else
    print_warning "Program exited with code $EXIT_CODE"
fi

exit $EXIT_CODE
