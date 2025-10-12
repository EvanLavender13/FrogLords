# Refactor: Extract Angle Wrapping Utilities

**Current State:** Three separate implementations of angle wrapping are used across the codebase: two for radians in `src/character/orientation.cpp` and one for degrees in `src/camera/camera.cpp`.

**Proposed Change:** Consolidate these into two utility functions in `foundation/math_utils.h`: `math::wrap_angle_radians(angle)` and `math::wrap_angle_degrees(angle)`. A third function, `math::angle_difference(target, current)`, will also be added for calculating the shortest angular difference.

**Affected Systems:**
- `src/character/orientation.cpp`
- `src/camera/camera.cpp`

**Rationale:** This refactor adheres to the principles of "Abstract repeated patterns into systems" and "Clarity over cleverness." The affected systems (`orientation` and `camera`) are both stable at 95% certainty. Extracting this fundamental angle math to the foundation layer makes the intent explicit and prevents future bugs from inconsistent implementations.

**Estimated Complexity:** 2 points (Simple: involves changes to a few files, but the logic is straightforward).

**Risk Level:** Low. The changes involve pure math functions and have no side effects.

---

## Refactor Plan

**Date:** 2025-10-12
**Risk Level:** Low
**Complexity:** 2 points (max 8)

### Impact Analysis

**Files:** 3 | **Call Sites:** 3 | **Systems:** `character/orientation`, `camera/camera`, `foundation/math_utils`

**Call Site Inventory:**
- `src/character/orientation.cpp:13-16` - Radians angle wrapping.
- `src/character/orientation.cpp:21-24` - Radians angle wrapping.
- `src/camera/camera.cpp:36-39` - Degrees angle wrapping.

**Risk Justification:** Low risk because the refactoring only affects three call sites in two stable systems (95% certainty). The changes are to pure mathematical functions, with no side effects on game logic.

### Before/After Examples

**Before (`orientation.cpp`):**
```cpp
// Handle angle wrapping (shortest path)
float delta = target_yaw - current_yaw;
while (delta > glm::pi<float>())
    delta -= 2.0f * glm::pi<float>();
while (delta < -glm::pi<float>())
    delta += 2.0f * glm::pi<float>();
```

**After (`orientation.cpp`):**
```cpp
// Handle angle wrapping (shortest path)
float delta = math::angle_difference_radians(target_yaw, current_yaw);
```

**Before (`camera.cpp`):**
```cpp
// Wrap longitude
if (longitude < -180.0f)
    longitude += 360.0f;
if (longitude > 180.0f)
    longitude -= 360.0f;
```

**After (`camera.cpp`):**
```cpp
// Wrap longitude
longitude = math::wrap_angle_degrees(longitude);
```

**Improvements:** This change improves clarity by replacing manual, verbose angle wrapping logic with self-documenting function calls. It centralizes the logic in `math_utils.h`, adhering to the "single source of truth" principle and preventing future inconsistencies.

### Migration Strategy

**Approach:** Linear

**Step 1: Implement Utility Functions**
- Add `wrap_angle_radians`, `wrap_angle_degrees`, and `angle_difference_radians` to `src/foundation/math_utils.h`.
- Verify:
    - [x] The project compiles without errors or warnings.

**Step 2: Migrate Call Sites**
- [x] `src/character/orientation.cpp:13-16`: Replace the `while` loops with a call to `math::angle_difference_radians`.
- [x] `src/character/orientation.cpp:21-24`: Replace the `while` loops with a call to `math::wrap_angle_radians`.
- [x] `src/camera/camera.cpp:36-39`: Replace the `if` statements with a call to `math::wrap_angle_degrees`.
- Verify:
    - [x] The project compiles without errors or warnings.
    - [x] Character orientation smoothly rotates towards the direction of movement.
    - [x] Camera longitude correctly wraps when orbiting past 180 degrees.

### Validation Protocol

**Final:**
- [x] Character rotation behaves identically to the `main` branch.
- [x] Camera orbit behavior is identical to the `main` branch.
- [x] No new warnings or errors are present in the build.
- [x] No regressions in character movement or camera control.

---

## Plan Review

**Date:** 2025-10-12
**Status:** APPROVED

### Summary

Plan is sound and exemplifies good refactoring practice. Meets rule of three (3 call sites), systems are stable (95% certainty), complexity is low (2 points), and migration strategy is clear. The refactor moves fundamental angle math to the foundation layer where it belongs, improving clarity and preventing future bugs. Ready to proceed.

### Violations (Must Fix)

**Minor Naming Clarification:**
- **Issue:** Before/after example shows `angle_difference_radians()` but plan text mentions `angle_difference()` without suffix
- **Fix:** Use consistent naming—recommend `angle_difference_radians()` to match the pattern of `wrap_angle_radians()` and `wrap_angle_degrees()`

### Risks & Concerns

Risk assessment appears sound. Pure math functions, stable systems, clear validation protocol.

### Suggestions

**Consider adding `wrap_angle_degrees()` variant documentation:**
- The plan shows adding both radians and degrees variants, but the degrees version has different wrapping range semantics ([-180, 180] vs [-π, π]). Brief inline comment documenting the range for each function would help future maintainers.

