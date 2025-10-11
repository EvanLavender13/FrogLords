# Refactor Backlog

**Purpose:** Tracking for architectural improvements, pattern extraction, and system simplification opportunities that improve code quality and maintainability.

**Status:** Living document (updated after system analysis)

**Last Review:** October 11, 2025

---

## Critical

(Blocking new work, major technical debt, or architectural flaws)

*No items currently tracked.*

---

## High

(Significant improvement to clarity, maintainability, or extensibility)

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

