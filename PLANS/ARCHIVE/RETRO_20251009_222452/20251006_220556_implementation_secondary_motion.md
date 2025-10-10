# Implementation Plan: Secondary Motion

### Graybox Implementation Checklist

#### 1. Data Structures & State

- [x] **File:** `src/character/secondary_motion.h`
    - [x] Create new header file with include guards and namespace `character`.
    - [x] Include required headers: `<glm/glm.hpp>`, `<glm/gtc/quaternion.hpp>`, `<vector>`, `"foundation/spring_damper.h"`, `"skeleton.h"`.
    - [x] Define `struct joint_spring_state` with members: `glm::quat angular_offset` (spring-driven rotation offset from parent), `glm::quat angular_velocity` (angular velocity for spring), `spring_damper rotation_spring` (1D spring for rotation magnitude).
    - [x] Define `struct secondary_motion_state` with members: `std::vector<joint_spring_state> joint_springs` (per-joint spring state), `bool enabled = false` (master toggle).
    - [x] Define `struct secondary_motion_config` with per-bone-type softness parameters: `float spine_softness = 0.3f`, `float upper_arm_softness = 0.5f`, `float forearm_softness = 0.6f`, `float thigh_softness = 0.4f`, `float shin_softness = 0.5f`, `float global_damping_ratio = 1.0f` (critical damping multiplier).
    - [x] Add function declaration: `void init_secondary_motion(secondary_motion_state& state, const skeleton& skel, const secondary_motion_config& config)`.
    - [x] Add function declaration: `void update_secondary_motion(skeleton& skel, secondary_motion_state& state, const secondary_motion_config& config, float dt)`.
    - [x] Add helper function declaration: `float get_bone_softness(const char* joint_name, const secondary_motion_config& config)` (maps joint name to appropriate softness parameter).

- [x] **File:** `src/character/secondary_motion.cpp`
    - [x] Create implementation file with include `"secondary_motion.h"`.
    - [x] Include additional headers: `<cstring>`, `<cmath>`, `<glm/gtx/quaternion.hpp>`.
    - [x] Implement `get_bone_softness()`: use `strstr()` to match joint name substrings ("spine" → `spine_softness`, "shoulder"/"elbow"/"wrist" → `upper_arm_softness`/`forearm_softness`, "hip"/"knee"/"ankle" → `thigh_softness`/`shin_softness`). Default to 0.0f (rigid) for unmatched joints (root, neck, head).
    - [x] Implement `init_secondary_motion()`: resize `joint_springs` to match skeleton joint count; for each joint, initialize spring state with `angular_offset = identity quaternion`, `angular_velocity = zero quaternion`, configure `rotation_spring` stiffness based on softness (convert softness [0,1] to stiffness range [0, 200]), set damping using `critical_damping(stiffness)` multiplied by `global_damping_ratio`.
    - [x] Implement `update_secondary_motion()`: skip if `!state.enabled`; for each joint (starting from index 1, skip root), compute angular velocity from change in parent's model_transform (extract rotation, compute delta from previous frame stored in spring state), apply spring force to `angular_offset` using `rotation_spring.update()`, convert spring offset to quaternion rotation, apply offset to joint's `local_transform` (multiply local rotation by offset quaternion), store current rotation for next frame's velocity calculation.

**Files Changed:**
- `src/character/secondary_motion.h` (created)
- `src/character/secondary_motion.cpp` (created)

#### 2. Integration into Animation System

- [x] **File:** `src/character/animation.h`
    - [x] Add include: `"secondary_motion.h"`.
    - [x] Add member to `animation_state` struct: `secondary_motion_state secondary_motion`.
    - [x] Add member to `animation_state` struct: `secondary_motion_config secondary_motion_config`.
    - [x] Add method declaration: `void init_secondary_motion(const skeleton& skel)` (initializes secondary motion state).
    - [x] Add method declaration: `void update_secondary_motion(skeleton& skel, float dt)` (wrapper that calls `character::update_secondary_motion()`).

- [x] **File:** `src/character/animation.cpp`
    - [x] Implement `animation_state::init_secondary_motion()`: call `character::init_secondary_motion(secondary_motion, skel, secondary_motion_config)`.
    - [x] Implement `animation_state::update_secondary_motion()`: call `character::update_secondary_motion(skel, secondary_motion, secondary_motion_config, dt)`.

**Files Changed:**
- `src/character/animation.h` (modified)
- `src/character/animation.cpp` (modified)

#### 3. Integration into Game World Update Loop

- [x] **File:** `src/app/game_world.h`
    - [x] Verify `game_world` struct has `skeleton t_pose_skeleton` member (already exists).

