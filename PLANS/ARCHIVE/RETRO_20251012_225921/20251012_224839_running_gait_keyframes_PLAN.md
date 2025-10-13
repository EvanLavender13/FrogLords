# Implementation Plan: Running Gait Keyframes

### Foundation Architecture Audit (Summary)

- Systems Involved: `keyframe` (pose data/functions), `skeleton` (joint transforms), `gui/character_panel` (manual pose selection + joint overrides).
- Ownership/Update:
  - Pose application lives in `src/character/keyframe.cpp` via `apply_pose()` and `apply_pose_with_overrides()`.
  - Manual override path is driven from UI in `src/gui/character_panel.cpp` and applied in `src/app/game_world.cpp` before secondary motion and transform propagation.
  - Procedural automatic animation uses `animation_state::update_skeletal_animation()` and is unaffected by new enum values unless selected in manual mode.
- References:
  - `src/character/keyframe.h`: declares `enum class pose_type { T_POSE, STEP_LEFT, NEUTRAL, STEP_RIGHT }` and pose APIs.
  - `src/character/keyframe.cpp`: defines pose creation functions, `get_keyframe_data(pose_type)`, `apply_pose(...)`, and `apply_pose_with_overrides(...)`.
  - `src/gui/character_panel.cpp`: manual pose dropdown (`pose_names[]`, `ImGui::Combo`) assumes four entries; needs extension.
  - `src/app/game_world.cpp`: selects manual vs automatic path; passes `panel_state.selected_pose` into animation or directly into `apply_pose_with_overrides`.
- Conflicts/Duplicates: None found. Single `t_pose_skeleton` instance in `game_world`; pose functions centralized. Manual UI already resets overrides on pose change.
- Mode Differences: Automatic animation's current-pose display switch only lists the walk poses; unchanged for this feature (manual-only exposure for run poses).

---

### Graybox Implementation Checklist

#### 1) Data Structures & Enum

- [x] File: `src/character/keyframe.h`
  - [x] Rename existing walk pose enum values for clarity:
    - `STEP_LEFT` → `WALK_REACH_LEFT`
    - `NEUTRAL` → `WALK_PASS_RIGHT` 
    - `STEP_RIGHT` → `WALK_REACH_RIGHT`
    - Add `WALK_PASS_LEFT` (same as current `NEUTRAL`, completes walk cycle symmetry)
  - [x] Add new running gait enum values:
    - `RUN_REACH_LEFT`, `RUN_PASS_RIGHT`, `RUN_REACH_RIGHT`, `RUN_PASS_LEFT`
  - [x] Final enum order: `T_POSE`, `WALK_REACH_LEFT`, `WALK_PASS_RIGHT`, `WALK_REACH_RIGHT`, `WALK_PASS_LEFT`, `RUN_REACH_LEFT`, `RUN_PASS_RIGHT`, `RUN_REACH_RIGHT`, `RUN_PASS_LEFT`
- Files touched (Step 1): `src/character/keyframe.h`

#### 2) Pose Data (Keyframes)

- [x] File: `src/character/keyframe.cpp`
  - [x] Rename existing walk pose creation functions:
    - `create_step_left_pose()` → `create_walk_reach_left_pose()`
    - `create_neutral_pose()` → `create_walk_pass_right_pose()`
    - `create_step_right_pose()` → `create_walk_reach_right_pose()`
  - [x] Add `create_walk_pass_left_pose()` (duplicate of current neutral/pass_right for symmetry)
  - [x] Add run pose creation functions (axes match walking; larger magnitudes for clear silhouettes):
    - `static keyframe create_run_reach_left_pose()`
    - `static keyframe create_run_pass_right_pose()`
    - `static keyframe create_run_reach_right_pose()`
    - `static keyframe create_run_pass_left_pose()`
  - [x] Initial seed: mirror walking structure with ~1.5x arm swing and leg extension; elbows/knees slightly more bent at REACH, closer to straight at PASS. Tune via UI.
  - [x] Update `keyframe get_keyframe_data(pose_type pose)` switch to return all renamed walk poses and new run poses.
  - [x] Update base-pose switch in `apply_pose_with_overrides(...)` to recognize all renamed walk enum cases and new run enum cases.

