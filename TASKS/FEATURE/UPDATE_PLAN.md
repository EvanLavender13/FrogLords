# Update Feature Plan or Scope

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles. This is essential for ensuring changes align with the project's foundation:
-   **Iteration over planning:** Prioritize finishing the original scope over expanding it.
-   **Dependency management:** Analyze the impact on the `DEPENDENCY_STACK.md` and cascading uncertainty.
-   **Certainty scoring:** Be realistic about how the change affects the feature's certainty.
-   **Graybox before polish:** New scope should also be a "stupid simple core."
-   **Gameplay-first control:** Changes must not compromise interruptibility or the input→acceleration mapping.

### 2. Identify Target Feature

Extract the feature name from the current git branch name (format: `feature/<feature_name>`).

### 3. Gather Context

1.  **Read Feature Description:** Read `PLANS/<feature_name>_FEATURE.md` for current scope and goals.
2.  **Read Implementation Plan:** Read `PLANS/<feature_name>_PLAN.md` for current task checklist.
3.  **Check Backlog & Dependencies:** Review `PLANS/DESIGN_BACKLOG.md` and `PLANS/DEPENDENCY_STACK.md`.

### 4. Analyze Impact

Determine the nature of the change:
-   **Scope Addition:** New requirements discovered during implementation
-   **Plan Modification:** Existing plan needs restructuring or clarification
-   **Both:** Scope changed and plan needs updating

Assess severity:
-   Is this a minor addition or a significant pivot?
-   Does it invalidate the core goal of the feature?
-   Does it introduce major unforeseen dependencies?
-   **Defer large changes to a new feature.**

### 5. Update Feature Description (if scope changed)

If the change involves new or modified scope:

1.  **Update Graybox Scope:** Modify the "Graybox Scope" section of `PLANS/<feature_name>_FEATURE.md` to include the new requirements.
2.  **Log the Change:** Add a changelog entry at the top or in a dedicated "Changelog" section detailing the added scope, the rationale, and the date.

### 6. Update Implementation Plan

1.  **Identify Deltas:** Cross-reference the feature description with the implementation plan to determine what needs to be added, removed, or changed.
2.  **Modify Tasks:** Edit `PLANS/<feature_name>_PLAN.md` to reflect the identified changes.
    -   Add new checklist items for new requirements.
    -   Remove checklist items for deprecated requirements.
    -   Modify existing checklist items to align with the updated scope.
3.  **Maintain Quality:** Ensure all tasks remain specific, atomic, and logically ordered, adhering to the conventions in `AGENTS.md`.
4.  **Log the Change:** Add a changelog entry at the bottom of `PLANS/<feature_name>_PLAN.md` to record the modification, including the rationale and date.

### 7. Propose for Review

Propose the updated feature description and/or implementation plan to the user for final review before proceeding with implementation.

### 8. Quality Gates

-   The scope change must not compromise the ability to meet the feature's original quality gates.
-   The change must be small enough to be implemented and tested within the remaining feature time.
-   The change must not introduce content restrictions.

### Tone & Constraints

-   Be conservative. Prioritize stability and finishing the original scope.
-   Reject scope changes that fundamentally alter the feature's goal or significantly increase uncertainty.
-   All changes must adhere to the principles in `AGENTS.md`.
-   Concise and direct; no preamble or postamble.
