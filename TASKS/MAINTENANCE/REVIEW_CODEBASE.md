# Review Codebase for Maintenance Items

### 1. Read Guiding Principles

Read `AGENTS.md` to ensure all evaluations align with development principles, code standards, and project philosophy.

### 2. Perform Random Walk Review

1.  **Scan Source Directory:** Navigate the `src/` directory tree and read implementation files
2.  **Target Areas:** Focus on implementation files (`.cpp`, `.h`) across all subsystems
3.  **Look For:**
    - **Code smells:** Dead code, commented-out blocks, temporary debugging code left in place
    - **Naming violations:** camelCase or PascalCase in user code (should be snake_case)
    - **Unused includes:** Headers included but not referenced
    - **Duplicate logic:** Similar code patterns that could share a helper function
    - **Magic numbers:** Hardcoded values that should be named constants
    - **Inconsistent formatting:** Spacing, indentation, or brace style deviations
    - **Principle violations:** Dependency flow breaks, reactive layers controlling core logic
    - **Leftover features:** Incomplete experiments, commented TODOs, orphaned systems
    - **Missing const-correctness:** Functions that could be const but aren't
    - **Brief documentation gaps:** Non-obvious public API without comments

### 3. Categorize Findings

Group discovered items into severity tiers:

-   **Critical:** Direct principle violations, bugs, or correctness issues
-   **High:** Code smells that impact readability or maintainability significantly
-   **Medium:** Minor inconsistencies or optimizations that would improve quality
-   **Low:** Nitpicks or polish items with minimal impact

### 4. Document in Maintenance Backlog

1.  **Target File:** `PLANS/MAINTENANCE_BACKLOG.md`
2.  **Format:** Use structured entries with file location, description, severity, and suggested fix
3.  **Group by Category:** Organize findings by subsystem (character, rendering, foundation, etc.)
4.  **Update Existing:** If backlog exists, append new findings; if not, create it

### 5. Propose High-Value Fixes

If any **Critical** or **High** severity items are discovered during the review:

1.  Summarize the findings concisely
2.  Propose immediate fixes for critical issues
3.  Suggest prioritization for high-severity items

Otherwise, inform the user of the total findings count and await guidance.

### Tone & Constraints

-   Concise and direct; no preamble or postamble
-   Focus on concrete, actionable findings over subjective opinions
-   Cite specific file paths and line ranges
-   Detail scales with severity (critical items get more explanation)
-   Avoid bikeshedding; flag real issues only
-   Do not propose changes during the review phase; document findings first

---

### Maintenance Backlog Template

```markdown
# Maintenance Backlog

**Purpose:** Tracking for small improvements, refactors, and cleanup items that don't constitute full features but benefit code quality.

**Status:** Living document (updated after codebase reviews)

**Last Review:** [Date]

---

## Critical

(Direct violations, bugs, correctness issues requiring immediate attention)

- **[Issue Title]**
  - *File:* `src/path/to/file.cpp:123`
  - *Severity:* Critical
  - *Description:* Brief description of the issue
  - *Principle Violated:* (if applicable)
  - *Suggested Fix:* Concrete action to resolve

## High

(Significant code smells impacting readability or maintainability)

## Medium

(Minor inconsistencies or quality improvements)

## Low

(Nitpicks and polish items)

---

## Completed

(Resolved items moved here for reference; prune periodically)
```
