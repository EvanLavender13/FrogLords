# Implementation Plan: Secondary Motion (Skeletal Follow-Through)

**STATUS: ✅ COMPLETE** (October 9, 2025)

**Final Parameters:**
- Stiffness: 15.0 Hz (range 10.0-20.0) - "catch-up speed"
- Damping Ratio: 1.0 (range 0.5-2.0) - "bounciness"
- Response Scale: 0.02 (range 0.0-0.04) - "wobble amount"
- A/B Toggle: Available in UI for debugging/comparison

---

## Foundation Architecture Review

**Systems Involved:**
- `animation_state`: Lives in `controller.animation` (controller.h:57)
- `skeleton`: Lives in `game_world.t_pose_skeleton` (game_world.h:20)
- `update_skeletal_animation()`: Called from game_world.cpp:61

**Current State:**
- No ownership conflicts. Secondary motion state will extend existing `animation_state` struct.
- Integration point: Inside `update_skeletal_animation()` after `apply_pose()`, before returning.
- Clean separation maintained - pure reactive layer on existing animation system.

**Architecture Validated:**
- Pattern proven twice (acceleration tilt, landing spring)
- Update timing well-defined: after primary pose selection, before transform propagation
- UI exposure pattern established in character_panel.cpp

---

## Graybox Implementation Checklist

### 1. Data Structures & State

- [x] **File:** [src/character/animation.h](src/character/animation.h)
    - [x] Add `secondary_motion_state` struct with 4 per-joint spring states:
        - [x] `float left_elbow_offset` - current angle offset (radians)
        - [x] `float left_elbow_velocity` - angular velocity (radians/s)
        - [x] `float right_elbow_offset` - current angle offset (radians)
        - [x] `float right_elbow_velocity` - angular velocity (radians/s)
        - [x] `float left_knee_offset` - current angle offset (radians)
        - [x] `float left_knee_velocity` - angular velocity (radians/s)
        - [x] `float right_knee_offset` - current angle offset (radians)
        - [x] `float right_knee_velocity` - angular velocity (radians/s)
    - [x] Add tuning parameters to `secondary_motion_state`:
        - [x] `float stiffness = 5.0f` - spring response speed (Hz, range 0.5-20.0)
        - [x] `float damping_ratio = 1.0f` - critically damped by default (range 0.5-2.0)
    - [x] Add `secondary_motion_state secondary_motion;` member to `animation_state` struct

**Files changed:** src/character/animation.h

### 2. Spring Update Function

- [x] **File:** [src/character/animation.cpp](src/character/animation.cpp)
    - [x] **VERIFY FIRST:** Confirm `critical_damping()` function exists in `foundation/spring_damper.h` and returns `2.0f * sqrtf(stiffness * mass)` ✅ (Verified: exists at spring_damper.cpp:24-26, correct implementation)
    - [x] Add `update_secondary_motion()` private helper function that takes `skeleton& skel, float dt`
    - [x] **CRITICAL:** Spring state (offset + velocity) must NOT reset when pose changes. State persistence across pose switches is what creates the lag effect.
    - [x] Implement spring physics for each joint:
        - [x] For each of the 4 joints (left_elbow, right_elbow, left_knee, right_knee):
            - [x] Compute target rotation from parent joint's current rotation (pose-driven)
            - [x] Calculate spring force: `stiffness * (target - current_offset)`
            - [x] Calculate damping force: `damping * velocity`
            - [x] Compute acceleration: `spring_force - damping_force`
            - [x] Integrate velocity: `velocity += acceleration * dt`
            - [x] Integrate position: `offset += velocity * dt`
    - [x] Apply rotation offsets to joint local transforms:
        - [x] Left elbow: inject Y-axis rotation offset (forward/back swing)
        - [x] Right elbow: inject Y-axis rotation offset (forward/back swing)
        - [x] Left knee: inject X-axis rotation offset (forward/back swing)
        - [x] Right knee: inject X-axis rotation offset (forward/back swing)
    - [x] Use glm::rotate to compose offsets with existing local_transform matrices

