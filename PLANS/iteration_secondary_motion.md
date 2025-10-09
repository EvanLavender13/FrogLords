# Iteration Plan: Secondary Motion (Skeletal Follow-Through)

### 1. Feature Overview

*   **Description:** Per-bone spring-damper lag applied to elbow and knee joints, creating natural follow-through as shoulders and hips drive the primary animation. Limbs "wobble slightly" behind their parent joints during pose transitions and speed changes.
*   **Core Principle Alignment:**
    *   **Simplicity:** Reuses proven spring-damper pattern from landing spring and acceleration tilt. Pure reactive layer with minimal new code (~40-60 lines).
    *   **Elegance through emergence:** Simple spring physics on 4 joints creates natural secondary motion without authored keyframes.
    *   **Iteration over planning:** High certainty (75%) due to proven prerequisites and pattern reuse enables tight iteration loop (1-2 days).
    *   **Proceduralism:** Per-bone softness parameters drive follow-through algorithmically; no manual animation variants needed.
    *   **Do no harm:** Reactive layer applied after primary pose selection; never overrides core animation logic.

### 2. Design Rationale

*   **Problem/Goal:** Primary skeletal animation (distance-phased pose switching) currently has hard transitions between discrete poses. Adding subtle lag to child joints (elbows/knees) will smooth visual transitions and make locomotion feel more organic and alive without changing core mechanics.
*   **Hypothesis:** Players will perceive limb motion as more natural and lifelike. The "wobble" will be most noticeable during pose transitions (STEP_LEFT ↔ NEUTRAL) and when starting/stopping movement, adding polish that validates the spring-damper pattern for skeletal animation. Expected feel: subtle follow-through similar to GDC reference ("helps smooth visual transitions"), not a primary visual feature.

### 3. Dependencies & Uncertainty

*   **Placement in Stack:** Builds directly on Primary Skeletal Animation ✅ (distance-phased pose switching complete). Sits in the Reactive Systems Layer alongside acceleration tilt and landing spring. Does not block future features; can be tuned or removed without cascading impact.
*   **Direct Dependencies:**
    *   Primary Skeletal Animation ✅ (provides parent motion to lag behind)
    *   Spring-damper foundation ✅ (critically damped spring math validated twice)
    *   Skeleton transform propagation ✅ (hierarchical joint updates working)
*   **Unlocks (Future Work):**
    *   Advanced secondary motion (head bobble, spine flex)
    *   Cloth/appendage physics patterns
    *   IK softness (spring-damped target reaching)
*   **Certainty Score:** 75% (increased from backlog 60%)
    *   **Why higher:** Prerequisites just validated (primary animation working), spring-damper pattern proven in two reactive systems (acceleration tilt, landing spring), architecture well-understood
    *   **Risks:** Tuning time to find natural feel (stiffness/damping balance); potential for visual artifacts if spring parameters unstable
    *   **Architecture risk:** Low - isolated reactive layer, no core logic changes, clean integration point identified
    *   **Cascading uncertainty:** Minimal - feature is additive polish, can be disabled without affecting core gameplay

### 4. Implementation & Testing Loop

*   **Graybox Scope (The "Stupid Simple Core"):**
    *   **Minimum viable implementation:**
        *   Add `secondary_motion_state` struct to `animation_state` (4 springs: left_elbow, right_elbow, left_knee, right_knee)
        *   Each spring tracks: current angle offset (float), velocity (float)
        *   Two global parameters: stiffness (Hz), damping ratio (critically damped by default)
        *   Implement `update_secondary_motion()` function: compute spring lag from pose target, integrate spring state, inject rotation offsets into joint transforms
        *   Integration point: Call after `apply_pose()`, before skeleton transform propagation
        *   Axes: elbows lag on Y-axis (forward/back swing), knees lag on X-axis (forward/back swing)
    *   **Key parameters to expose:**
        *   Stiffness (0.5-20.0 Hz slider) - controls spring response speed
        *   Damping ratio (0.5-2.0 slider, default 1.0 critically damped) - controls settle behavior
    *   **Tuning UI:**
        *   New collapsing header "Secondary Motion" in Animation Tuning panel
        *   Stiffness and damping sliders
        *   Live state display: current angle offset and velocity for all 4 joints
    *   **Physics-first principles:**
        *   Spring toward pose target rotation (set by primary animation)
        *   Critically damped by default (smooth settle, no overshoot)
        *   Integrate with forward Euler (matches existing spring implementations)
        *   Inject offsets as additional rotation in parent space after pose applied

