# Development Scripts

All build and quality scripts in `scripts/bash/`. Run in WSL; call Windows executables. Zero performance overhead.

## Location
```bash
cd /mnt/c/Users/EvanUhhh/source/repos/FrogLords
```

## Build
```bash
bash scripts/bash/configure.sh        # CMake (once or after clean)
bash scripts/bash/build.sh            # Compile
bash scripts/bash/run.sh              # Execute
bash scripts/bash/clean.sh            # Wipe build/

# Shortcuts
bash scripts/bash/dev.sh --run        # Build + run
bash scripts/bash/dev.sh --all        # Clean + configure + build + run
```

## Code Quality
```bash
bash scripts/bash/run_clang_format.sh  # Format
bash scripts/bash/run_clang_tidy.sh    # Lint
bash scripts/bash/run_cppcheck.sh      # Static analysis
```

## Git Workflow
```bash
bash scripts/bash/create_branch.sh feature <name>
bash scripts/bash/create_branch.sh improve <name>
bash scripts/bash/review_staged_changes.sh
bash scripts/bash/commit_changes.sh -m "message"
bash scripts/bash/commit_changes.sh -f commit_msg.txt
```

## Standard Workflow
```bash
# After code changes
bash scripts/bash/build.sh
bash scripts/bash/run_clang_format.sh
bash scripts/bash/run.sh

# Pre-commit
bash scripts/bash/review_staged_changes.sh
git add .
bash scripts/bash/commit_changes.sh -m "description"
```

## VSCode Integration
All tasks use these scripts. Default build: Ctrl+Shift+B.

## Implementation
Scripts run in WSL, call Windows executables (cmake.exe, ninja.exe, clang-format.exe, etc.). Paths auto-converted via `wslpath`. See `scripts/bash/common.sh` for path conversion and colored output helpers.

From Windows PowerShell: prefix commands with `wsl`, e.g., `wsl bash scripts/bash/build.sh`.
