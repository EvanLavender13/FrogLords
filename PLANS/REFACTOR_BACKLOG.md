# Refactor Backlog

**Purpose:** Tracking for architectural improvements, pattern extraction, and system simplification opportunities that improve code quality and maintainability.

**Status:** Living document (updated after system analysis)

**Last Review:** October 12, 2025

---

## Critical

(Blocking new work, major technical debt, or architectural flaws)

*No items currently tracked.*

---

## High

(Significant improvement to clarity, maintainability, or extensibility)

### Extract Walk/Run Blend Factor Calculation (Locomotion)
- **Category:** Pattern Extraction
- **Files:** `src/character/locomotion.cpp` (lines 26-34, 59-67)
- **Current State:** Identical blend factor calculation duplicated in `update()` and `get_current_pose()`
- **Proposed Change:** Extract `compute_walk_run_blend()` private method
- **Rationale:** "Abstract repeated patterns into systems" - Identical logic appearing twice with 100% certainty of third use (any new animation blending will need this).
- **Impact:** -12 lines, clearer intent, single source of truth for walk/run blending
- **Risk:** Very Low (private method, pure calculation)
- **Certainty:** Locomotion system is 95% certain per DEPENDENCY_STACK.md
- **Examples:**
  ```cpp
  // Current (2x duplicated):
  float blend = 0.0f;
  if (smoothed_speed <= walk_speed_threshold) {
      blend = 0.0f;
  } else if (smoothed_speed >= run_speed_threshold) {
      blend = 1.0f;
  } else {
      blend = (smoothed_speed - walk_speed_threshold) /
              (run_speed_threshold - walk_speed_threshold);
  }

  // Proposed:
  float blend = compute_walk_run_blend();
  ```

### Consolidate Duplicate Constants (WHEEL_RADIUS, TWO_PI) ❌ INVALID
- **Category:** Pattern Extraction
- **Files:** `src/rendering/debug_draw.cpp`, `src/app/game_world.cpp`
- **Current State:** ~~Identical constants duplicated in two files~~ **INVALID: Constants already centralized in `src/character/locomotion.h` and properly included.**
- **Verification:** Both `debug_draw.h` and `game_world.h` include `character/locomotion.h`, giving access to shared constants. No duplication exists.
- **Action:** Remove from backlog. Mark completed (already done).
- **Date Invalidated:** 2025-10-10

---

## Medium

(Nice-to-have refactors that would improve quality)

### Extract Joint Transform Application Lambda (Animation/Keyframe)
- **Category:** Pattern Extraction
- **Files:** `src/character/keyframe.cpp` (lines 180-186, 246-250), `src/character/animation.cpp` (lines 166-172, 247-253)
- **Current State:** Lambda pattern repeated 4 times (8+ individual calls): `apply_joint` / `apply_blended_joint`
- **Proposed Change:** Add to `character/skeleton.h`: `void set_joint_rotation(skeleton& skel, int joint_idx, const glm::quat& rotation)`
- **Rationale:** "Abstract repeated patterns into systems" - This is skeletal animation's fundamental operation, appearing 8+ times across 4 lambdas. Making it a named function clarifies intent and ensures consistency.
- **Impact:** -24 lines, eliminates 4 lambdas, clearer skeletal API
- **Risk:** Very Low (inline function, no behavior change)
- **Certainty:** Skeletal animation is 100% complete per DEPENDENCY_STACK.md

### Extract Buffer Pruning Helper (GUI)
- **Category:** Pattern Extraction
- **Files:** `src/gui/gui.cpp` (lines 110-120, 159-169)
- **Current State:** Identical buffer pruning logic duplicated in `plot_value()` and `plot_histogram()`
- **Proposed Change:** Extract `prune_plot_buffer(buffer, current_time, max_samples)` internal helper
- **Rationale:** "Abstract repeated patterns into systems" - Identical buffer management appearing twice in similar contexts.
- **Impact:** -16 lines, single source of truth for buffer pruning
- **Risk:** Low (internal helper, no API change)
- **Certainty:** Debug tooling is 100% complete per DEPENDENCY_STACK.md

### Extract Speed Gradient Calculation (Debug Visualization)
- **Category:** Pattern Extraction
- **Files:** `src/rendering/debug_draw.cpp` (lines 42-56, 393-403)
- **Current State:** Two instances of multi-stop gradient interpolation (speed gradient, age gradient)
- **Proposed Change:** Add to `foundation/easing.h`: `evaluate_gradient()` and `speed_gradient()` preset
- **Rationale:** "Parameters over assets" + "Wait for third use before abstracting" - This is close to third use (2 gradients + likely more for future debug vis). Extracting now prevents copy-paste when adding new visualizations.
- **Impact:** Clearer debug code, easier to add new gradients, -20 lines when third use appears
- **Risk:** Low (completed feature, pure visualization)
- **Certainty:** Debug tooling is 100% complete per DEPENDENCY_STACK.md

