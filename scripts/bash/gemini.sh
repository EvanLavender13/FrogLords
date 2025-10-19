#!/bin/bash

# Gemini CLI wrapper - extended context analysis
#
# Usage:
#   gemini.sh "your question"
#   gemini.sh "@src/physics/ Analyze spring-damper implementation"
#   gemini.sh "@src/ @tests/ Find test coverage gaps"

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

# Parse arguments
QUERY=""

if [[ $# -eq 0 ]]; then
    print_error "Error: No query provided"
    echo "Usage:"
    echo "  gemini.sh \"your question\""
    echo "  gemini.sh \"@src/physics/ Analyze implementation\""
    echo "  gemini.sh \"@./ Find accumulated state anti-patterns\""
    exit 1
fi

QUERY="$*"

# Temp files
STDOUT_FILE=$(mktemp)
STDERR_FILE=$(mktemp)
trap "rm -f $STDOUT_FILE $STDERR_FILE" EXIT

# Build command
GEMINI_CMD="gemini -p"

# Run in background
print_info "Analyzing with extended context..."
echo "$QUERY" | $GEMINI_CMD > "$STDOUT_FILE" 2>"$STDERR_FILE" &
GEMINI_PID=$!

# Progress dots
echo -n "Progress: "
while kill -0 $GEMINI_PID 2>/dev/null; do
    echo -n "."
    sleep 2
done
echo ""

# Check result
wait $GEMINI_PID
EXIT_CODE=$?

if [[ $EXIT_CODE -eq 0 ]]; then
    print_success "✓ Analysis complete"
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
