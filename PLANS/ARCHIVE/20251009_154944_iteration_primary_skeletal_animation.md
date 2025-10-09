# Iteration Plan: Primary Skeletal Animation

### 1. Feature Overview

*   **Description:** Distance-phased pose switching synchronized to surveyor-wheel locomotion. Limbs snap between discrete walk poses (STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL → repeat) based on distance traveled, no blending.
*   **Core Principle Alignment:**
    *   **Simplicity:** Threshold-based pose selection with no interpolation complexity
    *   **Iteration:** Minimal graybox (~30-40 lines) validates distance-phased triggering before adding blending
    *   **Proceduralism:** Extends proven surveyor-wheel pattern from body bounce to skeletal joints
    *   **Gameplay-first:** Pure reactive layer—animation follows locomotion state, no control theft

### 2. Design Rationale

*   **Problem/Goal:** Current skeletal system supports manual pose selection (Static Keyframe Preview validated), but locomotion feels disconnected from limb motion. Goal: synchronize limb animation to movement for visual feedback on stride timing.
*   **Hypothesis:** Distance-phased pose cycling will provide immediate visual feedback for tuning stride length. Watching legs alternate (left → neutral → right → neutral) while adjusting `cycle_length` slider will reveal "correct" timing through subjective feel. Validates triggering system before adding interpolation complexity.

### 3. Dependencies & Uncertainty

*   **Placement in Stack:** Builds directly on Static Keyframe Preview (100% certain). Sits between Reactive Animation Layer and future pose blending. Depends on:
    *   Validated quaternion keyframe architecture (`character::keyframe`)
    *   Existing surveyor-wheel distance tracking (`locomotion.distance_traveled`)
    *   Proven pose application system (`apply_pose()`)
*   **Certainty Score:** ~75%—simpler than original plan (no blending math). Only risk is finding the right `cycle_length` values, but that's pure tuning with real-time feedback. Increased from 60% (backlog estimate) because Static Keyframe Preview validated all underlying architecture.
*   **Cascading Uncertainty:** None—this is a leaf feature. Failure affects only skeletal animation system; locomotion and controller remain unaffected. Success unlocks pose blending (lerp/slerp) as natural next step.

### 4. Implementation & Testing Loop

*   **Graybox Scope (The "Stupid Simple Core"):**
    *   Add `cycle_length` parameter to animation state (default: 1.0m—one full stride per meter)
    *   In `character::animation::update()` (or new dedicated function), calculate phase:
        ```cpp
        phase = fmod(locomotion.distance_traveled, cycle_length) / cycle_length;  // 0.0-1.0
        ```
    *   Threshold-based pose selection:
        ```cpp
        if (phase < 0.25f) selected = STEP_LEFT;
        else if (phase < 0.5f) selected = NEUTRAL;
        else if (phase < 0.75f) selected = STEP_RIGHT;
        else selected = NEUTRAL;
        ```
    *   Call `apply_pose(skeleton, selected)` (reuses existing validated function)
    *   Expose `cycle_length` as tunable slider in character panel GUI
    *   Stop behavior: distance stops → phase stops → pose freezes (no special handling)
    *   Keep manual pose selection UI active but secondary (useful for debugging individual poses)
    *   **Files Modified:**
        *   `src/character/animation.h` (add `cycle_length` to animation_state)
        *   `src/character/animation.cpp` (add distance-phase logic)
        *   `src/gui/character_panel.cpp` (add cycle_length slider)
        *   `src/app/game_world.cpp` (call animation update before pose application)
    *   **Estimated Code:** ~30-40 lines total
*   **Testing Protocol:**
    *   **Self-test first:** Walk forward in test level, watch legs alternate. Verify STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL pattern repeats. Stop mid-stride, verify pose freezes with no pop to neutral. Adjust `cycle_length` slider while walking, verify stride frequency changes immediately.
    *   **Over-the-shoulder (if needed):** Ask tester to "run around and tell me what feels weird." Stay silent—don't explain the cycle length parameter. Watch for natural complaints about "legs moving too fast/slow" or "feet sliding."
    *   **Target sample size:** 2-3 self-tests (rapid iteration), 3-5 silent observer tests if feel is uncertain
    *   **Avoid leading questions:** Not "Does the stride length feel right?" but "Tell me the story of what just happened when you were running."
    *   **Metrics:** None—purely subjective feel at this stage
*   **Success Metrics (Definition of "Proven"):**
    *   Legs visually alternate in correct sequence (left-forward → neutral → right-forward → neutral observable)
    *   Arms counter-swing opposite to legs (verified by visual inspection of STEP_LEFT/RIGHT poses)
    *   Stopping mid-stride freezes pose with no snap/pop to neutral
    *   `cycle_length` adjustable in real-time, stride frequency changes immediately
    *   No crashes, visual artifacts, or T-pose fallbacks
    *   Survives 3+ playtests without "that looks broken" feedback
