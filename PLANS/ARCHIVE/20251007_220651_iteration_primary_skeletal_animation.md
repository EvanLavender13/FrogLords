# Iteration Plan: Primary Skeletal Animation

### 1. Feature Overview

* **Description:** Drive arm and leg keyframes from the existing surveyor-wheel locomotion phase so the debug skeleton cycles through a four-pose run gait (REACH-RIGHT → PASS-LEFT → REACH-LEFT → PASS-RIGHT) that locks elbows at 90° and maintains opposite arm/leg coordination.
* **Core Principle Alignment:** Builds elegant emergence by reusing locomotion distance instead of time; keeps scope simple (hardcoded run poses) to enable fast iteration; graybox-first via debug skeleton before committing to production assets; respects gameplay-first mandate by reading controller velocity/orientation without hijacking input flow; leverages procedural layering (core locomotion → primary skeletal motion → later reactive layers).

### 2. Design Rationale

* **Problem/Goal:** The skeleton currently mirrors a static T-pose or a sine-wave probe, leaving no readable limb motion to evaluate feel or unlock reactive animation. We need a locomotion-synced base layer so future systems (secondary motion, IK) have a moving foundation.
* **Hypothesis:** Mapping locomotion distance to discrete limb poses will instantly communicate speed changes, preserve rhythm through corners, and make acceleration/landing effects feel grounded. Testers should perceive the character as "running" even with graybox visuals, and tuning distances will feel intuitive because phase ties directly to ground travel.

### 3. Dependencies & Uncertainty

* **Placement in Stack:** Sits above the proven skeleton attach/debug layer and directly on top of locomotion + orientation (95% certain tier in `DEPENDENCY_STACK.md`). It feeds the reactive animation layer and future polish features (secondary motion, IK, weapon poses).
* **Certainty Score:** 80%. Technique is a direct extension of the locomotion surveyor wheel (low novelty), but limb tuning parameters and GUI workflow may require iteration. Cascading uncertainty: if limb angles feel wrong, reactive layers and secondary motion remain blocked.

### 4. Implementation & Testing Loop

* **Graybox Scope (The "Stupid Simple Core"):**
  * Author a `struct keyframe` with six joint angles (shoulders, hips, knees) and a fixed array of four run poses derived from the feature brief.
  * Compute locomotion phase as `(distance_traveled * frequency) % 1.0f`, then map to `0..3` frame index with instant switching. Frequency starts at 0.5 strides per meter (tunable).
  * Apply joint angles directly to `t_pose_skeleton` locals each frame before `character::update_global_transforms`, respecting existing root/world transform attach.
  * Keep system interruptible: limbs immediately freeze when controller velocity zeroes; no additional easing beyond locomotion smoothing.
  * Rotate model to velocity via existing orientation (already in place); do not inject new transforms into controller physics path.
  * Expose core parameters in GUI: frame selector, phase scrubber, per-joint sliders, phase frequency multiplier, enable/disable toggle.
* **Testing Protocol:**
  * Solo tuning loop first: adjust sliders while running straight, circling, start/stop; capture screenshots of angles that feel natural.
  * Over-the-shoulder observation with at least six testers (mix of Kleenex and experienced) once poses feel believable; remain silent, ask for "story" recap.
  * Record qualitative notes on readability, perceived responsiveness, and any mismatch between limb motion and locomotion speed; log phase jitters or skating if observed.
* **Success Metrics (Definition of "Proven"):**
  * Limbs maintain correct opposite coordination through 10+ direction changes with no desync.
  * Phase transitions remain visually clean (no popping beyond intentional discrete switch) across start/stop cases.
  * Testers describe movement as "running" or "makes sense" without prompting in ≥6 sessions and report no confusion about limb timing.
  * Zero new content restrictions introduced (feature works on current flat test level and any future layouts without special requirements).
* **Iteration Cadence & Horizon:**
  * Expect 2–3 hour loops: implement baseline (half-day), run immediate self-tests, then schedule follow-up tuning sessions within 24 hours.
  * Shorten cadence if testers flag readability issues; lengthen only after limb tuning stabilizes and GUI workflow feels efficient.

### 5. System Integration

