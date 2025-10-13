# Extract Joint Indices to Shared Header

**Current State:** Joint indices are defined independently in `src/character/animation.cpp` and `src/character/keyframe.cpp`, leading to duplication and risk of mismatch.

**Proposed Change:** Consolidate all 17 joint index constants into the `src/character/skeleton.h` header file, within a `character::joint_index` namespace.

**Affected Systems:**
- `src/character/animation.cpp`
- `src/character/keyframe.cpp`
- `src/character/skeleton.h`

**Rationale:** Aligns with the "single source of truth" principle. Centralizing fundamental skeleton data prevents mismatches and improves maintainability. The skeletal animation system is 100% complete, making this a low-risk change.

**Estimated Complexity:** 1 point (Simple: moving constants and updating references).

**Risk Level:** Low - The change involves moving compile-time constants, with no logic alteration. The compiler will catch any missed references.

---

## Refactor Plan

**Date:** 2025-10-12
**Risk Level:** Low
**Complexity:** 1 point

### Impact Analysis

**Files:** 3 | **Call Sites:** 2 | **Systems:** character (animation, keyframe, skeleton)

**Call Site Inventory:**
- `src/character/animation.cpp`: Removes local `joint_index` namespace definition.
- `src/character/keyframe.cpp`: Removes local `joint_index` namespace definition.
- `src/character/skeleton.h`: Adds consolidated `joint_index` namespace.

**Risk Justification:** The risk is **Low**. This refactor only moves `constexpr` integer constants. There is no logic change. The compiler will fail with an "undeclared identifier" error if any usage sites are missed, making verification trivial. The affected system (Skeletal Animation) is 100% complete and stable.

### Before/After Examples

**Before:** Two separate, conflicting definitions.

*In `src/character/keyframe.cpp` (17 joints):*
```cpp
// In character namespace
namespace joint_index {
constexpr int ROOT = 0;
constexpr int SPINE_LOWER = 1;
// ... and 15 more
}
```

*In `src/character/animation.cpp` (8 joints):*
```cpp
// In character namespace
namespace joint_index {
constexpr int LEFT_SHOULDER = 5;
constexpr int LEFT_ELBOW = 6;
// ... and 6 more
}
```

**After:** A single source of truth in `src/character/skeleton.h`.

```cpp
// In character namespace, inside skeleton.h
namespace joint_index {
constexpr int ROOT = 0;
constexpr int SPINE_LOWER = 1;
constexpr int SPINE_UPPER = 2;
constexpr int NECK = 3;
constexpr int HEAD = 4;
constexpr int LEFT_SHOULDER = 5;
constexpr int LEFT_ELBOW = 6;
constexpr int LEFT_WRIST = 7;
constexpr int RIGHT_SHOULDER = 8;
constexpr int RIGHT_ELBOW = 9;
constexpr int RIGHT_WRIST = 10;
constexpr int LEFT_HIP = 11;
constexpr int LEFT_KNEE = 12;
constexpr int LEFT_ANKLE = 13;
constexpr int RIGHT_HIP = 14;
constexpr int RIGHT_KNEE = 15;
constexpr int RIGHT_ANKLE = 16;
} // namespace joint_index
```

**Improvements:**
- **Single Source of Truth:** Eliminates duplication and prevents future mismatches.
- **Clarity:** `skeleton.h` becomes the canonical source for skeleton structure data.
- **Maintainability:** Changes to joint indices only need to be made in one place.

### Migration Strategy

**Approach:** Linear (Low Risk)

A single pass is sufficient for this low-risk change.

**Step 1: Consolidate and Clean Up**
- [x] **Modify `src/character/skeleton.h`:** Add the full `joint_index` namespace (17 constants) from `keyframe.cpp` into `skeleton.h` inside the `character` namespace.
- [x] **Modify `src/character/keyframe.cpp`:** Delete the entire `joint_index` namespace. Add `#include "skeleton.h"` at the top.
- [x] **Modify `src/character/animation.cpp`:** Delete the entire `joint_index` namespace. The file already includes `animation.h`, which should be updated to include `skeleton.h` if it doesn't already. A quick check shows `animation.h` does not include `skeleton.h`, but `animation.cpp` does not need it as it includes `animation.h` which has the `skeleton` type. I will add `#include "skeleton.h"` to `animation.cpp`.

### Validation Protocol

**Final Validation:**
- [x] **Compile:** The project must compile without any errors or warnings. A successful compilation is the primary indicator of success.
- [x] **Visual Inspection:** Run the application and visually confirm that the character animation, including T-pose and walk cycles, appears correct. Since the constants' values are unchanged, no visual difference is expected.
- [x] **No Regressions:** Confirm that the manual override controls in the character panel still function as expected.

---

## Plan Review

**Date:** 2025-10-12
**Status:** APPROVED

### Summary

