# Implementation Plan: Secondary Motion (Skeletal Follow-Through)

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

- [ ] **File:** [src/character/animation.h](src/character/animation.h)
    - [ ] Add `secondary_motion_state` struct with 4 per-joint spring states:
        - [ ] `float left_elbow_offset` - current angle offset (radians)
        - [ ] `float left_elbow_velocity` - angular velocity (radians/s)
        - [ ] `float right_elbow_offset` - current angle offset (radians)
        - [ ] `float right_elbow_velocity` - angular velocity (radians/s)
        - [ ] `float left_knee_offset` - current angle offset (radians)
        - [ ] `float left_knee_velocity` - angular velocity (radians/s)
        - [ ] `float right_knee_offset` - current angle offset (radians)
        - [ ] `float right_knee_velocity` - angular velocity (radians/s)
    - [ ] Add tuning parameters to `secondary_motion_state`:
        - [ ] `float stiffness = 5.0f` - spring response speed (Hz, range 0.5-20.0)
        - [ ] `float damping_ratio = 1.0f` - critically damped by default (range 0.5-2.0)
    - [ ] Add `secondary_motion_state secondary_motion;` member to `animation_state` struct

### 2. Spring Update Function

- [ ] **File:** [src/character/animation.cpp](src/character/animation.cpp)
    - [ ] **VERIFY FIRST:** Confirm `critical_damping()` function exists in `foundation/spring_damper.h` and returns `2.0f * sqrtf(stiffness * mass)` ✅ (Verified: exists at spring_damper.cpp:24-26, correct implementation)
    - [ ] Add `update_secondary_motion()` private helper function that takes `skeleton& skel, float dt`
    - [ ] **CRITICAL:** Spring state (offset + velocity) must NOT reset when pose changes. State persistence across pose switches is what creates the lag effect.
    - [ ] Implement spring physics for each joint:
        - [ ] For each of the 4 joints (left_elbow, right_elbow, left_knee, right_knee):
            - [ ] Compute target rotation from parent joint's current rotation (pose-driven)
            - [ ] Calculate spring force: `stiffness * (target - current_offset)`
            - [ ] Calculate damping force: `damping * velocity`
            - [ ] Compute acceleration: `spring_force - damping_force`
            - [ ] Integrate velocity: `velocity += acceleration * dt`
            - [ ] Integrate position: `offset += velocity * dt`
    - [ ] Apply rotation offsets to joint local transforms:
        - [ ] Left elbow: inject Y-axis rotation offset (forward/back swing)
        - [ ] Right elbow: inject Y-axis rotation offset (forward/back swing)
        - [ ] Left knee: inject X-axis rotation offset (forward/back swing)
        - [ ] Right knee: inject X-axis rotation offset (forward/back swing)
    - [ ] Use glm::rotate to compose offsets with existing local_transform matrices

**Pseudocode for one joint (left elbow):**
```cpp
// CRITICAL: The pose has already been applied by apply_pose() - it sets target rotations
// Spring state (offset + velocity) PERSISTS across pose switches
// When pose changes, the non-zero spring state creates visible lag as it catches up

float target_offset = 0.0f;  // We want to reach the pose target (zero offset = at target)
float current_offset = secondary_motion.left_elbow_offset;  // Current lag behind target

// Spring physics drives offset toward zero (catching up to pose target)
float spring_force = secondary_motion.stiffness * (target_offset - current_offset);
float damping = critical_damping(secondary_motion.stiffness) * secondary_motion.damping_ratio;
float damping_force = damping * secondary_motion.left_elbow_velocity;
float acceleration = spring_force - damping_force;

// Integrate (spring state carries over between frames AND between pose switches)
secondary_motion.left_elbow_velocity += acceleration * dt;
secondary_motion.left_elbow_offset += secondary_motion.left_elbow_velocity * dt;

// Apply lagged offset to joint (negative because offset represents "behind target")
// When pose switches, offset is non-zero, creating visible lag as spring catches up
//
// Rotation composition order: glm::rotate applies the rotation IN PARENT SPACE
// Current transform is: translate(t_pose_pos) * mat4_cast(pose_quat)
// After glm::rotate: translate(t_pose_pos) * mat4_cast(pose_quat) * rotate(Y, offset)
// This post-multiplies the offset, applying it AFTER the pose rotation
//
// Why this is correct: The offset is applied in the joint's local space (which IS parent space
// after the pose rotation). For elbows, Y-axis in parent space = forward/back swing axis.
// This creates the visual lag we want: the entire joint (pose rotation + offset) lags behind.
skel.joints[joint_index::LEFT_ELBOW].local_transform =
    glm::rotate(skel.joints[joint_index::LEFT_ELBOW].local_transform,
                -secondary_motion.left_elbow_offset,  // Lag applied as negative offset
                glm::vec3(0, 1, 0));  // Y-axis rotation for elbow forward/back swing
```

**Key Insight:** Spring state persistence is what creates the lag effect:
- Frame 1: Elbow at NEUTRAL pose (offset ≈ 0)
- Frame 2: Pose switches to STEP_LEFT instantly; elbow still has old offset/velocity
- Frames 3+: Spring drives offset back to zero, making elbow "catch up" to new pose

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

- [ ] **File:** [src/character/animation.cpp](src/character/animation.cpp)
    - [ ] In `update_skeletal_animation()` function:
        - [ ] After line 117 (`apply_pose(skel, current_automatic_pose);`), add call to `update_secondary_motion(skel, dt)`
        - [ ] This integrates secondary motion directly into the skeletal animation update (encapsulation)
        - [ ] Ensures secondary motion updates happen AFTER pose applied, BEFORE `update_global_transforms()` is called externally
        - [ ] Add `float dt` parameter to `update_skeletal_animation()` signature
        - [ ] Update call site in game_world.cpp to pass `dt`

