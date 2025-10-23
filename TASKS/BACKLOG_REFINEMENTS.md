# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Last Updated:** 2025-10-23

---

## Active Violations

### Critical Priority

None - All critical violations resolved ✅

### High Priority

None - All high-priority violations resolved ✅

### Medium Priority

None - All medium-priority violations resolved ✅


### Low Priority

**Function Length - controller::update**
- **Location:** `src/character/controller.cpp:109`
- **Tool:** lizard
- **Severity:** Low
- **Issue:** Length 121 (threshold 100) - physics update loop
- **Fix:** Extract sub-phases: ground detection, movement, collision response
- **Impact:** Controller maintainability

**Function Length - debug_viz::validation::run_startup_checks**
- **Location:** `src/rendering/debug_validation.cpp:9`
- **Tool:** lizard
- **Severity:** Low
- **Issue:** Length 150 (threshold 100) - startup validation checks
- **Fix:** Extract validation categories into separate functions or accept for debug code
- **Impact:** Debug code maintainability

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
