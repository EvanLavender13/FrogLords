# Add Scope to Iteration Plan

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles. This is essential for ensuring the new scope aligns with the project's foundation:
-   **Iteration over planning:** Prioritize finishing the original scope over expanding it.
-   **Dependency management:** Analyze the impact on the `DEPENDENCY_STACK.md` and cascading uncertainty.
-   **Certainty scoring:** Be realistic about how the change affects the iteration's certainty.
-   **Graybox before polish:** The new scope should also be a "stupid simple core."
-   **Gameplay-first control:** The change must not compromise interruptibility or the input→acceleration mapping.

### 2. Verify Request Scope

Extract the feature name from the current git branch name (format: `iteration/<feature_name>`).

1.  **Review Iteration Plan:** Read the current iteration plan at `PLANS/iteration_<feature_name>.md`.
3.  **Check Backlog & Dependencies:** Review `PLANS/DESIGN_BACKLOG.md` and `PLANS/DEPENDENCY_STACK.md`.
4.  **Analyze Impact:** Assess if the requested scope change is a minor addition or a significant pivot. Ensure it doesn't invalidate the core goal of the iteration or introduce major unforeseen dependencies. Defer large changes to a new iteration.

### 3. Modify Iteration Plan

If the scope addition is acceptable:

1.  **Update Graybox Scope:** Modify the "Graybox Scope" section of `PLANS/iteration_<feature_name>.md` to include the new requirements.
2.  **Log the Change:** Add a note to the plan's "Changelog" or "Notes" section detailing the added scope, the rationale, and the date.

### 4. Propose Next Steps

The iteration plan has changed. The implementation plan is now out of date. Propose to the user that a `PLAN_IMPLEMENTATION` task must be performed again to synchronize the implementation plan with the new iteration scope.

### 5. Quality Gates

-   The scope change must not compromise the ability to meet the iteration's original quality gates.
-   The change must be small enough to be implemented and tested within the remaining iteration time.
-   The change must not introduce content restrictions.

### Tone & Constraints

-   Be conservative. Prioritize stability and finishing the original scope.
-   Reject scope changes that fundamentally alter the iteration's goal or significantly increase uncertainty.
-   All changes must adhere to the principles in `AGENTS.md`.
-   Concise and direct; no preamble or postamble.