### Consolidate Tuning Constants (Character Controller)
- **Category:** Simplification
- **Files:** `src/character/tuning.cpp` (lines 12, 26)
- **Current State:** Magic constant `FRICTION_RATIO = 0.75f` duplicated in two functions (function-local scope)
- **Proposed Change:** Move to file-level anonymous namespace
- **Rationale:** "Clarity over cleverness" - Single source of truth for physics constants. Function-local constexpr is unnecessarily hidden.
- **Impact:** Clearer coupling between apply_to/read_from, single source of truth
- **Risk:** Very Low (same values, just moved to wider scope)
- **Certainty:** Character controller is 95% certain per DEPENDENCY_STACK.md

### Add Verbose Wireframe Drawing Helpers (Debug)
- **Category:** API Design
- **Files:** `src/rendering/debug_draw.cpp` (lines 20-23, 28-31, 58-61, 67-70, etc. - 10+ repetitions)
- **Current State:** Verbose pattern repeated for every instanced mesh (create mesh, set position/scale, draw)
- **Proposed Change:** Add helpers to `draw_context`: `draw_sphere()`, `draw_circle()`, etc.
- **Rationale:** "Simplicity over sophistication" - This pattern appears 10+ times in debug_draw.cpp. Reducing verbosity improves readability.
- **Impact:** -40 lines in debug_draw.cpp, clearer debug visualization code
- **Risk:** Low (internal debug helpers)
- **Certainty:** Debug tooling is 100% complete per DEPENDENCY_STACK.md

### Extract Y-Axis Up Vector Constant ✅
- **Completed:** 2025-10-11
- **Category:** Pattern Extraction
- **Files:** `src/foundation/math_utils.h`, `src/character/controller.cpp`, `src/character/animation.cpp`, `src/foundation/collision.cpp`, `src/rendering/debug_draw.cpp`, `src/rendering/wireframe.cpp`, `src/camera/camera.cpp`, `src/camera/camera.h`
- **Outcome:** Introduced `math::UP` constexpr and replaced 14 duplicate Y-up literals across core gameplay, rendering, and foundation systems.
- **Learnings:**
  - Regex scans can miss contextual literals (constructor defaults, helper locals); include targeted searches when enumerating call sites.
  - Centralizing axis constants clarifies coordinate conventions without adding abstraction cost.
- **Impact:** Clarified world-up usage at 14 call sites; future orientation changes require a single edit.

### Extract Character-Local Space Transform Utility
- **Category:** Utilities
- **Files:** `src/character/animation.cpp:25-28`
- **Current State:** World-to-local space rotation logic embedded in acceleration tilt
- **Proposed Change:** Extract reusable utility for rotating world vectors into character-local space:
  ```cpp
  namespace character {
  inline glm::vec2 world_to_local_horizontal(const glm::vec3& world_vector, float yaw) {
      float cos_yaw = std::cos(-yaw);
      float sin_yaw = std::sin(-yaw);
      return glm::vec2(
          world_vector.x * cos_yaw + world_vector.z * sin_yaw,   // local_right
          world_vector.z * cos_yaw - world_vector.x * sin_yaw    // local_forward
      );
  }
  }
  ```
- **Rationale:** Only 1 use currently, but will be needed for IK, foot placement, and other character-relative calculations. Premature but low-cost abstraction.
- **Impact:** 1 file currently, likely 2-3 future uses.
- **Risk:** Low — Pure math function.
- **Certainty:** Deferred until second use appears (rule of three not met).

### Simplify Velocity Clamping in Controller
- **Category:** Simplification
- **Files:** `src/character/controller.cpp:73-83, 88-93`
- **Current State:** Two similar blocks for friction deceleration and max speed capping. Both extract horizontal velocity, modify, reassign.
- **Proposed Change:** Extract helper function:
  ```cpp
  namespace {
  void clamp_horizontal_speed(glm::vec3& velocity, float max_speed) {
      glm::vec3 horiz = math::project_to_horizontal(velocity);
      float speed = glm::length(horiz);
      if (speed > max_speed) {
          horiz = horiz * (max_speed / speed);
          velocity.x = horiz.x;
          velocity.z = horiz.z;
      }
  }
  }
  ```
