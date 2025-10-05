# Principle Check: Dual-Sphere vs. Single-Sphere Collision

**Date:** October 5, 2025  
**Purpose:** Analyze the proposal to replace the dual-sphere (lifter/bumper) collision system with a single-sphere system.  
**Sources:** AGENTS.md, PLANS/collision_system_implementation.md

---

## 1. Executive Summary

The proposal to move to a **single-sphere collision model is strongly aligned with core project principles** and is a valid response to the complexities encountered in the current dual-sphere implementation.

The existing system, while functional, has required numerous bug fixes and special-case logic, moving it away from the project's ideal of "simplicity over sophistication." A single sphere offers a path back toward a "stupidly simple core."

The primary risk is the potential loss of the emergent "step-up" behavior provided by the weightlifter sphere. This document recommends a **time-boxed experiment** to evaluate the gameplay feel of a single-sphere system before committing to a full refactor.

---

## 2. Principle Analysis

This analysis evaluates the single-sphere proposal against the tenets outlined in `AGENTS.md`.

### ✅ **Clarity over Cleverness & Simplicity over Sophistication**

- **Single-Sphere:** A single collision primitive is the epitome of simplicity. All collision logic would be unified, eliminating the need to arbitrate responsibilities between two competing spheres. This directly serves the principle.
- **Dual-Sphere (Current):** The current implementation is clever, but not simple. The `collision_system_implementation.md` documents a history of subtle bugs arising from the interaction of the two spheres:
    - The "character slow sink" issue.
    - Weightlifter penetration on vertical walls.
    - Edge-sinking behavior near steps.
- The fixes involved adding overlapping responsibilities and adaptive penetration limits, increasing complexity. This indicates the dual-sphere model may be a "clever" design that violates the simplicity principle in practice.

### ✅ **Iteration over Planning & Honoring Serendipity**

- The project has a healthy precedent for adapting its plan based on empirical results (e.g., the surface-classified authority in Phase A, the discovery that explicit step-up logic was unnecessary in Phase B).
- The user's observation that the dual-sphere system is creating friction and edge cases is a valid discovery from the implementation process. Acting on this by proposing a simpler alternative is a perfect example of "iteration over planning."

### ✅ **Elegant Systems (Stupidly Simple Cores, Emergent Complexity)**

- **Single-Sphere:** Represents the simplest possible core for collision. Complexity would emerge from the character controller's forces interacting with the simple environment geometry.
- **Dual-Sphere (Current):** The *intent* was for two simple spheres to create emergent locomotion. However, the implementation has required explicit, non-emergent rules to manage the interaction *between* the spheres. This suggests the core may not be as "stupidly simple" as desired. The elegance of the *idea* has not fully translated into an elegant *implementation*.

---

## 3. Implementation & Gameplay Analysis

### The "Step-Up" Problem

The primary justification for the weightlifter sphere was to "move up over small obstacles." The key finding from `collision_system_implementation.md` (Phase B) is that this behavior **emerged naturally from the sphere's geometry and slope-handling code**, not from explicit step-up logic.

This presents the central trade-off:
- **Pro (Single-Sphere):** Eliminates a significant source of bugs and logical complexity.
- **Con (Single-Sphere):** We may lose the natural, emergent step-up capability. A single, larger sphere might bump into small obstacles instead of climbing them.

The question is not "can we build a step-up system with one sphere?" but rather, "does a single sphere *feel* better to control, even if it means the loss of some emergent climbing?"

---

## 4. Recommended Action: Time-Boxed Experiment

Instead of debating in the abstract, we should gather empirical data through a short, focused experiment.

**Goal:** To evaluate the *feel* of a single-sphere controller.

**Plan:**
1.  **Branch:** Create a new development branch to isolate the experiment.
    ```
    git checkout -b feature/single-sphere-collision-experiment
    ```
2.  **Modify:** In `src/character/controller.cpp`, temporarily disable the weightlifter sphere's logic. The controller should only use the primary bumper sphere for all collision detection and resolution.
3.  **Playtest:** Use the existing test environments, paying close attention to:
    - The graduated step obstacles from Phase B.
    - Running into corners and along walls.
    - General feel and responsiveness. Does the character get stuck unexpectedly?
4.  **Evaluate:** After 1-2 hours of implementation and testing, assess the results.

### Possible Outcomes

