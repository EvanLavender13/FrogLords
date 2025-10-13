# Refactor Backlog

**Purpose:** Tracking for architectural improvements, pattern extraction, and system simplification opportunities that improve code quality and maintainability.

**Status:** Living document (updated after system analysis)

**Last Review:** October 12, 2025

---

## Guidance

- **Rule-of-three exceptions:** Canonical data consolidation (e.g., skeleton joint indices, global constants) can be justified without 3+ occurrences when it establishes a single source of truth and systems are ≥70% certainty. Document the rationale explicitly in the plan.

## Critical

(Blocking new work, major technical debt, or architectural flaws)

*No items currently tracked.*

---

## High

(Significant improvement to clarity, maintainability, or extensibility)

### Extract Walk/Run Blend Factor Calculation (Locomotion)
- **Category:** Pattern Extraction
- **Files:** `src/character/locomotion.cpp` (blend calc in `update()` and `get_current_pose()`)
- **Current State:** Identical blend factor calculation duplicated in two methods
- **Proposed Change:** Extract `compute_walk_run_blend()` private method
- **Rationale:** Abstract repeated patterns; inevitable third use as locomotion expands
- **Impact:** -12 LOC, single source of truth, clearer intent
- **Risk:** Very Low (pure calculation)
- **Certainty:** Locomotion system ~95% certain per dependency stack

---

## Medium

(Nice-to-have refactors that would improve quality)

### Extract Joint Transform Application (Animation/Skeleton)
- **Category:** Pattern Extraction
- **Files:** `src/character/keyframe.cpp`, `src/character/animation.cpp`
- **Current State:** Repeated lambdas `apply_joint` / `apply_blended_joint`
- **Proposed Change:** Add `set_joint_rotation(skeleton&, int, const glm::quat&)` in `skeleton.h`
- **Rationale:** Core skeletal operation; naming clarifies intent and ensures consistency
- **Impact:** -20–30 LOC; removes 4 lambdas
- **Risk:** Very Low (inline helper)

### Extract Buffer Pruning Helper (GUI)
- **Category:** Pattern Extraction
- **Files:** `src/gui/gui.cpp` (plots)
- **Current State:** Identical buffer prune logic in `plot_value()` and `plot_histogram()`
- **Proposed Change:** `prune_plot_buffer(buffer, current_time, max_samples)` internal helper
- **Rationale:** Deduplicate identical maintenance code
- **Impact:** -16 LOC; easier to extend
- **Risk:** Low (internal helper)

### Extract Speed/Age Gradient Helpers (Debug Visualization)
- **Category:** Pattern Extraction
- **Files:** `src/rendering/debug_draw.cpp`
- **Current State:** Two multi-stop gradients hand-coded (speed ring, trail age)
- **Proposed Change:** Add `evaluate_gradient()` to `foundation/easing.h` + presets
- **Rationale:** Parameters over assets; likely 3rd+ use soon
- **Impact:** Clearer debug code; avoids future copy/paste
- **Risk:** Low (pure visualization)

### Consolidate Tuning Constants (Character Controller)
- **Category:** Simplification
- **Files:** `src/character/tuning.cpp`
- **Current State:** `FRICTION_RATIO = 0.75f` duplicated in two functions
- **Proposed Change:** Move to file-level anonymous namespace
- **Rationale:** Single source of truth; aligns with constant scoping convention
- **Impact:** Clearer coupling between `apply_to` and `read_from`
- **Risk:** Very Low (no behavior change)

### Simplify Velocity Clamping in Controller
- **Category:** Simplification
- **Files:** `src/character/controller.cpp`
- **Current State:** Two blocks extract/modify/reassign horizontal velocity
- **Proposed Change:** Local helper `clamp_horizontal_speed(glm::vec3&, float)`
- **Rationale:** Reduce duplication; improves readability
- **Impact:** -10 LOC; two call sites
- **Risk:** Low

### Consider Rotation Matrix Helper for Yaw
- **Category:** API Design (defer until rule-of-three)
- **Files:** `controller.cpp`, `debug_draw.cpp`
- **Current State:** Two uses of `glm::rotate(..., yaw, up)`
- **Proposed Change:** Add `math::rotation_y(float)` when 3rd use appears
- **Rationale:** Keep API small until justified
- **Impact:** N/A until threshold met
- **Risk:** Low

---

## Low

(Polish items with minimal impact)

### Simplify Box Collision Classification (Debug)
- **Category:** Simplification
- **Files:** `src/rendering/debug_draw.cpp`
- **Current State:** Deeply nested if-else chain for box types
- **Proposed Change:** `classify_collision_box()` + `box_color()` using enum
- **Rationale:** Clarity over cleverness; isolates heuristics
- **Impact:** Similar LOC; improved organization
- **Risk:** Low (debug-only)

### Break Up Long Function: draw_character_panel
- **Category:** Simplification
- **Files:** `src/gui/character_panel.cpp`
- **Current State:** ~200-line function with multiple collapsing sections
- **Proposed Change:** Split into section helpers (tuning, spring, tilt, skeleton, overrides)
- **Rationale:** Easier navigation and iteration
- **Impact:** Readability; internal refactor
- **Risk:** Low

### Add Exponential Decay Utility (Foundation)
- **Category:** Reusable Utilities
- **Files:** `animation.cpp`, `controller.cpp`
- **Current State:** Two decay patterns: `1 - exp(-rate*dt)` and `min(rate*dt, 1)`
- **Proposed Change:** `exponential_decay(rate, dt)` + `exponential_decay_clamped(rate, dt)`
- **Rationale:** Name the intent; unify smoothing
- **Impact:** Clearer semantics across systems
- **Risk:** Very Low (pure math)

### Extract Velocity Trail to Dedicated Type
- **Category:** System Design / Simplification
- **Files:** `src/app/game_world.*`
- **Current State:** Inline ring-buffer logic in `game_world::update()`
- **Proposed Change:** `struct velocity_trail { void update(...); }`
- **Rationale:** Single responsibility; reuse for other entities
- **Impact:** Cleaner world update; minor abstraction overhead
- **Risk:** Low; Defer until second user (rule-of-three)

---

## Deferred

(Opportunities waiting on 3rd use or stability evidence)

### Quaternion Hemisphere Alignment Utility
- **Reason:** Only 1 occurrence currently
- **Reconsider When:** Second use appears (e.g., more quaternion blending)
- **Proposed:** `math::quaternion_hemisphere_align()` in `foundation/math_utils.h`

### Renderer Buffer Caching Optimization
- **Reason:** Profile first; current simplicity wins
- **Reconsider When:** Profiling shows buffer creation hot path
- **Files:** `src/rendering/renderer.cpp`
- **Note:** Cache unit meshes if/when justified by data
