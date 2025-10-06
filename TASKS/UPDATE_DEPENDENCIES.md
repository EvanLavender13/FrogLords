# Update Dependency Stack

### 1. Review Current State

1.  Open `PLANS/dependency_stack.md`
2.  Identify current layer of work, marked with `← YOU ARE HERE`
3.  Review certainty scores of all systems, especially foundational ones

### 2. Update System Certainty

1.  **After Iteration:** If system iterated and tested, update certainty percentage. If stable and proven (95%+), mark with `✅`
2.  **On System Failure:** If playtesting revealed fundamental flaw, decrease certainty score to reflect new risk

### 3. Modify Stack Structure

1.  **Add New System:** When beginning work on new feature from `design_backlog.md`, add to top of stack
    -   Place above systems it depends on
    -   Assign low, realistic initial certainty score (e.g., `~40%`)
    -   Add corresponding entry to "Layer Descriptions" section
2.  **Refactor Dependencies:** If system relationships changed, update diagram to reflect new dependency graph

### 4. Update Development Focus

1.  Move `← YOU ARE HERE` marker to new layer actively being worked on
2.  Update `Development Strategy` section to reflect current focus and planned iteration loop length

### Tone & Constraints

-   Mechanical and concise language
-   Reflect only tested knowledge; avoid speculation
-   Prefer systemic additions over content
-   Avoid introducing content restrictions

---

### Example: Solidifying a System

1.  **Update Diagram:** Change the system's entry from `Character Controller (~95%)` to `Character Controller (100%) ✅`.
2.  **Update Description:** In the text below the diagram, update the system's description to reflect its new, stable status and any key learnings from its iteration.
3.  **Update Marker:** Move the `← YOU ARE HERE` marker up to the next system to be implemented (e.g., `Secondary Motion`).