*   **Testing Protocol:**
    *   **Self-test first:**
        1. Visual coherence test: Walk character, observe elbow/knee lag behind shoulder/hip swings
        2. Stop test: Release input, verify wobble settles smoothly to rest (no oscillation/jitter)
        3. Transition test: Trigger pose switches (STEP_LEFT ↔ NEUTRAL), look for visible follow-through
        4. Parameter sweep: Tune stiffness (0.5, 2.0, 5.0, 10.0, 20.0 Hz) and damping (0.5, 1.0, 2.0) in real-time
        5. Performance test: Verify <1ms frame time impact (profile with debug UI FPS display)
    *   **Over-the-shoulder testing:**
        *   Stay silent—don't coach. Ask, "What do you notice about how the character moves?"
        *   Target: 6-8 testers (mix of Kleenex and experienced)
        *   Look for: Do they notice the wobble? Does it feel natural or distracting?
    *   **Iteration cadence:** Hours per tuning loop (real-time parameter adjustment via GUI)

*   **Success Metrics (Definition of "Proven"):**
    *   **Visual:** Elbows/knees visibly lag behind shoulders/hips during walking (subtle but noticeable on close inspection)
    *   **Feel:** Motion looks natural, not floppy (under-damped) or stiff (over-damped)
    *   **Stop behavior:** Wobble settles smoothly when movement stops, no jitter or overshoot
    *   **Transition quality:** Follow-through visible during pose switches, smooths hard transitions
    *   **Performance:** <1ms frame time impact (spring updates trivial cost)
    *   **Tuning:** Parameters feel intuitive to adjust, defaults work for 80% case
    *   **Integration:** Works alongside existing reactive systems (tilt, landing spring) with no interference
    *   **Playtesting:** Survives 6+ tests without major negative feedback ("looks weird", "too distracting")

*   **Iteration Cadence & Horizon:**
    *   **Phase 1 (2-4 hours):** Implement spring state struct, update function for one joint (left elbow), verify basic lag visible
    *   **Phase 2 (1-2 hours):** Extend to all 4 joints, verify no crashes/artifacts
    *   **Phase 3 (2-4 hours):** Add tuning UI with live state display
    *   **Phase 4 (4-8 hours):** Feel tuning iteration (find natural stiffness/damping values)
    *   **Total horizon:** 1-2 days
    *   **Criteria for extending:** If initial implementation reveals unexpected complexity (e.g., axis confusion, instability at high speeds), extend horizon and reassess
    *   **Criteria for shortening:** If spring pattern ports cleanly and defaults feel good immediately, could complete in <1 day

### 5. System Integration

*   **System Ownership:**
    *   **Current state:**
        *   `skeleton`: Lives in `game_world.t_pose_skeleton` (game_world.h:20)
        *   `animation_state`: Lives in `controller.animation` (controller.h)
        *   `update_skeletal_animation()`: Called from game_world.cpp:61 on game_world's skeleton
    *   **No ownership conflicts:** Secondary motion state lives in `animation_state` (same as tilt/landing spring). Update function called in same location as primary animation update. Clean separation maintained.
    *   **Integration point:** Integrate `update_secondary_motion()` directly inside `update_skeletal_animation()` after `apply_pose()` to encapsulate all animation logic in one function call. This maintains clean separation while keeping related logic together.

*   **Mechanic Interactions:**
    *   **Primary skeletal animation:** Sets pose targets that secondary motion springs toward. Works at all speeds (surveyor-wheel distance accumulation).
    *   **Acceleration tilt:** Independent systems - tilt affects body orientation, secondary motion affects joint rotations. No interference expected.
    *   **Landing spring:** Independent systems - landing spring affects vertical body offset, secondary motion affects limb joints. Could create emergent "settling" feel when landing causes body bounce while limbs lag.
    *   **Locomotion speed:** Faster movement = faster pose transitions = more visible secondary motion wobble (emergence without special handling).
    *   **Stop behavior:** When distance_traveled stops accumulating, primary pose freezes, secondary motion springs naturally settle to rest. No special handling needed (validated pattern from landing spring).

*   **Interface & Feedback:**
    *   **Player understanding:** Secondary motion is passive visual feedback, not a mechanic players interact with directly. Should be subtle enough to feel natural without conscious recognition.
    *   **Visual hierarchy:** Wobble should enhance motion readability (smooth transitions) without distracting from primary gameplay (movement, jumping, collision).
    *   **Redundancy:** Not applicable - this is polish, not critical information.
    *   **Graybox feedback plan:**
        *   Visual: Existing wireframe skeleton rendering shows joint rotations clearly (no new visualization needed)
        *   Debug UI: Live spring state display (angle offsets, velocities) provides tuning feedback
        *   Future (if needed): Option to toggle secondary motion on/off for A/B comparison

