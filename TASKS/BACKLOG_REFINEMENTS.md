# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Foundation:** 99%+ ✅ TARGET EXCEEDED
**Last Updated:** 2025-10-19

---

## Active Violations

### High Priority

None - All high-priority violations resolved ✅

### Medium Priority

None - All medium-priority violations resolved ✅


### Low Priority

**#9: Magic Number for Collision Passes**
- **Location:** `src/foundation/collision.cpp:41`
- **Principles:** Mathematical Foundations
- **Severity:** Low
- **Type:** Magic number
- **Description:** Hard-coded `for (int pass = 0; pass < 3; ++pass)` for collision resolution. Number `3` is arbitrary, lacks mathematical guarantee.
- **Fix:** Document - Define as named constant `COLLISION_RESOLUTION_PASSES = 3` with comment explaining empirical choice balancing performance/accuracy.
- **Impact:** Code clarity

**#11: Hardcoded Test Level Literals**
- **Location:** `src/app/game_world.cpp:62-111`
- **Principles:** Radical Simplicity, Emergent Behavior
- **Severity:** Low
- **Type:** Scattered literals, Special cases
- **Description:** Test arena hard-coded as scattered literals without named primitives (tile size, offsets). Every tweak requires manual edit, reads as pile of special cases rather than composable system.
- **Fix:** Simplify - Extract named constants for dimensions/offsets. OR Accept - Temporary test code, note for future data-driven/procedural system.
- **Impact:** Code clarity, maintainability (non-critical - test only)
- **Audit Source:** **Codex** + Gemini convergence

**#12: Debug Draw Batching Complexity**
- **Location:** `src/rendering/debug_draw.cpp:19-53`
- **Principles:** Radical Simplicity
- **Severity:** Low
- **Type:** Premature optimization, Unnecessary complexity
- **Description:** Sorts debug lines by color with mutable copy, custom comparator, and batching logic. Adds significant complexity to non-critical debug path for performance gain that may not be measurable.
- **Fix:** Simplify - Remove batching unless profiling shows it's necessary. OR Document - Justify with benchmarks showing meaningful performance improvement.
- **Impact:** Debug code complexity, maintainability
- **Audit Source:** Gemini + Codex convergence

---

## Patterns Library

**Pattern detection and prevention guidance:**

@TASKS/PATTERNS.md

---

## Priority Order

**Foundation at 99%+ ✅ - 3 violations found (0 CRITICAL, 0 high, 0 medium, 3 low)**

**Next Actions:**
1. **Build Layer 4 systems** (foundation stable at 99%+)
2. Address low-priority violations opportunistically (#9, #11-#12)

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
