# Iteration Plan: Secondary Motion

### 1. Feature Overview

*   **Description:** Per-bone softness system that adds organic follow-through and wobble to skeleton joints using spring-damper dynamics. Each bone lags slightly behind its parent's motion, creating natural secondary motion without authored animation clips.
*   **Core Principle Alignment:**
    *   **Emergence:** Simple per-bone spring rules create rich, context-aware motion (acceleration → wobble, landing → settle, turning → lag)
    *   **Simplicity:** Parameters over assets; single softness value per bone type rather than animation variants
    *   **Proceduralism:** Physics-driven motion that reacts to any gameplay state automatically
    *   **Iteration:** Tune spring parameters in real-time via GUI; no rebuild iteration
    *   **Gameplay-first:** Pure reactive layer; never overrides core physics or adds input latency

### 2. Design Rationale

*   **Problem/Goal:** Current skeleton is rigid—limbs move instantly with the body. Real organic motion has follow-through, lag, and overshoot. Secondary motion adds life and weight perception without manual animation authoring.
*   **Hypothesis:** Visible wobble on limbs during movement will make the character feel more organic and readable. Players will subconsciously understand momentum and weight. Expected "juice" factor increases player satisfaction during core loop (run, jump, turn). Should feel natural without drawing conscious attention.

### 3. Dependencies & Uncertainty

*   **Placement in Stack:** Sits atop Skeleton System (100% certain) in the Reactive Animation Layer. Parallel to acceleration tilt and landing spring; shares same update timing and architectural pattern.
*   **Direct Dependencies:**
    *   `character::skeleton` (100% certain) ✅
    *   `spring_damper` primitive (100% certain) ✅
    *   `animation_state` pattern (proven twice) ✅
*   **What Depends On This:** Future IK systems, cloth simulation, equipment attachment (all in backlog)
*   **Certainty Score:** 85%
    *   Architecture pattern proven (acceleration tilt, landing spring both successful)
    *   Spring-damper primitive exists and tested
    *   Skeleton attachment validated
    *   Only uncertainty: tuning feel (softness per bone type)—solved through iteration
    *   Risk: minimal cascading uncertainty; purely additive reactive layer

### 4. Implementation & Testing Loop

*   **Graybox Scope (The "Stupid Simple Core"):**
    *   **Minimum viable:** Single spring per joint tracking rotational offset from parent-driven pose
    *   **Per-bone softness parameter:** Different spring stiffness for spine, upper arm, forearm, thigh, shin (5 bone categories)
    *   **Update timing:** After skeleton global transforms computed, before render
    *   **Transform application:** Rotational spring offset applied to joint's local transform
    *   **Initial targets:** Focus on arms/legs (most visible motion); skip fingers/toes initially
    *   **Key parameters to expose:**
        *   `spine_softness`, `upper_arm_softness`, `forearm_softness`, `thigh_softness`, `shin_softness` (0.0 = rigid, 1.0 = very soft)
        *   Global `secondary_motion_enabled` toggle
        *   Optional: `damping_ratio` multiplier (default to critical damping)
    *   **Implementation notes:**
        *   Store spring state per joint in new `secondary_motion_state` struct
        *   Compute angular velocity delta from parent motion each frame
        *   Apply spring force to rotational offset
        *   Convert spring offset back to local rotation matrix
        *   Must not affect skeleton data structure—only runtime computed transforms
*   **Testing Protocol:**
    *   **Self-test first:** Walk/run in circles; jump while moving; rapid direction changes. Watch for:
        *   Visible lag/wobble on limbs (should be subtle but noticeable)
        *   Natural settle after movement stops
        *   No jarring pops or instability
        *   Still feels responsive (not mushy/delayed)
    *   **Over-the-shoulder:** 6-8 tests, silent observation
    *   **Key question:** "Describe how the character's movement feels" (avoid mentioning arms/legs explicitly)
    *   **Look for:** Unprompted mentions of "natural," "organic," "bouncy," "alive" vs. "weird," "floaty," "delayed"
    *   **Metrics:** Frame time impact (should be <0.1ms for 20 joints); visual inspection for pops/artifacts
*   **Success Metrics (Definition of "Proven"):**
    *   Motion is visible but doesn't draw negative attention
    *   Testers describe character as "more natural" or don't consciously notice (both success)
    *   No gameplay feel degradation (responsiveness maintained)
    *   Parameters converge to stable values across multiple tuning sessions
    *   Zero physics instability or visual artifacts
    *   Performance cost negligible (<0.5% frame time)
