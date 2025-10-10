# Implement Maintenance Fix

### 1. Review Development Principles

Read `AGENTS.md` to ensure changes adhere to coding standards:
-   Naming conventions (snake_case)
-   File organization and dependency flow
-   Code formatting (4-space indent, braces on same line)
-   Documentation guidelines

### 2. Gather Context

1.  **Recall Selected Item:** Reference the item description from `SELECT_ITEM.md`
2.  **Read Affected Files:** Read all source files mentioned in the maintenance item (`.h` and `.cpp`)
3.  **Understand Current State:** Analyze the code smell or issue being addressed

### 3. Plan Changes

Before modifying code, state concise summary:
-   What is being changed (1 sentence)
-   Why it's being changed (reference maintenance backlog item)
-   Expected files affected

### 4. Execute Fix

Implement the suggested fix from the maintenance backlog:
-   Follow coding standards exactly (snake_case, formatting, organization)
-   Make minimal changes—fix only what's described in the item
-   Preserve existing behavior unless fixing a bug
-   Update related documentation if public API changes

### 5. Verify Mechanical Correctness

-   **Includes:** Check that new includes (if any) respect dependency flow
-   **Naming:** Verify all new symbols follow snake_case convention
-   **Formatting:** Ensure 4-space indentation and consistent brace style
-   **Build:** Mentally verify change won't break compilation (check all references if renaming/moving)

### 6. Track Changes

List all modified files with brief description of changes made in each.

### 7. Determine Next Step

-   **Path A (Trivial):** Proceed directly to `FINALIZE_FIX.md`
-   **Path B (Standard):** Proceed to `REVIEW_FIX.md` for verification

### Tone & Constraints

-   Concise and direct; summarize before acting
-   Strictly adhere to suggested fix from backlog
-   Avoid scope creep—fix only the stated item
-   No refactoring beyond what's specified
-   No "while I'm here" fixes (track those separately in backlog)
-   Preserve git blame history (avoid gratuitous reformatting)