- [x] **File:** `src/app/game_world.cpp`
    - [x] In `game_world::init()`, after `create_t_pose(t_pose_skeleton)` and `character::update_global_transforms(t_pose_skeleton)`, add call: `character.animation.init_secondary_motion(t_pose_skeleton)`.
    - [x] In `game_world::update()`, after existing animation updates (`update_landing_spring`, `update_acceleration_tilt`) and before the conditional skeleton animation block (`if (panel_state.animate_skeleton)`), add call: `character.animation.update_secondary_motion(t_pose_skeleton, dt)`.
    - [x] In `game_world::update()`, after `character.animation.update_secondary_motion()`, add call: `character::update_global_transforms(t_pose_skeleton)` to recompute global transforms with secondary motion applied.
    - [x] Move the existing `character::update_global_transforms(t_pose_skeleton)` call inside the `if (panel_state.animate_skeleton)` block to the end (after local_transform modification).

**Files Changed:**
- `src/app/game_world.cpp` (modified)

#### 4. Debug UI Controls

- [x] **File:** `src/gui/character_panel.cpp`
    - [x] In `draw_character_panel()`, after the "Skeleton" collapsing header section, add new collapsing header: `if (ImGui::CollapsingHeader("Secondary Motion"))`.
    - [x] Inside "Secondary Motion" section, add checkbox: `gui::widget::checkbox("Enable Secondary Motion", &character.animation.secondary_motion.enabled)`.
    - [x] Add slider: `gui::widget::slider_float("Spine Softness", &character.animation.secondary_motion_config.spine_softness, 0.0f, 1.0f)`.
    - [x] Add slider: `gui::widget::slider_float("Upper Arm Softness", &character.animation.secondary_motion_config.upper_arm_softness, 0.0f, 1.0f)`.
    - [x] Add slider: `gui::widget::slider_float("Forearm Softness", &character.animation.secondary_motion_config.forearm_softness, 0.0f, 1.0f)`.
    - [x] Add slider: `gui::widget::slider_float("Thigh Softness", &character.animation.secondary_motion_config.thigh_softness, 0.0f, 1.0f)`.
    - [x] Add slider: `gui::widget::slider_float("Shin Softness", &character.animation.secondary_motion_config.shin_softness, 0.0f, 1.0f)`.
    - [x] Add slider: `gui::widget::slider_float("Damping Ratio", &character.animation.secondary_motion_config.global_damping_ratio, 0.5f, 2.0f)`.
    - [x] Add read-only state display: compute and display max angular velocity across all joints using a loop over `character.animation.secondary_motion.joint_springs`, then `gui::widget::text("Max Angular Velocity: %.3f rad/s", max_angular_vel)`.
    - [x] Add button: `if (ImGui::Button("Reinitialize Springs"))` that calls `character.animation.init_secondary_motion(world.t_pose_skeleton)` (need to pass `world` reference or skeleton to panel function).

- [x] **File:** `src/gui/character_panel.h`
    - [x] Update `draw_character_panel()` signature to accept `skeleton& skel` parameter (needed for reinitialize button).

- [x] **File:** `src/app/runtime.cpp`
    - [x] Update call to `gui::draw_character_panel()` to pass `world.t_pose_skeleton` as additional argument.

**Files Changed:**
- `src/gui/character_panel.h` (modified)
- `src/gui/character_panel.cpp` (modified)
- `src/app/runtime.cpp` (modified)

#### 5. CMake Build System

- [x] **File:** `CMakeLists.txt`
    - [x] Verify that `src/character/secondary_motion.cpp` is included in source file list (or that wildcard pattern `src/character/*.cpp` includes it).
    - [x] If explicit file list, add `src/character/secondary_motion.cpp` to the appropriate target sources.

**Files Changed:**
- `CMakeLists.txt` (modified)

#### 6. Quality Gates

- [ ] **Build:** Run task "CMake: Configure (Debug)" to regenerate build files.
- [ ] **Build:** Run task "CMake: Build (Debug)" and verify no compile errors.
- [ ] **Format:** Run task "Format: clang-format src" to ensure code style consistency.
- [ ] **Lint:** Run task "Lint: clang-tidy src" and address any warnings.
- [ ] **Runtime:** Run task "Run (Debug)" and verify application launches without crashes.
- [ ] **Smoke Test:** In GUI, navigate to "Secondary Motion" section, toggle "Enable Secondary Motion", adjust softness sliders, observe skeleton joints wobble during character movement (walk/run/jump/turn).
- [ ] **Performance:** Verify frame time impact is negligible (<0.5ms increase when enabled with 17 joints).

