# Implement Maintenance Fix

### 1. Review Development Principles

Read `AGENTS.md` to ensure changes adhere to coding standards:
- Naming conventions (snake_case)
- File organization and dependency flow
- Code formatting (4-space indent, braces on same line)
- Documentation guidelines

### 2. Gather Context

1. Reference item from SELECT_ITEM
2. Read all affected source files (`.h` and `.cpp`)
3. Analyze code smell or issue

### 3. Plan Changes

State concise summary before modifying:
- What is being changed (1 sentence)
- Why (reference backlog item)
- Expected files affected

### 4. Execute Fix

Implement suggested fix from backlog:
- Follow coding standards exactly
- Make minimal changes—fix only what's described
- Preserve existing behavior unless fixing bug
- Update documentation if public API changes

### 5. Verify Mechanical Correctness

- **Includes:** New includes respect dependency flow?
- **Naming:** All new symbols follow snake_case?
- **Formatting:** 4-space indent, consistent braces?
- **Build:** Won't break compilation? (check references if renaming/moving)

### 6. Track Changes

List all modified files with brief description of changes.

### 7. Determine Next Step

- **Path A (Trivial):** Proceed to FINALIZE_FIX
- **Path B (Standard):** Proceed to REVIEW_FIX

### Tone & Constraints

- Concise; summarize before acting
- Strictly adhere to suggested fix
- Avoid scope creep—fix only stated item
- No refactoring beyond what's specified
- No "while I'm here" fixes (track separately)
- Preserve git blame history (avoid gratuitous reformatting)