- **Rationale:** Repeated pattern (2 uses in same function). Would simplify if horizontal_velocity utility is extracted.
- **Impact:** `controller.cpp` only. Reduces 10 lines to 2 call sites.
- **Risk:** Low — Local helper, easy to verify.
- **Certainty:** Depends on horizontal_velocity utility being extracted first.

### Consider Rotation Matrix Cache for Yaw Transforms
- **Category:** API Design
- **Files:** `src/character/controller.cpp:140`, `src/rendering/debug_draw.cpp:242`
- **Current State:** `glm::rotate(transform, yaw, glm::vec3(0, 1, 0))` pattern appears 2 times
- **Proposed Change:** If this pattern grows to 3+ uses, consider caching or extracting:
  ```cpp
  namespace math {
  inline glm::mat4 rotation_y(float angle) {
      return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
  }
  }
  ```
- **Rationale:** Only 2 uses currently. Wait for third use before extracting.
- **Impact:** 2 files currently.
- **Risk:** Low.
- **Certainty:** Deferred (rule of three not met).

---

## Low

(Polish items with minimal impact)

### Simplify Box Collision Heuristic Classification (Debug)
- **Category:** Simplification
- **Files:** `src/rendering/debug_draw.cpp` (lines 179-216)
- **Current State:** Deeply nested if-else chain (4 levels) with complex box classification heuristics
- **Proposed Change:** Extract `classify_collision_box()` and `get_box_type_color()` helpers using enum
- **Rationale:** "Clarity over cleverness" + "Simplicity over sophistication" - Breaking nested conditionals into separate classification and coloring functions improves readability and testability.
- **Impact:** Clearer intent, easier to add new box types, same LOC but better organized
- **Risk:** Low (debug visualization only, no gameplay impact)
- **Certainty:** Debug tooling is 100% complete per DEPENDENCY_STACK.md

### Make Collision Constants Visible (Controller)
- **Category:** Simplification
- **Files:** `src/character/controller.cpp` (lines 14-17)
- **Current State:** Anonymous namespace with undocumented constants (`BUMPER_RADIUS`, `STANDING_HEIGHT`)
- **Proposed Change:** Move to controller class as static constexpr with documentation
- **Rationale:** "Clarity over cleverness" - Making constants visible in header clarifies what's configurable.
- **Impact:** Better documentation, easier to find configuration values
- **Risk:** Very Low (no behavior change, just visibility)
- **Certainty:** Character controller is 95% certain per DEPENDENCY_STACK.md

### Break Up Long Function: draw_character_panel
- **Category:** Simplification
- **Files:** `src/gui/character_panel.cpp` (lines 23-220, 198 lines)
- **Current State:** Single 198-line function with 6 collapsing sections, deep nesting (4+ levels in joint override section)
- **Proposed Change:** Break into section functions: `draw_tuning_section()`, `draw_landing_spring_section()`, etc.
- **Rationale:** "Simplicity over sophistication" - Functions >50 lines are harder to understand. Each collapsing section is conceptually independent.
- **Impact:** Improved readability, easier to navigate, testable sections
- **Risk:** Low (internal refactor, no API change)
- **Certainty:** Debug tooling is 100% complete per DEPENDENCY_STACK.md

### Add Exponential Decay Utility (Foundation)
- **Category:** Reusable Utilities
- **Files:** `src/character/animation.cpp` (lines 70, 75), `src/character/controller.cpp` (line 100)
- **Current State:** Two different exponential decay patterns (`1.0f - glm::exp(-rate * dt)` and `std::min(rate * dt, 1.0f)`)
- **Proposed Change:** Add to `foundation/easing.h`: `exponential_decay(rate, dt)` and `exponential_decay_clamped(rate, dt, max_alpha)`
- **Rationale:** "Abstract repeated patterns" - Exponential decay is a fundamental smoothing operation appearing in multiple systems (95%+ certain). Naming it clarifies intent.
- **Impact:** Clearer intent, easier to find all smoothing operations
- **Risk:** Very Low (pure math function)
- **Certainty:** Both character controller and animation are stable (95%+)

### Extract Velocity Trail to Dedicated Type
- **Category:** System Design / Simplification
- **Files:** `src/app/game_world.h:16-21`, `src/app/game_world.cpp:29-45`
- **Current State:** Velocity trail logic embedded directly in `game_world::update()` with manual ring buffer management. 14 lines of inline logic.
- **Proposed Change:** Move to dedicated `velocity_trail` class with encapsulated update:
  ```cpp
  struct velocity_trail {
      void update(const glm::vec3& position, float dt);
      // existing state...
  };
  ```
