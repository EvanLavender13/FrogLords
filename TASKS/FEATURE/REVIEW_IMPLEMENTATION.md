# Perform Code Review

### 1. Identify Target Feature

Extract the feature name from the current git branch name (format: `feature/<feature_name>`).

### 2. Gather Context

1.  **Read Principles:** Read `AGENTS.md` for development principles and coding standards
2.  **Read Planning Documents:** Read `PLANS/iteration_<feature_name>.md` and `PLANS/implementation_<feature_name>.md`
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
2.  Propose adding an `## Approval` section to `PLANS/iteration_<feature_name>.md`

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

### 1. Summary

(A brief, one-paragraph summary of the findings. State whether the implementation is approved or if revisions are required.)

### 2. Violations & Required Changes

(A bulleted list of direct violations of principles that must be fixed. For each, cite the source file, the violated principle, and the specific code.)

- **Violation:** (e.g., Incorrect Naming Convention)
  - **File:** `src/character/controller.cpp`
  - **Principle:** "uniform snake_case for user code"
  - **Details:** The function `updateCharacterState` uses camelCase instead of snake_case.
  - **Suggestion:** Rename the function to `update_character_state`.

### 3. Misalignments & Suggestions

(A bulleted list of items that are not direct violations but could be improved to better align with the project's philosophy.)

- **Misalignment:** (e.g., Potential for Simplification)
  - **File:** `src/character/locomotion.cpp`
  - **Principle:** "Simplicity over sophistication"
  - **Details:** The jump logic uses a complex state machine that could be simplified to a few boolean flags.
  - **Suggestion:** Consider refactoring the jump logic to use a simpler state representation.

### 4. Approval

- **Status:** [Approved / Pending Revisions]
- **Reviewer:** [Your Name/Agent ID]
- **Date:** [Current Date]
```
