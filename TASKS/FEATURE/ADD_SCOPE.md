# Add Scope to Feature Description

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles. This is essential for ensuring the new scope aligns with the project's foundation:
-   **Iteration over planning:** Prioritize finishing the original scope over expanding it.
-   **Dependency management:** Analyze the impact on the `DEPENDENCY_STACK.md` and cascading uncertainty.
-   **Certainty scoring:** Be realistic about how the change affects the feature's certainty.
-   **Graybox before polish:** The new scope should also be a "stupid simple core."
-   **Gameplay-first control:** The change must not compromise interruptibility or the input→acceleration mapping.

### 2. Verify Request Scope

Extract the feature name from the current git branch name (format: `feature/<feature_name>`).

1.  **Review Feature Description:** Read the current feature description at `PLANS/feature_<feature_name>.md`.
2.  **Check Backlog & Dependencies:** Review `PLANS/DESIGN_BACKLOG.md` and `PLANS/DEPENDENCY_STACK.md`.
3.  **Analyze Impact:** Assess if the requested scope change is a minor addition or a significant pivot. Ensure it doesn't invalidate the core goal of the feature or introduce major unforeseen dependencies. Defer large changes to a new feature.

### 3. Modify Feature Description

If the scope addition is acceptable:

1.  **Update Graybox Scope:** Modify the "Graybox Scope" section of `PLANS/feature_<feature_name>.md` to include the new requirements.
2.  **Log the Change:** Add a changelog entry at the top or in a dedicated "Changelog" section detailing the added scope, the rationale, and the date.

### 4. Propose Next Steps

The feature description has changed. The implementation plan is now out of date. Propose to the user that a `MODIFY_PLAN` task must be performed to synchronize the implementation plan with the new feature scope.

### 5. Quality Gates

-   The scope change must not compromise the ability to meet the feature's original quality gates.
-   The change must be small enough to be implemented and tested within the remaining feature time.
-   The change must not introduce content restrictions.

### Tone & Constraints

-   Be conservative. Prioritize stability and finishing the original scope.
-   Reject scope changes that fundamentally alter the feature's goal or significantly increase uncertainty.
-   All changes must adhere to the principles in `AGENTS.md`.
-   Concise and direct; no preamble or postamble.