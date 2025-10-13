# Debug Draw Depends on App

**Severity/Priority:** Critical

**Current Issue:** `src/rendering/debug_draw.cpp` includes `app/game_world.h` to access `velocity_trail_state`, violating layered architecture. Rendering must not depend on App.

**Files Affected:**
- `src/rendering/debug_draw.cpp` (includes app/game_world.h)
- `src/app/game_world.h` (defines velocity_trail_state)
- `src/rendering/velocity_trail.h` (new - extract location)

**Proposed Fix:** Extract `velocity_trail_state` struct to `src/rendering/velocity_trail.h`. Update includes in debug_draw.cpp and game_world.h.

**Rationale:** Layered architecture (Foundation → Character → Rendering → App) is a core principle from AGENTS.md. Rendering layer must not pull in App dependencies. The velocity trail is a rendering/debug visualization concern, not app logic.

**Workflow Path:** Path A (Trivial)

**Estimated Complexity:** 2 points

**Risk Level:** Low - Pure structural change, no behavior modification. Single struct extraction with clear ownership.

**Tags:** #architecture #layers #critical

---

## PLAN

**Created:** 2025-10-13 21:41 UTC

**Branch:** `improve/debug_draw_app_dependency`

### Impact Analysis

**Affected Systems:**
- Rendering/Debug Draw (debug_draw.cpp, debug_draw.h)
- App/Game World (game_world.h)

**Current Dependencies:**
- `debug_draw.cpp` includes `app/game_world.h` (line 5) → VIOLATES layer architecture
- `debug_draw.h` forward-declares `velocity_trail_state` (line 13)
- `game_world.h` defines `velocity_trail_state` (lines 16-21) and uses it (line 33)

**Who Uses velocity_trail_state:**
- `game_world` struct has `trail_state` member (app layer)
- `draw_velocity_trail()` accepts const reference (rendering layer)
- Only these two systems; isolated usage

**Risk Level:** Low

**Risk Factors:**
- Simple struct with no dependencies (uses only std::vector and float)
- No methods, only data members
- Clear ownership: rendering concern, not app logic

**Mitigation:**
- Single atomic change: extract struct, update includes
- Verify build immediately after each stage
- No behavior changes; pure structural refactor

### Proposed Changes

**Stage 1: Create velocity_trail.h**
- Create `src/rendering/velocity_trail.h`
- Move `velocity_trail_state` struct (lines 16-21 from game_world.h)
- Add standard header guard
- Include `<vector>` and `<glm/glm.hpp>` (dependencies)
- Validation: File compiles independently

**Stage 2: Update debug_draw.h**
- Replace forward declaration (line 13) with `#include "rendering/velocity_trail.h"`
- Validation: debug_draw.h compiles

**Stage 3: Update debug_draw.cpp**
- Remove `#include "app/game_world.h"` (line 5)
- Add `#include "rendering/velocity_trail.h"` (after other rendering includes)
- Validation: debug_draw.cpp compiles

**Stage 4: Update game_world.h**
- Remove `velocity_trail_state` struct definition (lines 16-21)
- Add `#include "rendering/velocity_trail.h"` (after rendering/scene.h include)
- Keep `trail_state` member (line 33)
- Validation: game_world.h compiles

**Stage 5: Full Build Verification**
- Run clean build
- Verify no warnings introduced
- Check dependency flow: Rendering no longer depends on App

### Validation Checklist

#### Build & Compile
- [ ] Clean build succeeds (no errors, no warnings)
- [ ] All affected files compile
- [ ] New header compiles independently

#### Correctness
- [ ] Behavior unchanged (pure structural refactor)
- [ ] No new compiler warnings
- [ ] velocity_trail_state accessible where needed

#### Architecture
- [ ] Dependency flow correct: Rendering → App (not App ← Rendering)
- [ ] No reverse dependencies introduced
- [ ] Follows naming conventions (snake_case, all lowercase)
- [ ] Header guard follows pattern

#### Code Quality
- [ ] Minimal includes in new header (only std::vector, glm)
- [ ] Comments preserved if any
- [ ] No formatting changes to existing code
- [ ] clang-format applied to new file

### Rollback Strategy

**Abort Conditions:**
- Stage 1-2 fail: Delete velocity_trail.h, revert changes
- Stage 3-4 fail: Compilation errors in dependent files
- Stage 5 fail: Warnings or errors in clean build

**Rollback Steps:**
1. `git restore src/rendering/velocity_trail.h src/rendering/debug_draw.* src/app/game_world.h`
2. Delete `src/rendering/velocity_trail.h` if created
3. Verify build returns to baseline
4. Document issue in backlog if architectural constraint discovered

**Recovery:** Simple file-level revert; no complex state to untangle.

---

## Implementation Notes

**File Contents Preview:**