### 4. Debug UI Integration

- [ ] **File:** [src/gui/character_panel.cpp](src/gui/character_panel.cpp)
    - [ ] Add new collapsing header "Secondary Motion" after "Acceleration Tilt" section (around line 57)
    - [ ] Inside the section, add tuning controls:
        - [ ] `ImGui::SliderFloat("Stiffness (Hz)", &ctrl.animation.secondary_motion.stiffness, 0.5f, 20.0f)` - spring response speed
        - [ ] `ImGui::SliderFloat("Damping Ratio", &ctrl.animation.secondary_motion.damping_ratio, 0.5f, 2.0f)` - settle behavior (1.0 = critically damped)
    - [ ] Add live state display (read-only debug info):
        - [ ] `ImGui::Text("Left Elbow: %.3f rad (vel: %.3f)", ctrl.animation.secondary_motion.left_elbow_offset, ctrl.animation.secondary_motion.left_elbow_velocity)`
        - [ ] `ImGui::Text("Right Elbow: %.3f rad (vel: %.3f)", ctrl.animation.secondary_motion.right_elbow_offset, ctrl.animation.secondary_motion.right_elbow_velocity)`
        - [ ] `ImGui::Text("Left Knee: %.3f rad (vel: %.3f)", ctrl.animation.secondary_motion.left_knee_offset, ctrl.animation.secondary_motion.left_knee_velocity)`
        - [ ] `ImGui::Text("Right Knee: %.3f rad (vel: %.3f)", ctrl.animation.secondary_motion.right_knee_offset, ctrl.animation.secondary_motion.right_knee_velocity)`

### 5. Quality Gates

- [ ] **Build & Lint:**
    - [ ] Run `CMake: Configure` task (verify no configuration errors)
    - [ ] Run `CMake: Build (Debug)` task (verify clean build, no warnings)
    - [ ] Run clang-format task (verify code formatting compliance)
    - [ ] Run clang-tidy task (verify no lint warnings)

- [ ] **Runtime Smoke Test:**
    - [ ] Run `Run (Debug)` task
    - [ ] Verify application launches without crashes
    - [ ] Open Character Tuning panel, expand "Secondary Motion" section
    - [ ] Verify sliders are visible and respond to input

- [ ] **Visual Validation:**
    - [ ] **Rotation composition verification:** Confirm that `glm::rotate` applies offset in parent space correctly - elbow should lag in forward/back direction (not twist or raise/lower)
    - [ ] Walk character and observe elbow/knee lag behind shoulder/hip swings
    - [ ] Release input and verify wobble settles smoothly (no oscillation/jitter)
    - [ ] Trigger pose switches (STEP_LEFT ↔ NEUTRAL) and look for visible follow-through
    - [ ] Verify no visual artifacts (joint popping, instability, wrong-axis rotation)

### 6. Feel Tuning Iteration

- [ ] **Parameter Sweep:**
    - [ ] Test stiffness values: 0.5 Hz (very soft/floppy), 2.0 Hz (soft), 5.0 Hz (balanced), 10.0 Hz (stiff), 20.0 Hz (very stiff)
    - [ ] Test damping ratios: 0.5 (under-damped, bouncy), 1.0 (critically damped, smooth), 2.0 (over-damped, sluggish)
    - [ ] Find natural-feeling defaults (likely 5.0 Hz stiffness, 1.0 damping ratio)

- [ ] **Edge Case Testing:**
    - [ ] Test at very slow speeds (verify wobble settles when near-stationary)
    - [ ] Test at max speed (verify no instability or extreme offsets)
    - [ ] Test rapid direction changes (verify spring responds naturally)
    - [ ] Test jump transitions (verify secondary motion doesn't conflict with landing spring)

- [ ] **Performance Check:**
    - [ ] Monitor frame time in debug UI (should show <1ms impact)
    - [ ] Verify no frame drops during locomotion

### 7. Optional: A/B Testing Toggle

- [ ] **File:** [src/gui/character_panel.cpp](src/gui/character_panel.cpp) (if needed for validation)
    - [ ] Add `bool enable_secondary_motion = true;` to character_panel_state
    - [ ] Add checkbox: `ImGui::Checkbox("Enable Secondary Motion", &panel_state.enable_secondary_motion)`
    - [ ] Gate `update_secondary_motion()` call on this flag for A/B comparison

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
- Visual: Elbows/knees visibly lag behind shoulders/hips during walking (subtle but noticeable)
- Feel: Motion looks natural, not floppy (under-damped) or stiff (over-damped)
- Stop behavior: Wobble settles smoothly when movement stops, no jitter
- Transition quality: Follow-through visible during pose switches
- Performance: <1ms frame time impact
- Tuning: Parameters feel intuitive, defaults work for 80% case

---

## Changelog

- 2025-10-09: Initial implementation plan created via DECOMPOSE_PLAN process
- 2025-10-09: Corrected pseudocode after REVIEW_PLAN - clarified that spring state PERSISTS across pose switches (this creates the lag effect). Updated integration point to remove ambiguity (integrate inside `update_skeletal_animation()` for encapsulation).
- 2025-10-09: Addressed remaining actionable items from REVIEW_PLAN:
  - Added detailed rotation composition order documentation to pseudocode (explains `glm::rotate` post-multiplication in parent space)
  - Verified `critical_damping()` dependency exists and is correct (spring_damper.cpp:24-26)
  - Added visual validation step to confirm correct axis behavior (forward/back lag, not wrong-axis rotation)
  - All review issues resolved - plans ready for implementation