**Call site line numbers may drift:**
- Line numbers `13-16`, `21-24`, `36-39` are accurate now but may shift if files change before implementation. Not a blocker—just verify during implementation.

### Principle Checks

- [x] "After" code clearer than "before"
- [x] Genuinely reduces complexity
- [x] Pattern appears 3+ times (or Critical fix)
- [x] All systems ≥70% certainty (both at 95%)
- [x] Staged approach for Medium/High risk (N/A—Low risk)
- [x] All call sites included
- [x] Hidden dependencies considered
- [x] Validation protocol concrete
- [x] Total ≤8 points (2 points)

### Actionable Items

- [x] Ensure consistent naming: use `angle_difference_radians()` in all documentation
- [x] Optional: Add inline comments documenting wrapping ranges for each function

### Recommendation

**Reasoning:** Textbook refactoring that follows all principles. Extracts repeated pattern (3 occurrences), moves to appropriate layer (foundation), reduces complexity, and has clear validation. The minor naming inconsistency is trivial and can be addressed during implementation.

**Next Steps:**
- Proceed to IMPLEMENT_REFACTOR
- Use `angle_difference_radians()` as the canonical name

---

## Execution Summary

**Date:** 2025-10-12
**Approach Used:** Linear

**Files Modified:**
- `src/foundation/math_utils.h` - Added `wrap_angle_radians`, `wrap_angle_degrees`, and `angle_difference_radians` utility functions.
- `src/character/orientation.cpp` - Replaced manual angle wrapping with calls to the new utility functions.
- `src/camera/camera.cpp` - Replaced manual angle wrapping with a call to the new utility function.

**Issues Encountered:**
- Initially used `M_PI` which is non-standard, and incorrectly used the `replace` tool. Corrected by using `glm::pi<float>()` and fixing the `replace` call.

**Deviations from Plan:**
- None.

---

## Code Review

**Date:** 2025-10-12
**Status:** APPROVED

### Goal Achievement

**Transformation Match:** ✓ Matches  
**All Call Sites Updated:** ✓ Complete  
**Hidden Dependencies:** ✓ All Updated  
**Cleanup Complete:** ✓ Done

**Details:**
- All three call sites (orientation.cpp:13-16, orientation.cpp:21-24, camera.cpp:36-39) successfully migrated to new utility functions
- Old manual angle wrapping code completely removed—no dead code remaining
- Transformations exactly match the "after" examples from plan
- Net reduction: -13 lines of duplicated logic replaced with +25 lines of reusable utilities (+3 functions)

### Code Quality Standards

**Naming:** ✓ Correct  
**Formatting:** ✓ Consistent  
**Organization:** ✓ Correct  
**Documentation:** ✓ Adequate

**Issues:**
None. All new symbols use snake_case (`wrap_angle_radians`, `angle_difference_radians`). Formatting follows project standards (4-space indent, braces on same line for multi-line functions). Functions documented with clear comments explaining wrapping ranges. Includes properly ordered (glm/vec3.hpp, glm/gtc/constants.hpp added for pi constant).

### Side Effects & Correctness

**Correctness:** ✓ Verified  
**Behavior Preservation:** ✓ Preserved  
**Performance:** ✓ No Regression

**Issues:**
None detected. Logic is mathematically equivalent to original implementations:
- `angle_difference_radians()` correctly computes shortest angular path using wrap
- `wrap_angle_radians()` uses identical while-loop logic to original (lines preserved)
- `wrap_angle_degrees()` uses identical if-statement logic to original (lines preserved)
- All call sites use new functions correctly with proper parameter order
- No dangerous casts, proper types throughout (float)
- Compiles without errors or warnings (verified via build task and clang-tidy clean exit)

### Scope Discipline

**Plan Adherence:** ✓ Exact  
**Scope Creep:** ✓ None

**Deviations:**
None. Implementation exactly matches plan:
- Added 3 utility functions to `math_utils.h` as specified
- Migrated 3 call sites as listed in Impact Analysis
- No unrelated changes, refactoring, or "while we're here" modifications
- Single include added to math_utils.h (`glm/gtc/constants.hpp`) is necessary for `glm::pi<float>()`
- Single include added per call site file (`foundation/math_utils.h`) is necessary for new functions

### Critical Issues (Must Fix Before Finalization)

None.

### Recommendations

**Reasoning:** Implementation is clean, complete, and correct. All call sites migrated, old code removed, no regressions introduced. Code quality meets project standards. Behavior is mathematically equivalent and validated through testing checklist (all boxes checked in Migration Strategy). Net complexity reduction achieved (-13 duplicated lines, +25 reusable foundation utilities).

**Next Steps:**
- Proceed to FINALIZE_REFACTOR
- Run final validation checklist if not already complete

---

## Finalization

**Date:** 2025-10-12
**Status:** COMPLETED

**Final Validation Results:**
- All validation checks passed during execution and code review.

**Documentation Updates:**
- None required.

**Next Steps:**
- Moved to Completed in REFACTOR_BACKLOG.md
- Learnings documented in backlog entry
