# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Last Updated:** 2025-10-24 (Post-Coordinate System Refinement)

---

## Active Violations

### Critical Priority

None - All critical violations resolved ✅

### High Priority

**Vehicle Parameter Consolidation**
- **Location:** `src/character/tuning.h`, `src/vehicle/tuning.h`, `src/gui/character_panel.cpp`
- **Principle:** Single Source of Truth, Systems Not Features
- **Severity:** High
- **Description:** Vehicle parameters split across character and vehicle namespaces after incomplete pivot. `max_speed`, `accel`, `weight` in character::tuning_params but are vehicle physics parameters. Creates confusion and violates single source of truth for vehicle configuration.
- **Fix:** Simplify - Move all vehicle parameters to vehicle::tuning_params, remove vehicle sections from character_panel, consolidate in vehicle_panel
- **Source:** Retrospective (character→vehicle pivot)

**Missing Spring Damper Validation**
- **Location:** `src/foundation/spring_damper.cpp:5`
- **Principle:** Humble Validation
- **Severity:** High
- **Description:** `spring_damper::update` uses `delta_time` without validating it's positive and finite. Zero, negative, or NaN dt breaks simulation.
- **Fix:** Document - Add precondition assertion
- **Source:** Gemini

**Surveyor Wheel Accumulated State**
- **Location:** `src/app/debug_generation.cpp:159`
- **Principle:** Mathematical Foundation, Single Source of Truth
- **Severity:** High
- **Description:** Rotation angle uses `distance_traveled / wheel_radius`. When locomotion state changes, `wheel_radius` (derived from `cycle_length`) changes but `distance_traveled` continues accumulating, causing visual jump. Should derive from `locomotion.phase` instead.
- **Fix:** Simplify - Calculate rotation from phase: `rotation_angle = -phase * 2π`
- **Source:** Gemini

### Medium Priority

**Ground Geometry Duplication**
- **Location:** `src/app/game_world.cpp:161,167`
- **Principle:** Single Source of Truth
- **Severity:** Medium
- **Description:** Visual floor is 40x40 (`generate_grid_floor(40.0f, 40)`) while collision ground is 200x200 (half_extents 100x100). Player can walk beyond visible world.
- **Fix:** Simplify - Derive both from single constant or generate collision from visual dimensions
- **Source:** Gemini

**Spring Configuration Duplication**
- **Location:** `src/character/animation.cpp:14`, `src/character/orientation.cpp:10`
- **Principle:** Single Source of Truth
- **Severity:** Medium
- **Description:** Landing spring stiffness (400.0f) and yaw spring stiffness (25.0f) hardcoded in constructors. Critical feel parameters buried and not exposed in tuning system.
- **Fix:** Simplify - Add to `character::tuning_params`, derive damping using `critical_damping` function
- **Source:** Gemini

**Velocity Trail Time-Dependent Sampling**
- **Location:** `src/app/game_world.cpp:108`
- **Principle:** Mathematical Foundation
- **Severity:** Medium
- **Description:** Trail samples based on `time_since_last_sample` accumulated dt. Frame-rate dependent - high FPS creates dense clusters, low FPS creates sparse trail.
- **Fix:** Simplify - Sample based on distance traveled instead of time
- **Source:** Gemini

**Velocity Trail Timestamp Divergence**
- **Location:** `src/app/game_world.cpp:107`, `src/rendering/velocity_trail.h:8`
- **Principle:** Single Source of Truth, Radical Simplicity
- **Severity:** Medium
- **Description:** `timestamps` vector accumulates synthetic timeline by adding `sample_interval`. Any pause in sampling desynchronizes stored timeline from reality, creating dead state that diverges from truth.
- **Fix:** Delete - Remove timestamp vector and related maintenance
- **Source:** Codex


### Low Priority

