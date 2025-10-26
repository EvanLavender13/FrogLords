# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Last Updated:** 2025-10-26

---

## Active Violations

### Critical Priority

None - All critical violations resolved ✅

### High Priority

None - All high priority violations resolved ✅

### Medium Priority

**Ground Geometry Duplication**
- **Location:** `src/app/game_world.cpp:161,167`
- **Principle:** Single Source of Truth
- **Severity:** Medium
- **Description:** Visual floor is 40x40 (`generate_grid_floor(40.0f, 40)`) while collision ground is 200x200 (half_extents 100x100). Player can walk beyond visible world.
- **Fix:** Simplify - Derive both from single constant or generate collision from visual dimensions
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

**Unused debug_text::font_size**
- **Location:** `src/rendering/debug_primitives.h:40`
- **Principle:** Radical Simplicity
- **Severity:** Low
- **Description:** `font_size` member never consulted when drawing text. Dead state in every debug text record.
- **Fix:** Delete - Remove member or wire through if sizing needed
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
