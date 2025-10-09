# Implementation Plan: Primary Skeletal Animation

**Feature:** Distance-phased pose switching synchronized to surveyor-wheel locomotion

**Architecture:** Extends existing `character::keyframe` module with distance-phase calculation logic. Integrates with existing `game_world.t_pose_skeleton` and `locomotion.distance_traveled` state.

**Scope:** ~30-40 lines of code across 2 files (animation module + GUI panel)

---

## Foundation Architecture Audit

### Systems Involved
- **skeleton**: Single instance at `game_world.t_pose_skeleton`
  - Created in `game_world::init()` via `create_t_pose(t_pose_skeleton)`
  - Root attached to character transform each frame: `t_pose_skeleton.joints[0].local_transform = character.get_world_transform()`
  - Pose applied via `character::apply_pose(t_pose_skeleton, panel_state.selected_pose)`
  - Rendered from `runtime.cpp` via `debug::draw_skeleton(debug_ctx, world.t_pose_skeleton, ...)`

- **locomotion**: Single instance at `game_world.locomotion`
  - Updated in `game_world::update()` after physics and orientation
  - Provides `distance_traveled` (read-only access for animation)
  - No conflicts—clean read access pattern

- **animation**: Currently minimal, will add cycle_length parameter
  - No existing distance-phase logic
  - Clean slate for new feature

### Current Integration Flow (game_world::update)
```
1. Physics update (character.update)
2. Skeleton root attachment (t_pose_skeleton.joints[0].local_transform = character.get_world_transform())
3. Orientation update (character.orientation.update)
4. Animation updates (landing_spring, acceleration_tilt)
5. Locomotion update (locomotion.update)
6. Pose application (character::apply_pose or apply_pose_with_overrides)
```

### No Architecture Conflicts Found
- Single skeleton instance (no duplicates)
- Clean ownership: game_world owns all systems
- Read-only access to locomotion.distance_traveled
- Pose application happens after all updates (correct timing)

---

## Graybox Implementation Checklist

### 1. Animation State Extension

- [x] **File:** [src/character/animation.h](src/character/animation.h)
  - [x] Add `float cycle_length = 1.0f;` to `animation_state` struct (default: one stride per meter)
  - [x] Add `character::pose_type current_automatic_pose = character::pose_type::NEUTRAL;` to track automatic pose selection

**Files changed:**
- [src/character/animation.h](src/character/animation.h) - Added cycle_length and current_automatic_pose fields to animation_state struct

### 2. Distance-Phase Calculation Logic

- [x] **File:** [src/character/animation.h](src/character/animation.h)
  - [x] Add method signature: `void update_skeletal_animation(character::skeleton& skeleton, float distance_traveled, character::pose_type manual_override_pose, bool use_manual_override);`

- [x] **File:** [src/character/animation.cpp](src/character/animation.cpp)
  - [x] Implement `update_skeletal_animation()` method:
    - [x] If `use_manual_override` is true, apply `manual_override_pose` and return early (preserves existing manual pose selection UI)
    - [x] Calculate phase: `float phase = std::fmod(distance_traveled, cycle_length) / cycle_length;` (normalized 0.0-1.0)
    - [x] Threshold-based pose selection:
      ```cpp
      if (phase < 0.25f) {
          current_automatic_pose = pose_type::STEP_LEFT;
      } else if (phase < 0.5f) {
          current_automatic_pose = pose_type::NEUTRAL;
      } else if (phase < 0.75f) {
          current_automatic_pose = pose_type::STEP_RIGHT;
      } else {
          current_automatic_pose = pose_type::NEUTRAL;
      }
      ```
    - [x] Apply selected pose: `character::apply_pose(skeleton, current_automatic_pose);`

**Files changed:**
- [src/character/animation.h](src/character/animation.h) - Added update_skeletal_animation() method signature
- [src/character/animation.cpp](src/character/animation.cpp) - Implemented update_skeletal_animation() with distance-phase calculation and threshold-based pose selection

**Pseudocode:**
```
function update_skeletal_animation(skeleton, distance_traveled, manual_pose, use_manual):
    if use_manual:
        apply_pose(skeleton, manual_pose)
        return

    phase = (distance_traveled % cycle_length) / cycle_length

    if phase < 0.25:
        selected = STEP_LEFT
    else if phase < 0.5:
        selected = NEUTRAL
    else if phase < 0.75:
        selected = STEP_RIGHT
    else:
        selected = NEUTRAL

    current_automatic_pose = selected
    apply_pose(skeleton, selected)
```

### 3. Integration into Game Loop

- [x] **File:** [src/gui/character_panel.h](src/gui/character_panel.h)
  - [x] Add `bool use_manual_pose_selection = false;` to `character_panel_state` struct

