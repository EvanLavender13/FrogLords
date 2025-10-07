# Identify Next Feature

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles. Essential for selecting features that align with:
-   Core loop alignment (movement, physics, feel)
-   Systemic design and emergence over content
-   Smallest scope and tight iteration loops
-   Avoiding content restrictions

### 2. Identify the Frontier
1.  Open `PLANS/DEPENDENCY_STACK.md`.
2.  Locate the `← YOU ARE HERE` marker. The layers at or just below this marker represent the stable foundation—our current frontier of certainty.

### 3. Find Candidate Features
1.  Open `PLANS/DESIGN_BACKLOG.md`.
2.  Scan the backlog for features whose `Prerequisite` is met by the stable frontier you just identified.
3.  Compile a short list of these now-unblocked candidate features.

### 4. Select the "Purest" Feature
1.  From your list of candidates, select the **single best feature** to implement next based on the following criteria, in order of importance:
    *   **Core Loop Alignment:** Which feature most directly enhances the current core gameplay loop of movement, physics, and feel?
    *   **Smallest Scope:** Which feature is the smallest, most self-contained "stupid simple core" that can be grayboxed and tested in a short iteration loop (hours or a few days)?
    *   **Most Systemic:** Which feature is a reusable system or mechanic, rather than a one-off piece of content? Does it have the potential for emergent interactions?
    *   **Highest Certainty / Lowest Risk:** Which feature has the highest initial `Certainty` score in the backlog and depends on the fewest *other* uncertain systems?

### 5. Write High-Level Feature Description

Create a concise, actionable feature description following this structure:

**[Feature Name]**

-   **Core Mechanic:** What is the essential mechanic? (1-2 sentences)
-   **Pattern Reuse:** What proven patterns/systems does this extend or build upon?
-   **Graybox Scope:** 
    -   Minimum testable implementation
    -   Key parameters to expose for tuning
    -   Estimated line count
-   **Minimal Implementation:** Where does it hook into existing code? (e.g., update order, files affected)
-   **Unlocks:** What future features does this enable? What backlog items does it unblock?
-   **Test Loop:** How to verify success? What to test first? Expected iteration time?
-   **Certainty:** Percentage + justification (reference dependencies, novelty, risk)

### 6. Save Feature Description

Save the feature description to `PLANS/feature_<feature_name>.md` (use snake_case, e.g., `feature_primary_skeletal_animation.md`).

### Tone & Constraints

-   Concise, imperative, gameplay‑first
-   Keep it concrete and mechanical (avoid fiction)
-   Mention dependencies and expected test loop length
-   Prefer systemic, reusable mechanics
-   Avoid content restrictions
-   Include line count estimates and specific file/function integration points

---

### Example Feature Description

**Primary Skeletal Animation (Locomotion-Driven Limb Cycles)**

**Core Mechanic:** Distance-phased arm swing and leg stride synchronized to the surveyor-wheel locomotion system. Limbs animate proportionally to distance traveled, not time, ensuring visual coherence with body movement.

**Pattern Reuse:** Extends the proven surveyor-wheel pattern from body bounce to skeletal joints. Phase-offset arm swings (left/right alternate), leg strides synchronized to foot placement, all driven by `locomotion.distance_traveled`. Walk/run pose blending follows existing `walk_factor` logic.

**Graybox Scope:** 
- Simple sinusoidal joint rotations (shoulders, elbows, hips, knees) driven by locomotion phase
- Two keyframe poses: walk stride (small angles) and run stride (large angles)
- Cubic interpolation between poses based on `walk_factor` (already computed)
- Expose tuning parameters: arm swing magnitude, leg stride length, phase offsets

**Minimal Implementation:** ~50-80 lines. Update `character::animation::update()` after orientation, before reactive systems. Inject rotations into `skeleton` joint local transforms. No new state machine—pure procedural.

**Unlocks:** Secondary motion (wobble/follow-through), IK foot placement, realistic limb reactions. This is the missing foundation that blocks multiple backlog items.

**Test Loop:** Visual coherence test—do limbs match body speed? Does stopping feel instant? Does walk-to-run transition smoothly? Self-test in minutes, iterate tuning parameters in real-time via existing animation UI pattern.

**Certainty:** ~80%—direct extension of proven locomotion system. Risk is tuning time, not architecture.
