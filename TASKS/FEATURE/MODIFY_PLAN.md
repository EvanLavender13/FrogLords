# Modify Implementation Plan

### 1. Identify Target Feature

Extract the feature name from the current git branch name (format: `feature/<feature_name>`).

### 2. Gather Context

1.  **Updated Feature Description:** Read the latest version of the feature description at `PLANS/feature_<feature_name>.md`.
2.  **Existing Implementation Plan:** Read the implementation plan that needs to be updated at `PLANS/implementation_<feature_name>.md`.
3.  **Principles:** Read and synthesize core principles from `AGENTS.md` to ensure the updated plan remains aligned.

### 3. Analyze Differences

1.  **Compare Scopes:** Cross-reference the "Graybox Scope" section of the updated feature description with the task checklist in the existing implementation plan.
2.  **Identify Deltas:** Create a list of what needs to be added, removed, or changed in the implementation plan to match the new feature scope.

### 4. Update Implementation Plan

1.  **Modify Tasks:** Edit `PLANS/implementation_<feature_name>.md` to reflect the identified changes.
    -   Add new checklist items for new requirements.
    -   Remove checklist items for deprecated requirements.
    -   Modify existing checklist items to align with the updated scope.
2.  **Maintain Quality:** Ensure all tasks remain specific, atomic, and logically ordered, adhering to the conventions in `AGENTS.md`.
3.  **Log the Change:** Add a changelog entry at the bottom of `PLANS/implementation_<feature_name>.md` to record that the plan was modified to align with an updated feature description, including the date.

### 5. Propose for Review

Propose the newly modified implementation plan to the user for final review before proceeding with implementation.

### Tone & Constraints

-   Concise and direct; no preamble or postamble.
-   The implementation plan must remain a direct, actionable translation of the feature description's graybox scope.
-   All changes must adhere to the principles in `AGENTS.md`.
