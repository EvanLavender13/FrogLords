# Request Feature

Use this task to request a new feature for consideration in the project. The request will be evaluated against existing design principles, the current design backlog, and the dependency stack to determine its viability and placement.

**Use this task when:**
- You have an idea for a new gameplay mechanic, system, or content feature.
- You want to formally propose a feature that is not currently in the `DESIGN_BACKLOG.md`.
- You believe a feature could solve a known problem or create a new opportunity for players.

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles. This is essential for ensuring the feature request aligns with:
-   Elegance through emergence, simplicity over sophistication.
-   Iteration over planning, graybox before polish.
-   Dependency management and certainty scoring.
-   Gameplay-first control and procedural foundations.

### 2. Define the Feature Request

Provide a clear and concise description of the proposed feature. Include:
-   **Feature Name:** A short, descriptive name (e.g., "Wall Jump," "Dynamic Weather System").
-   **Description:** What is the feature? How would it work from the player's perspective?
-   **Problem/Goal:** What user-facing problem does this solve, or what is the primary goal?
-   **Hypothesis:** What is the expected player experience or outcome? How will this feel?

### 3. Analyze Backlog and Dependencies

1.  **Review `PLANS/DESIGN_BACKLOG.md`:**
    -   Check if a similar feature already exists.
    -   Identify potential relationships or conflicts with existing backlog items.
2.  **Review `PLANS/DEPENDENCY_STACK.md`:**
    -   Consider where the proposed feature might fit in the stack.
    -   Identify its potential dependencies (what must be built first?).
    -   Consider what future features might depend on this one.

### 4. Evaluate Feature Viability

Based on the principles and analysis, answer the following:
-   **Principle Alignment:** Does the feature align with the core principles (simplicity, emergence, iteration, etc.)?
-   **Necessity:** Is this feature necessary now? Does it solve a problem that is currently blocking progress or player experience?
-   **Certainty:** How much uncertainty is associated with this feature's design and implementation? Is it a well-understood problem, or is it highly speculative?

### 5. Add to Design Backlog

If the feature is deemed viable and appropriate, add a new entry to `PLANS/DESIGN_BACKLOG.md`.

-   Use the existing format in the backlog.
-   Include the feature name, description, problem/goal, and hypothesis.
-   Assign an initial **Certainty Score** (1-100%) and justify it.
-   List any identified dependencies.
-   If the feature is foundational, consider adding it to the `DEPENDENCY_STACK.md` in the appropriate layer.

### 6. Propose for Review

After adding the feature to the backlog, announce that the new feature has been requested and is available for review.

### Tone & Constraints

-   Be objective and critical. Not all ideas are good ideas for *this* project at *this* time.
-   Focus on the "why" (the problem or goal) before the "what" (the implementation).
-   A feature request is the start of a conversation, not a final decision.
