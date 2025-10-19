# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Foundation:** 99%+ ✅ TARGET EXCEEDED
**Last Updated:** 2025-10-19

---

## Active Violations

### Critical Priority

**#12: Collision Face Normal Fallback Enables Wall Climbing**
- **Location:** `src/foundation/collision.cpp:35-37`
- **Principles:** Mathematical Foundations, Emergent Behavior
- **Severity:** Critical
- **Type:** Special case
- **Description:** When sphere is deeply penetrated into box (low frame rate, teleport), `compute_face_normal` cannot classify the contact and returns `math::UP` as fallback. This makes wall contacts degrade into floor normals, allowing player to stick to or climb walls instead of sliding off. Breaks physics truth layer and destabilizes emergent movement.
- **Fix:** Delete - Compute real face normal from penetration geometry (largest axis, box bounds). Never return arbitrary UP.
- **Impact:** Foundation stability - enables physics-breaking wall climbing exploit
- **Audit Source:** **Codex** (High severity, elevated to Critical)

**#13: Camera Basis NaN Propagation**
- **Location:** `src/camera/camera.cpp:12,21`
- **Principles:** Mathematical Foundations, Consistency
- **Severity:** Critical
- **Type:** Unjustified decision
- **Description:** `get_forward_horizontal()` normalizes `center - eye_pos` without zero-length check (line 12). `get_right()` normalizes cross product without validating input (line 21). If eye and target coincide or forward collapses to zero, both functions output NaNs. NaNs flow into movement input (camera-relative axes), corrupting player velocity and violating mathematical guarantees.
- **Fix:** Simplify - Guard against zero-length vectors (return fallback, keep previous basis, or assert). Add degenerate case test.
- **Impact:** Foundation stability - NaN corruption of player velocity
- **Audit Source:** **Codex** (High severity, elevated to Critical)

### High Priority

None - All high-priority violations resolved ✅

### Medium Priority

**#14: Mouse Scroll Events Overwrite Instead of Accumulate**
- **Location:** `src/input/input.cpp:75`
- **Principles:** Consistency
- **Severity:** Medium
- **Type:** Lost responsiveness
- **Description:** Scroll events overwrite `mouse_scroll_delta_y` instead of accumulating. Multiple wheel ticks in a single frame reduce to only the last one. Players lose chunks of intended zoom input, undermining "same input → same outcome" contract.
- **Fix:** Simplify - Accumulate scroll deltas or process events immediately.
- **Impact:** Input loss, unpredictable camera zoom response
- **Audit Source:** **Codex**

**#15: Input Scaling Outside Camera System**
- **Location:** `src/app/runtime.cpp:76-77,83`
- **Principles:** Consistency, Composable Functions
- **Severity:** Medium
- **Type:** Lost responsiveness
- **Description:** Mouse delta and scroll are scaled by magic `0.5f` before passing to camera system. This violates "Input is Intent" - input should be pure player intent, scaling should live in camera system as tunable parameters. Makes camera feel less responsive and harder to tune.
- **Fix:** Delete - Remove scaling from input processing. Move sensitivity parameters into camera system.
- **Impact:** Camera tuning difficulty, responsiveness perception
- **Audit Source:** **Gemini**

### Low Priority

**#16: Unused Controller State Fields**
- **Location:** `src/character/controller.h:59-60`
- **Principles:** Radical Simplicity
- **Severity:** Low
- **Type:** Complexity
- **Description:** `ground_normal` and `ground_height` are maintained but never read anywhere. They duplicate derived state without consumers, bloating controller state and inviting future divergence from single source of truth.
- **Fix:** Delete - Remove unused fields or justify their existence.
- **Impact:** Code clarity, state bloat
- **Audit Source:** **Codex**

**#9: Magic Number for Collision Passes**
- **Location:** `src/foundation/collision.cpp:136`
- **Principles:** Mathematical Foundations
- **Severity:** Low
- **Type:** Magic number (DOCUMENTED)
- **Description:** Hard-coded `for (int pass = 0; pass < 3; ++pass)` for collision resolution. Number `3` is arbitrary, lacks mathematical guarantee. NOTE: Already documented with rationale in comments (lines 133-135).
- **Fix:** Accept - Already documented with clear rationale. Consider named constant if reused elsewhere.
- **Impact:** Code clarity
- **Status:** Effectively resolved via documentation ✅

**#11: Hardcoded Test Level Literals**
- **Location:** `src/app/game_world.cpp:62-111`
- **Principles:** Radical Simplicity, Emergent Behavior
- **Severity:** Low
- **Type:** Scattered literals, Special cases
- **Description:** Test arena hard-coded as scattered literals without named primitives (tile size, offsets). Every tweak requires manual edit, reads as pile of special cases rather than composable system.
- **Fix:** Simplify - Extract named constants for dimensions/offsets. OR Accept - Temporary test code, note for future data-driven/procedural system.
- **Impact:** Code clarity, maintainability (non-critical - test only)
- **Audit Source:** **Codex** + Gemini convergence

---

## Patterns Library

**Pattern detection and prevention guidance:**

@TASKS/PATTERNS.md

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
