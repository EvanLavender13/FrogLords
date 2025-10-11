# Implementation Plan: Pose Blending

**Branch:** `iteration/pose_blending`

**Goal:** Replace discrete pose threshold switching with continuous quaternion interpolation (slerp) to eliminate visible pops at phase boundaries.

**Scope:** 40-60 lines (net +30), single function modification + new keyframe getter.

---

## Foundation Architecture Review

**Systems Involved:**
- `animation_state` (owned by `controller.animation`)
- `skeleton` (owned by `game_world.t_pose_skeleton`, passed by reference)
- `keyframe` utilities (stateless functions)

**Current State:**
- `animation_state::update_skeletal_animation()`: [animation.cpp:110-144](../src/character/animation.cpp#L110-L144)
- Called from: [game_world.cpp:79-81](../src/app/game_world.cpp#L79-L81)
- Pose creation functions: [keyframe.cpp:84-143](../src/character/keyframe.cpp#L84-L143)

**Architecture Status:** Clean. No ownership conflicts. Modification isolated to animation system.

---

## Graybox Implementation Checklist

### 1. Extract Keyframe Getter Function

**Goal:** Provide direct access to raw keyframe quaternions (currently buried inside `apply_pose()`).

#### 1.1 Add Function Declaration

- [x] **File:** `src/character/keyframe.h`
    - [x] Add function declaration after `apply_pose()` (around line 30):
    ```cpp
    /// Get raw keyframe data for a pose without applying to skeleton.
    /// Returns quaternions for all 8 joints (shoulders, elbows, hips, knees).
    keyframe get_keyframe_data(pose_type pose);
    ```

#### 1.2 Implement Keyframe Getter

- [x] **File:** `src/character/keyframe.cpp`
    - [x] Add `get_keyframe_data()` implementation after `create_step_right_pose()` (around line 143):
    - [x] Function signature: `keyframe get_keyframe_data(pose_type pose)`
    - [x] Use switch statement on `pose` parameter
    - [x] Cases: `T_POSE`, `STEP_LEFT`, `NEUTRAL`, `STEP_RIGHT`
    - [x] For `T_POSE`: return `create_identity_pose()`
    - [x] For `STEP_LEFT`: return `create_step_left_pose()`
    - [x] For `NEUTRAL`: return `create_neutral_pose()`
    - [x] For `STEP_RIGHT`: return `create_step_right_pose()`
    - [x] Default case: return `create_identity_pose()`
    - [x] **Estimated lines:** ~15

**Pseudocode:**
```
function get_keyframe_data(pose_type pose) -> keyframe:
    switch pose:
        case T_POSE: return create_identity_pose()
        case STEP_LEFT: return create_step_left_pose()
        case NEUTRAL: return create_neutral_pose()
        case STEP_RIGHT: return create_step_right_pose()
        default: return create_identity_pose()
```

---

### 2. Replace Threshold Logic with Segment-Based Blending

**Goal:** Modify `animation_state::update_skeletal_animation()` to blend between keyframe poses using slerp.

#### 2.1 Remove Discrete Pose Selection

- [x] **File:** `src/character/animation.cpp`
    - [x] Delete lines 125-138 (threshold-based pose selection)
    - [x] Keep phase calculation (lines 122-123)
    - [x] Keep manual override early return (lines 114-120)
    - [x] **Lines removed:** -12

#### 2.2 Implement Segment-Based Blending

- [x] **File:** `src/character/animation.cpp` (replace deleted section)
    - [x] Define 4 blend segments after phase calculation (around line 125):
        - Segment 1: phase 0.00-0.25 → blend STEP_LEFT to NEUTRAL
        - Segment 2: phase 0.25-0.50 → blend NEUTRAL to STEP_RIGHT
        - Segment 3: phase 0.50-0.75 → blend STEP_RIGHT to NEUTRAL
        - Segment 4: phase 0.75-1.00 → blend NEUTRAL to STEP_LEFT (wrap continuity)
    - [x] Use if-else chain to determine current segment
    - [x] For each segment, calculate:
        - `segment_start` (float): segment start phase
        - `segment_end` (float): segment end phase
        - `source_pose` (pose_type): start pose
        - `target_pose` (pose_type): end pose
        - `t` (float): blend factor = `(phase - segment_start) / (segment_end - segment_start)`
    - [x] Get keyframe data: `keyframe source_kf = get_keyframe_data(source_pose)`
    - [x] Get keyframe data: `keyframe target_kf = get_keyframe_data(target_pose)`
    - [x] **Estimated lines:** ~25

**Pseudocode:**
```
// After phase calculation (line 123)
pose_type source_pose, target_pose
float segment_start, segment_end, t

if phase < 0.25:
    source_pose = STEP_LEFT, target_pose = NEUTRAL
    segment_start = 0.0, segment_end = 0.25
else if phase < 0.5:
    source_pose = NEUTRAL, target_pose = STEP_RIGHT
    segment_start = 0.25, segment_end = 0.5
else if phase < 0.75:
    source_pose = STEP_RIGHT, target_pose = NEUTRAL
    segment_start = 0.5, segment_end = 0.75
else:
    source_pose = NEUTRAL, target_pose = STEP_LEFT
    segment_start = 0.75, segment_end = 1.0

t = (phase - segment_start) / (segment_end - segment_start)
keyframe source_kf = get_keyframe_data(source_pose)
keyframe target_kf = get_keyframe_data(target_pose)
```

#### 2.3 Blend and Apply Quaternions

- [x] **File:** `src/character/animation.cpp` (continuation of blending section)
    - [x] Create helper lambda inside `update_skeletal_animation()` to apply blended quaternion:
        - Lambda signature: `auto apply_blended_joint = [&](int joint_idx, const glm::quat& blended_rotation)`
        - Extract T-pose position from skeleton joint: `glm::vec3 t_pose_pos = glm::vec3(skel.joints[joint_idx].local_transform[3])`
        - Build transform: `skel.joints[joint_idx].local_transform = glm::translate(glm::mat4(1.0f), t_pose_pos) * glm::mat4_cast(blended_rotation)`
    - [x] **Pattern:** Copy from `apply_pose()` ([keyframe.cpp:181-187](../src/character/keyframe.cpp#L181-L187))
    - [x] Blend and apply all 8 joints using `glm::slerp(source, target, t)`:
        - `apply_blended_joint(joint_index::LEFT_SHOULDER, glm::slerp(source_kf.left_shoulder, target_kf.left_shoulder, t))`
        - `apply_blended_joint(joint_index::LEFT_ELBOW, glm::slerp(source_kf.left_elbow, target_kf.left_elbow, t))`
        - `apply_blended_joint(joint_index::RIGHT_SHOULDER, glm::slerp(source_kf.right_shoulder, target_kf.right_shoulder, t))`
        - `apply_blended_joint(joint_index::RIGHT_ELBOW, glm::slerp(source_kf.right_elbow, target_kf.right_elbow, t))`
        - `apply_blended_joint(joint_index::LEFT_HIP, glm::slerp(source_kf.left_hip, target_kf.left_hip, t))`
        - `apply_blended_joint(joint_index::LEFT_KNEE, glm::slerp(source_kf.left_knee, target_kf.left_knee, t))`
        - `apply_blended_joint(joint_index::RIGHT_HIP, glm::slerp(source_kf.right_hip, target_kf.right_hip, t))`
        - `apply_blended_joint(joint_index::RIGHT_KNEE, glm::slerp(source_kf.right_knee, target_kf.right_knee, t))`
    - [x] Call `update_global_transforms(skel)` after applying all joints
    - [x] Update `current_automatic_pose` to reflect the target pose of the current segment (for GUI display)
    - [x] **Estimated lines:** ~18

**Pseudocode:**
```
// Helper lambda (similar to keyframe.cpp:181-187)
auto apply_blended_joint = [&](int joint_idx, glm::quat rotation):
    vec3 t_pose_pos = extract_translation(skel.joints[joint_idx].local_transform)
    skel.joints[joint_idx].local_transform = translate(t_pose_pos) * mat4_cast(rotation)

// Blend all 8 joints
apply_blended_joint(LEFT_SHOULDER, slerp(source_kf.left_shoulder, target_kf.left_shoulder, t))
apply_blended_joint(LEFT_ELBOW, slerp(source_kf.left_elbow, target_kf.left_elbow, t))
apply_blended_joint(RIGHT_SHOULDER, slerp(source_kf.right_shoulder, target_kf.right_shoulder, t))
apply_blended_joint(RIGHT_ELBOW, slerp(source_kf.right_elbow, target_kf.right_elbow, t))
apply_blended_joint(LEFT_HIP, slerp(source_kf.left_hip, target_kf.left_hip, t))
apply_blended_joint(LEFT_KNEE, slerp(source_kf.left_knee, target_kf.left_knee, t))
apply_blended_joint(RIGHT_HIP, slerp(source_kf.right_hip, target_kf.right_hip, t))
apply_blended_joint(RIGHT_KNEE, slerp(source_kf.right_knee, target_kf.right_knee, t))

update_global_transforms(skel)

// Update GUI state (show target pose of current segment)
current_automatic_pose = target_pose
```

#### 2.4 Handle Root Transform and Secondary Motion

- [x] **File:** `src/character/animation.cpp`
    - [x] Before applying blended quaternions, store root transform: `glm::mat4 root_transform = skel.joints[0].local_transform`
    - [x] After blending, restore root transform: `skel.joints[0].local_transform = root_transform`
    - [x] Keep secondary motion code unchanged (lines 140-143) - springs operate on blended output
    - [x] **Pattern:** Copy root handling from `apply_pose()` ([keyframe.cpp:146-153](../src/character/keyframe.cpp#L146-L153))
    - [x] **Estimated lines:** +2 (root store/restore)

---

### 3. Include Required Headers

- [x] **File:** `src/character/animation.cpp`
    - [x] Verify `#include <glm/gtx/quaternion.hpp>` is present (provides `glm::slerp`)
    - [x] If missing, add after existing GLM includes

---

### 4. Quality Gates

- [ ] **Build:** Run `CMake: Build (Debug)` task
    - [ ] Verify zero compiler errors
    - [ ] Verify zero compiler warnings (treat new warnings as errors)

- [ ] **Format:** Run `clang-format` on modified files
    - [ ] `src/character/keyframe.h`
    - [ ] `src/character/keyframe.cpp`
    - [ ] `src/character/animation.cpp`

- [ ] **Smoke Test:** Launch application (`Run (Debug)`)
    - [ ] Application starts without crashes
    - [ ] Character animation panel is visible
    - [ ] Toggle between automatic/manual pose selection works
    - [ ] Walk character in circles - observe limbs for smoothness

---

## Testing Protocol (Post-Implementation)

### Visual Inspection (Self-Test)

- [ ] **Smoothness Check:**
    - [ ] Walk character in continuous circles
    - [ ] Watch limbs at phase values: 0.25, 0.5, 0.75 (check animation panel for phase)
    - [ ] Success: No visible pops or discontinuities

- [ ] **Wrap Continuity:**
    - [ ] Watch animation panel phase value near 1.0 → 0.0 transition
    - [ ] Observe limbs blending from NEUTRAL to STEP_LEFT
    - [ ] Success: Seamless transition with no snap

- [ ] **Spring Behavior:**
    - [ ] Enable secondary motion in animation panel
    - [ ] Compare wobble/follow-through feel to previous discrete version (git stash if needed)
    - [ ] Success: Natural follow-through without excessive wobble or dead motion

- [ ] **Performance:**
    - [ ] Monitor frame time in debug panel
    - [ ] Success: No measurable increase (< 1% difference)

### Edge Cases

- [ ] **Manual Override Mode:**
    - [ ] Toggle "Use Manual Pose Selection" in animation panel
    - [ ] Select each pose (T_POSE, STEP_LEFT, NEUTRAL, STEP_RIGHT)
    - [ ] Success: Poses display instantly with no blending (early return path)

- [ ] **Secondary Motion Toggle:**
    - [ ] Disable secondary motion
    - [ ] Walk character - observe stiff limb motion
    - [ ] Re-enable secondary motion
    - [ ] Success: Springs apply to blended output correctly

---

## Implementation Notes

**GLM Slerp:**
- Function signature: `glm::quat glm::slerp(const glm::quat& x, const glm::quat& y, float a)`
- Parameter `a` ∈ [0, 1]: 0 = full source, 1 = full target
- Handles shortest-path interpolation automatically

**Joint Indices:**
- Defined in `keyframe.cpp:32-50` (namespace `joint_index`)
- Shoulders: 5 (left), 8 (right)
- Elbows: 6 (left), 9 (right)
- Hips: 11 (left), 14 (right)
- Knees: 12 (left), 15 (right)

**Pattern Reuse:**
- Root transform store/restore: [keyframe.cpp:146-153](../src/character/keyframe.cpp#L146-L153)
- Apply quaternion to joint: [keyframe.cpp:181-187](../src/character/keyframe.cpp#L181-L187)
- T-pose position extraction: [keyframe.cpp:183](../src/character/keyframe.cpp#L183)

**Segment Design Rationale:**
- Last segment (0.75-1.0) targets STEP_LEFT (not NEUTRAL) for seamless phase wrap
- Four equal segments (0.25 duration each) create symmetric gait
- Neutral poses act as "passing through" frames between steps

---

## Scope Summary

**Lines Changed:**
- `keyframe.h`: +4 (declaration + comment)
- `keyframe.cpp`: +15 (new function)
- `animation.cpp`: +35, -12 (net +23)
- **Total:** ~42 lines (within 40-60 estimate)

**Files Modified:**
1. `src/character/keyframe.h` (add declaration)
2. `src/character/keyframe.cpp` (implement getter)
3. `src/character/animation.cpp` (replace threshold logic with blending)

**Files NOT Modified:**
- `game_world.cpp` (call site unchanged)
- `character_panel.cpp` (no new parameters)
- `controller.h` (animation state unchanged)

**No Migrations:** All systems already in correct ownership locations.
