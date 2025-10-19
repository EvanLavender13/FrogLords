#!/bin/bash

# Codex CLI wrapper - minimal progress, clean output
#
# Usage:
#   codex.sh "your question"
#   codex.sh --resume "follow-up question"

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

# Parse arguments
RESUME=false
QUERY=""

while [[ $# -gt 0 ]]; do
    case $1 in
        --resume|-r)
            RESUME=true
            shift
            ;;
        *)
            QUERY="$*"
            break
            ;;
    esac
done

# Validate query
if [[ -z "$QUERY" ]]; then
    print_error "Error: No query provided"
    echo "Usage:"
    echo "  codex.sh \"your question\""
    echo "  codex.sh --resume \"follow-up\""
    exit 1
fi

# Temp files
STDOUT_FILE=$(mktemp)
STDERR_FILE=$(mktemp)
trap "rm -f $STDOUT_FILE $STDERR_FILE" EXIT

# Build command
if [[ "$RESUME" == true ]]; then
    CODEX_CMD="codex e resume --last"
else
    CODEX_CMD="codex e"
fi

# Run in background
print_info "Consulting Codex..."
echo "$QUERY" | $CODEX_CMD > "$STDOUT_FILE" 2>"$STDERR_FILE" &
CODEX_PID=$!

# Progress dots
echo -n "Progress: "
while kill -0 $CODEX_PID 2>/dev/null; do
    echo -n "."
    sleep 2
done
echo ""

# Check result
wait $CODEX_PID
EXIT_CODE=$?

if [[ $EXIT_CODE -eq 0 ]]; then
    print_success "✓ Complete"
    echo ""
    cat "$STDOUT_FILE"
    exit 0
else
    print_error "✗ Failed (exit $EXIT_CODE)"

    # Show errors
    if [[ -s "$STDERR_FILE" ]]; then
        echo ""
        print_warning "Errors:"
        tail -20 "$STDERR_FILE" >&2  # Last 20 lines only
    fi

    # Show partial output
    if [[ -s "$STDOUT_FILE" ]]; then
        echo ""
        print_warning "Partial output:"
        cat "$STDOUT_FILE"
    fi
    exit $EXIT_CODE
fi
