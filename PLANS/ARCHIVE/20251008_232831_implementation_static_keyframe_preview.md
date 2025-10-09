# Implementation Plan: Static Keyframe Preview

**Feature:** Manual GUI selection between hardcoded skeletal poses to validate quaternion-based keyframe data structure.

**Scope:** 3 walking keyframes (STEP_LEFT, NEUTRAL, STEP_RIGHT) + T_POSE. Minimum 8 joints (shoulders, elbows, hips, knees). Instant pose switching, no blending.

**Architecture:** New `character::keyframe` module for pose data and application logic. Integrates with existing `game_world.t_pose_skeleton` and `gui::character_panel`.

---

## Foundation Architecture Audit

### T-Pose Baseline Verification

**Finding (verified 2025-10-07):** Inspected `src/character/t_pose.cpp` - all joint `local_transform` fields are **pure translations with identity rotations**.

```cpp
// Example from t_pose.cpp:
skel.joints.push_back({
    glm::mat4(1.0f),  // model_transform (identity)
    glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.05f, 0.0f)),  // local_transform (translation only)
    2,  // parent_index
    "left_shoulder"
});
```

**Implication:** T-pose local rotations are identity. No need to extract quaternions from existing transforms. Pose application can directly build: `local_transform = translate(t_pose_pos) * mat4_cast(pose_quat)`.

### Systems Involved
1. **skeleton** - Hierarchical joint transforms
2. **GUI** - Character panel pose selection
3. **game_world** - Integration point for pose application

### Current State
- `skeleton`: Single instance at `game_world.t_pose_skeleton`
- Initialized: `game_world::init()` calls `create_t_pose()`
- Updated: `game_world::update()` sets root transform, conditionally updates global transforms
- Rendered: `runtime.cpp` draws skeleton when `panel_state.show_skeleton == true`
- GUI: `character_panel` has "Skeleton" section with `show_skeleton` and `animate_skeleton` checkboxes

### Joint Indices (from t_pose.cpp)
```
0:  root          5:  left_shoulder   11: left_hip
1:  spine_lower   6:  left_elbow      12: left_knee
2:  spine_upper   7:  left_wrist      13: left_ankle
3:  neck          8:  right_shoulder  14: right_hip
4:  head          9:  right_elbow     15: right_knee
                  10: right_wrist     16: right_ankle
```

### Joint Set for Walk Validation (Iteration 1)
**8 joints:** left_shoulder(5), left_elbow(6), right_shoulder(8), right_elbow(9), left_hip(11), left_knee(12), right_hip(14), right_knee(15)

**Future extensions (deferred):** Add spine_upper(2) for torso lean; add ankles(13,16) for grounded feet. Defer until 8-joint validation succeeds and reveals specific need.

### No Architectural Conflicts
- New keyframe system is purely additive
- No duplicate skeleton instances
- No ownership conflicts
- Integration point: `game_world::update()` applies selected pose before rendering

---

## Graybox Implementation Checklist

### 1. Keyframe Data Structure

**File:** `src/character/keyframe.h` (new file)

- [x] Create `character` namespace
- [x] Define `enum class pose_type { T_POSE, STEP_LEFT, NEUTRAL, STEP_RIGHT };`
- [x] Define `struct keyframe` with 8 `glm::quat` members for minimum joint set:
  - [x] `glm::quat left_shoulder;`
  - [x] `glm::quat left_elbow;`
  - [x] `glm::quat right_shoulder;`
  - [x] `glm::quat right_elbow;`
  - [x] `glm::quat left_hip;`
  - [x] `glm::quat left_knee;`
  - [x] `glm::quat right_hip;`
  - [x] `glm::quat right_knee;`
- [x] Declare `void apply_pose(skeleton& skel, pose_type pose);` function
- [x] Include `<glm/gtc/quaternion.hpp>` and `"skeleton.h"`

**File:** `src/character/keyframe.cpp` (new file)

- [x] Include `"keyframe.h"`, `<glm/gtc/matrix_transform.hpp>`, `<glm/gtc/type_ptr.hpp>`
- [x] Create helper function `keyframe create_identity_pose()` that returns all quaternions as identity `glm::quat(1,0,0,0)`

**Files Changed:**
- `src/character/keyframe.h` (created)
- `src/character/keyframe.cpp` (created)

### 2. Hardcoded Walk Poses

**File:** `src/character/keyframe.cpp`

Reference: walk gait images (NOTES/Images/walk*.png) show counter-rotation arm swing and leg stride.