* **System Ownership:**
  * `game_world` currently owns `locomotion` and `t_pose_skeleton`, updating both each frame. Skeleton root attaches to controller transform in `game_world::update` before optional probe animation.
  * `character_panel` drives debug toggles and will host new tuning controls. No other systems currently mutate joint locals.
  * Architectural question: primary skeletal animation logic should live in a dedicated `character::skeletal_animation` module (invoked from `game_world::update`) rather than inline in `game_world` to avoid controller/skeleton ownership confusion. Need to confirm whether future controller refactors plan to own skeleton to prevent duplicate state.
* **Mechanic Interactions:**
  * Uses locomotion phase computed from smoothed speed—ensures compatibility with existing wheel debug draw and acceleration tilt.
  * Must play nicely with landing spring/tilt matrices: order of operations should remain `root -> spring/tilt -> skeletal joints` so reactive offsets stack cleanly.
  * Future emergence: secondary motion can read pose deltas; IK foot placement can overlay onto animated legs without fighting base pose.
* **Interface & Feedback:**
  * Extend character panel's Skeleton section with: run toggle (default on), keyframe selector with pose lock, phase scrubber slider (0..1), per-limb angle sliders, frequency multiplier, and readouts of current locomotion distance/phase for debugging.
  * Visual hierarchy: keep sliders grouped by limb (Shoulders, Hips, Knees) with redundant labels (left/right) for quick scanning.
  * Debug draw: reuse existing skeleton rendering; optionally add a subtle color highlight for the currently applied keyframe.

### 6. Risks & Future Work

* **Potential Risks:**
  * Phase jitter if locomotion smoothing is insufficient at low speeds (may require dead zone or hysteresis).
  * Instant pose switching could look too harsh; may need micro-blend later, but defer until proven necessary.
  * GUI clutter risk—ensure new controls remain collapsible to preserve clarity.
* **Content Restrictions:** None anticipated; mechanic relies solely on locomotion data and works at any speed or level scale. Avoid assumptions about ground height or ceiling clearance.
* **Decision Impacts (Checklist):**
  * Implementation cost: Moderate (~1–2 days for system + GUI); manageable.
  * Immaturity burden: Low—encapsulated module minimizes surface area.
  * Critical failure risk: Medium—bad phase math could desync visuals, but easy to revert.
  * Process burden: Low—single-developer iteration with existing tooling.
  * Political/Cultural: Positive—unlocks downstream animation work, reinforces procedural-first culture.
  * Decision cost: Acceptable—clear prerequisites satisfied; fast feedback loops available.
* **Backlog Candidates:**
  * Log "Walk gait keyframes" and "Pose reset helper" as follow-on tasks once run cycle validated.
  * Capture findings for future "Secondary motion softness" tuning parameters if motion reveals obvious lag opportunities.

---

## Deferral

**Date:** 2025-10-07

**Reason:** Fundamental data structure limitation - single-float-per-joint cannot represent 3D rotation

**Evidence Gap:** Original plan used `struct run_keyframe { float left_shoulder; float right_shoulder; ... }` storing one angle per joint. This is architecturally insufficient for skeletal animation:
- Real 3D joint rotation requires 3+ degrees of freedom (pitch/yaw/roll or quaternion)
- Single float cannot encode arbitrary 3D rotation
- Transform composition with T-pose baselines produces incorrect results when axis-of-rotation varies by joint orientation
- Multiple implementation attempts (custom swing vectors, direct axis rotation, baseline-ignoring transforms) all failed due to fundamental data limitation

**Missing Prerequisites:**
- Quaternion-based (or Euler-angle-based) keyframe data structure
- Clear separation of keyframe authoring from animation playback
- Understanding of per-joint rotation axes and composition rules

**Reconsideration Criteria:** After implementing reduced-scope **Static Keyframe Preview** feature to validate quaternion storage/application:
1. Upgrade keyframe struct to quaternions
2. Store 4 hardcoded poses (no authoring UI)
3. GUI manual selection only (no locomotion phase)
4. Validates quaternion storage/application before adding animation

**Technical Insight:** Attempted to shortcut proper keyframe representation; "simplicity" became "impossibility." Attempting to encode 3D rotation in 1D was too clever by half. Should have validated keyframe representation works before adding locomotion integration.

**Review:** Architectural issue discovered during Step 4 GUI implementation review, not during principle review. See `PLANS/ARCHIVE/20251007_220651_implementation_primary_skeletal_animation.md` header for detailed technical analysis.

**Implementation Notes:** Steps 1-4 of original plan completed but non-functional due to data structure issue. Code preserved on branch `iteration/primary_skeletal_animation` for reference.
