#!/bin/bash

# Creates a new git branch with workflow-specific prefix
# Branches: feature/<name> or improve/<name>

set -e

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

usage() {
    echo "Usage: $0 <type> <name>"
    echo ""
    echo "Arguments:"
    echo "  type    Workflow type: feature or improve"
    echo "  name    Branch name (without prefix)"
    echo ""
    echo "Examples:"
    echo "  $0 feature static-keyframe-preview"
    echo "  $0 improve yaw-direction-utilities"
    exit 1
}

# Validate arguments
if [ $# -ne 2 ]; then
    usage
fi

TYPE="$1"
NAME="$2"

# Validate type
if [ "$TYPE" != "feature" ] && [ "$TYPE" != "improve" ]; then
    print_error "Invalid type: $TYPE"
    echo "Valid types: feature, improve"
    exit 1
fi

if [ -z "$NAME" ]; then
    print_error "Branch name is required"
    usage
fi

BRANCH_NAME="$TYPE/$NAME"

print_info "Creating and switching to branch: $BRANCH_NAME"
git.exe checkout -b "$BRANCH_NAME"

EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    echo ""
    print_success "Branch created successfully!"
    print_info "Current branch: $BRANCH_NAME"
else
    print_error "Failed to create branch"
    exit $EXIT_CODE
fi

exit 0
