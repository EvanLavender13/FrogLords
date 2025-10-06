# Create Iteration Plan

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles. Essential for ensuring iteration plans align with:
-   Elegance through emergence, simplicity over sophistication
-   Iteration over planning, graybox before polish
-   Dependency management and certainty scoring
-   Gameplay-first control (input→acceleration, interruptibility)
-   Procedural foundation and animation principles

### 2. Create Branch

Run the script to create and switch to a new feature branch.

```powershell
./scripts/create_iteration_branch.ps1 <feature_name>
```

### 3. Understand Feature

Receive a high-level feature description from the user. This may be a single sentence or a paragraph.

### 4. Generate Plan

Generate a detailed iteration plan based on the template below. The plan must distill the user's request and align with core design principles (elegance, emergence, iteration, dependency management).

-   **Analyze Dependencies:** Consult `PLANS/DEPENDENCY_STACK.md` and `PLANS/DESIGN_BACKLOG.md` to determine feature placement and certainty. Consider cascading uncertainty
-   **Be Specific:** Provide concrete, actionable steps for graybox implementation and testing protocol
-   **Stay Grounded:** Ensure the plan is realistic and directly addresses the feature request

### 5. Save and Propose

Save the generated plan to a new file in `PLANS/` directory, then propose for review.

-   **Filename:** `iteration_[feature_name].md` (e.g., `iteration_tongue_grapple.md`)

### Tone & Constraints

-   Concise and direct; favor clarity over cleverness
-   Gameplay‑first: input→acceleration; interruptible transitions; rotate to velocity
-   Prefer spring‑damper and cubic interpolation
-   Short planning horizon when uncertainty is high
-   Avoid content restrictions

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

*   **Placement in Stack:** (Where does this feature fit in `DEPENDENCY_STACK.md`? What are its direct dependencies? What will depend on it?)
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
*   **Backlog Candidates:** (What related ideas have come up that should be captured in `DESIGN_BACKLOG.md`?)
```
