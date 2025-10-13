# Select and Initialize Next Feature

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles. Essential for selecting features that align with:
-   Core loop alignment (movement, physics, feel)
-   Systemic design and emergence over content
-   Smallest scope and tight iteration loops
-   Avoiding content restrictions
-   Gameplay-first control (input→acceleration, interruptibility)
-   Procedural foundation and animation principles

### 2. Identify the Frontier
1.  Open `PLANS/DEPENDENCY_STACK.md`.
2.  Locate the `← YOU ARE HERE` marker in the dependency tree. This marker indicates the frontier—the boundary between stable foundation (below/behind) and uncertain future work (above/ahead).
3.  Review the tree structure to understand:
    - Which systems are in "Completed Features" (100% ✅)
    - Which systems are in "Core Gameplay Layer" (90-95% certain)
    - Which systems are in "Foundation Layer" (90-100% certain)
4.  The stable frontier is everything at or below the marker—these systems are proven and safe to build upon.

### 3. Find Candidate Features
1.  Open `PLANS/DESIGN_BACKLOG.md`.
2.  Scan the backlog for features whose `Prerequisite` is met by the stable frontier you just identified.
3.  Compile a short list of these now-unblocked candidate features.

### 4. Select the "Purest" Feature
1.  From your list of candidates, select the **single best feature** to implement next based on the following criteria, in order of importance:
    *   **Core Loop Alignment:** Which feature most directly enhances the current core gameplay loop of movement, physics, and feel?
    *   **Smallest Scope:** Which feature is the smallest, most self-contained "stupid simple core" that can be grayboxed and tested in a short iteration loop (1-3 complexity points)?
    *   **Most Systemic:** Which feature is a reusable system or mechanic, rather than a one-off piece of content? Does it have the potential for emergent interactions?
    *   **Highest Certainty / Lowest Risk:** Which feature has the highest initial `Certainty` score in the backlog and depends on the fewest *other* uncertain systems?

### 4.5. Problem Evidence Check

**Before writing the feature description**, verify the feature addresses a documented problem or gap:

1.  **Review backlog notes:** Check for warning flags like "may not be necessary," "low priority," or uncertainty about value
2.  **Check for problem evidence:** Has this problem been observed during playtesting? When was it last encountered?
3.  **Yellow flags requiring extra scrutiny:**
    -   Backlog contains uncertainty notes about necessity
    -   Feature is purely "nice to have" without specific gameplay moment motivating it
    -   No documented observation of the problem this feature solves

4.  **If problem evidence is unclear:**
    -   Document the specific gameplay moment or observation that motivates this feature
    -   Example: "During 2025-10-06 playtest, character tilt magnitude felt identical at walk vs sprint speeds, making speed changes harder to read"
    -   If no such moment exists, consider deferring feature or running focused playtest session first

**Purpose:** Prevents premature features (like speed_animation_scaling deferral - backlog warned "may not be necessary" but feature was selected anyway).

### 4.6. Certainty Boost for Debug Tools

**Apply certainty boost for features on stable infrastructure:**

1.  **Check feature category:** Is this a debug/iteration tool (GUI panels, visualizations, parameter exposure)?
2.  **Check infrastructure stability:** Does it extend systems with 90%+ certainty in DEPENDENCY_STACK.md?
3.  **If both true:** Apply +20-30% certainty boost to base estimate
    -   Example: Debug visual extending proven debug_draw system (90% certain) → base 60% + 25% boost = 85%
    -   Example: Parameter exposure on stable controller (95% certain) → base 60% + 25% boost = 85%

4.  **Rationale:**
    -   Debug tools have zero gameplay risk (isolated to iteration workflow)
    -   Stable infrastructure means clear integration points
    -   Pattern validated in Retro 2: debug visuals and walk transition both underestimated by 40 points

**Purpose:** Prevents systematic underestimation of debug tool features (Retro 2 pattern: 60% predicted, completed smoothly → should have been 85-90%).

### 5. Create Feature Branch

Run the script to create and switch to a new feature branch:

```powershell
./scripts/create-branch.ps1 feature <feature_name>
```

This establishes the workspace before documenting the feature.

### 6. Write Feature Description

Create a comprehensive feature description following this structure:

**[Feature Name]**

-   **Core Mechanic:** What is the essential mechanic? (1-2 sentences)
-   **Pattern Reuse:** What proven patterns/systems does this extend or build upon?
-   **Graybox Scope:**
    -   Minimum testable implementation
    -   Key parameters to expose for tuning
    -   Estimated line count
-   **Minimal Implementation:** Where does it hook into existing code? (e.g., update order, files affected, implementation approach options)
-   **Problem Evidence:** What documented observation or gameplay moment motivates this feature? When was the problem last encountered?
-   **Unlocks:**
    -   Immediate: What does this complete or validate?
    -   Future: What features does this enable? What backlog items does it unblock?
-   **Test Loop:**
    -   How to verify success? What to test first?
    -   Expected iteration time and self-test loop characteristics
-   **Certainty:** Percentage + justification (reference dependencies, novelty, risk)

### 7. Save Feature Description

Save the feature description to `PLANS/<feature_name>_FEATURE.md` (use snake_case, e.g., `primary_skeletal_animation_FEATURE.md`).

The feature description is now complete and serves as the planning artifact. Proceed directly to PLAN_IMPLEMENTATION for granular decomposition.

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
