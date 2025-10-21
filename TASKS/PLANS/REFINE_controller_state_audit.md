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
