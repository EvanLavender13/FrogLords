# Update Dependency Stack

### 1. Review Current State
1.  Open `PLANS/dependency_stack.md`.
2.  Identify the current layer of work, marked with `← YOU ARE HERE`.
3.  Review the certainty scores of all systems, especially those foundational to the current work.

### 2. Update System Certainty
1.  **After an Iteration:** If a system has been iterated on and tested, update its certainty percentage to reflect the new knowledge. If it is now considered stable and proven (95%+), mark it with a `✅`.
2.  **On System Failure:** If playtesting or development has revealed a fundamental flaw in a system, decrease its certainty score to reflect the new risk. This signals that all dependent systems are now built on a shakier foundation.

### 3. Modify the Stack Structure
1.  **Add a New System:** When beginning work on a new feature from the `design_backlog.md`, add it to the top of the stack.
    -   Place it above the systems it depends on.
    -   Assign it a low, realistic initial certainty score (e.g., `~40%`).
    -   Add a corresponding entry to the "Layer Descriptions" section.
2.  **Refactor Dependencies:** If a system's relationship to other systems has changed, update the diagram to reflect the new dependency graph.

### 4. Update Development Focus
1.  Move the `← YOU ARE HERE` marker to the new layer that is actively being worked on.
2.  Update the `Development Strategy` section to reflect the current focus and the planned iteration loop length.

Tone/constraints:
- Keep language mechanical and concise. Reflect only tested knowledge; avoid speculation.
- Prefer systemic additions over content. Avoid introducing content restrictions.

---

### Example: Solidifying a System

1.  **Update Diagram:** Change the system's entry from `Character Controller (~95%)` to `Character Controller (100%) ✅`.
2.  **Update Description:** In the text below the diagram, update the system's description to reflect its new, stable status and any key learnings from its iteration.
3.  **Update Marker:** Move the `← YOU ARE HERE` marker up to the next system to be implemented (e.g., `Secondary Motion`).