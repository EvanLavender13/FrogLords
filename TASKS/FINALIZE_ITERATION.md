# Finalize Iteration

### 1. Identify Target Feature

Extract the feature name from the current git branch name (format: `iteration/<feature_name>`).

### 2. Verify Approval

1.  Confirm `PLANS/implementation_<feature_name>.md` has an `## Approval` section with reviewer signature and timestamp
2.  Confirm `PLANS/iteration_<feature_name>.md` has an `## Approval` section with reviewer signature and timestamp
3.  If either approval is missing, stop and request completion of `TASKS/REVIEW_IMPLEMENTATION.md` first

### 3. Update Dependency Stack

Execute `TASKS/UPDATE_DEPENDENCIES.md` to:
-   Update feature's certainty score based on implementation learnings
-   Mark as `✅` if stable and proven (95%+)
-   Move `← YOU ARE HERE` marker to next layer
-   Update `Development Strategy` section

### 4. Update Design Backlog

Execute `TASKS/UPDATE_BACKLOG.md` to:
-   Mark feature as `✅ COMPLETE`
-   Add `Learning:` section documenting key knowledge gained
-   Update certainty scores for newly unblocked dependent features

### 5. Archive Planning Documents

1.  Add a `## Completion` section to end of `PLANS/iteration_<feature_name>.md`:
    -   Date completed
    -   Final certainty score
    -   Link to code review document
    -   Brief outcome summary (1-2 sentences)
2.  Add a `## Completion` section to end of `PLANS/implementation_<feature_name>.md`:
    -   Date completed
    -   All checklist items confirmed complete
    -   Link to code review document

### 6. Propose Next Steps

Recommend running `TASKS/NEXT_FEATURE.md` to identify next work after user handles git operations.

### Tone & Constraints

-   Concise and systematic
-   Verify all prerequisite approvals before proceeding
-   Document learnings while knowledge is fresh
-   Ensure dependency stack accurately reflects new certainty
-   Quality gates must pass before merge