- [x] **File:** [src/app/game_world.cpp](src/app/game_world.cpp)
  - [x] Replace existing pose application logic (lines 65-74) with call to `character.animation.update_skeletal_animation()`
  - [x] Pass required parameters: `t_pose_skeleton`, `locomotion.distance_traveled`, `panel_state.selected_pose`, `panel_state.use_manual_pose_selection`
  - [x] Ensure call happens after locomotion update and before skeleton rendering
  - [x] Preserve joint overrides functionality (apply after automatic animation if enabled)

**Files changed:**
- [src/gui/character_panel.h](src/gui/character_panel.h) - Added use_manual_pose_selection field to character_panel_state struct
- [src/app/game_world.cpp](src/app/game_world.cpp) - Integrated update_skeletal_animation() into game loop with joint override support

**Current code to replace:**
```cpp
// Lines 65-74 in game_world.cpp
if (panel_state.enable_joint_overrides) {
    character::apply_pose_with_overrides(...);
} else {
    character::apply_pose(t_pose_skeleton, panel_state.selected_pose);
}
```

**New code:**
```cpp
character.animation.update_skeletal_animation(
    t_pose_skeleton,
    locomotion.distance_traveled,
    panel_state.selected_pose,
    panel_state.use_manual_pose_selection
);
```

### 4. Debug UI Integration

- [x] **File:** [src/gui/character_panel.h](src/gui/character_panel.h)
  - [x] Add `bool use_manual_pose_selection = false;` to `character_panel_state` struct

- [x] **File:** [src/gui/character_panel.cpp](src/gui/character_panel.cpp)
  - [x] In "Skeleton" section (after line 78), add checkbox for manual pose mode:
    ```cpp
    gui::widget::checkbox("Manual Pose Selection", &state.use_manual_pose_selection);
    ```
  - [x] Make pose combo box and joint overrides conditionally visible (only show when manual mode enabled):
    ```cpp
    if (state.use_manual_pose_selection) {
        // Existing pose selection combo
        // Existing joint overrides section
    }
    ```
  - [x] In "Locomotion" section (after "Animation Phase" display), add cycle length slider:
    ```cpp
    gui::widget::slider_float("Cycle Length (m)", &character.animation.cycle_length, 0.5f, 3.0f);
    ```
  - [x] Add read-only display for current automatic pose:
    ```cpp
    const char* pose_name = "Unknown";
    switch (character.animation.current_automatic_pose) {
        case character::pose_type::T_POSE: pose_name = "T-Pose"; break;
        case character::pose_type::STEP_LEFT: pose_name = "Step Left"; break;
        case character::pose_type::NEUTRAL: pose_name = "Neutral"; break;
        case character::pose_type::STEP_RIGHT: pose_name = "Step Right"; break;
    }
    gui::widget::text("Current Pose: %s", pose_name);
    ```

**Files changed:**
- [src/gui/character_panel.cpp](src/gui/character_panel.cpp) - Added manual pose selection checkbox, made pose combo/joint overrides conditional, added cycle length slider and current automatic pose display

### 4.1 Walk Speed Lock (Quick Feature)

- [x] **File:** [src/character/controller.h](src/character/controller.h)
  - [x] Add `float walk_speed = 2.0f;` parameter (shift key speed limit)
  - [x] Add `bool is_walking = false;` state flag

- [x] **File:** [src/character/controller.cpp](src/character/controller.cpp)
  - [x] In `apply_input()`, set `is_walking` based on SHIFT key state (left or right)
  - [x] In `update()`, cap speed to `walk_speed` when `is_walking` is true

**Rationale:** Enables precise control for tuning walk cycle timing. Holding SHIFT locks max speed to walk_speed_threshold (2.0 m/s), making it easier to observe STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL transitions without overshooting into run speeds.

**Files changed:**
- [src/character/controller.h](src/character/controller.h) - Added walk_speed parameter and is_walking flag
- [src/character/controller.cpp](src/character/controller.cpp) - Added shift key detection and speed capping logic

### 5. Quality Gates

- [x] Build passes (`CMake: Configure` then `CMake: Build (Debug)`)
- [x] Lint/format clean (`clang-format` and `clang-tidy` tasks)
- [x] Smoke run: Launch debug build, walk forward, verify legs alternate STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL
- [x] Tuning test: Adjust `cycle_length` slider while walking, verify stride frequency changes immediately
- [x] Stop test: Stop mid-stride, verify pose freezes with no snap to neutral
- [x] Manual mode test: Enable "Manual Pose Selection" checkbox, verify manual pose combo works

**Files changed:**
- [src/gui/character_panel.cpp](src/gui/character_panel.cpp) - Fixed clang-tidy warning (dead store false positive) by moving default case to switch statement

---

## Testing Protocol