- **Rationale:**
  - Single Responsibility: Trail management is distinct from world update
  - Reduces `game_world::update()` complexity
  - Makes trail reusable for other tracked entities
  - Only 1 use currently (below rule of three threshold)
- **Impact:** 2 files. Would clean up game_world but adds abstraction overhead.
- **Risk:** Low — Isolated feature with clear boundaries.
- **Certainty:** Deferred—wait until second entity needs trail tracking.

### Consolidate Anonymous Namespace Constants
- **Category:** Pattern Extraction
- **Files:** Various (e.g., `controller.cpp`, `debug_draw.cpp`, `collision.cpp`)
- **Current State:** File-local constants use anonymous namespaces inconsistently (some use constexpr at function scope)
- **Proposed Change:** Establish convention: prefer anonymous namespace for file-scope constants
- **Rationale:** Consistency improves readability. Already noted in maintenance backlog as low-severity.
- **Impact:** Multiple files. Purely stylistic.
- **Risk:** None — No behavior change.
- **Certainty:** Stable.

---

## Deferred

(Opportunities that failed "wait for third use" or stability checks)

### Quaternion Hemisphere Correction Utility
- **Reason:** Only 1 occurrence (`src/character/animation.cpp:207-210`)
- **Reconsider When:** Second use appears (e.g., blending between animation clips, quaternion interpolation elsewhere)
- **Proposed:** Add `math::quaternion_hemisphere_align()` to foundation/math_utils.h

### Renderer Buffer Caching Optimization
- **Reason:** Profile before optimizing - current design is simple and correct (sokol handles deferred destruction)
- **Reconsider When:** Profiling shows buffer creation in hotspot
- **Files:** `src/rendering/renderer.cpp` (lines 70-109)
- **Note:** Creates/destroys GPU buffers every draw call; could cache unit meshes (sphere, circle) used 50+ times per frame

### Controller/Orientation Coupling Architecture
- **Reason:** Architectural concern with only one instance; wait for more evidence before changing
- **Reconsider When:** Second similar coupling pattern appears or character system needs major redesign
- **Files:** `src/character/controller.h`, `src/app/game_world.cpp`
- **Note:** Unclear ownership - controller contains orientation/animation but game_world also updates them

### Vector Spring-Damper Template
- **Reason:** Only scalar springs used currently; templating would be premature
- **Reconsider When:** Vector springs needed 3+ times (currently 0 uses)
- **Files:** `src/foundation/spring_damper.h`

### Collision Multi-Pass Iteration Count
- **Reason:** Experimental code (marked "EXPERIMENT: Single-sphere collision")
- **Reconsider When:** Collision system finalizes and needs tuning
- **Files:** `src/character/controller.cpp:210` (hardcoded to 3 passes)

### Velocity Trail Encapsulation
- **Reason:** Only 1 use (trail tracking for character position only)
- **Reconsider When:** Second entity needs trail tracking (e.g., projectiles, NPCs) or trail logic grows beyond 20 lines

### Character-Local Transform Utility
- **Reason:** Only 1 use currently (`animation.cpp:25-28`)
- **Reconsider When:** Second use appears (e.g., IK foot placement, look-at target transforms)

### Rotation Matrix Y-Axis Helper
- **Reason:** Only 2 uses (rule of three not met)
- **Reconsider When:** Third use appears or rotation helpers are needed for other axes

### Velocity + Speed Struct
- **Reason:** Considered extracting `horizontal_velocity` + `speed` as a struct, but velocity is used independently in most contexts
- **Reconsider When:** Systems consistently need velocity magnitude + direction as paired data

---

## Completed

(Finished refactors moved here for reference; prune periodically)

### Extract Angle Wrapping Utilities (Foundation)
- **Completed:** 2025-10-12
- **Category:** Pattern Extraction
- **Files:** `src/foundation/math_utils.h`, `src/character/orientation.cpp`, `src/camera/camera.cpp`
- **Outcome:** Consolidated three separate implementations of angle wrapping into `math::wrap_angle_radians`, `math::wrap_angle_degrees`, and `math::angle_difference_radians` in `foundation/math_utils.h`.
- **Learnings:**
  - A linear approach was effective for this low-risk refactor.
  - The `replace` tool can be tricky with complex, multi-line strings. Using a broader search pattern helped identify the correct locations.
  - The estimate of 2 complexity points was accurate.
- **Impact:** Reduced code duplication by 13 lines and improved clarity at 3 call sites.