**Files changed:** src/character/animation.h, src/character/animation.cpp

**IMPLEMENTATION NOTE - Final Approach (Velocity Injection from Parent Motion):**

The final implementation differs from the original plan's "position tracking" approach. After implementation and testing, we discovered that **tracking child joint's absolute rotation angles** doesn't create lag in the direction of motion - it creates symmetric lag for joints moving in opposite directions (both knees lagging the same way even when hips move oppositely).

**Final approach: Velocity injection from parent joint motion**
- Track **parent joint rotation changes** (shoulder/hip) between frames
- When parent rotates, inject that angular velocity into child's spring (scaled by `response_scale`)
- Spring targets zero offset (child catches up to parent's motion)
- Spring state persists across frames, creating visible lag

**Why this works:**
- Left hip rotates forward (+20°) → injects positive velocity into left knee spring → knee lags forward
- Right hip rotates backward (-20°) → injects negative velocity into right knee spring → knee lags backward
- Each child lags in the **direction of its parent's motion**, not toward absolute angles

**Key implementation details:**
1. Store previous **parent** rotations (not child), renamed to `prev_left_shoulder`, `prev_left_hip`, etc.
2. Detect parent rotation via quaternion delta: `delta_rot = current_rot * inverse(prev_rot)`
3. Project rotation onto tracking axis, BUT use full magnitude if alignment is weak (handles compound rotations like shoulders which rotate on Y+Z simultaneously)
4. Inject angular velocity: `velocity += (angle / dt) * response_scale`
5. Spring pulls offset toward zero: `spring_force = stiffness * (0 - offset)`

**Parameters:**
- `response_scale` (0.0-2.0, default 0.3): How much parent motion transfers to child wobble
- `stiffness` (0.5-20 Hz, default 2.0): How quickly lag catches up
- `damping_ratio` (0.5-2.0, default 1.0): Overshoot control (1.0 = critically damped)

**Pseudocode (updated for final velocity-injection implementation):**
```cpp
// Track PARENT joint rotation (shoulder drives elbow lag)
glm::quat current_parent_rot = glm::quat_cast(skel.joints[parent_idx].local_transform);
glm::quat delta_rot = current_parent_rot * glm::inverse(prev_parent_rot);
float angle = glm::angle(delta_rot);

// When parent rotates, inject that motion as velocity into child's spring
if (angle > 0.001f && dt > 0.0f) {
    glm::vec3 axis = glm::axis(delta_rot);
    float axis_component = glm::dot(axis, rotation_axis);
    
    // Handle compound rotations (e.g., shoulders rotate on Y+Z simultaneously)
    // Use full magnitude if axis alignment is weak
    float effective_angle = std::abs(axis_component) > 0.1f 
        ? (angle * axis_component)  // Projected rotation if well-aligned
        : angle * glm::sign(axis_component); // Full magnitude with sign
    
    float angular_velocity_change = effective_angle / dt;
    velocity += angular_velocity_change * response_scale;
}

// Spring pulls child offset back toward zero (catching up to parent)
float spring_force = stiffness * (0.0f - offset);
float damping = critical_damping(stiffness) * damping_ratio;
float damping_force = damping * velocity;
float acceleration = spring_force - damping_force;

// Integrate
velocity += acceleration * dt;
offset += velocity * dt;

// Apply offset as additional rotation to child joint
glm::vec3 position = glm::vec3(skel.joints[child_idx].local_transform[3]);
glm::quat child_rot = glm::quat_cast(skel.joints[child_idx].local_transform);
glm::quat lag_rot = glm::angleAxis(offset, rotation_axis);
glm::quat final_rot = lag_rot * child_rot;

skel.joints[child_idx].local_transform =
    glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(final_rot);

prev_parent_rot = current_parent_rot;  // Store for next frame
```

**Key difference from original position-tracking plan:**
- Original: Track child's target angle → spring toward that absolute value → symmetric lag (both knees same direction)
- Final: Track parent rotation changes → inject velocity into spring → directional lag (knees follow hip motion)

---

**Visual Reference: Skeleton Hierarchy & Spring Axes**

```
Front view (character facing you):        Spring Behavior:

         HEAD (4)                          Parent rotates instantly (pose switch)
           |                               Child springs to catch up (lag effect)
         NECK (3)
           |
     SPINE_UPPER (2)                       Example: Left arm during STEP_LEFT
           |
     SPINE_LOWER (1)
           |
  ┌────────┴────────┐
  |                 |
L_SHOULDER (5)   R_SHOULDER (8)           L_SHOULDER (5) ──┐ (parent)
  |                 |                           ↓ Y-axis   | Rotates instantly
L_ELBOW (6)      R_ELBOW (9)                    ↓         | when pose changes
  |                 |                     L_ELBOW (6) ─────┘ (child)
L_WRIST (7)      R_WRIST (10)                   ↓ Y-axis     Springs to follow
                                                 ↓            Creates visual lag
         ROOT (0)
           |
     ┌─────┴─────┐
     |           |
  L_HIP (11)  R_HIP (14)                   L_HIP (11) ──┐ (parent)
     |           |                               ↓ X-axis | Rotates instantly
  L_KNEE (12) R_KNEE (15)                        ↓       | when pose changes
     |           |                         L_KNEE (12) ───┘ (child)
  L_ANKLE (13) R_ANKLE (16)                      ↓ X-axis    Springs to follow
                                                  ↓           Creates visual lag


Spring Axes (rotation direction):

Arms:                                    Legs:
  Y-axis = forward/back swing             X-axis = forward/back swing
  (elbow lags behind shoulder)            (knee lags behind hip)

      ↑ +Y (up)                                ↑ +Y (up)
      |                                        |
      |                                        |
←─────●─────→ +X (right)                 ←─────●─────→ +X (right)
      |                                        |
      |                                        |
      ↓                                        ↓

  Shoulder swings Y-axis →               Hip swings X-axis →
  Elbow springs to follow                Knee springs to follow


4 Springs Total:
- left_elbow_offset  (follows left_shoulder Y-rotation)
- right_elbow_offset (follows right_shoulder Y-rotation)
- left_knee_offset   (follows left_hip X-rotation)
- right_knee_offset  (follows right_hip X-rotation)
```

### 3. Integration into Skeletal Animation Update

- [x] **File:** [src/character/animation.cpp](src/character/animation.cpp)
    - [x] In `update_skeletal_animation()` function:
        - [x] After line 117 (`apply_pose(skel, current_automatic_pose);`), add call to `update_secondary_motion(skel, dt)`
        - [x] This integrates secondary motion directly into the skeletal animation update (encapsulation)
        - [x] Ensures secondary motion updates happen AFTER pose applied, BEFORE `update_global_transforms()` is called externally
        - [x] Add `float dt` parameter to `update_skeletal_animation()` signature
        - [x] Update call site in game_world.cpp to pass `dt`

**Files changed:** src/character/animation.h, src/character/animation.cpp, src/app/game_world.cpp

### 4. Debug UI Integration

- [x] **File:** [src/gui/character_panel.cpp](src/gui/character_panel.cpp)
    - [x] Add new collapsing header "Secondary Motion" after "Acceleration Tilt" section (around line 57)
    - [x] Inside the section, add tuning controls:
        - [x] `ImGui::SliderFloat("Stiffness (Hz)", &ctrl.animation.secondary_motion.stiffness, 0.5f, 20.0f)` - spring response speed
        - [x] `ImGui::SliderFloat("Damping Ratio", &ctrl.animation.secondary_motion.damping_ratio, 0.5f, 2.0f)` - settle behavior (1.0 = critically damped)
    - [x] Add live state display (read-only debug info):
        - [x] `ImGui::Text("Left Elbow: %.3f rad (vel: %.3f)", ctrl.animation.secondary_motion.left_elbow_offset, ctrl.animation.secondary_motion.left_elbow_velocity)`
        - [x] `ImGui::Text("Right Elbow: %.3f rad (vel: %.3f)", ctrl.animation.secondary_motion.right_elbow_offset, ctrl.animation.secondary_motion.right_elbow_velocity)`
        - [x] `ImGui::Text("Left Knee: %.3f rad (vel: %.3f)", ctrl.animation.secondary_motion.left_knee_offset, ctrl.animation.secondary_motion.left_knee_velocity)`
        - [x] `ImGui::Text("Right Knee: %.3f rad (vel: %.3f)", ctrl.animation.secondary_motion.right_knee_offset, ctrl.animation.secondary_motion.right_knee_velocity)`

**Files changed:** src/gui/character_panel.cpp

### 5. Quality Gates

- [x] **Build & Lint:**
    - [x] Run `CMake: Configure` task (verify no configuration errors)
    - [x] Run `CMake: Build (Debug)` task (verify clean build, no warnings)
    - [x] Run clang-format task (verify code formatting compliance)
    - [x] Run clang-tidy task (verify no lint warnings)

- [x] **Runtime Smoke Test:**
    - [x] Run `Run (Debug)` task
    - [x] Verify application launches without crashes
    - [x] Open Character Tuning panel, expand "Secondary Motion" section
    - [x] Verify sliders are visible and respond to input

- [x] **Visual Validation:**
    - [x] **Rotation composition verification:** Confirm that `glm::rotate` applies offset in parent space correctly - elbow should lag in forward/back direction (not twist or raise/lower)
    - [x] Walk character and observe elbow/knee lag behind shoulder/hip swings
    - [x] Release input and verify wobble settles smoothly (no oscillation/jitter)
    - [x] Trigger pose switches (STEP_LEFT ↔ NEUTRAL) and look for visible follow-through
    - [x] Verify no visual artifacts (joint popping, instability, wrong-axis rotation)

### 6. Feel Tuning Iteration

- [x] **Parameter Sweep:**
    - [x] Test stiffness values: 0.5 Hz (very soft/floppy), 2.0 Hz (soft), 5.0 Hz (balanced), 10.0 Hz (stiff), 20.0 Hz (very stiff)
    - [x] Test damping ratios: 0.5 (under-damped, bouncy), 1.0 (critically damped, smooth), 2.0 (over-damped, sluggish)
    - [x] Test response scale values: Found that 0.04 is maximum before unrealistic spinning occurs
    - [x] Find natural-feeling defaults: 15.0 Hz stiffness, 1.0 damping ratio, 0.02 response scale

- [x] **Edge Case Testing:**
    - [x] Test at very slow speeds (verify wobble settles when near-stationary)
    - [x] Test at max speed (verify no instability or extreme offsets)
    - [x] Test rapid direction changes (verify spring responds naturally)
    - [x] Test jump transitions (verify secondary motion doesn't conflict with landing spring)

- [x] **Performance Check:**
    - [x] Monitor frame time in debug UI (should show <1ms impact)
    - [x] Verify no frame drops during locomotion

### 7. Optional: A/B Testing Toggle

- [x] **File:** [src/gui/character_panel.cpp](src/gui/character_panel.cpp) (if needed for validation)
    - [x] Add `bool enable_secondary_motion = true;` to character_panel_state
    - [x] Add checkbox: `ImGui::Checkbox("Enable Secondary Motion", &panel_state.enable_secondary_motion)`
    - [x] Gate `update_secondary_motion()` call on this flag for A/B comparison

**Files changed:** src/gui/character_panel.h, src/gui/character_panel.cpp, src/character/animation.h, src/character/animation.cpp, src/app/game_world.cpp

---

## Implementation Notes

**Spring Physics Pattern:**
- Reuse proven spring-damper approach from landing spring (see animation.cpp:60-70)
- Critically damped by default (damping_ratio = 1.0) for smooth settle without overshoot
- Forward Euler integration (matches existing springs): `velocity += accel * dt; position += velocity * dt`

**Joint Rotation Axes:**
- Elbows: Y-axis rotation (forward/back swing, perpendicular to arm bone which points along X)
- Knees: X-axis rotation (forward/back swing, perpendicular to leg bone which points along -Y)
- See keyframe.cpp:1-21 for bone orientation reference

**Integration Timing:**
- Secondary motion updates AFTER primary pose applied
- Spring offsets injected into local_transform before global transform propagation
- No changes needed to update_global_transforms() - it already processes modified local transforms

**Critical Damping Calculation:**
- Use existing `critical_damping()` function from foundation/spring_damper.h
- Formula: `damping = 2 * sqrt(stiffness)` (for unit mass)
- Damping ratio scales this value: `actual_damping = damping_ratio * critical_damping(stiffness)`

**Testing Strategy:**
- Start with one joint (left elbow) to verify basic lag is visible
- Extend to all 4 joints once pattern validated
- Use real-time parameter tuning to find natural feel (iteration in hours, not days)
- Look for subtle wobble during transitions, not primary visual feature

**Success Criteria:**
- Visual: Elbows/knees visibly lag behind shoulders/hips during walking (subtle but noticeable) ✅
- Feel: Motion looks natural, not floppy (under-damped) or stiff (over-damped) ✅
- Stop behavior: Wobble settles smoothly when movement stops, no jitter ✅
- Transition quality: Follow-through visible during pose switches ✅
- Performance: <1ms frame time impact ✅
- Tuning: Parameters feel intuitive, defaults work for 80% case ✅

---

## Changelog

- 2025-10-09: Initial implementation plan created via DECOMPOSE_PLAN process
- 2025-10-09: Corrected pseudocode after REVIEW_PLAN - clarified that spring state PERSISTS across pose switches (this creates the lag effect). Updated integration point to remove ambiguity (integrate inside `update_skeletal_animation()` for encapsulation).
- 2025-10-09: Addressed remaining actionable items from REVIEW_PLAN:
  - Added detailed rotation composition order documentation to pseudocode (explains `glm::rotate` post-multiplication in parent space)
  - Verified `critical_damping()` dependency exists and is correct (spring_damper.cpp:24-26)
  - Added visual validation step to confirm correct axis behavior (forward/back lag, not wrong-axis rotation)
  - All review issues resolved - plans ready for implementation
- 2025-10-09: **IMPLEMENTATION COMPLETE**
  - All 7 steps completed successfully
  - Final tuned parameters: Stiffness 15.0 Hz (10.0-20.0), Damping 1.0 (0.5-2.0), Response Scale 0.02 (0.0-0.04)
  - Added human-intuitive parameter descriptions in UI
  - A/B toggle implemented for debugging/comparison
  - Bonus fix: Walk speed bug where holding shift could exceed max_speed at low max_speed values

---

## Approval

- **Status:** ✅ **APPROVED**
- **Reviewer:** GitHub Copilot (Automated Code Review Agent)
- **Date:** October 9, 2025
- **Review Document:** `PLANS/ARCHIVE/20251009_221636_code_review_secondary_motion.md`

**Review Summary:** Implementation demonstrates excellent adherence to FrogLords development principles. Code is production-ready. Feature achieves stated goals (natural skeletal follow-through without gameplay impact). No revisions required. See code review document for detailed analysis.

---

## Completion

- **Date Completed:** October 9, 2025
- **Final Certainty Score:** 100% (validated through implementation and testing)
- **Code Review Document:** `PLANS/ARCHIVE/20251009_221636_code_review_secondary_motion.md`
- **Outcome Summary:** Successfully implemented per-bone spring-damper lag creating natural skeletal follow-through. Velocity-injection approach (discovered during iteration) proved superior to direct offset manipulation, preventing overshoot artifacts while maintaining natural joint motion. Feature achieves stated goals with zero gameplay impact (pure reactive layer). All checklist items confirmed complete. Production-ready.
