# Perform Code Review

### 1. Identify Target Feature

Extract the feature name from the current git branch name (format: `feature/<feature_name>`).

### 2. Gather Context

1.  **Read Principles:** Read `AGENTS.md` for development principles and coding standards
2.  **Read Planning Documents:** Read `PLANS/feature_<feature_name>.md` and `PLANS/implementation_<feature_name>.md`
3.  **Verify Implementation Completeness:** Check if all checklist items in `PLANS/implementation_<feature_name>.md` are marked as complete (`[x]`). If not, report this and stop; a review cannot be performed on incomplete work.
4.  **Read Implemented Code:** Read the contents of all source files (`.h` and `.cpp`) that were modified as part of the implementation plan.

### 3. Perform Code Review

Cross-reference the implemented code against the principles from `AGENTS.md` and the goals from the iteration plan.

-   **Code Standards:** Does the code adhere to naming conventions (snake_case), formatting, and file organization?
-   **Gameplay-First Principles:** Does the implementation respect the "do no harm" rule? (input maps to acceleration, interruptible transitions, physics-first, etc.)
-   **Simplicity & Clarity:** Is the code clear and simple? Are there overly clever or complex solutions where simpler ones would suffice?
-   **Systemic Design:** Does the implementation favor reusable systems over one-off solutions? Does it align with the goal of emergence?
-   **Dependency Flow:** Does the code respect the dependency flow outlined in `AGENTS.md`? (e.g., no sideways includes at high layers).

### 4. Save and Propose

Generate a new markdown document summarizing the findings.

-   **Filename:** `PLANS/code_review_<feature_name>.md`
-   **Content:** Use the template below
-   Propose the document to the user for review

### 5. Approve (If Review Passes)

If the review is successful and the implementation is complete:

1.  Propose adding an `## Approval` section to `PLANS/implementation_<feature_name>.md`

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
**Status:** APPROVED | FIX ISSUES

### Summary

[One paragraph: Is implementation approved? Major issues? Recommendation?]

### Violations (Must Fix)

- **[Category]:** [Issue]
  - **File:** [src/path/to/file.cpp:line]
  - **Principle:** [Which principle violated]
  - **Fix:** [How to address]

**If none:** No violations found.

### Suggestions for Improvement

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

### Recommendation

**Reasoning:** [Brief justification for status]

**Next Steps:**
- If APPROVED: Proceed to FINALIZE_FEATURE
- If FIX ISSUES: Address items, re-review
```
