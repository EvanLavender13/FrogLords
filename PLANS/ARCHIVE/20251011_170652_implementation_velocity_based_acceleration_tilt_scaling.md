# Implementation Plan: Velocity-Based Acceleration Tilt Scaling

### Graybox Implementation Checklist

#### 1. Animation API Update

- [x] **File:** `src/character/animation.h`
    - [x] Update the `update_acceleration_tilt` declaration to document the parameter as a reference speed rather than the current `max_speed`.
    - [x] (Optional rename for clarity) Adjust the argument name to `reference_speed` to match its new role.
- [x] **File:** `src/character/animation.cpp`
    - [x] Mirror the signature/argument rename in the definition.
    - [x] Replace the `velocity_scale` calculation with logic that divides by a clamped reference speed:
        ```
        float ref_speed = glm::max(reference_speed, 0.01f);
        float velocity_scale = glm::clamp(velocity_magnitude / ref_speed, 0.0f, 1.0f);
        ```
      ensuring we never divide by values near zero.
    - [x] Update inline comments to reflect that scaling is based on absolute speed rather than the smoothed max-speed state.

**Files touched (Step 1):** `src/character/animation.h`, `src/character/animation.cpp`

#### 2. Runtime Call Site

- [x] **File:** `src/app/game_world.cpp`
    - [x] Pass `character.run_speed` (or a guarded copy) into `update_acceleration_tilt` in place of `character.max_speed`.
    - [x] Keep the call positioned between `update_landing_spring` and locomotion syncing, matching the feature description.
- [x] **File:** `src/character/animation.cpp`
    - [x] Ensure the legacy `animation_state::update(const animation_update_params&)` helper calls `update_acceleration_tilt` with an explicit reference speed (reuse the 8.0f constant or introduce a struct field if future callers need custom values).

**Files touched (Step 2):** `src/app/game_world.cpp`, `src/character/animation.cpp`

#### 3. Documentation & Cleanup

- [x] **File:** `PLANS/ARCHIVE/20251011_170652_feature_velocity_based_acceleration_tilt_scaling.md`
    - [x] After coding, annotate the checklist outcome (line count, guard behaviour) if new insights emerge during implementation.
    - [x] Capture new insight: log a possible future backlog item to try a non-smoothed speed ring (pure velocity magnitude) and an acceleration-driven ring to visualize tilt input sources.

**Files touched (Step 3):** `PLANS/ARCHIVE/20251011_170652_feature_velocity_based_acceleration_tilt_scaling.md`

### Quality Gates

- [x] Build succeeds (`cmake --build` or existing project build task).
- [x] Smoke run: launch the sandbox build, toggle walk/run, and confirm tilt magnitude now tracks absolute speed (subtle lean while walking, stronger lean while running).
- [x] No new warnings or regressions in debug UI sliders (run speed adjustments remain responsive).

## Approval

- Reviewer: Codex (GPT-5)
- Date: 2025-10-11

