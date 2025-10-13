# Extract Spherical Coordinate Calculation

**Current State:** Identical spherical-to-cartesian coordinate calculation appears 3 times in the camera system (`src/camera/camera.cpp`).

**Proposed Change:** Extract the logic into a private helper function `compute_spherical_position(center, distance, latitude, longitude)` to remove duplication.

**Affected Systems:**
- `src/camera/camera.cpp`

**Rationale:** Aligns with the "Abstract repeated patterns into systems" principle. The same mathematical calculation is used in multiple places. Extracting it to a single function improves clarity, reduces code duplication, and makes the logic easier to maintain and test in isolation.

**Estimated Complexity:** 1 point (Simple)

**Risk Level:** Low - This is a pure function extraction with no side effects. The change is localized to a single file.

---

## Refactor Plan

**Date:** 2025-10-12
**Risk Level:** Low
**Complexity:** 1 point (max 8)

### Impact Analysis

**Files:** 1 | **Call Sites:** 3 | **Systems:** Camera

**Call Site Inventory:**
- `src/camera/camera.cpp:17-20` - `update_eye_position()`
- `src/camera/camera.cpp:50-53` - `zoom()`
- `src/camera/camera.cpp:88-91` - `follow_update()`

**Risk Justification:** Low risk. The change is confined to a single file (`camera.cpp`). It affects only 3 call sites, all within the same class. The camera system itself is rated at 95% certainty. The change is a pure function extraction, so no side effects are anticipated.

### Before/After Examples

**Before:**
```cpp
// src/camera/camera.cpp:17-20
float lat_rad = glm::radians(latitude);
float lon_rad = glm::radians(longitude);

eye_pos.x = center.x + distance * cosf(lat_rad) * sinf(lon_rad);
eye_pos.y = center.y + distance * sinf(lat_rad);
eye_pos.z = center.z + distance * cosf(lat_rad) * cosf(lon_rad);
```

**After:**
```cpp
// New private helper function in camera.h
namespace {
glm::vec3 compute_spherical_position(const glm::vec3& center, float dist, float lat, float lon) {
    float lat_rad = glm::radians(lat);
    float lon_rad = glm::radians(lon);
    glm::vec3 pos;
    pos.x = center.x + dist * cosf(lat_rad) * sinf(lon_rad);
    pos.y = center.y + dist * sinf(lat_rad);
    pos.z = center.z + dist * cosf(lat_rad) * cosf(lon_rad);
    return pos;
}
} // namespace

// src/camera/camera.cpp:17
eye_pos = compute_spherical_position(center, distance, latitude, longitude);
```

**Improvements:** This change eliminates 12 lines of duplicated code across three functions and replaces them with a single, well-named helper function. This improves readability and maintainability by centralizing the core logic.

### Migration Strategy

**Approach:** Linear

**Steps:**
1.  Add the private helper function `compute_spherical_position` to an anonymous namespace in `camera.cpp`.
2.  Replace the spherical coordinate calculation in `update_eye_position()` with a call to the new helper function.
3.  Replace the spherical coordinate calculation in `zoom()` with a call to the new helper function.
4.  Replace the spherical coordinate calculation in `follow_update()` with a call to the new helper function.

### Validation Protocol

**Final:**
- [x] The project compiles without errors or warnings.
- [x] Camera orbit, zoom, and follow modes function identically to before the change. Visual verification is sufficient.
- [x] Run the application and test:
    - Orbiting the camera around the character.
    - Zooming in and out.
    - Switching to follow mode and moving the character.

---

## Execution Summary

**Date:** 2025-10-12
**Approach Used:** Linear

**Files Modified:**
- `src/camera/camera.cpp` - Extracted spherical coordinate calculation to a private helper function and replaced 3 call sites.

**Issues Encountered:**
- None

**Deviations from Plan:**
- None

---

## Finalization

**Date:** 2025-10-12
**Status:** COMPLETED

**Final Validation Results:**
- [x] The project compiles without errors or warnings.
- [x] Visual verification confirms that camera orbit, zoom, and follow modes function identically to before the change.

**Documentation Updates:**
- None required.

**Next Steps:**
- Moved to Completed in REFACTOR_BACKLOG.md
- Learnings documented in backlog entry

---

## Plan Review

**Date:** 2025-10-12
**Status:** APPROVED

### Summary

Plan is sound and exemplifies good refactoring practice. Satisfies rule of three (3 identical instances), targets a stable system (Camera at 95% certainty), and demonstrates clear improvement in readability. The 1-point complexity estimate is accurate for a straightforward function extraction. Migration strategy is appropriate for low-risk linear approach. No violations or significant concerns identified.

### Violations (Must Fix)

No violations found.

### Risks & Concerns

Risk assessment appears sound.

### Suggestions

No suggestions—plan is comprehensive and appropriate for this simple refactor.

### Principle Checks

- [x] "After" code clearer than "before"
- [x] Genuinely reduces complexity
- [x] Pattern appears 3+ times (or Critical fix)
- [x] All systems ≥70% certainty
- [x] Staged approach for Medium/High risk (N/A - Low risk)
- [x] All call sites included
- [x] Hidden dependencies considered
- [x] Validation protocol concrete
- [x] Total ≤8 points

### Actionable Items

Ready for IMPLEMENT_REFACTOR.

### Recommendation

**Reasoning:** This is a textbook example of appropriate refactoring. The pattern appears exactly 3 times (satisfying rule of three), the camera system is stable (95% certainty per DEPENDENCY_STACK.md), and the extracted function has a clear, single responsibility. The "after" code is objectively clearer—`eye_pos = compute_spherical_position(center, distance, latitude, longitude)` immediately communicates intent vs. 6 lines of trigonometry. Risk is genuinely low: pure function, no side effects, single file scope. Complexity estimate (1 point) is realistic.

**Next Steps:**
- Proceed to IMPLEMENT_REFACTOR
- Follow the 4-step linear migration
- Validate using the concrete test protocol (orbit, zoom, follow)
