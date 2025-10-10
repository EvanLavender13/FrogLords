# Finalize Maintenance Fix

### 1. Review Development Principles

Read `AGENTS.md` to ensure backlog updates follow documentation standards and capture learnings accurately.

### 2. Verify Completion

Confirm fix is ready to finalize:
-   **Path A (Trivial):** Implementation complete
-   **Path B (Standard):** Review passed with approval

If review identified issues and they haven't been resolved, stop and return to `IMPLEMENT_FIX.md`.

### 3. Update Maintenance Backlog

1.  Open `PLANS/MAINTENANCE_BACKLOG.md`
2.  Locate the completed item in its severity section (Critical/High/Medium/Low)
3.  Cut the entire item block (including description, files, suggested fix)
4.  Paste into `## Completed` section at the bottom
5.  Add completion metadata:
    ```markdown
    ### [Item Name]
    - **Files:** [original files]
    - **Severity:** [original severity]
    - **Description:** [original description]
    - **Resolution:** [brief summary of what was changed - 1 sentence]
    - **Completed:** [current date]
    ```

### 4. Document Learnings (Optional)

If fix revealed insights about code quality or patterns:
-   Add note to completed item about what was learned
-   If pattern appears elsewhere, add new item to appropriate severity section
-   If fix suggests process improvement, note for future workflow refinement

### 5. Check for Related Items

Scan backlog for similar items that could be batched:
-   If 2+ similar items exist (e.g., multiple redundant includes), note for future batch processing
-   If fix revealed additional issues, add them to appropriate severity section

### 6. Prepare Commit

Execute `TASKS/COMMIT.md` with maintenance-specific guidance:
-   **Domain:** Use code domain (e.g., `character`, `rendering`, `gui`) or `chore` for cross-cutting cleanup
-   **Description:** Brief imperative statement (e.g., "remove redundant includes", "fix include case sensitivity")
-   **Why paragraph:** Reference maintenance item severity and impact
-   **Changes list:** Keep minimal—maintenance fixes are usually single-line items
-   **Outcome:** State what's cleaner/fixed

Example commit message:
```
chore: remove redundant includes from character_panel

Eliminates redundant #include "character/tuning.h" already provided
via character_panel.h, reducing compilation dependencies.

- Remove duplicate include from character_panel.cpp

Include chain now correctly flows through header.
```

### 7. Recommend Next Steps

After user commits, suggest running `SELECT_ITEM.md` to select next maintenance item, or return to feature development.

### Tone & Constraints

-   Concise and systematic
-   Ensure backlog accurately reflects completion
-   Capture learnings while knowledge is fresh
-   Recommend batching similar items if pattern emerges
-   Keep completed section pruned (archive old items periodically)
-   Do not push to repository—user handles git operations
