# Implementation Plan: Speed-Based Animation Scaling

### Graybox Implementation Checklist

#### 1. Data Structures & State

- [ ] **File:** `src/character/animation.h`
    - [ ] Add `float speed_scaling_factor = 0.1f;` to the `animation_state` struct.

#### 2. Core Logic

- [ ] **File:** `src/character/animation.cpp`
    - [ ] In `update_acceleration_tilt()`, modify the `effective_tilt_magnitude` calculation.
    - [ ] Change the line: `float effective_tilt_magnitude = tilt_magnitude * (0.5f + velocity_scale * 1.0f);`
    - [ ] To: `float effective_tilt_magnitude = tilt_magnitude * (1.0f + velocity_magnitude * speed_scaling_factor);`

#### 3. Debug UI

- [ ] **File:** `src/gui/character_panel.cpp`
    - [ ] In the "Acceleration Tilt" section, add a new `ImGui::SliderFloat` for "Speed Scaling Factor".
    - [ ] Add the slider after the "Tilt Magnitude (rad)" slider.
    - [ ] `gui::widget::slider_float("Speed Scaling Factor", &character.animation.speed_scaling_factor, 0.0f, 1.0f);`

#### 4. Quality Gates

- [ ] Build passes (`CMake: Build (Debug)`).
- [ ] Lint/format clean (run clang-format/clang-tidy tasks).
- [ ] Smoke run compiles and launches (`Run (Debug)`), and feature toggle works.

## Deferral

**Date:** 2025-10-06

**Reason:** Current constant magnitude feels good; no evidence of problem requiring speed scaling.

**Evidence Gap:** No documented gameplay moments where constant tilt magnitude fails (e.g., "tilt too subtle at max speed").

**Reconsideration Criteria:** After playtesting identifies specific moments where tilt magnitude is inadequate at high speeds or distracting at low speeds.

**Review:** See `PLANS/ARCHIVE/20251006_120000_plan_review_speed_animation_scaling.md` for full analysis