---

### Implementation Notes

**Spring Implementation Strategy:**
- Use rotational springs (quaternion-based angular offsets) rather than positional springs.
- Store spring state per joint: current angular offset, angular velocity, 1D spring for magnitude.
- Compute parent angular velocity from frame-to-frame rotation delta.
- Apply spring force proportional to parent motion (faster parent motion → larger offset).
- Convert spring magnitude to quaternion rotation around cross product of parent velocity and up axis.

**Bone Category Mapping:**
- **Spine:** "spine_lower", "spine_upper" → `spine_softness`
- **Upper Arm:** "left_shoulder", "right_shoulder", "left_elbow", "right_elbow" → `upper_arm_softness`, `forearm_softness`
- **Legs:** "left_hip", "right_hip", "left_knee", "right_knee", "left_ankle", "right_ankle" → `thigh_softness`, `shin_softness`
- **Rigid (no wobble):** "root", "neck", "head" → softness = 0.0f

**Tuning Starting Values:**
- Start with all softness at 0.0 (disabled), test each bone type independently.
- Expected converged values (from biomechanics):
  - Arms: ~0.5-0.7 (most visible, high amplitude)
  - Legs: ~0.3-0.5 (more constrained by locomotion)
  - Spine: ~0.2-0.4 (subtle, affects posture)
- Damping ratio: start at 1.0 (critical damping), increase if wobble too pronounced.

**Update Timing:**
- **After:** Character physics, orientation, landing spring, acceleration tilt
- **Before:** Final skeleton global transform computation, rendering
- **Pattern:** Match existing reactive animation layers (landing spring, acceleration tilt)

**Transform Application:**
- Modify `joint.local_transform` by multiplying with spring-driven quaternion offset.
- Do not modify skeleton data structure—only runtime computed transforms.
- Recompute `model_transform` via `update_global_transforms()` after applying offsets.

**Interruptibility:**
- Secondary motion is purely reactive—no control flow blocking.
- Springs update every frame; no discrete state transitions.
- Can toggle on/off instantly via `enabled` flag.

**Fallback/Safety:**
- If spring becomes unstable (magnitude > threshold), clamp or reset.
- Master toggle allows instant disable if visual artifacts occur.
- Per-bone softness at 0.0 effectively disables that bone's spring (no overhead).

---

### Testing Protocol

**Self-Test Checklist (2-4 hours):**
1. Build and launch with secondary motion disabled—verify no regression.
2. Enable secondary motion, set all softness to 0.5, walk in circles—observe subtle limb lag.
3. Jump while moving forward—arms should extend during flight, swing back on landing.
4. Rapid direction changes (zigzag running)—arms trail behind body rotation.
5. Come to sudden stop—limbs should settle naturally over ~0.5 seconds.
6. Tune individual bone types (arms first, then legs, then spine)—find natural feel.
7. Test extreme values (softness = 0.0 and 1.0)—verify no crashes or visual artifacts.
8. Profile frame time—verify <0.5ms impact.

**Over-the-Shoulder Tests (6-8 participants):**
- **Setup:** Character moving in test level, secondary motion enabled with tuned parameters.
- **Observation:** Watch participants play for 2-3 minutes (walk, run, jump, explore).
- **Key Question:** "Describe how the character's movement feels."
- **Look For:** Unprompted mentions of "natural," "organic," "alive," "bouncy" (positive signals) vs. "weird," "floaty," "delayed," "mushy" (negative signals).
- **Success:** Testers don't consciously notice wobble OR describe it positively as "more lifelike."

**Metrics:**
- Frame time delta (enabled vs. disabled): target <0.5ms, acceptable <1.0ms.
- Visual artifact count: zero tolerance for pops, flips, or extreme poses.
- Parameter convergence: same values feel good across multiple tuning sessions.

---

### Risks & Mitigations

**Risk: Tuning Difficulty**
- *Symptom:* Can't find natural feel after many iterations.
- *Mitigation:* Start from biomechanics reference (arm ~2Hz natural frequency → stiffness ~150). Use presets ("tight", "loose") as starting points.

**Risk: Visual Artifacts (overshoot/instability)**
- *Symptom:* Limbs flip, snap, or overshoot into unnatural poses.
- *Mitigation:* Use critical damping by default (`damping_ratio = 1.0`). Add angle clamps if needed (defer to backlog unless necessary). Test extreme velocities (sprinting, falling).

**Risk: Performance Impact**
- *Symptom:* Frame time increases noticeably with secondary motion enabled.
- *Mitigation:* Profile early. Springs are cheap (2 multiply-adds per joint per frame). 17 joints * 60fps = ~1000 updates/sec; should be <0.1ms. If costly, optimize quaternion ops or reduce update frequency.

