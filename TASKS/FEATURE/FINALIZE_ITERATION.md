# Finalize Iteration

### 1. Review Development Principles

Read `AGENTS.md` to synthesize principles for finalizing iterations. Essential for:
-   Accurate certainty scoring based on testing outcomes
-   Capturing learnings that inform future design decisions
-   Understanding dependency impacts and cascade effects
-   Strategic planning horizon adjustments

### 2. Identify Target Feature

Extract the feature name from the current git branch name (format: `iteration/<feature_name>`).

### 3. Verify Approval

1.  Confirm `PLANS/implementation_<feature_name>.md` has an `## Approval` section with reviewer signature and timestamp
2.  Confirm `PLANS/iteration_<feature_name>.md` has an `## Approval` section with reviewer signature and timestamp
3.  If either approval is missing, stop and request completion of `TASKS/REVIEW_IMPLEMENTATION.md` first

### 4. Update Dependency Stack

Read and execute `TASKS/UPDATE_DEPENDENCIES.md` to:
-   Update feature's certainty score based on implementation learnings
-   Mark as `✅` if stable and proven (95%+)
-   Move `← YOU ARE HERE` marker to next layer
-   Update `Development Strategy` section

### 5. Update Design Backlog

Read and execute `TASKS/UPDATE_BACKLOG.md` to:
-   Mark feature as `✅ COMPLETE`
-   Add `Learning:` section documenting key knowledge gained
-   Update certainty scores for newly unblocked dependent features

### 6. Archive Planning Documents

1.  Add a `## Completion` section to end of `PLANS/iteration_<feature_name>.md`:
    -   Date completed
    -   Final certainty score
    -   Link to code review document
    -   Brief outcome summary (1-2 sentences)
2.  Add a `## Completion` section to end of `PLANS/implementation_<feature_name>.md`:
    -   Date completed
    -   All checklist items confirmed complete
    -   Link to code review document

### 6.5. Iteration Reflection (for Retrospectives)

Add a `## Reflection` section to end of `PLANS/iteration_<feature_name>.md` answering:

1.  **What went well?** (1-3 bullet points)
    -   What practices, decisions, or planning approaches helped this iteration succeed?
    -   Were there any particularly effective moments in planning, implementation, or review?
2.  **What caused friction?** (1-3 bullet points)
    -   What slowed progress or required rework?
    -   Were there planning gaps, unclear requirements, or unexpected blockers?
    -   Did any assumptions prove incorrect?
3.  **What would you do differently?** (1-3 bullet points)
    -   Knowing what you know now, what would you change about the approach?
    -   Are there signals you missed or steps you'd add to the process?

**Purpose:** Capture fresh insights while context is still loaded. These reflections feed into RETROSPECTIVE task for process improvement.

**NOTE:** Be specific. "Planning was good" is less useful than "Clarifying skeletal data structure before planning prevented implementation surprises."

### 7. Propose Next Steps

Recommend running `TASKS/NEXT_FEATURE.md` to identify next work after user handles git operations.

### Tone & Constraints

-   Concise and systematic
-   Verify all prerequisite approvals before proceeding
-   Document learnings while knowledge is fresh
-   Ensure dependency stack accurately reflects new certainty
-   Quality gates must pass before merge
