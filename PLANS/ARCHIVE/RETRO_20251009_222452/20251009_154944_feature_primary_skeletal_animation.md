# Primary Skeletal Animation (Locomotion-Driven Pose Cycling)

**Core Mechanic:** Distance-phased pose switching synchronized to surveyor-wheel locomotion. Limbs snap between discrete walk poses (STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL → repeat) based on distance traveled, no blending.

**Pattern Reuse:** Extends proven surveyor-wheel distance-phased triggering from body bounce. Reuses validated `character::keyframe` quaternion architecture and existing walk poses from Static Keyframe Preview.

---

## Visual Reference

**Reference Images:** `NOTES/Images/walk*.png`, `NOTES/Images/gait.png`
- Walk cycle alternates legs forward/back with counter-swinging arms
- Gait diagram shows synchronized blend concept (walk ↔ run intensity)

**Defined Poses** (from `src/character/keyframe.cpp`):
- **STEP_LEFT:** Left leg forward (-20° hip pitch), right leg back (20° hip pitch), right arm forward (-45° shoulder yaw), left arm back (45° shoulder yaw)
- **NEUTRAL:** Arms down at sides (90° shoulder roll), legs straight (T-pose)
- **STEP_RIGHT:** Mirror of STEP_LEFT (right leg forward, left arm forward)

**Animated Joints:** 8 total
- Arms: left_shoulder, left_elbow, right_shoulder, right_elbow
- Legs: left_hip, left_knee, right_hip, right_knee

---

## Graybox Scope

**Distance-Phase Calculation:**
- `phase = (locomotion.distance_traveled % cycle_length) / cycle_length` (normalized 0.0-1.0)
- `cycle_length = 1.0m` (initial value, one full stride per meter traveled)

**Threshold-Based Pose Selection:**
- `0.0 ≤ phase < 0.25` → STEP_LEFT
- `0.25 ≤ phase < 0.5` → NEUTRAL
- `0.5 ≤ phase < 0.75` → STEP_RIGHT
- `0.75 ≤ phase < 1.0` → NEUTRAL

**Implementation:**
- Hook into `character::animation::update()` after locomotion distance updates
- Calculate phase, select pose via thresholds, call `apply_pose(skeleton, selected_pose)`
- Expose `cycle_length` as tunable parameter in animation UI

**Stop Behavior:**
- When `distance_traveled` stops increasing, pose freezes on current state (no snap to NEUTRAL)
- No special handling needed (distance stops → phase stops → same pose selected)

**Backward Movement:** Out of scope (character cannot walk backwards currently)

**GUI Integration:** Keep manual pose selection UI active but secondary to automatic cycling (useful for debugging individual poses)

**Estimated Code:** ~30-40 lines

**Files Modified:**
- `src/character/animation.cpp` (add distance-phase logic to `update()`)
- `src/character/animation.h` (add `cycle_length` to animation state struct)

---

## Success Criteria

**Visual Tests:**
- Legs visually alternate (left-forward → neutral → right-forward → neutral pattern observable)
- Arms counter-swing opposite to legs
- Stopping mid-stride freezes pose (no pop/snap to neutral)

**Tuning Target:**
- `cycle_length` adjustable in real-time via UI
- "Correct" cycle length = subjective feel when tuning (foot contact timing looks natural)
- Initial 1.0m value provides starting point for iteration

**Validation:**
- Walk forward → poses cycle through STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL → repeat
- Stop walking → pose holds frozen (whatever phase was active)
- Adjust `cycle_length` slider → stride frequency changes immediately

---

## Unlocks

Immediate visual feedback for stride tuning (cycle length, phase timing). Once cycle feels right, enables natural next step: pose blending (lerp/slerp between keyframes). Validates distance-phased triggering before adding interpolation complexity.

---

## Certainty

~75%—simpler than original plan (no blending math). Only risk is finding the right cycle length values, but that's pure tuning with real-time feedback.