- [x] Define `keyframe create_step_left_pose()`:
  - [x] Left arm back: `left_shoulder = glm::quat(glm::radians(vec3(-30, 0, 0)))` (pitch back)
  - [x] Left arm straight: `left_elbow = identity`
  - [x] Right arm forward: `right_shoulder = glm::quat(glm::radians(vec3(45, 0, 0)))` (pitch forward)
  - [x] Right arm slight bend: `right_elbow = glm::quat(glm::radians(vec3(20, 0, 0)))`
  - [x] Left leg forward: `left_hip = glm::quat(glm::radians(vec3(30, 0, 0)))` (flex forward)
  - [x] Left knee bend: `left_knee = glm::quat(glm::radians(vec3(15, 0, 0)))`
  - [x] Right leg back: `right_hip = glm::quat(glm::radians(vec3(-20, 0, 0)))` (extend back)
  - [x] Right knee straight: `right_knee = identity`

- [x] Define `keyframe create_neutral_pose()`:
  - [x] All joints near identity (legs vertical, arms at sides)
  - [x] Use identity quaternions or small transitional values

- [x] Define `keyframe create_step_right_pose()`:
  - [x] Mirror of STEP_LEFT (negate left/right joint rotations)
  - [x] Right arm back, left arm forward
  - [x] Right leg forward, left leg back

**Pseudocode for create_step_right_pose:**
```
step_right = create_step_left_pose()
swap(step_right.left_shoulder, step_right.right_shoulder)
swap(step_right.left_elbow, step_right.right_elbow)
swap(step_right.left_hip, step_right.right_hip)
swap(step_right.left_knee, step_right.right_knee)
```

**Files Changed:**
- `src/character/keyframe.cpp`

### 3. Pose Application Logic

**File:** `src/character/keyframe.cpp`

- [x] Implement `void apply_pose(skeleton& skel, pose_type pose)`:
  - [x] Get keyframe based on pose type:
    - [x] `if (pose == T_POSE)` → return early (no modification, T-pose is default)
    - [x] `if (pose == STEP_LEFT)` → `kf = create_step_left_pose()`
    - [x] `if (pose == NEUTRAL)` → `kf = create_neutral_pose()`
    - [x] `if (pose == STEP_RIGHT)` → `kf = create_step_right_pose()`
  - [x] Apply keyframe quaternions to skeleton joints; compose with T-pose baseline using GLM functions
  - [x] Apply to joints by index:
    - [x] Joint 5 (left_shoulder): `kf.left_shoulder`
    - [x] Joint 6 (left_elbow): `kf.left_elbow`
    - [x] Joint 8 (right_shoulder): `kf.right_shoulder`
    - [x] Joint 9 (right_elbow): `kf.right_elbow`
    - [x] Joint 11 (left_hip): `kf.left_hip`
    - [x] Joint 12 (left_knee): `kf.left_knee`
    - [x] Joint 14 (right_hip): `kf.right_hip`
    - [x] Joint 15 (right_knee): `kf.right_knee`
  - [x] For each joint: extract T-pose position, build transform as `translate(t_pose_pos) * mat4_cast(pose_quat)`
  - [x] Call `update_global_transforms(skel)` to propagate hierarchy

**Implementation Notes:**
- **T-pose rotations are identity** (verified in Foundation Architecture Audit above)
- Extract position: `glm::vec3 t_pose_pos = glm::vec3(skel.joints[i].local_transform[3])`
- Build new transform: `skel.joints[i].local_transform = glm::translate(glm::mat4(1.0f), t_pose_pos) * glm::mat4_cast(pose_quat)`
- Transform composition order: translation first, then rotation (`T * R`, not `R * T`)
- Use GLM's `glm::mat4_cast()` for quaternion-to-matrix conversion

### 4. GUI Pose Selection

**File:** `src/gui/character_panel.h`

- [x] Add `#include "character/keyframe.h"` at top
- [x] In `character_panel_state` struct, add `character::pose_type selected_pose = character::pose_type::T_POSE;`

**File:** `src/gui/character_panel.cpp`

- [x] In "Skeleton" collapsing header section (after `animate_skeleton` checkbox):
  - [x] Add `ImGui::Separator();`
  - [x] Add `ImGui::Text("Pose Selection:");`
  - [x] Add combo box/dropdown:
    ```cpp
    const char* pose_names[] = {"T-Pose", "Step Left", "Neutral", "Step Right"};
    int current_pose = static_cast<int>(state.selected_pose);
    if (ImGui::Combo("Pose", &current_pose, pose_names, 4)) {
        state.selected_pose = static_cast<character::pose_type>(current_pose);
    }
    ```

