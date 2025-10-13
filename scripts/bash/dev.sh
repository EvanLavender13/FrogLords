#!/bin/bash

# All-in-one build script with various options
# This script runs in WSL and orchestrates the build process

set -e

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

# Parse arguments
CLEAN=false
CONFIGURE=false
BUILD=true
RUN=false

usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --clean       Clean build directory before building"
    echo "  --configure   Run CMake configuration"
    echo "  --run         Run the executable after building"
    echo "  --all         Clean, configure, build, and run"
    echo "  -h, --help    Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                  # Just build"
    echo "  $0 --configure      # Configure and build"
    echo "  $0 --run            # Build and run"
    echo "  $0 --all            # Full rebuild and run"
    exit 0
}

for arg in "$@"; do
    case $arg in
        --clean)
            CLEAN=true
            ;;
        --configure)
            CONFIGURE=true
            ;;
        --run)
            RUN=true
            ;;
        --all)
            CLEAN=true
            CONFIGURE=true
            BUILD=true
            RUN=true
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "Unknown option: $arg"
            usage
            ;;
    esac
done

# Execute build steps
if [ "$CLEAN" = true ]; then
    print_info "Step 1: Cleaning..."
    bash "$SCRIPT_DIR/clean.sh"
    echo ""
fi

if [ "$CONFIGURE" = true ]; then
    print_info "Step 2: Configuring..."
    bash "$SCRIPT_DIR/configure.sh"
    echo ""
fi

if [ "$BUILD" = true ]; then
    print_info "Step 3: Building..."
    bash "$SCRIPT_DIR/build.sh"
    echo ""
fi

if [ "$RUN" = true ]; then
    print_info "Step 4: Running..."
    bash "$SCRIPT_DIR/run.sh"
fi

print_success "All steps completed successfully!"
exit 0