### Extract Spherical Coordinate Calculation (Camera)
- **Completed:** 2025-10-12
- **Category:** Pattern Extraction
- **Files:** `src/camera/camera.cpp`
- **Outcome:** Extracted spherical coordinate calculation to a private helper function, replacing 3 duplicated blocks of code.
- **Learnings:**
  - A linear approach was effective for this low-risk refactor.
  - The `replace` tool can be tricky with complex, multi-line strings. Using a broader search pattern helped identify the correct locations.
  - The estimate of 1 complexity point was accurate.
- **Impact:** Reduced code duplication by 12 lines and improved clarity at 3 call sites.

### Extract Yaw-to-Direction Vector Utilities ✅
- **Completed:** 2025-10-10
- **Category:** Utilities
- **Files:** `src/foundation/math_utils.h` (added functions), `src/rendering/debug_draw.cpp` (4 call sites updated)
- **Outcome:** Created `math::yaw_to_forward()` and `math::yaw_to_right()` inline utility functions; replaced 4 inline trigonometric calculations
- **Learnings:**
  - Linear approach appropriate for single-file, 4-call-site refactor (matched 2-point estimate exactly)
  - All call sites identified during planning; no hidden dependencies discovered
  - Inline functions provide zero-overhead abstraction while significantly improving clarity
  - Self-documenting function names eliminated need for mental trigonometry translation
  - Rule of three validated: 4 instances justified extraction
  - Debug-only code made visual verification straightforward
  - Pattern establishes foundation for future geometric utilities in math namespace
- **Impact:** Reduced duplication from 4 inline calculations to 2 reusable utilities; improved code clarity at all call sites; documented coordinate system convention (Y-up, Z-forward)
- **Documentation:** See `PLANS/refactor_yaw_direction_utilities.md` (complete refactor history)

### Extract Horizontal Velocity Projection Utility ✅
- **Completed:** 2025-10-10
- **Category:** Utilities
- **Outcome:** Created `src/foundation/math_utils.h` with `math::project_to_horizontal()` function
- **Impact:** Replaced 5 call sites across 4 files (controller, animation, game_world, debug_draw)
- **Result:** Zero regressions, all validation tests passed
- **Documentation:** See `PLANS/refactor_horizontal_velocity_utility_COMPLETE.md`

### Extract Joint Slider Widget Pattern ✅
- **Completed:** 2025-10-11
- **Category:** Pattern Extraction / API Design
- **Outcome:** Created `draw_joint_angles_widget()` helper function in `src/gui/character_panel.cpp`
- **Impact:** Replaced 8 identical joint angle control blocks (83 lines → 40 lines, 48% reduction)
- **Result:** Zero regressions, all GUI controls work identically, visual verification passed
- **Learnings:**
  - Linear approach appropriate for single-file GUI refactor (matched 2-point estimate exactly)
  - All 8 call sites were identical in structure—no edge cases discovered
  - Anonymous namespace helper function appropriate for file-local GUI utility
  - Self-documenting function name improves clarity at all call sites
  - Makes adding future joints trivial (1 line vs 7 lines per joint)
  - Rule of three validated: 8 instances far exceeded extraction threshold (267% over minimum)
  - Debug GUI code allowed immediate visual verification without complex testing
- **Documentation:** See `PLANS/refactor_joint_slider_widget.md` (complete refactor history)

### Extract Joint Indices to Shared Header (Skeleton) ✅
- **Completed:** 2025-10-12
- **Category:** Pattern Extraction / Data Integrity
- **Files:** `src/character/skeleton.h` (added constants), `src/character/animation.cpp` (16 usages), `src/character/keyframe.cpp` (16 usages)
- **Outcome:** Consolidated fragmented joint index definitions (8 joints in animation.cpp, 17 joints in keyframe.cpp) into single `character::joint_index` namespace in `skeleton.h`
- **Learnings:**
  - "Rule of three" exception validated: Only 2 instances, but fragmented canonical data (conflicting scopes) justified consolidation for data integrity
  - Single source of truth principle applies to architectural constants even with <3 instances
  - Transitive includes worked perfectly (`animation.cpp` → `animation.h` → `keyframe.h` → `skeleton.h`)
  - Compiler verification sufficient for constexpr constants—no runtime testing needed
  - Estimated 1 complexity point was exact—trivial execution matches simple plan
  - Zero deviations from plan; textbook linear refactor execution
- **Impact:** Eliminated fragmented ownership of 17 joint indices; prevented future mismatch bugs; established `skeleton.h` as canonical source for skeleton structure data
- **Documentation:** See `PLANS/REFACTOR_extract_joint_indices.md` (complete refactor history)
