# Refinement: Test Level Literals

Extract named constants for test arena geometry.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-22
**Complexity:** Trivial (1 pt)
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/app/game_world.cpp:141-199`
**Principle:** Radical Simplicity
**Severity:** Low
**Type:** Scattered literals without semantic meaning

**Current state:**
Test arena construction uses bare numeric literals for all geometry:
- Platform heights/spacing: `1.0f`, `1.5f`, `4.0f`
- Wall positions: `6.0f`, `-10.0f`, `8.0f`
- Platform extents: `2.0f`, `0.2f`
- Step heights: `0.15f`

Every adjustment requires hunting through function body to find and understand magic numbers.

**Why violation:**
- Literals lack semantic meaning—what is `1.5f` at line 153?
- Changes require manual edits throughout function
- Reads as pile of special cases rather than system with parameters
- No clear distinction between dimensions, offsets, and spacing

**Impact:**
- Blocks: None (test-only code)
- Cascades to: Maintainability of test arena only
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**FROM:** Scattered numeric literals throughout function body.

**TO:** Named constants at function scope declaring semantic meaning.

**Implementation:** Extract constants for platform system (base height, height increment, spacing, dimensions), wall geometry (thickness, positions), and step geometry (base height, step increment, dimensions).

**Check duplicates:** Platform thickness `0.2f` appears multiple times—consolidate.
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Change:** Extracted 15 named constants (UPPER_CASE) for test arena geometry
**Files Modified:**
- `src/app/game_world.cpp:141-218` - Added constexpr constants, replaced all magic numbers
- `CONVENTIONS.md:15-17` - Documented UPPER_CASE convention for constexpr constants

**Constants extracted:**
- Platform system: base height, height increment, z start/spacing, dimensions, count
- Wall geometry: thickness (consolidated 5 occurrences of `0.2f`)
- Step geometry: height increment, x start/spacing, dimensions, count

**Tests:** All passing
**Manual verification:** Visual behavior unchanged
**Result:** ✓ Violation removed
<!-- END: REFINE/COMPLETED -->

---

<!-- BEGIN: MEASURE/METRICS -->
## Metrics

**Files:**
- `src/app/game_world.cpp`: 199 → 218 (+19 lines)
- `CONVENTIONS.md`: 64 → 65 (+1 line)

**Total:** +20 lines (added constant declarations, net positive for clarity)

**Violations removed:**
- Magic number literals: 35 occurrences replaced with 15 named constants
- Duplicate literals: `0.2f` (6×) → `WALL_THICKNESS` (1 definition)
<!-- END: MEASURE/METRICS -->

---

<!-- BEGIN: MEASURE/LEARNING -->
## Learning

**Root cause:** Test-only code written quickly without extraction discipline—literals used directly in geometry calculations.

**Prevention:** Extract constants immediately when defining test geometry. Group related constants with comment headers. Use UPPER_CASE for all constexpr values per conventions.

**Pattern:** Magic numbers accumulate in test/setup code because "it's just test data"—apply same standards everywhere.
<!-- END: MEASURE/LEARNING -->
