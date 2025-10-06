# Create Iteration Plan

### 1. Create Branch

Run the script to create and switch to a new feature branch.

```powershell
./scripts/create_iteration_branch.ps1 <feature_name>
```

### 2. Understand Feature

Receive a high-level feature description from the user. This may be a single sentence or a paragraph.

### 3. Generate Plan

Generate a detailed iteration plan based on the template below. The plan must distill the user's request and align it with the project's core design principles (elegance, emergence, iteration, dependency management).

- **Analyze Dependencies:** Before writing, consult `PLANS/dependency_stack.md` and `PLANS/design_backlog.md` to determine the feature's correct placement and certainty. Consider cascading uncertainty and assign a provisional certainty tag.
- **Be Specific:** Provide concrete, actionable steps for the graybox implementation and testing protocol.
- **Stay Grounded:** Ensure the plan is realistic and directly addresses the feature request.
- **Gameplay‑First:** Apply the "do no harm" rule—input maps to acceleration; animation layers never steal control; transitions remain interruptible; rotate model to velocity; prefer spring‑damper and cubic interpolation.
- **Planning Horizon:** Choose a short horizon when uncertainty is high or test cost is low; expand only as certainty rises.

### 4. Save and Propose

Save the generated plan to a new file in the `PLANS/` directory, then propose it for review.

- **Filename:** `iteration_[feature_name].md` (e.g., `iteration_tongue_grapple.md`).
- Keep tone concise and direct. Favor clarity over cleverness.

---

### Iteration Plan Template

```markdown
# Iteration Plan: [Feature Name]

### 1. Feature Overview

*   **Description:** (Brief, clear description of the feature.)
*   **Core Principle Alignment:** (How does this feature align with simplicity, elegance, iteration, proceduralism, and gameplay-first control?)

### 2. Design Rationale

*   **Problem/Goal:** (What user-facing problem is this solving or what is the primary goal?)
*   **Hypothesis:** (What is the expected player experience or outcome? How will this feel?)

### 3. Dependencies & Uncertainty

*   **Placement in Stack:** (Where does this feature fit in `dependency_stack.md`? What are its direct dependencies? What will depend on it?)
*   **Certainty Score:** (Estimated certainty: 1-100%. Justify the score based on novelty and dependencies; call out cascading uncertainty.)

### 4. Implementation & Testing Loop

*   **Graybox Scope (The "Stupid Simple Core"):**
    *   (What is the absolute minimum version that can be built to test the core mechanic?)
    *   (What are the key parameters to expose for real-time tuning?)
    *   (For animation/movement: physics-first controller; rotate-to-velocity; acceleration-tilt; spring‑damper transitions; always interruptible.)
*   **Testing Protocol:**
    *   (Self-test first; then over-the-shoulder.)
    *   (Stay silent—don’t coach. Ask, "Tell me the story of what just happened.")
    *   (Target sample size per loop: 6–12 across a mix of Kleenex and experienced testers.)
    *   (Avoid leading questions; record metrics if helpful.)
*   **Success Metrics (Definition of "Proven"):**
    *   (How do we know when the graybox is successful? e.g., "Survives 6+ playtests without major negative feedback," "Players intuitively understand the mechanic," "No content restrictions introduced.")
*   **Iteration Cadence & Horizon:**
    *   (Hours/days to next loop; criteria for shortening/lengthening.)

### 5. System Integration

*   **Mechanic Interactions:** (How will this feature interact with other existing mechanics? Where is the potential for emergence?)
*   **Interface & Feedback:** (How will the player understand and interact with this feature? What metaphors will be used? Apply visual hierarchy and redundancy. What is the plan for visual/audio feedback, even in graybox?)

### 6. Risks & Future Work

*   **Potential Risks:** (What are the biggest risks? e.g., "May not feel good without significant tuning," "Could introduce physics instability.")
*   **Content Restrictions:** (Could this feature impose restrictions on future content? e.g., "Requires all levels to have ceilings of a certain height.")
*   **Decision Impacts (Checklist):** (Implementation cost; immaturity burden; critical failure risk; process burden; political/cultural effects; decision cost.)
*   **Backlog Candidates:** (What related ideas have come up that should be captured in `design_backlog.md`?)
```
