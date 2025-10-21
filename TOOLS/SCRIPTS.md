# Bash Scripts for WSL

Scripts run in WSL, operate on Windows files, call Windows executables.

## Core Build

**configure.sh** - Configure CMake (Debug, Ninja)
**build.sh** - Build with existing configuration
**run.sh** - Execute the built binary
**clean.sh** - Remove build directory

**dev.sh** - All-in-one orchestrator
```bash
bash scripts/bash/dev.sh              # Build only
bash scripts/bash/dev.sh --run        # Build and run
bash scripts/bash/dev.sh --configure  # Configure and build
bash scripts/bash/dev.sh --all        # Clean, configure, build, run
```

## Code Quality

**format.sh** - Format all C/C++ sources with clang-format
**lint.sh** - Run clang-tidy on all translation units or specified files
```bash
bash scripts/bash/lint.sh                           # All .cpp files
bash scripts/bash/lint.sh src/physics/spring.cpp    # Specific file
```
**analyze.sh** - Run cppcheck static analysis

All call Windows `.exe` binaries. All require compile database except format.

## AI Tools

**codex.sh** - Codex CLI wrapper for second opinions
```bash
bash scripts/bash/codex.sh "your question"
bash scripts/bash/codex.sh --resume "follow-up"  # Continue session
```

**gemini.sh** - Gemini CLI wrapper for extended context analysis
```bash
bash scripts/bash/gemini.sh "your question"
bash scripts/bash/gemini.sh "@src/physics/ Analyze spring-damper implementation"
bash scripts/bash/gemini.sh "@./ Find accumulated state anti-patterns"
```

## Workflow

**One-time setup:**
```bash
bash scripts/bash/configure.sh
```

**Development loop:**
```bash
# Edit code, then:
bash scripts/bash/dev.sh --run
```

**Before commit:**
```bash
bash scripts/bash/format.sh
bash scripts/bash/lint.sh
```

**Clean rebuild:**
```bash
bash scripts/bash/dev.sh --all
```

## Common Utilities

**common.sh** provides path conversion and colored output:
- `to_win_path <wsl_path>` - WSL → Windows path
- `to_wsl_path <win_path>` - Windows → WSL path
- `get_repo_root` - Repository root (WSL path)
- `print_success/error/warning/info <msg>` - Colored output

## Philosophy

Zero overhead—build and execution happen on Windows. Only script orchestration runs in WSL.

All scripts source `common.sh` for path handling between WSL and Windows.
