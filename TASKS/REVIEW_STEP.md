# Review Implementation Step

### 1. Identify Target Feature

Extract the feature name from the current git branch name (format: `iteration/<feature_name>`).

### 2. Gather Context

1.  **Read Principles:** Read `AGENTS.md` for development principles and coding standards
2.  **Read Planning Documents:**
    *   `PLANS/feature_<feature_name>.md` for feature intent and goals
    *   `PLANS/iteration_<feature_name>.md` for high-level implementation approach
    *   `PLANS/implementation_<feature_name>.md` for ordered checklist of tasks
3.  **Identify Last Completed Step:** Find the most recently marked `[x]` major step (e.g., "####" heading) in the implementation plan
4.  **Read Modified Code:** Read all source files (`.h` and `.cpp`) listed as changed in the last completed step

### 3. Perform Step Review

Cross-reference the completed step against planning documents and project principles.

-   **Plan Alignment:** Does the implemented code match the stated goal of the completed step?
-   **System Ownership Check:** If step adds or modifies a system (e.g., locomotion, skeleton):
    -   Is this the only instance of the system, or are there duplicates elsewhere?
    -   If a system was moved, were all references updated (debug_draw, GUI, rendering)?
    -   Do a quick `grep_search` for the system name to verify no orphaned references
-   **Code Standards:** Does code adhere to naming conventions (snake_case), formatting (4-space indent, braces on same line), and file organization (Foundation → Character → Rendering → App)?
-   **Gameplay-First Principles:** Does implementation respect core tenets?
    -   Input maps to acceleration; animation never steals control
    -   Interruptible transitions; decomposed into small steps
    -   Physics-first; reactive layers don't override core logic
-   **Simplicity & Clarity:** Is code clear over clever? Simple over sophisticated?
-   **Dependency Flow:** Does code respect bottom-up layering? No sideways includes at higher layers?
-   **Procedural Foundation:** Are spring-damper or cubic interpolation used appropriately? Avoid linear interpolation for organic motion?
-   **Documentation:** Are non-obvious public APIs commented? No noise on obvious code?

### 4. Report Findings

Provide concise inline analysis structured as:

1.  **Step Summary:** One sentence describing what was completed
2.  **Alignment Check:** Confirm implementation matches step goal (or note deviations)
3.  **Issues Found:** List violations or concerns with specific file/line references
4.  **Recommendations:** Actionable fixes (or confirm step is clean)

If step is clean, state: "Step verified. No issues found."

### 5. Await Guidance

Do not proceed to next step or make changes. Await user decision on whether to:
-   Fix identified issues before continuing
-   Accept issues as acceptable tradeoffs
-   Proceed to next implementation step

### Tone & Constraints

-   Concise and direct; no preamble or postamble
-   Detail scales with risk and complexity
-   Cite specific principles from `AGENTS.md` when noting violations
-   Focus on actionable feedback over opinions
-   Flag content restrictions (e.g., hardcoded jump heights that break worlds)
-   Distinguish critical violations from polish opportunities
