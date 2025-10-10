# Check Plans Against Principles

### 1. Identify Target Feature

Extract the feature name from the current git branch name (format: `iteration/<feature_name>`).

### 2. Gather Context

1.  **Principles:** Read and synthesize core principles from `AGENTS.md`. Use `NOTES/DesigningGames/DG_Manifesto.md` as secondary reference
2.  **Planning Documents:** Read the following files:
    *   `PLANS/feature_<feature_name>.md` (feature description and rationale)
    *   `PLANS/iteration_<feature_name>.md`
    *   `PLANS/implementation_<feature_name>.md` (must exist)
    *   `PLANS/DEPENDENCY_STACK.md`
    *   `PLANS/DESIGN_BACKLOG.md`

### 3. Perform Analysis

Cross-reference the feature description, iteration plan, and implementation plan against the synthesized principles. The goal is to identify any contradictions, misalignments, or unaddressed risks.

-   **Verify System Ownership (Architecture Check):** Does the implementation plan correctly identify where systems currently live and where they need to be? Check for duplicate system instances (e.g., locomotion in both game_world and controller). Does the plan include migration steps if systems need to move? Are all references (rendering, debug, GUI) accounted for?
-   **Check for Contradictions:** Does any part of the feature, iteration, or implementation plan directly violate a directive from `AGENTS.md`? (e.g., planning polish before graybox, letting reactive layers control core logic, creating content restrictions)
-   **Verify Scope Alignment:** Does the iteration plan appropriately scope the feature description? Does it correctly identify the "stupid simple core" from the broader feature concept? Is anything essential missing or unnecessarily included?
-   **Verify Dependency & Certainty:** Does the feature's placement in `DEPENDENCY_STACK.md` seem correct? Are the certainty scores realistic? Are the plans building on an unstable foundation?
-   **Assess Design Rationale:** Does the feature and iteration plan align with core principles like "Elegance Through Emergence" and "Multi-use tools"? Is the "stupid simple core" truly minimal?
-   **Verify Implementation Decomposition:** Does the `implementation_plan.md` correctly break down the graybox scope into atomic, ordered steps? Are all necessary systems and files identified?
-   **Check Consistency:** Do the feature description, iteration plan, and implementation plan align? Does the implementation actually build what the iteration plan describes? Does the iteration plan deliver the core of what the feature description promises?
-   **Review Process:** Does the iteration plan reflect a tight feedback loop? Is the planning horizon appropriate for the stated uncertainty?

### 4. Save and Propose

Generate a new markdown document summarizing the findings.

-   **Filename:** `PLANS/plan_review_<feature_name>.md`
-   **If the review file already exists:** 
    -   Prepend a changelog entry at the top noting the re-review date and reason (e.g., "Plan modified via MODIFY_PLAN")
    -   Update the analysis sections (Summary, Violations, Misalignments, Actionable Items) with current findings
    -   Preserve the overall structure and previous context where relevant
-   **Content:** Use the template below
-   Propose the document to the user for review

### Tone & Constraints

-   Concise and direct; no preamble or postamble
-   Detail scales with risk and complexity
-   Cite specific principles from `AGENTS.md`
-   Focus on actionable recommendations

---

### Review Document Template

```markdown
# Principle Review: [Feature Name]

### 1. Summary
(A brief, one-paragraph summary of the findings. State whether the plans are generally aligned or if significant issues were found.)

### 2. Violations & Contradictions
(A bulleted list of direct violations of principles found in the plans. For each, cite the source file, the violated principle from AGENTS.md, and the specific conflicting text.)

**NOTE FOR RETROSPECTIVES:** When documenting violations, categorize them to help identify patterns:
- **Category:** Premature Polish | Missing Prerequisites | Scope Creep | Reactive Control | System Duplication | Other
- This categorization helps RETROSPECTIVE task identify recurring failure modes

- **Violation:** (e.g., Premature Polish)
  - **Category:** Premature Polish
  - **Source:** `PLANS/implementation_...md`
  - **Principle:** "Graybox before polish"
  - **Details:** The plan includes tasks for creating final art assets before the core mechanic has been playtested and validated.
  - **Suggestion:** Defer all art-related tasks until the graybox is proven successful according to the metrics in the iteration plan.

- **Violation:** (e.g., Missing System Migration)
  - **Category:** System Duplication
  - **Source:** `PLANS/implementation_...md`
  - **Principle:** "Bottom-up dependencies" / "Single source of truth"
  - **Details:** Plan adds `controller.locomotion` but doesn't remove existing `game_world.locomotion` or update all references (debug_draw, character_panel, etc.). This creates duplicate instances and sync issues.
  - **Suggestion:** Add explicit steps to: (1) Remove old instance, (2) Update all call sites, (3) Verify no remaining references to old instance.

### 3. Misalignments & Risks
(A bulleted list of items that, while not direct violations, may be misaligned with the spirit of the principles or introduce unnecessary risk.)

- **Misalignment:** (e.g., Overly Complex Core)
  - **Source:** `PLANS/iteration_...md`
  - **Principle:** "Simplicity over sophistication" / "Stupid Simple Core"
  - **Details:** The defined graybox scope includes three secondary systems that are not essential for testing the core hypothesis. This increases the implementation time for the first loop.
  - **Suggestion:** Revise the graybox scope to focus only on the absolute minimum required to test the core mechanic. Defer the secondary systems to a future iteration.

- **Misalignment:** (e.g., Scope Mismatch)
  - **Source:** `PLANS/feature_...md` vs `PLANS/iteration_...md`
  - **Principle:** "Iteration over planning" / "Short planning horizons"
  - **Details:** The feature description promises X, Y, and Z, but the iteration plan only delivers X. While appropriate scoping down is good, the iteration plan may not deliver enough to validate the core feature hypothesis.
  - **Suggestion:** Either expand the iteration scope to include the minimal Y component needed for validation, or revise the feature description to clarify that this iteration focuses solely on X.

### 4. Actionable Items
(A concise checklist of the specific actions that should be taken to address the findings in the report.)

- [ ] Revise `implementation_...md` to remove tasks related to final art assets.
- [ ] Update `iteration_...md` to simplify the graybox scope.
- [ ] Re-evaluate the certainty score for this feature in `DEPENDENCY_STACK.md` based on the identified risks.
```
