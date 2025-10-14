# Implementation Plan: Run Gait Blending (2D Pose Blending)

**Complexity:** 3 points (feature)

## Foundation Architecture Review

Feature: Run gait blending (bilinear slerp between walk/run keyframes across gait phase)

Systems Involved: skeleton, animation_state, keyframe poses, locomotion_system, character_panel (debug)

Current State Audit:
- skeleton:
  - Owned by: `game_world.t_pose_skeleton` (single instance)
  - Rendered from: `src/app/runtime.cpp` via debug draw helpers
- animation_state:
  - Distance-phased update implemented in `animation_state::update_skeletal_animation()`
  - Currently blends WALK poses only (4 segments, 1D along phase)
  - Applies contact-weight spring for air/ground amplitude
- keyframe poses:
  - `pose_type` includes WALK_ and RUN_ variants (8 total)
  - `get_keyframe_data()` returns both walk and run keyframes
- locomotion_system:
  - Computes `phase` (0..1) from surveyor-wheel distance
  - Holds `smoothed_speed`, `walk_speed_threshold`, `run_speed_threshold`
  - Blends stride and body bounce internally using a speed-based blend (not exposed)
- controller/orientation:
  - Not directly involved beyond providing speed/phase via `game_world`

Architecture Issues/Risks:
1. `animation_state::update_skeletal_animation()` lacks access to walk/run blend factor (aka `walk_factor`).
2. Locomotion's speed-blend is not exposed; must replicate or add a small getter.
3. Skeleton ownership lives in `game_world` (OK for current scope). No duplicate skeletons detected.

Migration/Consolidation Plan:
- Keep skeleton and locomotion ownership in `game_world` for this feature (no move).
- Add minimal API to pass a `walk_factor` to animation (avoid coupling).

Multi-Mode Comparison:
- N/A for this feature (camera modes unaffected).

---

## Graybox Implementation Checklist

### 0. Planning Checks
- [x] Gate Mechanism Review documented: Amplitude gating uses `contact_weight_spring`, blending uses bilinear slerp; no timeline gating introduced.
- [x] UI Ordering Verified: `pose_type` enum order matches GUI pose list.
- Files touched (Step 0): `PLANS/ARCHIVE/20251013_163313_run_gait_blending_PLAN.md`

### 1. Data Structures & State
- [x] File: `src/character/animation.h`
  - [x] Change signature of `update_skeletal_animation(...)` to accept `float walk_factor`.
    - Before: `void update_skeletal_animation(skeleton&, float distance_traveled, pose_type manual_override_pose, bool use_manual_override, float dt);`
    - After:  `void update_skeletal_animation(skeleton&, float distance_traveled, float walk_factor, pose_type manual_override_pose, bool use_manual_override, float dt);`
- Files touched (Step 1): `src/character/animation.h`, `src/character/animation.cpp`

### 2. Locomotion ? Animation Parameter
- [x] File: `src/app/game_world.cpp`
  - [x] Compute `walk_factor` from locomotion speed: `run_blend = saturate((smoothed_speed - walk_threshold) / (run_threshold - walk_threshold)); walk_factor = 1.0f - run_blend;`
  - [x] Pass `walk_factor` to `animation.update_skeletal_animation(...)`.
- [x] (Optional) File: `src/character/locomotion.h/.cpp`
  - [x] Add `float get_run_blend() const;` returning computed blend from thresholds.
  - [x] Use this in `game_world` instead of duplicating formula (prefer single source if reused elsewhere).
- Files touched (Step 2): `src/app/game_world.cpp`, `src/character/tuning.cpp`, `src/character/locomotion.h`, `src/character/locomotion.cpp`

### 3. Core Logic (Bilinear Slerp)
- [x] File: `src/character/animation.cpp`
  - [x] In `update_skeletal_animation(...)` (automatic path):
    - [x] Compute `phase` from `distance_traveled` (existing code).
    - [x] For the current segment, map WALK source/target: `[WALK_REACH_LEFT → WALK_PASS_RIGHT]`, `[WALK_PASS_RIGHT → WALK_REACH_RIGHT]`, `[WALK_REACH_RIGHT → WALK_PASS_LEFT]`, `[WALK_PASS_LEFT → WALK_REACH_LEFT]`.
    - [x] Mirror the mapping for RUN poses.
    - [x] Horizontal blends: `walk_blended = slerp(walk_src, walk_tgt, t)` and `run_blended = slerp(run_src, run_tgt, t)` per joint.
    - [x] Vertical blend: `final = slerp(walk_blended, run_blended, 1.0f - walk_factor)` per joint.
    - [x] Amplitude gating: `final_with_contact = slerp(identity, final, contact_weight)`.
    - [x] Apply to joints preserving T-pose translations, then restore root transform.
    - [x] Maintain manual override fast-path unchanged.
- Files touched (Step 3): `src/character/animation.cpp`

Pseudocode (per joint):
```
quat walk_blend = slerp(walk_src, walk_tgt, t);
quat run_blend  = slerp(run_src,  run_tgt,  t);
quat final      = slerp(walk_blend, run_blend, 1.0f - walk_factor);
quat posed      = slerp(identity, final, contact_weight);
apply_to_joint(joint_idx, posed);
```

### 4. Debug UI (Optional but helpful)
- [x] File: `src/gui/character_panel.h/.cpp`
  - [x] Add enum/select for Blend Mode: `Walk Only`, `Mixed (Speed)`, `Run Only`.
  - [x] When `Walk Only`, force `walk_factor = 1.0f`; when `Run Only`, force `walk_factor = 0.0f`; otherwise compute from locomotion.
  - [x] Display read-only `Walk Factor` and `Contact Weight` values for tuning.
- [x] Tuning: Added exponential smoothing to `walk_factor` to improve visual transition feel.
- Files touched (Step 4): `src/gui/character_panel.h`, `src/gui/character_panel.cpp`, `src/app/game_world.cpp`, `src/app/game_world.h`

### 5. Quality Gates
- [x] Build passes (Debug) via CMake target.
- [x] Lint/format clean (clang-format/clang-tidy). Follow 4-space indent, snake_case.
- [x] Smoke run launches; pose transitions continuous across speed changes; no pops.

---

## File-Level Change List

- `src/character/animation.h/.cpp`: add `walk_factor` param; implement bilinear slerp; keep manual override.
- `src/app/game_world.cpp`: compute `walk_factor` from locomotion thresholds and pass through.
- `src/character/locomotion.h/.cpp` (optional): provide `get_run_blend()` to centralize threshold logic.
- `src/gui/character_panel.h/.cpp` (optional): debug blend mode and telemetry.

## Risks and Mitigations
- Visual transition harshness at threshold: use smoothed speed (`smoothed_speed`) and `contact_weight` gating; can add mild exponential smoothing on `walk_factor` only if needed.
- Quaternion hemisphere flips: preserve existing shortest-path handling (use `glm::slerp` and maintain per-joint consistency by not negating inputs between passes).
- API touchpoints: single signature change with one call site (`game_world.cpp`).

## Notes
- No content restrictions introduced. No control latency added.
- Preserves distance-driven gait phase (surveyor-wheel) and air/ground continuity.
- Secondary motion remains layered on top; unaffected by blend change.

---

## Approval

**Reviewer:** Gemini
**Date:** 2025-10-13