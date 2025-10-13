#!/bin/bash

# Commit staged changes with provided message
# Supports both inline messages and message files for multi-line commits

set -e

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

usage() {
    echo "Usage: $0 [-m MESSAGE] [-f MESSAGE_FILE]"
    echo ""
    echo "Options:"
    echo "  -m MESSAGE       Commit message (inline)"
    echo "  -f MESSAGE_FILE  Commit message from file"
    echo ""
    echo "Examples:"
    echo "  $0 -m 'Fix bug in controller'"
    echo "  $0 -f commit_message.txt"
    exit 1
}

MESSAGE=""
MESSAGE_FILE=""

# Parse arguments
while getopts "m:f:h" opt; do
    case $opt in
        m)
            MESSAGE="$OPTARG"
            ;;
        f)
            MESSAGE_FILE="$OPTARG"
            ;;
        h)
            usage
            ;;
        *)
            usage
            ;;
    esac
done

# Validate input
if [ -z "$MESSAGE" ] && [ -z "$MESSAGE_FILE" ]; then
    print_error "Either -m or -f must be provided"
    usage
fi

if [ -n "$MESSAGE" ] && [ -n "$MESSAGE_FILE" ]; then
    print_error "Cannot specify both -m and -f"
    usage
fi

if [ -n "$MESSAGE_FILE" ]; then
    # Commit using message file
    if [ ! -f "$MESSAGE_FILE" ]; then
        print_error "Message file not found: $MESSAGE_FILE"
        exit 1
    fi
    
    # Convert to Windows path if needed
    WIN_MESSAGE_FILE="$(to_win_path "$MESSAGE_FILE")"
    git.exe commit -F "$WIN_MESSAGE_FILE"
else
    # Commit using inline message
    git.exe commit -m "$MESSAGE"
fi

EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    print_success "Commit successful"
else
    print_error "Commit failed with exit code $EXIT_CODE"
    exit $EXIT_CODE
fi

exit 0