```cpp
// src/rendering/velocity_trail.h
#pragma once

#include <vector>
#include <glm/glm.hpp>

struct velocity_trail_state {
    std::vector<glm::vec3> positions;
    std::vector<float> timestamps;
    float sample_interval = 0.1f;
    float time_since_last_sample = 0.0f;
};
```

**Include Order (debug_draw.cpp):**
```cpp
#include "rendering/debug_draw.h"
#include "rendering/scene.h"
#include "rendering/velocity_trail.h"  // NEW - replaces app/game_world.h
#include "character/skeleton.h"
#include "foundation/math_utils.h"
// Remove: #include "app/game_world.h"
```

**Estimated Time:**
- Stage 1: 5 min (create file)
- Stage 2: 2 min (update header)
- Stage 3: 2 min (update cpp)
- Stage 4: 3 min (update game_world)
- Stage 5: 5 min (build verification)
- **Total: ~15-20 minutes**

---

## EXECUTION (Trivial Path)

**Executed:** 2025-10-13 21:57 UTC

### Changes Made

**File:** `src/rendering/velocity_trail.h` (NEW)
- Created new header with `velocity_trail_state` struct
- Includes: `<vector>`, `<glm/glm.hpp>`
- Standard `#pragma once` guard

**File:** `src/rendering/debug_draw.h`
- Line 5: Added `#include "rendering/velocity_trail.h"`
- Removed: Lines 12-13 (forward declaration and comment)

**File:** `src/rendering/debug_draw.cpp`
- Line 3: Added `#include "rendering/velocity_trail.h"`
- Removed: Line 5 `#include "app/game_world.h"` (architectural violation)

**File:** `src/app/game_world.h`
- Line 9: Added `#include "rendering/velocity_trail.h"`
- Removed: Lines 16-21 (`velocity_trail_state` struct definition)

### Verification

- [x] Build succeeds (clean build via `scripts/bash/build.sh`)
- [x] clang-tidy clean (no new warnings in user code)
- [x] No new warnings
- [x] Architectural violation eliminated (rendering no longer depends on app)
- [x] All files using `velocity_trail_state` compile correctly

### Dependencies Verified

**Before:**
- `debug_draw.cpp` → `app/game_world.h` ❌ (violates Foundation → Character → Rendering → App)

**After:**
- `debug_draw.cpp` → `rendering/velocity_trail.h` ✅
- `debug_draw.h` → `rendering/velocity_trail.h` ✅
- `game_world.h` → `rendering/velocity_trail.h` ✅

**Dependency flow now correct:** App depends on Rendering (allowed), Rendering does not depend on App.

### Notes

- All stages completed without issues
- Build succeeded on first attempt after all changes
- No behavior changes; pure structural refactor
- `velocity_trail_state` now correctly owned by rendering layer
- Actual duration: ~10 minutes (faster than estimated)

---

## PLAN REVIEW

**Date:** 2025-10-13
**Status:** APPROVED

### Summary

The plan to extract `velocity_trail_state` is approved. It corrects a critical architectural violation and is a low-risk, high-reward change.

### Violations (Must Fix)

No violations found.

### Risks & Concerns

Plan appears well-aligned.

### Risk Assessment

**Risk Level:** Low
**Reward Level:** High
**Decision:** Proceed

### Stage Quality

**Stage Breakdown:** ✓
**Validation Checklist:** ✓

### Actionable Items

Ready to proceed with implementation.

### Recommendation

**Reasoning:** Critical architectural fix with clear, minimal stages. Low risk, high reward.

**Next Steps:**
- Proceed to EXECUTE

---

## FINALIZATION

**Finalized:** 2025-10-13 22:00 UTC

### Changes Summary

**Files Modified:**
- `src/rendering/velocity_trail.h` - (new) Extracted `velocity_trail_state` struct.
- `src/rendering/debug_draw.h` - Included new header, removed forward declaration.
- `src/rendering/debug_draw.cpp` - Replaced `app/game_world.h` include with `rendering/velocity_trail.h`.
- `src/app/game_world.h` - Included new header, removed struct definition.

**Impact:**
Corrected a critical architectural violation where the Rendering layer depended on the App layer. The `velocity_trail_state` is now correctly owned by the Rendering layer, reinforcing the project's layered architecture.

### Backlog Updates

**Item Status:** Moved to Completed in IMPROVE_BACKLOG.md

**New Items Discovered:**
- None.

**Batch Opportunities Noted:**
- None.

### Learnings

**What Worked Well:**
- The plan was clear and easy to follow.
- The change was purely structural, which made it low-risk.
- The build system and static analysis tools quickly verified the correctness of the change.

**What Was Harder Than Expected:**
- Nothing. The change was straightforward.

**Insights for Future Work:**
- Addressing architectural violations promptly is crucial. A small violation can quickly become a larger problem if not addressed.
- The "trivial path" workflow is effective for these kinds of small, targeted improvements.

**Time Estimate Accuracy:**
- Estimated: 2 points / ~15-20 minutes
- Actual: 2 points / ~10 minutes
- Variance: Accurate (faster than estimated)