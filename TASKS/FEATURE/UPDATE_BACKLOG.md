# Update Design Backlog

### 1. Review Recent Developments
1.  Consider the results of the latest iteration loop, including playtest feedback, serendipitous discoveries, and newly stabilized systems in `PLANS/DEPENDENCY_STACK.md`.
2.  Review any new ideas or insights that have emerged since the last update.

### 2. Add and Update Backlog Items

1.  **Add New Ideas:** For any new feature or system idea, create a new entry in `PLANS/DESIGN_BACKLOG.md`
    -   Place under most relevant category
    -   Write single, concise line for description
    -   Add required metadata (`Prerequisite`, `Certainty`, `Note`)
2.  **Update Existing Items:**
    -   If feature implemented and proven, mark as `✅ COMPLETE` and add `Learning:` section
    -   If feature now blocked or irrelevant, mark as `CUT` with brief explanation
    -   If foundational system certainty changed, update dependent items accordingly
    -   If dependency relationships shifted, reflect in `PLANS/DEPENDENCY_STACK.md`
3.  **Prune Completed Items:** To keep backlog focused on future work:
    -   Remove items marked `✅ COMPLETE` from backlog (learnings are preserved in implementation/iteration archives)
    -   Keep completed items only if they serve as reference for similar future work
    -   Remove entire subsections if all items are complete (e.g., "Reactive Animation (Architecture Proven ✅)")
    -   Rationale: Backlog is for *future* work; completed work lives in dependency stack, archived plans, and code

### 3. Propose for Review

1.  After updating `PLANS/DESIGN_BACKLOG.md`, inform the user of the changes
2.  If a previously blocked feature is now unblocked, propose it using `TASKS/FEATURE/SELECT_FEATURE.md`

### Tone & Constraints

-   Concise and mechanical
-   Focus on dependencies, certainty, and testability
-   Assign low, honest certainty scores; avoid optimism
-   Document learnings from completed features

---

### Example: Capturing a New Idea

```markdown
- **Tongue Grapple:** Use frog tongue as a long-range grapple.
  - *Prerequisite:* Core character controller stable (95%+)
  - *Certainty:* Low (~15%) - Physics could be complex, and it might not be fun.
  - *Note:* Inspired by classic platformers. Could be used for traversal and combat. A classic "frog" trope that might be cool.
```

---

### Example: Completing an Item

```markdown
- **Secondary Motion System:** Procedural physics for cloth and appendages. ✅ COMPLETE
  - *Prerequisite:* Skeleton & Animation Systems (100%)
  - *Certainty:* 100%
  - *Note:* Intended to add life and readability to character movement.
  - *Learning:* Spring-damper physics on a per-bone basis provides excellent results with minimal performance cost. Key is to expose damping/stiffness parameters for real-time tuning.
```

### Example: Deferred Item (for Retrospectives)

When marking items as deferred or cut, include categorization to help RETROSPECTIVE task identify patterns:

```markdown
- **Speed-Based Animation Scaling:** Scale tilt magnitude based on character velocity. ❌ CUT
  - *Prerequisite:* Locomotion system (100%)
  - *Certainty:* N/A
  - *Deferral Phase:* Planning Phase
  - *Deferral Category:* Premature Feature
  - *Reason:* No evidence of need; current constant magnitude feels good. Test-first principle—wait for actual problem before building solution.
  - *Reconsider:* After playtesting identifies specific moments where constant tilt is inadequate
```
