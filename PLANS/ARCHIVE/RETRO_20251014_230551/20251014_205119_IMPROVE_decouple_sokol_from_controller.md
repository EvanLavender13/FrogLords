# Decouple sokol_app.h from character/controller.cpp

**Severity/Priority:** Medium

**Current Issue:** `src/character/controller.cpp` includes the large `sokol_app.h` header solely for access to `SAPP_KEYCODE_*` constants. This creates an unnecessary dependency on a platform-specific header in a core gameplay file.

**Files Affected:**
- `src/character/controller.cpp`
- `src/input/keycodes.h` (new)

**Proposed Fix:** Create a new header, `src/input/keycodes.h`, that defines the necessary keycodes, and include that instead. This isolates the dependency.

**Rationale:** Reduces coupling and improves compilation times by avoiding the inclusion of a large, unrelated header. This aligns with the principle of maintaining a layered architecture.

**Workflow Path:** Path A (Trivial)

**Estimated Complexity:** 2 points

**Risk Level:** Low - This is a simple header replacement with no logic changes.

**Tags:** #dependencies #cleanup #decoupling

---

## FINALIZATION

**Finalized:** 2025-10-15T00:44:00Z

### Changes Summary

**Files Modified:**
- `src/input/keycodes.h` (new) - Created enum with SAPP_KEYCODE constants
- `src/character/controller.cpp` - Replaced sokol_app.h include with input/keycodes.h

**Impact:**
Removed unnecessary platform layer dependency from character controller. Controller no longer includes the large sokol_app.h header, improving compilation times and reducing coupling between gameplay code and platform layer.

### New Items Discovered

None.

### Learnings

**What Worked Well:**
- Simple extraction pattern - isolate constants to appropriate layer
- Quick verification with existing build/format/lint tools

**What Was Harder Than Expected:**
- Nothing; straightforward as expected for trivial path

**Insights for Future Work:**
- Look for other platform-specific includes in gameplay code
- Input layer is natural home for input-related constants

**Time Estimate Accuracy:**
- Estimated: 2 points
- Actual: ~10 minutes
- Variance: Accurate