**Files Changed:**
- `src/gui/character_panel.h`
- `src/gui/character_panel.cpp`

### 5. Integration with Game World

**File:** `src/app/game_world.cpp`

- [x] Add `#include "character/keyframe.h"` at top
- [x] In `game_world::update()`, after setting skeleton root transform (line ~32):
  - [x] Add conditional pose application:
    ```cpp
    // Apply selected pose if not animating
    if (!panel_state.animate_skeleton) {
        character::apply_pose(t_pose_skeleton, panel_state.selected_pose);
    }
    ```
  - [x] Move existing `update_global_transforms()` call inside the `if (!panel_state.animate_skeleton)` block OR ensure `apply_pose` calls it internally

**Current game_world::update structure:**
```
Line ~32: t_pose_skeleton.joints[0].local_transform = character.get_world_transform();
Line ~64-69: if (panel_state.animate_skeleton) { ... update_global_transforms() }
```

**Modified structure:**
```
Line ~32: t_pose_skeleton.joints[0].local_transform = character.get_world_transform();
NEW:      if (!panel_state.animate_skeleton) {
              character::apply_pose(t_pose_skeleton, panel_state.selected_pose);
          }
Line ~64-69: Keep existing animate_skeleton block unchanged (or remove its update_global_transforms call)
```

**Files Changed:**
- `src/app/game_world.cpp`

### 6. Build System Updates

**File:** `CMakeLists.txt`

- [x] Add `src/character/keyframe.cpp` to the source list (likely in the section where `skeleton.cpp`, `animation.cpp` are listed)

**Files Changed:**
- `CMakeLists.txt`

### 7. Testing & Validation

- [x] Build passes (`CMake: Build (Debug)`)
- [x] Lint/format clean (run `Format: clang-format src` task)
- [x] Run application (`Run (Debug)`)
- [x] Open Character Panel → Skeleton section
- [x] Test pose selection dropdown:
  - [x] Select "T-Pose" → skeleton displays in T-pose (arms horizontal, legs vertical)
  - [x] Select "Step Left" → left arm back, right arm forward, left leg forward, right leg back
  - [x] Select "Neutral" → legs near-vertical, arms at sides
  - [x] Select "Step Right" → right arm back, left arm forward, right leg forward, left leg back
- [x] Visual checks:
  - [x] No inverted joints (limbs pointing wrong direction)
  - [x] No broken hierarchy (child joints detached from parents)
  - [x] Instant switching (no delay or flicker)
  - [x] Skeleton maintains attachment to character body during movement

### 8. Iteration & Tuning

**Primary Workflow (Edit-Rebuild):**

1. Select pose in dropdown
2. Observe visual result
3. If incorrect, note which joint needs adjustment
4. Edit hardcoded Euler angles in `keyframe.cpp` (e.g., change `glm::radians(vec3(30, 0, 0))` to `glm::radians(vec3(35, 0, 0))`)
5. Rebuild and re-test
6. Repeat until poses match reference images

**Status:** COMPLETED - Poses manually tuned to acceptable "walk" gait visual quality.

**Optional: Real-Time Tuning UI (Completed - KEEPING IN CODEBASE)**

Only add if the edit-rebuild loop becomes intolerable (e.g., after 5+ rebuild cycles with slow iteration). Principle: "wait for the third use before abstracting."

- [x] Add debug UI for per-joint Euler angle tweaking:
  - [x] In `character_panel_state`, add Euler angle storage for all 8 joints
  - [x] In `character_panel.cpp`, add collapsing sub-section "Joint Overrides"
  - [x] For each joint, add 3 sliders (pitch, yaw, roll in degrees)
  - [x] In `keyframe.h/cpp`, add `apply_pose_with_overrides()` function
  - [x] Convert slider values to quaternion and update keyframe in real-time
  - [x] In `game_world.cpp`, use override function when `enable_joint_overrides` is true
  - [x] Use this to find correct angles, then hardcode them back into `create_*_pose()` functions
  - [x] **Decision: Keep UI for future pose authoring** (useful for iterating on animation poses)

**Files Changed:**
- `src/gui/character_panel.h` (added joint override state)
- `src/gui/character_panel.cpp` (added joint override UI controls)
- `src/character/keyframe.h` (added `apply_pose_with_overrides` declaration)
- `src/character/keyframe.cpp` (added `apply_pose_with_overrides` implementation)
- `src/app/game_world.cpp` (integrated override system)

---

## Quality Gates

