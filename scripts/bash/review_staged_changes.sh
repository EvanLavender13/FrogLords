#!/bin/bash

# Review staged changes before committing
# This script runs in WSL and calls Windows git.exe

set -e

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_info "=== Staged Files ==="
git.exe status --short

echo ""
print_info "=== Staged Changes ==="
git.exe diff --staged
