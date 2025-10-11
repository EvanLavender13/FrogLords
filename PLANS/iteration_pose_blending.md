# Iteration Plan: Pose Blending

### 1. Feature Overview

**Description:** Replace discrete pose threshold switching with continuous quaternion interpolation (slerp) between walk cycle keyframes to eliminate visible pops at phase boundaries.

**Core Principle Alignment:**
- **Simplicity:** Extends existing distance-phased pattern without architectural changes
- **Elegance:** Single interpolation primitive (slerp) replaces branching threshold logic
- **Iteration:** Builds on proven quaternion foundation; 85% certainty enables confident planning
- **Proceduralism:** Continuous blending is the natural procedural expression of discrete keyframes
- **Gameplay-first:** Zero latency impact; animation quality improves without touching control

### 2. Design Rationale

**Problem/Goal:** Current threshold-based pose selection ([animation.cpp:127-138](../src/character/animation.cpp#L127-L138)) creates instant pose snaps at phase values 0.25, 0.5, 0.75. Limbs teleport between positions, producing visible pops. Secondary motion springs currently compensate for these discontinuities, but blended input would produce natural follow-through.

**Hypothesis:** Smooth quaternion interpolation between keyframes will eliminate pops and validate the skeletal animation architecture's readiness for richer content (run cycles, speed blending). Players will see fluid limb transitions throughout the walk cycle. Spring behavior will shift from "compensating for pops" to "natural follow-through on smooth motion."

### 3. Dependencies & Uncertainty

**Placement in Stack:** Sits directly above "Primary Skeletal Animation (100%)" and "Secondary Motion (100%)" in the dependency stack. Extends distance-phased animation pattern. Enables 2D blend spaces (walk↔run) and speed-based animation scaling as next steps.

**Direct Dependencies (all proven):**
- Quaternion keyframe architecture (validated in Static Keyframe Preview)
- Distance-phased animation (surveyor-wheel pattern proven)
- GLM slerp function (industry-standard)
- Secondary motion system (spring parameters tested)

**What Depends On This:** Future animation content (run gaits, 2D blend space), speed-based stride scaling, custom easing curves for non-linear phase mapping.

**Certainty Score: 85%**
- Architecture validated through 3 prior keyframe iterations (static preview, primary animation, secondary motion)
- Slerp is standard, well-understood operation with zero novelty risk
- GDC-validated approach (Overgrowth: linear slerp for locomotion, springs for secondary)
- Known constraint: Last segment (0.75-1.0) must target STEP_LEFT (not NEUTRAL) for seamless phase wrapping
- Minor uncertainty: Spring parameters may need retuning after blend (test with current values first)
- **No cascading uncertainty:** Changes isolated to single function; spring system decoupled

### 4. Implementation & Testing Loop

**Graybox Scope (The "Stupid Simple Core"):**

Modify `animation_state::update_skeletal_animation()` ([animation.cpp:110-144](../src/character/animation.cpp#L110-L144)) to replace threshold-based selection with segment-based blending.

1. **Extract keyframe getter** (new function in `keyframe.cpp`):
   ```cpp
   keyframe get_keyframe_data(pose_type pose);
   ```
   Returns raw quaternions for all 8 joints from hardcoded poses (T_POSE, STEP_LEFT, NEUTRAL, STEP_RIGHT).

2. **Replace threshold logic** (lines 127-138) with blend segments:
   - 0.00-0.25: STEP_LEFT → NEUTRAL
   - 0.25-0.50: NEUTRAL → STEP_RIGHT
   - 0.50-0.75: STEP_RIGHT → NEUTRAL
   - 0.75-1.00: NEUTRAL → STEP_LEFT (seamless wrap)

3. **Blend calculation per segment:**
   ```cpp
   float t = (phase - segment_start) / (segment_end - segment_start);
   glm::quat blended = glm::slerp(source.joint, target.joint, t);
   ```

4. **Apply blended quaternions directly to skeleton:**
   - Bypass `apply_pose()` in automatic mode
   - Compose with T-pose baseline (copy existing pattern from `keyframe.cpp`)
   - Apply to all 8 joints: shoulders(5,8), elbows(6,9), hips(11,14), knees(12,15)

5. **Secondary motion unchanged:** Springs operate on blended output (lines 140-143)

**Scope Estimate:** 40-60 lines (net: +30 lines)
- Add `get_keyframe_data()`: ~15 lines
- Replace threshold logic with segment blending: ~35 lines
- Remove discrete selection: -12 lines
- Manual override path unchanged

**Key Parameters (Already Exposed via GUI):**
- `cycle_length`: Distance per full walk cycle (tuned in locomotion panel)
- Spring parameters: `stiffness`, `damping_ratio`, `response_scale` (tuned in animation panel)
- No new parameters needed for core blending

**Testing Protocol:**

Self-test via visual inspection and animation panel:

1. **Smoothness Check:**
   - Walk character in circles, observe limbs at phase transitions
   - Watch for pops at thresholds: 0.25, 0.5, 0.75
   - Success: No visible discontinuities in limb motion

2. **Wrap Continuity:**
   - Watch phase value in animation panel near 1.0 → 0.0 transition
   - Success: Limbs blend smoothly from NEUTRAL to STEP_LEFT across wrap

3. **Spring Behavior:**
   - Compare spring wobble before/after blending
   - Test with current parameters first (no retuning pass initially)
   - Success: Natural follow-through without excessive wobble or damped-out motion

4. **Performance:**
   - Monitor frame time (already displayed in debug panel)
   - Calculation: 8 joints × 60 fps = 480 slerps/sec
   - Success: No measurable performance impact (GLM slerp is fast)

**Success Metrics (Definition of "Proven"):**
- Zero visible pops during continuous locomotion
- Phase wrap at 1.0 → 0.0 is seamless
- Spring behavior feels natural (not fighting the blend)
- Frame time unchanged (< 1% difference)
- Feature enables confident planning of run cycle content

**Iteration Cadence & Horizon:**
- **Complexity: 1-2 points** (single-function modification, proven architecture)
- **Test loop: < 5 minutes** (instant visual feedback via animation panel)
- **Iteration count: 1-2 expected** (slerp logic + optional spring retuning)
- **Horizon extension criteria:** If successful, plan 2D blend space (walk↔run) as next 2-point feature

### 5. System Integration

**System Ownership:**
- `animation_state`: Owned by `controller` ([controller.h:57](../src/character/controller.h#L57)) ✓
- `skeleton`: Shared resource in `game_world.t_pose_skeleton` ([game_world.h:29](../src/app/game_world.h#L29)), passed by reference ✓
- `keyframe` functions: Stateless utilities ✓
- **No ownership conflicts.** Modification confined to `animation_state::update_skeletal_animation()` which already operates on `skeleton&`.

**Mechanic Interactions:**
- **Secondary motion springs:** Currently compensate for discrete pops; after blending, will express natural follow-through. May require slight parameter adjustment (test unchanged first).
- **Manual override mode:** Unaffected (early return path unchanged)
- **Locomotion distance tracking:** Unchanged (phase calculation reused)
- **Acceleration tilt / landing spring:** Independent systems; no interaction

**Emergence Potential:**
- Smooth blending unlocks 2D blend space (walk↔run speed blending)
- Enables non-linear phase curves (anticipation, overshoot) via custom easing
- Foundation for IK layering (blended FK base + IK adjustments)
- Validates quaternion pipeline for future content (creature variants, equipment stance swaps)

**Interface & Feedback:**
- **Player understanding:** Transparent improvement; no new mechanics to learn
- **Visual feedback:** Limbs move continuously (existing skeleton debug visualization sufficient)
- **Graybox fidelity:** White wireframe skeleton already communicates limb motion clearly
- **Metaphor:** None needed (internal animation quality upgrade)
- **Animation panel:** Existing phase display and spring visualizations provide full observability

### 6. Risks & Future Work

**Potential Risks:**
- **Spring retuning required:** If current parameters were tuned to "fight" discrete pops, blended input may produce different behavior (underdamped wobble or overdamped staleness). Mitigation: Test unchanged first; small parameter adjustments likely sufficient.
- **Phase wrap edge case:** Last segment (0.75-1.0) must target STEP_LEFT for seamless loop. Incorrect target (NEUTRAL) will produce a visible snap. Mitigation: Feature plan explicitly calls this out; implementation will be deliberate.
- **GLM slerp correctness:** Assumes GLM's `glm::slerp(quat, quat, float)` handles shortest-path interpolation. Mitigation: Standard library function; validated in thousands of projects.

**Content Restrictions:** None. Feature removes jank without imposing constraints. Future work (run cycles, blend spaces) remains unrestricted.

**Decision Impacts:**
- **Implementation cost:** Low (40-60 lines, single function)
- **Maintenance burden:** None (replaces branching logic with loop)
- **Immaturity burden:** None (no other systems depend on this yet)
- **Critical failure risk:** Low (visual artifact only; gameplay unaffected)
- **Process burden:** None (solo project)
- **Decision cost:** Low (85% certainty, GDC-validated approach)

**Backlog Candidates:**
- **2D Blend Space (Walk↔Run):** Blend between walk/run cycles based on velocity magnitude. Natural next step after slerp proven. 70% certainty.
- **Custom Phase Easing:** Non-linear phase mapping (anticipation, overshoot). Adds character to gait. 60% certainty (depends on walk↔run first).
- **Speed-Based Stride Scaling:** Adjust `cycle_length` dynamically to avoid foot sliding. 75% certainty (proven surveyor-wheel pattern).
- **IK Foot Placement:** Layer 2-bone IK over blended FK for terrain conformance. 40% certainty (needs varied terrain first; flat ground currently).

**Future Architecture Notes:**
- Blended FK becomes stable base layer for future IK adjustments
- Pattern extends to upper body (reach, aim) and creature variants
- Consider extracting `blend_segment()` utility if pattern repeats (wait for third use)