- [x] Build passes (CMake: Configure/Build Debug tasks)
- [x] Lint/format clean (clang-format/clang-tidy tasks)
- [x] Application launches without crashes
- [x] Pose dropdown appears in Character Panel → Skeleton section
- [x] All 4 poses (T-Pose, Step Left, Neutral, Step Right) are selectable
- [x] Skeleton visual changes when switching poses
- [x] No console errors or warnings during pose switching
- [x] Skeleton remains attached to character body during movement

---

## Success Criteria

**Primary Test (STEP_LEFT vs T_POSE):**
- STEP_LEFT displays visibly different from T_POSE
- Left arm swings back, right arm swings forward (counter-rotation visible)
- Left leg extends forward, right leg extends back (stride visible)
- Limbs point in correct directions (not inverted or twisted)
- No visual artifacts (gimbal lock, broken joints, detached limbs)

**Full Validation:**
- All 4 poses (T_POSE, STEP_LEFT, NEUTRAL, STEP_RIGHT) are selectable and visually distinct
- Switching between poses is instant and stable (no crashes, no flicker)
- Quaternion composition with T-pose baseline works (positions preserved, rotations added)
- STEP_RIGHT mirrors STEP_LEFT (symmetry check)
- NEUTRAL shows transitional pose (legs near-vertical, arms transitioning)
- Code is simple and maintainable (<150 lines total for keyframe module)

**Acceptance:** Quaternion keyframe architecture is validated. Ready to add locomotion-driven animation (distance-phased pose selection, pose interpolation).

**Note:** Angle values in hardcoded poses (e.g., `glm::radians(vec3(30, 0, 0))`) are implementation targets for achieving visual correctness, not acceptance criteria. The test is visual quality, not numeric precision.

**Debug UI Retention:** Joint override UI will be kept in codebase as authoring tool for future pose iteration and animation development.

---

## File Modifications Summary

**New Files:**
- `src/character/keyframe.h` (data structure and function declarations)
- `src/character/keyframe.cpp` (pose creation and application logic)

**Modified Files:**
- `src/gui/character_panel.h` (added keyframe include, pose selection state, joint override state)
- `src/gui/character_panel.cpp` (added pose dropdown UI, joint override sliders)
- `src/app/game_world.h` (forward declarations for keyframe integration)
- `src/app/game_world.cpp` (integrated pose application with override system)
- `src/app/runtime.cpp` (minor adjustments for skeleton rendering)
- `CMakeLists.txt` (added keyframe.cpp to source list)

**Total Implementation:** ~200+ lines of new code (including optional real-time tuning UI that was completed).

---

## Changelog

- **2025-10-07:** Initial implementation plan created based on `ARCHIVE/20251008_232831_iteration_static_keyframe_preview.md` scope.
- **2025-10-08:** All implementation steps completed. Manual testing and tuning completed - poses validated as acceptable "walk" gait. Feature ready for review and finalization.
- **2025-10-08:** Decision made to retain joint override debug UI as pose authoring tool for future animation work (not removing as originally planned).
- **2025-10-08:** Code review performed. Naming convention violation fixed (enum values changed to UPPER_CASE). Tooling updated (`.clang-tidy`, `AGENTS.md`).

---

## Approval

**Status:** ✅ **APPROVED**

**Code Review:** See `PLANS/ARCHIVE/20251008_232831_code_review_static_keyframe_preview.md`

**Key Accomplishments:**
- ✅ Quaternion keyframe architecture validated (core hypothesis proven)
- ✅ All 4 poses (T_POSE, STEP_LEFT, NEUTRAL, STEP_RIGHT) selectable and visually distinct
- ✅ No visual artifacts, crashes, or stability issues
- ✅ Code standards compliance (enum naming fixed, clang-tidy clean)
- ✅ Excellent documentation quality

**Reviewer:** GitHub Copilot (AI Assistant)  
**Date:** October 8, 2025

---

## Completion

**Date Completed:** October 8, 2025

**All Checklist Items Confirmed Complete:**
- ✅ Keyframe data structure (8 quaternions for minimum joint set)
- ✅ Hardcoded walk poses (STEP_LEFT, NEUTRAL, STEP_RIGHT) + T_POSE
- ✅ Pose application logic (quaternion composition with T-pose baseline)
- ✅ GUI integration (pose selection dropdown in character panel)
- ✅ Build verification (CMake, clang-format, clang-tidy all pass)
- ✅ Runtime validation (no crashes, visual artifacts, or instability)
- ✅ Code standards (UPPER_CASE enums, documentation, naming conventions)

**Code Review:** See `PLANS/ARCHIVE/20251008_232831_code_review_static_keyframe_preview.md`

**Final Status:** Feature complete and ready for merge. Primary skeletal animation (locomotion-driven) now unblocked.
