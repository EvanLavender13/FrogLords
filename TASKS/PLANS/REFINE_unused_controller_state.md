# Refinement: Unused Controller State Fields

Delete unused `ground_normal` and `ground_height` fields from controller state

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-20
**Complexity:** Trivial (1 pt)
**Path:** A (trivial deletion)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/character/controller.h:59-60`
**Principle:** Radical Simplicity, Single Source of Truth
**Severity:** Low
**Type:** State bloat - unused fields maintained without consumers

**Current state:**
```cpp
struct controller {
    // ... active fields ...
    glm::vec3 ground_normal;  // Line 59 - never read
    float ground_height;       // Line 60 - never read
};
```

**Why violation:**
- Fields are written during collision resolution but never read anywhere
- Violates "every element must justify its existence"
- Duplicates derived state without consumers
- Invites future divergence from single source of truth
- Bloats controller state unnecessarily

**Impact:**
- Blocks: Clarity of what state actually matters
- Cascades to: Future confusion about whether these should be used
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Delete

**DELETE:**
- Remove: `ground_normal` and `ground_height` fields from `controller` struct
- Remove: Any code that writes to these fields (should be in collision resolution)
- Impact: Zero - no code reads these values, pure deletion
- Tests: Compile + run to verify no references remain

**Verification:**
- Grep for `ground_normal` and `ground_height` usage
- Confirm no reads exist
- Remove writes
- Build passes
- Game runs identically
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] `ground_normal` and `ground_height` deleted from struct
- [ ] All writes to these fields removed
- [ ] Build passes
- [ ] Game runs with no behavioral changes
- [ ] No references remain in codebase

**Metrics:**
- Before: LOC (controller.h + writes in collision code)
- After: LOC (reduced)
<!-- END: SELECT/SUCCESS -->
