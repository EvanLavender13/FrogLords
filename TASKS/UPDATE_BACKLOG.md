# Update Design Backlog

### 1. Review Recent Developments
1.  Consider the results of the latest iteration loop, including playtest feedback, serendipitous discoveries, and newly stabilized systems in `PLANS/dependency_stack.md`.
2.  Review any new ideas or insights that have emerged since the last update.

### 2. Add and Update Backlog Items
1.  **Add New Ideas:** For any new feature or system idea, create a new entry in `PLANS/design_backlog.md`.
    -   Place it under the most relevant category.
    -   Write a single, concise line for the description.
  -   Add the required metadata (`Prerequisite`, `Certainty`, `Note`). Assign a low, honest `Certainty` score; avoid optimism.
2.  **Update Existing Items:**
    -   If a feature has been implemented and proven, find its entry in the backlog, mark it as `✅ COMPLETE`, and add a `Learning:` section to document the knowledge gained.
    -   If a feature idea is now blocked or made irrelevant by other changes, mark it as `CUT` and add a brief note explaining why.
  -   If the certainty of a foundational system has changed, review dependent backlog items and update their `Certainty` scores accordingly.
  -   If a dependency relationship has shifted, reflect that in `PLANS/dependency_stack.md`.

### 3. Propose for Review
1.  After updating `PLANS/design_backlog.md`, inform the user of the changes.
2.  If a previously blocked feature is now unblocked, propose it using `TASKS/NEXT_FEATURE.md`.

Tone/constraints:
- Be concise and mechanical. Focus on dependencies, certainty, and testability.

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
