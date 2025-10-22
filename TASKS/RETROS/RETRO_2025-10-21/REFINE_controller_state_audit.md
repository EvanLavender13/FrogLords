# Refinement: Controller State Audit

Systematic audit to identify and remove unused or write-only fields

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-21
**Complexity:** Trivial (1 pt)
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** System: character controller
**Principle:** Radical Simplicity, Single Source of Truth
**Severity:** Low
**Type:** State bloat - potential unused/write-only fields

**Current state:**
Controller struct contains fields that may be write-only or unused, similar to previously removed `ground_normal` and `ground_height`.

**Why violation:**
- **Radical Simplicity**: Every field must justify its existence
- **Single Source of Truth**: Unused fields suggest confusion about what state matters
- State bloat compounds maintenance burden

**Impact:**
- Blocks: None - isolated cleanup
- Cascades to: None - removal improves clarity
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Delete

**DELETE:**
- Remove: `last_acceleration` field (write-only)
- Impact: Written in `controller::update:280`, never read
- Tests: None required - pure deletion of unused state

**Verification:**
Grep confirms `last_acceleration` is:
- Written: `controller.cpp:280`
- Declared: `controller.h:30`
- Never read in codebase

Comment claims "for animation" but no animation system reads it.

**Note:** Other fields (`input_direction`, `collision_contact_debug`) ARE used by reactive systems and debug generation - keep these.
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Change:** Deleted write-only `last_acceleration` field
**Files Modified:**
- `src/character/controller.h:30` - Removed field declaration
- `src/character/controller.cpp:280` - Removed write assignment and comment

**Tests:** All passing (clean build)
**Result:** ✓ Violation removed - no animation system reads this field
<!-- END: REFINE/COMPLETED -->

---

<!-- BEGIN: MEASURE/METRICS -->
## Metrics

**Files:**
- `src/character/controller.h`: 156 → 155 (-1 line)
- `src/character/controller.cpp`: 298 → 295 (-3 lines)

**Total:** -4 lines (code only, excluding documentation)

**Violations removed:**
- Write-only fields: 1 → 0 (-1)
- Unused field references: 2 → 0 (-2)
<!-- END: MEASURE/METRICS -->

---

<!-- BEGIN: MEASURE/LEARNING -->
## Learning

**Root cause:** Optimistic field added for future animation system that never materialized.

**Prevention:** Delete speculative fields immediately when consumer doesn't exist within same work session.

**Pattern:** Controller audit reveals write-only pattern - other structs may have similar violations.
<!-- END: MEASURE/LEARNING -->