*   **Iteration Cadence & Horizon:**
    *   **Initial implementation:** 1-2 hours (known architecture, small scope)
    *   **Tuning loop:** 30-minute increments (adjust `cycle_length`, walk/run, repeat)
    *   **Horizon:** 1 day total—feature is binary (either poses cycle or they don't)
    *   **Criteria for shortening:** If first implementation works perfectly, stop
    *   **Criteria for lengthening:** If threshold values (0.33/0.66) feel wrong, may need parametrization—but avoid premature complexity

### 5. System Integration

*   **System Ownership:**
    *   `skeleton`: Lives in `game_world.t_pose_skeleton` (no conflicts—already integrated)
    *   `locomotion`: Lives in `game_world.locomotion` (provides `distance_traveled`—read-only access)
    *   `animation`: Lives in `controller.animation` (add `cycle_length` parameter here)
    *   **Integration point:** `game_world::update()` already calls animation updates and pose application. Slot new logic between locomotion update and skeleton transform update.
    *   **No ownership conflicts detected**—clean read from locomotion, write to skeleton, state in animation
*   **Mechanic Interactions:**
    *   **Surveyor-wheel body bounce:** Both driven by `distance_traveled`—should feel synchronized (legs step when body bounces)
    *   **Acceleration tilt:** Independent reactive layer—tilt applies after pose selection, no conflicts
    *   **Landing spring:** Independent reactive layer—crouch applies after pose selection, no conflicts
    *   **Static keyframe preview:** Manual pose selection becomes secondary (override mode for debugging)
    *   **Walk speed lock (SHIFT key):** Quick addition—holding SHIFT caps max_speed to walk_speed (2.0 m/s) for easier tuning observation. Enables precise control when adjusting cycle_length slider.
    *   **Emergence potential:** Distance-phased triggering pattern could extend to other cyclic behaviors (tail swing, ear flaps, breathing)
*   **Interface & Feedback:**
    *   **Player understanding:** Passive—player doesn't control this directly. Visual metaphor: "character walks like a character" (legs alternate, arms swing)
    *   **Tuning interface:** Single slider (`cycle_length`) follows existing animation tuning UI pattern—collapsing header with live parameter adjustment
    *   **Visual hierarchy:** Limb motion should be visible but not distracting. Wireframe skeleton shows motion clearly without art noise.
    *   **Redundancy:** Locomotion feedback already exists (body bounce, orientation). Skeletal animation adds redundant "I am moving" signal through limb motion—passive redundancy when primary (body position) is unclear.
    *   **Audio feedback:** None in graybox (defer footstep sounds until distance-phased triggering proven visually)

### 6. Risks & Future Work

*   **Potential Risks:**
    *   **Cycle length tuning frustration:** May be hard to find "correct" value without blending. Mitigation: start with 1.0m (one stride per meter), adjust by 0.1m increments until legs match body motion.
    *   **Snapping looks robotic:** Discrete pose switching may feel jarring. Mitigation: acceptable for graybox—validates triggering before adding interpolation. If unbearable, add lerp/slerp in next iteration.
    *   **Threshold values feel arbitrary:** 0.25/0.5/0.75 split may not match visual expectation of "step." Mitigation: tune visually; consider parametrizing if fixed values fail.
    *   **Distance stops at zero speed:** If character velocity is exactly zero, distance stops updating, animation freezes. Mitigation: intended behavior—no special handling needed (matches body bounce).
*   **Content Restrictions:**
    *   **None.** Feature is purely reactive to existing locomotion state. No level design constraints, no animation asset requirements beyond existing hardcoded poses.
*   **Decision Impacts (Checklist):**
    *   **Implementation cost:** Low (~1 day, small scope, known architecture)
    *   **Immaturity burden:** Low (leaf feature, no dependencies from other systems)
    *   **Critical failure risk:** Very low (crashes limited to animation update; core physics unaffected)
    *   **Process burden:** None (solo project, no coordination overhead)
    *   **Political/cultural effects:** None
    *   **Decision cost:** Low—one-day iteration, reversible if proven doesn't hold
*   **Backlog Candidates:**
    *   **Pose blending (lerp/slerp):** If discrete switching feels too robotic, add interpolation between keyframes. Natural next step after distance-phased triggering validated.
    *   **Speed-based cycle length scaling:** Faster movement = shorter stride cycle (legs move faster). Defer until base cycle length tuned.
    *   **Running gait keyframes:** Add RUN_STEP_LEFT/NEUTRAL/RIGHT poses, blend based on `locomotion.walk_factor`. Already in backlog—this iteration validates triggering for walk poses.
    *   **Backward walking:** Out of scope (character can't walk backwards currently). Add if locomotion system supports reverse movement.
    *   **Animation state machine:** If pose selection logic grows complex, extract to dedicated state machine. Defer until third use case appears (avoid premature abstraction).

---

## Approval

- **Status:** Approved
- **Reviewer:** Claude (FrogLords Code Review Agent)
- **Date:** 2025-10-09
- **Review Document:** [code_review_primary_skeletal_animation.md](code_review_primary_skeletal_animation.md)
- **Summary:** Distance-phased skeletal animation successfully implemented and validated. All success metrics met, no gameplay-first principles violated. Minor suggestions for future refactoring documented in review.

---

## Completion

- **Date Completed:** October 9, 2025
- **Final Certainty Score:** 100%
- **Code Review:** [code_review_primary_skeletal_animation.md](code_review_primary_skeletal_animation.md)
- **Outcome:** Distance-phased pose switching validated successfully. Surveyor-wheel pattern extends cleanly to skeletal animation, threshold-based pose selection sufficient for graybox validation. Walk speed lock feature added for precise tuning observation. All success metrics met with no gameplay-first principle violations.
