# Bash Scripts for WSL

Cross-platform bash scripts that run in WSL but operate on Windows files and call Windows executables.

## Overview

These scripts enable LLM CLI agents and other tools to work in their native bash environment while maintaining full Windows build tooling performance. They work by:

1. Running in WSL (Linux) environment
2. Operating on Windows files via `/mnt/c/...` mounts
3. Calling Windows `.exe` binaries directly (e.g., `clang-format.exe`, `cmake.exe`)
4. Converting paths between WSL and Windows formats as needed

## Prerequisites

- WSL installed and configured
- Windows executables in PATH (clang-format, cmake, ninja, etc.)
- Repository accessible from WSL at `/mnt/c/Users/.../FrogLords`

## Usage

From PowerShell or CMD (using WSL):
```powershell
# Quick build
wsl bash scripts/bash/build.sh

# Build and run
wsl bash scripts/bash/dev.sh --run

# Full rebuild
wsl bash scripts/bash/dev.sh --all

# Format code
wsl bash scripts/bash/run_clang_format.sh
```

From inside WSL:
```bash
cd /mnt/c/Users/EvanUhhh/source/repos/FrogLords

# Quick build
bash scripts/bash/build.sh

# Build and run
bash scripts/bash/dev.sh --run

# Configure, build, and run
bash scripts/bash/dev.sh --configure --run
```

## Available Scripts

### Build & Run
- `configure.sh` - Configure CMake project (Debug build with Ninja)
- `build.sh` - Build the project using existing configuration
- `run.sh` - Run the built executable
- `clean.sh` - Remove the build directory
- `dev.sh` - All-in-one build script with options (see `--help`)

### Code Quality
- `run_clang_format.sh` - Format all C/C++ sources with clang-format
- `run_clang_tidy.sh` - Run clang-tidy static analysis on all translation units
- `run_cppcheck.sh` - Run cppcheck static analysis using compile database

### Git Workflow
- `create_branch.sh` - Create a new feature or improve branch
- `review_staged_changes.sh` - Review staged files and diff before committing
- `commit_changes.sh` - Commit with message (inline or from file)

## Common Utilities

`common.sh` provides shared functions:

- `to_win_path <wsl_path>` - Convert WSL path to Windows path
- `to_wsl_path <win_path>` - Convert Windows path to WSL path
- `get_repo_root` - Get repository root directory (WSL path)
- `convert_paths_to_windows <paths...>` - Convert array of paths to Windows format
- `print_success <msg>` - Print green success message
- `print_error <msg>` - Print red error message
- `print_warning <msg>` - Print yellow warning message
- `print_info <msg>` - Print cyan info message

## Creating New Scripts

Template:
```bash
#!/bin/bash
set -e

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

REPO_ROOT="$(get_repo_root)"

# Your logic here
# Use .exe suffix for Windows binaries
# Convert paths with to_win_path when passing to Windows executables

print_success "Done!"
```

## Workflow Examples

**Typical development workflow:**
```bash
# One-time setup
bash scripts/bash/configure.sh

# Edit code, then build and test
bash scripts/bash/dev.sh --run

# Format before commit
bash scripts/bash/run_clang_format.sh
```

**LLM Agent workflow:**
```bash
# Make code changes, then validate
bash scripts/bash/build.sh                  # Compile
bash scripts/bash/run_clang_format.sh       # Format
bash scripts/bash/run_clang_tidy.sh         # Lint
bash scripts/bash/run.sh                    # Test
```

**Clean rebuild:**
```bash
bash scripts/bash/dev.sh --all
# or individual steps:
bash scripts/bash/clean.sh
bash scripts/bash/configure.sh
bash scripts/bash/build.sh
```

## Path Handling

WSL paths: `/mnt/c/Users/EvanUhhh/source/repos/FrogLords/...`
Windows paths: `C:\Users\EvanUhhh\source\repos\FrogLords\...`

When passing paths to Windows executables, always convert using `wslpath -w` or `convert_paths_to_windows`.

## Performance

Zero overhead compared to native Windows execution - the build and execution happen entirely on Windows. Only the script orchestration runs in WSL.
