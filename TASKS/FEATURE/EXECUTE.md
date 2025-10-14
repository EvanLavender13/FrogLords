# Execute and Verify Implementation Step

### 1. Identify Target Feature

Extract the feature name from the current git branch name (format: `feature/<feature_name>`).

### 2. Read Guiding Principles

Read `AGENTS.md` to ensure all actions adhere to development principles and coding standards.

### 3. Review Plans

1.  **Read Feature Description:** Read `PLANS/<feature_name>_FEATURE.md` for high-level goals and context
2.  **Read Implementation Plan:** Read `PLANS/<feature_name>_PLAN.md` for ordered checklist of tasks

### 4. Execute ONE Step (Then Stop)

**CONSTRAINT: Complete exactly one numbered step from the implementation plan, then stop and report.**

1.  **Identify Next Incomplete Step:** Find the first top-level numbered step (e.g., "### 1.", "### 2.") in the implementation plan with unchecked boxes
2.  **Analyze Code:** Read relevant source files (`.h` and `.cpp`) to understand current state
3.  **Perform Actions:** 
    - Provide concise summary of changes referencing the specific step number
    - Execute all sub-items for ONLY that numbered step
    - **STOP** - Do not proceed to the next numbered step
4.  **Run Quality Gates:** After completing the step, run applicable quality checks:
    - Build/compile the code (if the step touched source files)
    - Format check (run clang-format if applicable)
    - Smoke test (if the step completes a major section)
5.  **Self-Verify:** Quickly review the completed work:
    - Does it match the implementation plan's intent?
    - Any obvious violations of `AGENTS.md` principles?
    - System ownership conflicts (duplicates, missing reference updates)?
    - If issues found: fix them before marking complete
6.  **Mark Complete & HALT:**
    - Update `PLANS/<feature_name>_PLAN.md` marking completed sub-items as `[x]`
    - Update the plan with files changed during this step
    - **Report completion using the template below and WAIT for explicit instruction to continue**

**Step Completion Report Template:**
```
Completed: [Step number and title]
Files changed: [list]
Quality gates: [pass/fail for each]
Next step: [number and title] - AWAITING INSTRUCTION
```

### 5. Explicit Halt

**DO NOT:**
- Proceed to the next numbered step automatically
- Continue "while you're at it" changes
- Anticipate or prepare future steps

**DO:**
- Stop immediately after completing one numbered step
- Report what was completed using the template
- Wait for user to explicitly say "continue" or "next step"

### Tone & Constraints

-   Concise and imperative
-   Adhere to `AGENTS.md` coding standards (snake_case, file organization, dependency flow)
-   Respect gameplay-first principles (interruptible transitions, physics-first control)
-   Complete all sub-items for current major step only
-   Do not proceed to next major step without guidance