Plan is sound and ready for implementation. This refactor consolidates fragmented ownership of canonical skeleton data (joint indices) into a single source of truth. While the pattern appears only twice (not meeting the strict "rule of three"), this is a data integrity concern rather than premature abstraction—the same architectural constants are split across files with partial/overlapping definitions (8 joints in animation.cpp, 17 joints in keyframe.cpp). The skeletal system is 100% complete, making this a low-risk, high-value cleanup that prevents future mismatches.

### Violations (Must Fix)

No violations found.

### Risks & Concerns

Risk assessment appears sound. Migration strategy is clear and appropriate for the low risk level.

**Minor Clarification (already correct in practice):**
- Migration Step 1 mentions uncertainty about `animation.cpp` needing to include `skeleton.h`. Verification shows transitive inclusion already works: `animation.cpp` → `animation.h` → `keyframe.h` → `skeleton.h`. The constants will be accessible without additional includes.

### Suggestions

Plan appears comprehensive. No additional suggestions.

### Principle Checks

- [x] "After" code clearer than "before" — Yes, single source of truth in `skeleton.h` is obvious home for joint indices
- [x] Genuinely reduces complexity — Yes, eliminates fragmented ownership and mismatch risk
- [x] Pattern appears 3+ times (or Critical fix) — **Exception justified**: Only 2 instances, but this is fragmented canonical data (8 vs 17 joints), not simple duplication. Data integrity concern overrides rule of three.
- [x] All systems ≥70% certainty — Yes, Skeletal Animation is 100% complete (DEPENDENCY_STACK.md)
- [x] Staged approach for Medium/High risk — N/A, Low risk uses linear approach appropriately
- [x] All call sites included — Yes, both files identified (animation.cpp, keyframe.cpp)
- [x] Hidden dependencies considered — Yes, compile-time constants have no runtime dependencies
- [x] Validation protocol concrete — Yes, compile + visual + manual test
- [x] Total ≤8 points — Yes, 1 point estimate is realistic

### Actionable Items

**If none:** Ready for IMPLEMENT_REFACTOR.

### Recommendation

**Reasoning:** This refactor directly supports the "single source of truth" principle from AGENTS.md. Joint indices are architectural constants that define the skeleton structure—fragmenting them across files creates data integrity risk even with only 2 instances. The skeletal system is 100% stable, the change is trivial (moving constants), and compiler errors will catch any missed references. Strong rationale, low risk, high clarity gain.

**Next Steps:**
- Proceed to IMPLEMENT_REFACTOR.md

---

## Execution Summary

**Date:** 2025-10-12
**Approach Used:** Linear

**Files Modified:**
- `src/character/skeleton.h`: Added the consolidated `joint_index` namespace.
- `src/character/keyframe.cpp`: Removed the local `joint_index` namespace and added `#include "skeleton.h"`.
- `src/character/animation.cpp`: Removed the local `joint_index` namespace and added `#include "skeleton.h"`.

**Issues Encountered:**
- None

**Deviations from Plan:**
- None

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
- `skeleton.h`: All 17 joint indices added to `joint_index` namespace (lines 17-36)
- `animation.cpp`: Local 8-joint namespace removed, `#include "skeleton.h"` added, 16 usages verified
- `keyframe.cpp`: Local 17-joint namespace removed, `#include "skeleton.h"` added, 16 usages verified
- No other files reference `joint_index::`

### Code Quality Standards

**Naming:** ✓ Correct
**Formatting:** ✓ Consistent
**Organization:** ✓ Correct
**Documentation:** ✓ Adequate

**Issues:**
None

### Side Effects & Correctness

**Correctness:** ✓ Verified
**Behavior Preservation:** ✓ Preserved
**Performance:** ✓ No Regression

**Issues:**
None detected

### Scope Discipline

**Plan Adherence:** ✓ Exact
**Scope Creep:** ✓ None

**Deviations:**
None

### Critical Issues (Must Fix Before Finalization)

None

### Recommendations

**Reasoning:** Implementation is flawless. All 3 files modified exactly as planned. The joint indices are now consolidated in `skeleton.h` as the single source of truth. Both `animation.cpp` and `keyframe.cpp` properly include the header and use the constants without issue. Code compiles cleanly with no errors or warnings. No scope creep, no deviations, no side effects. This is a textbook refactor execution.

**Next Steps:**
- Proceed to FINALIZE_REFACTOR and run validation checklist

---

## Finalization

**Date:** 2025-10-12
**Status:** COMPLETED

**Final Validation Results:**
- ✅ Compile: Project builds without errors or warnings
- ✅ Visual Inspection: Application runs successfully, character animation appears correct
- ✅ No Regressions: All systems functioning as expected
- ✅ Manual Controls: Character panel controls work correctly

**Documentation Updates:**
None required (internal constant consolidation, no public API changes)

**Next Steps:**
- Moved to Completed in REFACTOR_BACKLOG.md
- Learnings documented in backlog entry
- Ready for commit

