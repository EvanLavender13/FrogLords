# Perform Code Review

### 1. Review Development Principles

Read `AGENTS.md` for development principles and coding standards:
- snake_case naming conventions
- 4-space indent, braces on same line
- Dependency flow (Foundation → Character → Rendering → App)
- Gameplay-first principles (input→acceleration, interruptible)

### 2. Gather Context

Extract the feature name from the current git branch name (format: `feature/<feature_name>`).

1.  **Read Planning Documents:** Read `PLANS/<feature_name>_FEATURE.md` and `PLANS/<feature_name>_PLAN.md`
2.  **Verify Implementation Completeness:** Check if all checklist items in `PLANS/<feature_name>_PLAN.md` are marked as complete (`[x]`). If not, report this and stop; a review cannot be performed on incomplete work.
3.  **Read Implemented Code:** Read the contents of all source files (`.h` and `.cpp`) that were modified as part of the implementation plan.

### 3. Perform Code Review

Cross-reference the implemented code against the principles from `AGENTS.md` and the goals from the iteration plan.

-   **Code Standards:** Does the code adhere to naming conventions (snake_case), formatting, and file organization?
-   **Gameplay-First Principles:** Does the implementation respect the "do no harm" rule? (input maps to acceleration, interruptible transitions, physics-first, etc.)
-   **Simplicity & Clarity:** Is the code clear and simple? Are there overly clever or complex solutions where simpler ones would suffice?
-   **Systemic Design:** Does the implementation favor reusable systems over one-off solutions? Does it align with the goal of emergence?
-   **Dependency Flow:** Does the code respect the dependency flow outlined in `AGENTS.md`? (e.g., no sideways includes at high layers).

### 4. Save and Propose

Generate a new markdown document summarizing the findings.

-   **Filename:** `PLANS/<feature_name>_CODE_REVIEW.md`
-   **Content:** Use the template below
-   Propose the document to the user for review

### 5. Approve (If Review Passes)

If the review is successful and the implementation is complete:

1.  Propose adding an `## Approval` section to `PLANS/<feature_name>_PLAN.md`

The approval section should contain reviewer signature and timestamp.

### Tone & Constraints

-   Concise and direct; no preamble or postamble
-   Detail scales with risk and complexity
-   Cite specific principles and code locations
-   Focus on actionable fixes over opinions

---

### Code Review Template

```markdown
# Code Review: [Feature Name]

**Date:** [YYYY-MM-DD]
**Status:** APPROVED | FIX_ISSUES | ROLLBACK

### Summary

[One paragraph: Is implementation approved? Major issues? Recommendation?]

### Violations (Must Fix)

- **[Category]:** [Issue]
  - **File:** [src/path/to/file.cpp:line]
  - **Principle:** [Which principle violated]
  - **Fix:** [How to address]

**If none:** No violations found.

### Suggestions (Consider)

- **[Description]:** [Why helpful]
  - **File:** [src/path/to/file.cpp:line]
  - **Impact:** [How much better]

**If none:** Implementation looks solid.

### Code Quality Checks

- [ ] Naming: snake_case
- [ ] Formatting: 4-space indent, braces on same line
- [ ] Dependency flow respected
- [ ] Gameplay-first principles (input→acceleration, interruptible, physics-first)
- [ ] Simplicity over sophistication

### Actionable Items

- [ ] [Specific action needed]

**If none:** Ready to proceed with finalization.

### Recommendation

**Reasoning:** [Brief justification for status]

**Next Steps:**
- If APPROVED: Proceed to FINALIZE
- If FIX_ISSUES: Address items, re-review
- If ROLLBACK: Defer feature, update backlog
```