1.  **Success:** The single sphere feels responsive and fluid. The loss of the automatic step-up is negligible or even desirable.
    - **Action:** Proceed with refactoring the collision system to permanently use the single-sphere model.
2.  **Failure:** The character feels clumsy and gets stuck on trivial obstacles. The loss of the weightlifter is immediately noticeable and detrimental.
    - **Action:** Abandon the experiment and merge nothing. The current dual-sphere system, despite its complexity, is validated as necessary.
3.  **Hybrid Potential:** The single sphere is promising but needs a minor adjustment to handle small ledges.
    - **Action:** Explore a simpler alternative to the second sphere. For example, a procedural "nudge" force could be applied when the single sphere collides with a low obstacle below a certain velocity. This would still be less complex than a fully independent physics object.

This experimental approach directly aligns with the project's "Graybox before polish" and "Iteration over planning" principles, providing a low-cost way to de-risk a significant architectural change.

---

## 5. Implementation Review & Analysis

**Reviewer:** GitHub Copilot  
**Date:** October 5, 2025  
**Context:** Review conducted after Phase A and Phase B completion

### Current Implementation State

The dual-sphere system has been fully implemented with the following three-phase collision resolution:

**Phase 1a:** Bumper handles flat ground (`normal.y > 0.9f` / ~25°)
- Zero-penetration contact
- Instant, responsive ground feel
- Primary collision volume for clean surfaces

**Phase 1b:** Weightlifter handles slopes (`0.5f < normal.y ≤ 0.9f` / 60°-25°) AND walls (`normal.y ≤ 0.5f`)
- Intentional burial depth (0.10m) on slopes for stability
- Adaptive penetration limits (0.02m on flat, 0.10m on slopes)
- Wall collision prevention (added after bug fix)

**Phase 2:** Bumper handles walls (`normal.y ≤ 0.5f` / 60°+)
- Slide-along behavior
- Velocity projection for smooth wall running

### Bug History Analysis

The implementation has required **three significant bug fixes**:

1. **Character Slow Sink** (`ISSUES/character_slow_sink.md`)
   - **Cause:** Lifter claimed authority over flat ground, gradually settling toward 0.10m burial
   - **Fix:** Split authority by surface angle (bumper for flat, lifter for slopes)
   - **Complexity Added:** Three-phase collision loop instead of two-phase

2. **Weightlifter Penetration on Vertical Walls**
   - **Cause:** Phase 1b only handled slopes, not walls; lifter could penetrate vertical step surfaces
   - **Fix:** Added wall collision branch to Phase 1b
   - **Complexity Added:** Duplicate wall handling logic (Phase 1b and Phase 2)

3. **Edge Sinking Issue**
   - **Cause:** Weightlifter Phase 1b didn't handle flat surfaces, allowing gradual sinking near edges
   - **Fix:** (Not fully documented in provided files)
   - **Complexity Added:** (Unknown without full implementation)

### Principle Violation Assessment

**"Clarity over Cleverness" / "Simplicity over Sophistication"**

The current system violates these principles through:
- **Three collision loops per frame** (originally planned as two)
- **Overlapping responsibilities** (both spheres handle walls, both handle some ground cases)
- **Surface classification thresholds** (0.9f flat, 0.5f slope, requiring careful tuning)
- **Adaptive penetration limits** (0.02m vs 0.10m depending on surface type)
- **Bug-driven complexity** (each fix added special-case logic)

**"Stupidly Simple Cores, Emergent Complexity"**

The implementation has moved away from this ideal:
- **Original intent:** Two simple spheres → emergent locomotion
- **Current reality:** Two spheres + explicit authority arbitration + adaptive thresholds
- **Evidence:** Three bug fixes required to manage sphere interaction, not environment interaction

### Counterargument: Has the System Stabilized?

**Devil's Advocate Position:**

The dual-sphere system may have reached a stable equilibrium:
- Phase B completed **without** explicit step-up logic (major win for emergent behavior)
- Natural obstacle traversal works through geometry alone
- Bug fixes addressed edge cases, but core logic is now sound
- Three-phase structure provides clear, deterministic coverage

**Supporting Evidence:**
- Phase B completion note: "No special step-up logic needed"
- Natural collision handling validated through playtesting
- Emergent traversal behavior is exactly what was desired