- [x] File: `src/character/animation.cpp`
  - [x] Update automatic animation's pose selection logic to use renamed walk enum values:
    - All references to `STEP_LEFT` → `WALK_REACH_LEFT`
    - All references to `NEUTRAL` → `WALK_PASS_RIGHT`
    - All references to `STEP_RIGHT` → `WALK_REACH_RIGHT`
  - [x] Update phase-to-pose mapping (~6 switch cases in `update_skeletal_animation()`).

- Files touched (Step 2): `src/character/keyframe.cpp`, `src/character/animation.cpp`, `src/character/animation.h`

#### 3) Manual Pose UI (Debug Panel)

- [x] File: `src/gui/character_panel.cpp`
  - [x] Update manual pose dropdown `pose_names[]` to reflect renamed walk poses:
    - "T-Pose", "Walk – Reach Left", "Walk – Pass Right", "Walk – Reach Right", "Walk – Pass Left"
  - [x] Add 4 new run entries in this cycle order:
    - "Run – Reach Left", "Run – Pass Right", "Run – Reach Right", "Run – Pass Left"
  - [x] Update the `ImGui::Combo` item count from 4 to 9 (T-Pose + 4 walk + 4 run), keeping enum order alignment with `pose_type`.
  - [x] Preserve existing behavior: reset joint overrides on pose change; keep joint override widgets and secondary motion toggles unchanged.

- Files touched (Step 3): `src/gui/character_panel.cpp`

#### 4) Integration Order

- [x] Verify call order remains unchanged in `src/app/game_world.cpp`:
  - [x] Apply pose (manual or automatic) -> apply secondary motion -> update global transforms.
  - [x] No changes required to locomotion/orientation systems.

- Files touched (Step 4): _(none)_

#### 5) Smoke Test Loop

- [x] Build (Debug) and run.
- [x] In Character Panel → Skeleton:
  - [x] Enable "Manual Pose Selection".
  - [x] Cycle through the 4 new run poses; confirm distinct silhouettes vs walk poses.
  - [x] Enable "Enable Joint Overrides"; adjust shoulder/hip angles to tune run poses.
  - [x] Toggle secondary motion on/off to verify order (pose first, then lag).

#### 6) Quality Gates

- [x] Configure/Build Debug (CMake tasks).
- [x] clang-format/clang-tidy clean (existing tasks).
- [x] Smoke run launches; manual run poses selectable and adjustable via joint overrides.

---

### Notes & Rationale

- Gameplay-first: manual selection preserves input→acceleration mapping; animation remains non-authoritative.
- Interpolation: no blending/speed switching in this feature; run keyframes are discrete for graybox testing.
- IK/secondary motion untouched; run poses applied via existing `apply_pose()`/`apply_pose_with_overrides()` flow.

### Changelog

- 2025-10-13: Initial plan created from branch `feature/running-gait-keyframes` and `PLANS/ARCHIVE/20251012_224839_running_gait_keyframes_FEATURE.md`.
- 2025-10-13: Walk pose enum rename un-deferred per user request. Scope now includes: rename `STEP_LEFT/NEUTRAL/STEP_RIGHT` → `WALK_REACH_LEFT/WALK_PASS_RIGHT/WALK_REACH_RIGHT`, add `WALK_PASS_LEFT`, update all references in keyframe.cpp, animation.cpp, character_panel.cpp. Total enum count: 9 (T_POSE + 4 walk + 4 run).

---

## Approval

**Reviewer:** Frog-Claude  
**Date:** 2025-10-13  
**Status:** APPROVED

Implementation complete and reviewed. All checklist items verified. Code quality meets standards. See `PLANS/ARCHIVE/20251012_224839_running_gait_keyframes_CODE_REVIEW.md` for detailed review.
