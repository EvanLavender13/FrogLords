# Refinement: Debug Draw Batching

Remove premature optimization from debug rendering path.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-19
**Complexity:** Standard
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/rendering/debug_draw.cpp:19-53`
**Principle:** Radical Simplicity | Score: Low
**Severity:** Low
**Type:** Premature optimization, Unnecessary complexity

**Current state:**
```
Debug line rendering sorts by color, creates batches, uses custom comparator.
Significant complexity for performance gain that may not be measurable.
```

**Why violation:**
- Adds complexity to non-critical debug path
- No evidence that batching provides measurable benefit
- Debug rendering is not performance-critical
- Violates "addition by subtraction" - optimization added without proof of need

**Impact:**
- Blocks: Code clarity, maintainability
- Cascades to: Future debug visualization changes
- Foundation: 99%+ (not blocking)
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**SIMPLIFY:**
- From: Sort + batch debug lines by color with custom comparator
- To: Direct rendering without batching
- Derive/Generalize: If profiling shows performance issue, reintroduce with data
- Check duplicates: No other debug systems use this pattern

**Process:**
1. Read current implementation to understand batching logic
2. Replace with straightforward rendering:
   - Iterate debug lines directly
   - Set color per line
   - Draw immediately
3. Verify debug visualizations still work
4. If performance degrades noticeably, measure and document before re-optimizing

**Validation:**
- Run game with heavy debug visualization
- Observe frame time remains acceptable
- Verify all debug primitives render correctly
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Batching logic removed
- [ ] Debug rendering simplified
- [ ] All debug visualizations working
- [ ] No measurable performance regression

**Metrics:**
- Before: LOC ~35, Principle 7/10, Complexity High
- After: LOC ~15 (-20), Principle 9/10 (+2), Complexity Low
<!-- END: SELECT/SUCCESS -->
