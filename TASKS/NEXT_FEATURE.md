# Identify Next Feature

### 1. Identify the Frontier
1.  Open `PLANS/dependency_stack.md`.
2.  Locate the `← YOU ARE HERE` marker. The layers at or just below this marker represent the stable foundation—our current frontier of certainty.

### 2. Find Candidate Features
1.  Open `PLANS/design_backlog.md`.
2.  Scan the backlog for features whose `Prerequisite` is met by the stable frontier you just identified.
3.  Compile a short list of these now-unblocked candidate features.

### 3. Select the "Purest" Feature
1.  From your list of candidates, select the **single best feature** to implement next based on the following criteria, in order of importance:
    *   **Core Loop Alignment:** Which feature most directly enhances the current core gameplay loop of movement, physics, and feel?
    *   **Smallest Scope:** Which feature is the smallest, most self-contained "stupid simple core" that can be grayboxed and tested in a short iteration loop (hours or a few days)?
    *   **Most Systemic:** Which feature is a reusable system or mechanic, rather than a one-off piece of content? Does it have the potential for emergent interactions?
    *   **Highest Certainty / Lowest Risk:** Which feature has the highest initial `Certainty` score in the backlog and depends on the fewest *other* uncertain systems?

### 4. Propose the Feature

1.  State the name of the selected feature
2.  Provide a brief (1–3 sentence) justification, referencing the criteria above

### Tone & Constraints

-   Concise, imperative, gameplay‑first
-   Keep it concrete and mechanical (avoid fiction)
-   Mention dependencies and expected test loop length
-   Prefer systemic, reusable mechanics
-   Avoid content restrictions

---

### Example Proposal

> Propose: Secondary Motion. Small, systemic wobble on limbs builds directly on the stable skeleton system and improves movement feel. Minimal scope, fast graybox loop (hours), and high emergence potential with low risk.