**Risk of Premature Optimization:**
If we abandon the dual-sphere system now, we may:
- Lose the emergent step-up capability permanently
- Discover the single-sphere system has different, equally complex edge cases
- Waste implementation effort on a lateral move, not an improvement

### My Assessment: **STRONG AGREEMENT with Single-Sphere Proposal**

Despite the counterargument, the evidence favors simplification:

#### 1. **Bug Pattern Indicates Fundamental Complexity**

Three bugs in ~100 lines of collision code is a high defect density. More critically, **all three bugs involved managing the interaction between the spheres**, not the spheres interacting with the environment. This suggests the complexity is intrinsic to the dual-sphere design, not the implementation.

#### 2. **The Step-Up "Feature" May Not Be Worth the Cost**

The primary justification for the weightlifter is automatic step-up. However:
- Maximum traversable height is **only ~0.15m** (based on Phase B testing)
- This is smaller than typical platformer design conventions (0.30-0.50m jumps are standard)
- Players may not even notice the difference between "walks up 0.15m steps" vs "jumps over 0.15m steps"
- A single-sphere system could use explicit jump input for small obstacles

**Question for Playtesting:** Would players prefer:
- **A:** Automatic 0.15m step-up, but occasional edge-case bugs (settling, penetration)
- **B:** No automatic step-up, but perfectly stable collision and explicit jump control

My prediction: **Most players would choose B.** Responsive, predictable control trumps subtle convenience features.

#### 3. **Emergent Behavior Did Work... But Not as Intended**

Phase B's success is often cited as validation of emergent design. However:
- The "emergent" traversal only works for very small obstacles (≤0.15m)
- Larger obstacles require explicit jumping (player input)
- The dual-sphere system doesn't eliminate player agency; it slightly reduces input frequency for trivial obstacles

**Re-framing:** The dual-sphere system reduces input on obstacles that don't matter (0.15m curbs) while still requiring input on obstacles that do matter (0.30m+ platforms). This is a **marginal convenience feature**, not a core locomotion mechanic.

#### 4. **Code Complexity Metrics**

Comparing hypothetical single-sphere vs. current dual-sphere:

| Aspect | Single-Sphere | Dual-Sphere (Current) |
|--------|---------------|----------------------|
| Collision loops per frame | 1 | 3 |
| Authority arbitration logic | None | Surface classification (2 thresholds) |
| Adaptive parameters | 0 | 2 (flat vs slope penetration) |
| Debug visualization complexity | 1 sphere | 2 spheres + 2 contact states |
| Lines of collision code | ~50 (est.) | ~150 (actual) |
| Bug fixes required | 0 (not implemented) | 3 (documented) |

The dual-sphere system is **~3x more complex** by code volume and execution cost.

#### 5. **The "Elegance" Test**

From `AGENTS.md`:
> "Every game exists already, hidden in the logic of the universe. We don't create them. We find them--not by adding, but by removing excess material that obscures the form within."

**Question:** Is the dual-sphere system revealing the elegant form, or obscuring it?

The bug history suggests **obscuring**. Each fix added logic to manage the spheres' interaction, not to express the character's relationship with the environment. The true "form" may be simpler than what we've built.

### Recommended Experimental Protocol (Revised)

I **strongly support** the time-boxed experiment, with these modifications:

#### Phase 1: Pure Single-Sphere Test (2 hours)
1. **Branch:** `feature/single-sphere-collision-experiment`
2. **Modify:** Disable weightlifter sphere entirely in `controller.cpp`
3. **Parameters:**
   - Single bumper sphere (0.50m radius)
   - Simple collision: push out of penetration, project velocity
   - No special-case logic, no authority arbitration
4. **Playtest Focus:**
   - General responsiveness and control feel
   - Wall sliding and corner navigation
   - Graduated step obstacles (0.15m, 0.30m, 0.40m, 0.60m)
   - **Critical question:** Does lack of auto-step-up feel limiting or liberating?

#### Phase 2: Single-Sphere + Explicit Jump Assist (1 hour, if needed)
If Phase 1 feels good but small steps are annoying:
5. **Add:** Simple "jump over small obstacle" helper
   - Detect ground collision with obstacle < 0.20m high
   - If player is pressing forward + space, apply 20% more jump force
   - No automatic lift, just assisted jumping
6. **Playtest:** Does this feel better than automatic step-up?