**Self-Test First:**
1. Walk forward in test level—watch legs alternate through STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL sequence
2. Verify arms counter-swing opposite to legs (STEP_LEFT has right arm forward, STEP_RIGHT has left arm forward)
3. Stop mid-stride—verify pose holds frozen at current phase (no pop to neutral)
4. Adjust `cycle_length` slider while walking—verify stride frequency changes in real-time
5. Enable "Manual Pose Selection"—verify manual pose combo and joint overrides still work
6. Disable manual mode—verify automatic cycling resumes from current distance_traveled

**Success Criteria:**
- Legs visually alternate in correct sequence
- Arms counter-swing opposite to legs
- Stopping freezes pose (no snapping)
- Cycle length adjustable in real-time
- Manual pose selection mode preserves existing debug functionality
- No crashes, visual artifacts, or T-pose fallbacks

**Sample Size:** 2-3 self-tests (rapid iteration)

**Avoid Over-Testing:** This is graybox validation—subjective feel tuning only. Defer "correct" cycle length values until next iteration (pose blending).

---

## Architecture Notes

**Why cycle_length in animation_state?**
- Follows existing pattern (tilt_smoothing, landing_impulse_scale all in animation_state)
- Direct UI binding without intermediate abstractions
- Public member access for tuning (no getters/setters)

**Why update_skeletal_animation() method?**
- Encapsulates distance-phase logic in animation module (clean separation of concerns)
- Accepts skeleton reference (no ownership transfer)
- Manual override parameter preserves existing debug UI functionality
- Single call site in game_world::update (clear integration point)

**Stop Behavior:**
- When `distance_traveled` stops increasing, `phase` stops changing
- Same threshold range selected → same pose applied
- No special handling needed (intended behavior)

**Future Extension Points:**
- Pose blending: Replace discrete pose selection with lerp/slerp between keyframes based on phase
- Speed-based scaling: Modulate `cycle_length` based on `locomotion.smoothed_speed`
- Running gait: Add speed-based pose set selection (walk vs run keyframes)

---

## Changelog

**Distance Accumulation Fix (2025-10-09):**
- Fixed `locomotion.distance_traveled` to accumulate continuously (surveyor-wheel odometer pattern)
- Previous implementation: `distance_traveled = phase * blended_stride` (wrapped at stride length, desync'd skeletal animation)
- New implementation: `distance_traveled += drive_speed * dt` (cumulative, independent phase management)
- Impact: Skeletal animation now cycles cleanly at all speeds, no skipped neutral poses
- Files changed: `src/character/locomotion.cpp`

**Walk Speed Lock Feature Added (Section 4.1):**
- Added SHIFT key modifier to lock character movement to walk speed (2.0 m/s)
- Enables precise tuning observation of walk cycle without overshooting into run speeds
- Implementation: 3 files changed, ~8 lines of code
- Pattern: Input modifier flag (`is_walking`) + conditional speed cap
- Aligns with "Control feel" principle (indirect control via priming/nudging)
- Note: Hard velocity cap approach used (brief overshoot acceptable for debug/tuning feature)

**Debug Visual Backlog Item Added:**
- Created "Debug Visual Overhaul" entry in DESIGN_BACKLOG.md
- Documents observed issues: red circle larger than green when holding SHIFT, velocity sphere scale accuracy
- Priority: Medium (polish, not critical) - defer until primary skeletal animation validated
- Rationale: Visual debugging is core to rapid iteration; mismatched visuals create false signals

_Initial implementation plan created from iteration_primary_skeletal_animation.md_

---

## Approval

- **Status:** Approved
- **Reviewer:** Claude (FrogLords Code Review Agent)
- **Date:** 2025-10-09
- **Review Document:** [code_review_primary_skeletal_animation.md](code_review_primary_skeletal_animation.md)

---

## Completion

- **Date Completed:** October 9, 2025
- **All Checklist Items Confirmed:** ✅
  - [x] Animation state extension (cycle_length, current_automatic_pose)
  - [x] Distance-phase calculation logic (update_skeletal_animation method)
  - [x] Integration into game loop (manual override support, joint overrides preserved)
  - [x] Debug UI integration (manual pose checkbox, cycle length slider, current pose display)
  - [x] Walk speed lock feature (SHIFT key speed cap for tuning)
  - [x] Quality gates passed (build, lint, smoke tests, tuning tests)
- **Code Review:** [code_review_primary_skeletal_animation.md](code_review_primary_skeletal_animation.md)
- **Key Learnings:**
  - Distance accumulation fix critical: `distance_traveled += drive_speed * dt` (cumulative) vs `distance_traveled = phase * stride` (wrapped)
  - Threshold-based pose selection (0.25/0.5/0.75) sufficient before interpolation
  - Manual override parameter preserves debug UI alongside automatic animation
  - Walk speed lock essential for precise tuning observation