**Risk: Interaction with Future IK**
- *Symptom:* Secondary motion fights with IK constraints (e.g., hand locked to object).
- *Mitigation:* Design for IK override—apply secondary motion only to non-IK joints. Add per-joint `is_ik_controlled` flag to skip spring update. Defer implementation until IK system added.

---

### Changelog

- **2025-10-06:** Initial implementation plan decomposed from `iteration_secondary_motion.md`.
- **2025-10-06:** Feature deferred after implementation attempts revealed missing prerequisite.

---

## Deferral

**Date:** 2025-10-06

**Reason:** Implementation revealed missing critical prerequisite: primary skeletal animation system. Secondary motion is a reactive animation layer that needs base skeletal motion to react to. Current static T-pose has no motion for springs to lag behind.

**Missing Prerequisites:**
-   Primary locomotion-driven skeletal animation (walk/run cycles with arm swing)
-   Distance-phased limb animation synchronized to surveyor wheel
-   Stable base skeletal poses for reactive offsets to layer upon

### Implementation Attempts

**Attempt 1: Parent Rotation Lag (Initial Plan)**
- **Approach:** Measure frame-to-frame rotation changes in parent joints. Use spring to drive lag offset behind parent motion.
- **Implementation:** Created `joint_spring_state` with quaternion tracking. Computed rotation delta, extracted axis-angle, drove 1D spring with angular velocity magnitude.
- **Failure:** T-pose joints don't rotate between frames—skeleton is static. Only character controller position changes; joint local transforms remain constant. No parent motion exists to lag behind.
- **Code artifacts:** `secondary_motion.h/cpp` with quaternion-based spring state (reverted)

**Attempt 2: Acceleration-Driven Wobble**
- **Approach:** Drive joint rotations directly from character acceleration and turning. Forward/back accel → pitch wobble, lateral accel + turning → roll wobble. Springs track rotation angles in local space.
- **Implementation:** Rewrote `joint_spring_state` with two 1D springs (swing_x, swing_z). Transformed character acceleration to local space, computed pitch/roll targets, applied quaternion offsets to local transforms.
- **Failure:** Produced wild unstable spinning. Springs drove absolute rotations rather than subtle offsets. Without base animation pose, springs had no "rest position" to offset from—they were creating primary motion, not secondary motion.
- **Code artifacts:** `secondary_motion.h/cpp` with dual-spring approach (reverted)

**Attempt 3: Rotation Lag with Slerp**
- **Approach:** Simplified rotation lag using slerp between identity and inverse rotation delta, driven by scalar spring.
- **Failure:** Same root cause—no rotation delta exists on static T-pose. Spring tracked zero lag because there was nothing to lag behind.
- **Code artifacts:** (reverted during Attempt 2)

### Key Technical Insights

1. **Static data ≠ animation:** Having a skeleton data structure (`character::skeleton`) is insufficient. Reactive animation layers require **motion** in that data (changing joint transforms), not just the existence of the data.

2. **Reactive layers need primary layers:** Per AGENTS.md procedural animation principles, secondary motion sits atop primary animation. The dependency isn't just "skeleton exists" but "skeleton is animating." Current T-pose provides no base animation.

3. **Spring rest positions matter:** Secondary motion springs need a base pose to offset from. Without primary animation providing that base, springs either do nothing (lag from zero) or create unstable primary motion (acceleration-driven absolute rotations).

4. **Dependency analysis gaps:** Initial certainty score (85%) missed that "skeleton system" dependency meant "animated skeleton system." Checklist verification (✅ skeleton exists) was insufficient—should have verified skeleton **motion** exists.

### Code State After Deferral

All secondary motion code reverted:
- Removed `src/character/secondary_motion.h/cpp`
- Removed integration from `animation.h/cpp`
- Removed integration from `game_world.cpp`
- Removed UI controls from `character_panel.cpp`
- No artifacts remain; clean return to pre-implementation state

### Reconsideration Criteria

Implement after building primary skeletal animation system:
1. **Distance-phased limb animation:** Arm swing and leg movement synchronized to locomotion (surveyor wheel phase)
2. **Walk/run animation blending:** Pose interpolation based on speed
3. **Stable base transforms:** Joints have predictable local transforms that change smoothly each frame

Once joints are animating (moving predictably through poses), secondary motion will work as designed—springs will lag behind that motion, creating organic wobble.

**Estimated effort after prerequisite:** 2-4 hours (original estimate still valid once foundation exists)
