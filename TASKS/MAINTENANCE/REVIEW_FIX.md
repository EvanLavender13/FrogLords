# Review Maintenance Fix

### 1. Review Development Principles

Read `AGENTS.md` for coding standards and quality guidelines to verify fix compliance.

### 2. Gather Context

1.  **Recall Maintenance Item:** Reference the original item description from `SELECT_ITEM.md`
2.  **Review Implementation:** Check list of modified files from `IMPLEMENT_FIX.md`
3.  **Read Modified Code:** Read all changed source files (`.h` and `.cpp`)

### 3. Verify Fix Quality

Cross-reference implementation against standards:

-   **Issue Resolution:** Does the fix address the maintenance item completely?
-   **Code Standards:**
    -   Naming: snake_case for all symbols
    -   Formatting: 4-space indent, braces on same line
    -   Organization: Correct file structure (header guards, include order)
-   **Dependency Flow:**
    -   New includes respect bottom-up layering (Foundation → Character → Rendering → App)
    -   No circular dependencies introduced
-   **Scope Discipline:**
    -   Only the stated issue was fixed
    -   No unrelated changes or "while I'm here" modifications
-   **Behavior Preservation:**
    -   Existing functionality unchanged (unless fixing bug)
    -   No new edge cases introduced

### 4. Check for Side Effects

-   **References:** If symbols were renamed/moved, verify all call sites updated
-   **Headers:** If includes were changed, verify no missing dependencies
-   **Constants:** If values were consolidated, verify all uses point to new location

### 5. Report Findings

Provide concise analysis:

1.  **Fix Summary:** One sentence describing what was changed
2.  **Verification:** Confirm fix resolves the maintenance item (or note gaps)
3.  **Issues Found:** List any violations with specific file/line references
4.  **Recommendation:** Either "Fix verified. Ready to finalize." or "Address issues before proceeding."

### 6. Await Guidance

Do not proceed to finalization. Await user decision on whether to:
-   Fix identified issues
-   Accept minor deviations as acceptable
-   Proceed to `FINALIZE_FIX.md`

### Tone & Constraints

-   Concise and direct; detail scales with complexity
-   Focus on standards compliance and completeness
-   Flag any scope creep or unrelated changes
-   Distinguish critical issues from polish opportunities
-   Verify fix doesn't introduce new maintenance items
