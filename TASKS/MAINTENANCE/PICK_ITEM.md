# Pick Maintenance Item

### 1. Review Development Principles

Read `AGENTS.md` to synthesize coding standards and quality guidelines. Essential for:
-   Identifying highest-impact maintenance work
-   Understanding severity classifications
-   Prioritizing based on code health and stability

### 2. Review Maintenance Backlog

1.  Open `PLANS/MAINTENANCE_BACKLOG.md`
2.  Scan sections in priority order: Critical → High → Medium → Low
3.  Look for items that:
    -   Have clear, actionable descriptions
    -   Affect stable, well-understood code (avoid touching uncertain systems)
    -   Can be completed independently (no complex dependencies)

### 3. Select Item

Choose **one item** based on:
-   **Severity:** Higher severity first (Critical > High > Medium > Low)
-   **Impact:** Prefer fixes that improve multiple call sites or prevent future bugs
-   **Clarity:** Select items with concrete "Suggested Fix" sections
-   **Isolation:** Prefer single-file fixes over multi-file changes when possible

### 4. Verify Item Details

For the selected item, confirm:
-   Files and line numbers are specified
-   Expected behavior change is clear
-   No dependencies on other incomplete maintenance items

### 5. Classify Complexity

Determine workflow path:

**Path A: Trivial Fix** (< 5 lines, single file, mechanical change)
-   Examples: Remove redundant include, fix typo, adjust formatting
-   Next step: Proceed directly to `IMPLEMENT_FIX.md` then `FINALIZE_FIX.md` (skip review)

**Path B: Standard Fix** (Multi-line, behavior change, or multi-file)
-   Examples: Extract constants, refactor logic, fix case sensitivity issues
-   Next step: Proceed to `IMPLEMENT_FIX.md` → `REVIEW_FIX.md` → `FINALIZE_FIX.md`

### 6. Document Selection

State selected item clearly:
-   Item name/description
-   Severity level
-   Files affected
-   Workflow path (A or B)

### Tone & Constraints

-   Concise and direct
-   Prefer high-severity items over low-severity
-   Avoid touching uncertain or actively-changing systems
-   Select only one item at a time (batch similar items later if pattern emerges)
-   Skip items that require architectural decisions or design discussion