*   **Iteration Cadence & Horizon:**
    *   **First loop:** 2-4 hours (implement, tune, self-test)
    *   **Subsequent loops:** 30-60 minutes (parameter adjustment based on feedback)
    *   **Criteria for extending:** If feel doesn't converge after 3 tuning sessions, may need architectural change (e.g., positional vs. rotational springs)
    *   **Criteria for shortening:** If first implementation feels immediately good, move to validation tests sooner

### 5. System Integration

*   **Mechanic Interactions (Emergence Potential):**
    *   **Acceleration tilt → wobble:** Body tilt causes limbs to swing outward
    *   **Landing spring → settle:** Crouch/recover on landing propagates through limbs
    *   **Orientation changes → lag:** Quick turns cause arms to trail behind body rotation
    *   **Locomotion speed → amplitude:** Faster movement = more dramatic wobble (natural physics)
    *   **Jump → extension:** Limbs extend during flight, compress on landing
    *   All interactions automatic—no explicit coupling required
*   **Interface & Feedback:**
    *   **Visual feedback:** Motion is self-evident; wobble visible during any movement
    *   **Metaphor:** Physical pendulum/spring on joints (player intuition: "things have weight")
    *   **Visual hierarchy:** Subtle enough to enhance without dominating visual attention
    *   **Redundancy:** Motion reinforces existing animation cues (acceleration, landing)
    *   **GUI tuning panel:** Extend `character_panel` with collapsing header "Secondary Motion"
        *   Enable/disable toggle
        *   Per-bone-type softness sliders
        *   Live state display: current max angular velocity across all joints

### 6. Risks & Future Work

*   **Potential Risks:**
    *   **Tuning difficulty:** May require many iterations to find natural feel
        *   *Mitigation:* Start with reference values from biomechanics (arm ~2Hz natural frequency); iterate from there
    *   **Visual artifacts:** Springs could overshoot and create unnatural poses
        *   *Mitigation:* Use critical damping by default; add angle clamps if needed
    *   **Performance:** 20 springs * 60fps = 1200 updates/sec
        *   *Mitigation:* Profile early; springs are cheap (2 multiply-adds); likely <0.1ms
    *   **Interaction conflicts:** Could fight with future IK system
        *   *Mitigation:* Design for IK override (secondary motion only on non-IK joints)
*   **Content Restrictions:**
    *   None. System is purely additive and parameter-driven; doesn't constrain level design, animation authoring, or gameplay mechanics
*   **Decision Impacts (Checklist):**
    *   **Implementation cost:** Low (2-4 hours; proven pattern)
    *   **Immaturity burden:** Low (isolated reactive layer; doesn't block other work)
    *   **Critical failure risk:** Very low (can disable with single toggle; no gameplay impact)
    *   **Process burden:** None (solo project; no coordination needed)
    *   **Political/cultural effects:** N/A (solo project)
    *   **Decision cost:** Low (architecture proven; small parameter space to explore)
*   **Backlog Candidates:**
    *   **Positional springs:** If rotational springs insufficient, try positional lag (e.g., tail dragging behind body)
    *   **Angle clamps:** Limit maximum spring rotation to prevent extreme poses
    *   **Velocity-scaled damping:** Increase damping at high speeds to reduce wobble
    *   **Per-joint spring visualization:** Debug draw spring state as colored spheres at joint positions
    *   **Spring presets:** Named configurations (e.g., "floppy," "tight," "ragdoll")
    *   **Asymmetric softness:** Different values for left/right limbs (personality/injury representation)
    *   **External forces:** Wind/impacts could add impulses to springs

---

### Implementation Checklist

- [ ] Create `src/character/secondary_motion.h/cpp`
- [ ] Define `secondary_motion_state` struct (per-joint spring storage)
- [ ] Implement `update_secondary_motion(skeleton&, secondary_motion_state&, float dt)`
- [ ] Add softness parameters to `animation_state` or new config struct
- [ ] Integrate into character update loop (after skeleton global transforms)
- [ ] Add GUI controls to `character_panel.cpp`
- [ ] Self-test: walk/run/jump with various softness values
- [ ] Profile performance impact
- [ ] Document learnings in iteration review

### Testing Notes

**Observations to capture:**
- Which bone types need the most softness?
- Does wobble enhance or distract from core movement feel?
- Are there specific movements where it looks wrong?
- Does it make the character feel heavier or lighter?
- Can you tune to invisible-but-better feel, or is it always noticeable?

**Parameter search strategy:**
1. Start all softness at 0.0 (disabled)
2. Enable only upper arms, moderate softness (0.5)
3. Iterate until arms feel good
4. Add legs, tune independently
5. Add spine last (most subtle)
6. Global tuning pass for coherence

---

**Estimated Completion:** 2-4 hours for graybox; additional 1-2 hours for tuning iteration

**Success Definition:** Character movement feels more organic without compromising responsiveness or introducing visual artifacts. Parameters converge to stable values.