**Redundant Tuning Parameter Application**
- **Location:** `src/app/runtime.cpp:165-180`
- **Principle:** Single Source of Truth, Radical Simplicity
- **Severity:** Low
- **Description:** `world.character_params.apply_to(world.character)` called inside loop for every parameter change, recalculating all derived data each time.
- **Fix:** Simplify - Apply all parameter changes first, make single `apply_to` call after loop
- **Source:** Gemini

**Split Camera Control Direction Logic**
- **Location:** `src/app/runtime.cpp:65`
- **Principle:** Single Source of Truth
- **Severity:** Low
- **Description:** Mouse `delta_x` negated at call site in runtime before passing to camera system. Control direction responsibility split between runtime and camera.
- **Fix:** Simplify - Remove negation from runtime, adjust `orbit_sensitivity` sign in camera_follow
- **Source:** Gemini

**Redundant GUI Camera Validation**
- **Location:** `src/gui/camera_panel.cpp:90`
- **Principle:** Radical Simplicity
- **Severity:** Low
- **Description:** GUI displays warning if `min_distance > max_distance`, but `apply_camera_commands` already enforces invariant through clamping. Warning is redundant complexity.
- **Fix:** Delete - Remove warning logic and `invariant_valid` check
- **Source:** Gemini

**Hardcoded Input Toggles**
- **Location:** `src/app/game_world.cpp:25`, `src/app/runtime.cpp:90`
- **Principle:** Systems, Not Features
- **Severity:** Low
- **Description:** 'T' key hardcoded for control scheme toggle, 'F3' for debug viz toggle. Feature-centric approach. Should use generic input binding system.
- **Fix:** Document - Acceptable at current scale, add TODO for future input binding system
- **Source:** Gemini

**Unused contact_debug_info Fields**
- **Location:** `src/character/controller.h:35-40`, `src/character/controller.cpp:166`
- **Principle:** Radical Simplicity, Single Source of Truth
- **Severity:** Low
- **Description:** Fields `from_box`, `penetration`, `vertical_penetration` defined but never read. Only `penetration` written (line 166). Dead state duplicating collision data.
- **Fix:** Delete - Remove unused fields
- **Source:** Codex

**Unused debug_text::font_size**
- **Location:** `src/rendering/debug_primitives.h:40`
- **Principle:** Radical Simplicity
- **Severity:** Low
- **Description:** `font_size` member never consulted when drawing text. Dead state in every debug text record.
- **Fix:** Delete - Remove member or wire through if sizing needed
- **Source:** Codex

**Unmutated show_velocity_trail Flag**
- **Location:** `src/gui/character_panel.h:14`, `src/app/debug_generation.cpp:345`
- **Principle:** Radical Simplicity
- **Severity:** Low
- **Description:** `show_velocity_trail` flag never mutated (no UI hook), always true. Guard at line 345 is effectively hardcoded true.
- **Fix:** Delete - Remove flag or add missing toggle widget
- **Source:** Codex

**Excessive Function Arguments**
- **Location:** `src/gui/gui.cpp:108`
- **Tool:** lizard
- **Severity:** Low
- **Issue:** plot_histogram takes 6 arguments (threshold 5)
- **Fix:** Group parameters into config struct or accept for utility functions
- **Impact:** API usability

**Magic Number for Collision Passes**
- **Location:** `src/foundation/collision.cpp:136`
- **Principles:** Mathematical Foundations
- **Severity:** Low
- **Type:** Magic number (DOCUMENTED)
- **Description:** Hard-coded `for (int pass = 0; pass < 3; ++pass)` for collision resolution. Number `3` is arbitrary, lacks mathematical guarantee. NOTE: Already documented with rationale in comments (lines 133-135).
- **Fix:** Accept - Already documented with clear rationale. Consider named constant if reused elsewhere.
- **Impact:** Code clarity
- **Status:** Effectively resolved via documentation ✅


---

## Patterns Library

**Pattern detection and prevention guidance:**

@TASKS/PATTERNS.md

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