### 6. Risks & Future Work

*   **Potential Risks:**
    *   **Tuning time:** Finding natural-feeling stiffness/damping values may take longer than expected. Mitigation: Start with critically damped (ratio=1.0) and stiffness=5.0 Hz (middle of range), iterate from there.
    *   **Visual artifacts:** Spring instability (too stiff, wrong axis, numerical issues) could cause jitter or unnatural motion. Mitigation: Use proven forward Euler integration, clamp extreme values, test at high speeds.
    *   **Performance:** 4 springs updated per frame should be trivial (<1ms), but verify with profiling. Mitigation: If needed, skip updates when character stationary (optimization, not expected to be necessary).
    *   **Axis confusion:** Elbow/knee rotation axes must match parent swing motion. Mitigation: Feature plan already clarifies axes (elbows Y, knees X) from prior investigation. Verify early in Phase 1.
    *   **Scope creep:** Temptation to add more joints (spine, ankles, wrists) before validating core 4. Mitigation: Strict adherence to graybox scope (elbows/knees only).

*   **Content Restrictions:**
    *   **None expected.** Secondary motion is purely reactive to existing animation. No constraints on level design, character abilities, or future features.
    *   **Validation check:** Ensure secondary motion doesn't introduce physics instability or break IK in future (if IK applied to hands/feet, ensure secondary motion on elbows/knees doesn't conflict).

*   **Decision Impacts (Checklist):**
    *   **Implementation cost:** Low (~40-60 lines, 1-2 days). Follows proven pattern.
    *   **Immaturity burden:** Low. Isolated system, can be disabled with one flag if issues arise. No other features depend on it.
    *   **Critical failure risk:** Very low. Pure visual polish; failure mode is "looks weird", not "game crashes" or "gameplay breaks".
    *   **Process burden:** Minimal. Self-contained feature, no cross-team coordination needed (solo project).
    *   **Political/cultural effects:** N/A (solo project).
    *   **Decision cost:** Low. High certainty, clear scope, proven pattern. No deep analysis required.

*   **Backlog Candidates (Related Ideas to Defer):**
    *   **Advanced secondary motion:**
        *   Head bobble (spring lag behind spine)
        *   Spine flex (mid-spine lag behind root/hips)
        *   Ankle wobble (minor, likely low value)
        *   Wrist wobble (only if hands become visually important)
        *   **Certainty:** 40% - depends on core 4-joint validation proving value
    *   **Per-joint damping ratios:** Independent stiffness/damping per joint instead of global parameters
        *   **Certainty:** 50% - only if global parameters prove insufficient during tuning
    *   **Speed-based secondary motion scaling:** Reduce wobble at very low speeds, increase at high speeds
        *   **Certainty:** 30% - premature optimization until feel tuning reveals need
    *   **Collision-aware secondary motion:** Limbs react to world impacts (e.g., arm hits wall, spring compresses)
        *   **Certainty:** 20% - complex, low value without evidence of need
    *   **Cloth/appendage physics:** Extend spring-damper pattern to capes, tails, ears
        *   **Certainty:** 60% - validated pattern, but requires mesh/bones for appendages
    *   **IK softness:** Spring-damped IK target reaching (hand reaches for ledge with slight overshoot/settle)
        *   **Certainty:** 70% - natural extension once IK system exists

---

## Notes

*   **Previous deferral resolved:** Feature was deferred 2025-10-06 because static T-pose had no parent motion to lag behind. Primary skeletal animation (completed 2025-10-09) now provides distance-phased limb swings, unblocking this feature.
*   **Pattern validation:** Spring-damper pattern proven twice (acceleration tilt, landing spring). Critically damped defaults work well. UI exposure pattern established. Direct reuse reduces risk.
*   **Architecture continuity:** Follows established `animation_state` pattern. Update timing matches other reactive systems (after core state update, before render). No architectural changes needed.
*   **Testing emphasis:** Visual quality is subjective. Plan for longer feel tuning phase (Phase 4, 4-8 hours). Use A/B toggle (enable/disable secondary motion) to validate value.
*   **Scope discipline:** Resist temptation to add more joints or complexity before validating core 4-joint implementation. Graybox first, polish later.
