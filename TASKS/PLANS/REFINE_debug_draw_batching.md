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

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

### Step 1: Simplify Line Rendering
**Changes:** `src/rendering/debug_draw.cpp:25-72` - Remove sorting, batching, comparator
**From:** Sort lines by color, build batches, flush on color change
**To:** Iterate lines directly, draw each individually
**Tests:** None (debug visualization)
**Validation:** Run game with debug collision shapes enabled

### Step 2: Verify Debug Visualizations
**Manual test:** Enable all debug visualizations
- Collision shapes (bodies, tree)
- Camera bounds
- Navigation/AI paths
**Validation:** All primitives render correctly, no performance degradation

## Rollback
`git reset --hard HEAD`
<!-- END: REFINE/PLAN -->

---

<!-- BEGIN: REFINE/REVIEW -->
## Second Opinion Review

**Tool:** Codex CLI
**Date:** 2025-10-19

**Question asked:**
Review this refinement plan to remove premature optimization from debug rendering. Does the simplification align with Radical Simplicity principle? Is direct iteration per-line acceptable for debug rendering performance? Are there any edge cases or correctness concerns? Should any of the batching logic be preserved?

**Concerns evaluated:**
- Alignment with Radical Simplicity principle
- Performance implications of direct iteration
- Edge cases and correctness
- Whether any batching should remain

**Feedback received:**
- Simplification matches Radical Simplicity: removes unproven complexity from non-critical path
- Direct iteration fine for debug use: even hundreds of extra draw calls stay under threshold
- Edge case: construct fresh wireframe_mesh per line so vertices/edges reset cleanly
- No batching logic needs to survive: reintroduce with evidence if profiling shows need

**Impact on implementation:**
- Confirmed approach: remove all batching logic
- Implementation detail: create fresh wireframe_mesh for each line
- No changes to plan needed
<!-- END: REFINE/REVIEW -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Change:** Removed color sorting, batching, and custom comparator from debug line rendering
**From:** 48 lines with sorting, batch building, color change detection
**To:** 8 lines with direct iteration and individual draws
**Code reduction:** -40 lines in debug_draw.cpp, removed `<algorithm>` include
**Tests:** Build passing, manual verification complete
**Result:** ✓ Violation removed - Radical Simplicity restored

**Verification:**
- All debug primitives render correctly
- Colors display properly
- No visual artifacts
- No performance degradation
<!-- END: REFINE/COMPLETED -->
