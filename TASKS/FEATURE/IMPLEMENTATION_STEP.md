# Execute and Verify Implementation Step

### 1. Identify Target Feature

Extract the feature name from the current git branch name (format: `feature/<feature_name>`).

### 2. Read Guiding Principles

Read `AGENTS.md` to ensure all actions adhere to development principles and coding standards.

### 3. Review Plans

1.  **Read Feature Description:** Read `PLANS/feature_<feature_name>.md` for high-level goals and context
2.  **Read Implementation Plan:** Read `PLANS/implementation_<feature_name>.md` for ordered checklist of tasks

### 4. Execute One Step

1.  **Identify Next Major Step:** Find the first major step in implementation plan with unchecked boxes (e.g., "####" heading)
2.  **Analyze Code:** Read relevant source files (`.h` and `.cpp`) to understand current state
3.  **Perform Actions:** Before taking action, provide concise summary of changes referencing implementation plan. Then execute all actions for that single major step only
4.  **Run Quality Gates:** After completing the step, run applicable quality checks:
    - Build/compile the code (if the step touched source files)
    - Format check (run clang-format if applicable)
    - Smoke test (if the step completes a major section)
5.  **Self-Verify:** Quickly review the completed work:
    - Does it match the implementation plan's intent?
    - Any obvious violations of `AGENTS.md` principles?
    - System ownership conflicts (duplicates, missing reference updates)?
    - If issues found: fix them before marking complete
6.  **Mark Complete:**
    - Propose change to `PLANS/implementation_<feature_name>.md` marking completed items as `[x]`
    - Update the implementation plan with a running list of files changed during this step

### 5. Await Guidance

Await further instructions before proceeding to next step or committing changes.

### Tone & Constraints

-   Concise and imperative
-   Adhere to `AGENTS.md` coding standards (snake_case, file organization, dependency flow)
-   Respect gameplay-first principles (interruptible transitions, physics-first control)
-   Complete all sub-items for current major step only
-   Do not proceed to next major step without guidance
