# Refinement: Enum Naming Conventions

Enforce UPPER_CASE for enum constants per CONVENTIONS.md

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-20
**Complexity:** Trivial
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** Multiple files (22 instances)
- `src/character/controller.h:126` - locomotion_speed_state: walk, run, sprint
- `src/gui/parameter_command.h:20-28` - parameter_type: 9 constants
- `src/gui/camera_command.h:14-17` - camera_parameter: 4 constants

**Principle:** Consistency
**Severity:** Medium
**Type:** Naming convention violation

**Current state:**
```cpp
// controller.h
enum class locomotion_speed_state { walk, run, sprint };

// parameter_command.h
enum class parameter_type {
    max_speed,
    acceleration,
    // ... 7 more snake_case constants
};

// camera_command.h
enum class camera_parameter {
    orbit_latitude,
    orbit_longitude,
    // ... 2 more snake_case constants
};
```

**Why violation:**
- CONVENTIONS.md explicitly requires UPPER_CASE for enum constants
- Current code uses snake_case, inconsistent with standard
- Enforced by `.clang-tidy` but violations exist
- Breaks consistency principle

**Impact:**
- Blocks: Code style consistency across codebase
- Cascades to: All usage sites (mechanical updates required)
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**SIMPLIFY:**
- From: snake_case enum constants across 3 files
- To: UPPER_CASE enum constants per CONVENTIONS.md

**Mechanical transformation:**
```
locomotion_speed_state::walk  → WALK
locomotion_speed_state::run   → RUN
locomotion_speed_state::sprint → SPRINT

parameter_type::max_speed     → MAX_SPEED
parameter_type::acceleration  → ACCELERATION
parameter_type::drag          → DRAG
parameter_type::gravity       → GRAVITY
parameter_type::jump_height   → JUMP_HEIGHT
parameter_type::jump_buffer   → JUMP_BUFFER
parameter_type::coyote_time   → COYOTE_TIME
parameter_type::collision_radius → COLLISION_RADIUS
parameter_type::collision_height → COLLISION_HEIGHT

camera_parameter::orbit_latitude  → ORBIT_LATITUDE
camera_parameter::orbit_longitude → ORBIT_LONGITUDE
camera_parameter::orbit_distance  → ORBIT_DISTANCE
camera_parameter::field_of_view   → FIELD_OF_VIEW
```

**Steps:**
1. Update enum declarations in headers
2. Update all usage sites across codebase (search for each constant)
3. Verify compilation succeeds
4. Run clang-tidy to confirm violations cleared

**Check duplicates:** N/A (mechanical rename, no duplicate logic)
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved
- [ ] Principle upheld (Consistency)
- [ ] Tests passing (build succeeds)
- [ ] No regressions
- [ ] clang-tidy confirms violations cleared

**Metrics:**
- Before: 22 snake_case enum constants
- After: 22 UPPER_CASE enum constants
- LOC change: 0 (pure rename)
<!-- END: SELECT/SUCCESS -->
