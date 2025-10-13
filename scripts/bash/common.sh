#!/bin/bash

# Common utilities for bash scripts running in WSL that interact with Windows tools

# Convert WSL path to Windows path
to_win_path() {
    wslpath -w "$1"
}

# Convert Windows path to WSL path
to_wsl_path() {
    wslpath -u "$1"
}

# Get the repository root directory (WSL path)
get_repo_root() {
    local script_dir="$(cd "$(dirname "${BASH_SOURCE[1]}")" && pwd)"
    echo "$(cd "$script_dir/../.." && pwd)"
}

# Convert array of WSL paths to Windows paths
# Usage: win_paths=($(convert_paths_to_windows "${wsl_paths[@]}"))
convert_paths_to_windows() {
    local paths=("$@")
    for path in "${paths[@]}"; do
        wslpath -w "$path"
    done
}

# Print colored output
print_success() {
    echo -e "\033[32m$1\033[0m"
}

print_error() {
    echo -e "\033[31m$1\033[0m" >&2
}

print_warning() {
    echo -e "\033[33m$1\033[0m"
}

print_info() {
    echo -e "\033[36m$1\033[0m"
}