#### Phase 3: Decision Point
7. **Success Criteria:**
   - Single-sphere feels more responsive and predictable
   - Loss of auto-step-up is negligible or unnoticeable
   - Code is demonstrably simpler (fewer lines, fewer branches)
   - **Action:** Commit to single-sphere refactor

8. **Failure Criteria:**
   - Character feels clumsy or limited without auto-step-up
   - Players repeatedly try to walk over 0.15m obstacles
   - Manual jumping feels like a regression
   - **Action:** Abandon experiment, keep dual-sphere system

9. **Hybrid Criteria:**
   - Single-sphere is good, but needs minor traversal assistance
   - **Action:** Implement Phase 2 jump assist, validate feel

### Critical Success Factors

**For Single-Sphere to Win:**
- Must feel **more responsive** (instant contact, no settling)
- Must feel **more predictable** (no edge-case penetration bugs)
- Must **not feel limiting** (players don't miss auto-step-up)

**For Dual-Sphere to Win:**
- Must prove auto-step-up is **valuable** (players notice and appreciate it)
- Must prove **stability** (no more bugs emerge in extended playtesting)
- Must prove **elegance** (the three-phase logic is the "right" solution)

### My Prediction

Based on the evidence, I predict:
- Single-sphere will feel **more responsive and stable**
- Loss of auto-step-up will be **barely noticeable** (0.15m is too small to matter)
- Code simplification will be **dramatic** (~100 lines removed, 3 thresholds eliminated)
- **Result:** Single-sphere wins

But I could be wrong. The only way to know is to **build it and playtest it**.

### Final Recommendation

**PROCEED WITH THE EXPERIMENT IMMEDIATELY.**

This is a perfect application of "Iteration over planning." We've reached the point where debate is less valuable than empirical data. A 3-hour time-boxed experiment will answer definitively whether the single-sphere system is superior.

**Alignment Check:**
- ✅ **Clarity over cleverness** - Single sphere is maximally clear
- ✅ **Simplicity over sophistication** - Removing features to find the elegant core
- ✅ **Iteration over planning** - Test, don't theorize
- ✅ **Graybox before polish** - This is the graybox test of a simpler design
- ✅ **Honor serendipity** - We discovered complexity; now discover if simplicity works

**Risk Assessment:** LOW
- Experiment is isolated on a branch
- 3 hours of effort, zero commitment to merge
- Can revert instantly if results are negative
- Worst case: We validate the dual-sphere system through comparison

**Expected Outcome:** The single-sphere system will feel better and become the new foundation for Phases C-E.

---

## 6. Concerns & Counterpoints

### Concern 1: Loss of Overgrowth Fidelity

**Argument:** The dual-sphere system is directly inspired by Overgrowth (GDC 2013). Abandoning it means diverging from a proven reference design.

**Response:**
- Overgrowth's context is different (realistic 3D character action, not a FrogLords gameplay style)
- David Rosen didn't document bug fixes or edge cases in the GDC talk
- We're not building Overgrowth; we're finding **our** game's elegant form
- Inspiration ≠ prescription

### Concern 2: Step-Up Capability Is Core to Game Feel

**Argument:** Automatic traversal of small obstacles is a quality-of-life feature that reduces player frustration.

**Response:**
- Agreed **if** the obstacles are common and the traversal height is meaningful (0.30m+)
- Current system: 0.15m max traversal (may be too small to matter)
- Alternative: Design levels without trivial 0.15m obstacles
- Alternative: Use explicit jump assist (Phase 2 of experiment)

### Concern 3: Premature Simplification

**Argument:** The dual-sphere system has stabilized after bug fixes. Further simplification is premature optimization.

**Response:**
- Simplification **now** is better than after Phases C-E build on unstable foundation
- Three bugs in ~100 lines suggests instability, not maturity
- Experiment has **zero cost** (isolated branch, 3 hours)
- If dual-sphere is truly stable, experiment will validate it

### Concern 4: Single-Sphere May Have Different Edge Cases

**Argument:** A single-sphere system might introduce new, unforeseen bugs.

**Response:**
- **Possible**, but less likely due to reduced complexity
- Single sphere = single authority, no arbitration, fewer branches
- Simpler systems have smaller bug surfaces
- Experiment will surface any major issues immediately

### My Conclusion on Concerns

None of the counterarguments are strong enough to prevent the experiment. The risk is low, the potential benefit is high, and the effort is minimal. **Proceed with confidence.**
