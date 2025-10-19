# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Foundation:** 97%+ ✅ TARGET EXCEEDED
**Last Updated:** 2025-10-18
**Last Audit:** 2025-10-18 (Gemini + Codex dual analysis)

---

## Active Violations

### High Priority

None - all high-priority violations resolved ✅

### Medium Priority

**#5: Special Casing for Camera Modes**
- **Location:** `src/camera/camera.cpp:50-58`
- **Principles:** Composable Functions
- **Severity:** Medium
- **Type:** Special case, Overlapping responsibilities
- **Description:** `zoom()` has if/else for FOLLOW vs ORBIT modes using separate state (`follow_distance` vs `distance`). Modes not cleanly composed.
- **Fix:** Simplify - Refactor. Base camera handles view/projection, separate `Follower` component updates target position. Orthogonal systems.
- **Impact:** Camera system architecture

**#7: Magic Numbers in Debug Visualization**
- **Location:** `src/app/debug_generation.cpp:170-180` (generate_collision_state_primitives)
- **Principles:** Radical Simplicity, Mathematical Foundations, Consistency
- **Severity:** Medium
- **Type:** Magic numbers, Arbitrary logic
- **Description:** Color logic uses if/else with unexplained constants (0.4f, 1.2f, 1.0f, 0.3f). No mathematical or physical justification. Inconsistent color rule application.
- **Fix:** Document - Define named constants explaining semantic meaning. OR Simplify - Use continuous gradient function instead of branching.
- **Impact:** Code clarity, debug visualization consistency
- **Audit Source:** Gemini + Codex convergence


### Low Priority

**#9: Magic Number for Collision Passes**
- **Location:** `src/foundation/collision.cpp:41`
- **Principles:** Mathematical Foundations
- **Severity:** Low
- **Type:** Magic number
- **Description:** Hard-coded `for (int pass = 0; pass < 3; ++pass)` for collision resolution. Number `3` is arbitrary, lacks mathematical guarantee.
- **Fix:** Document - Define as named constant `COLLISION_RESOLUTION_PASSES = 3` with comment explaining empirical choice balancing performance/accuracy.
- **Impact:** Code clarity

**#10: Unused Delta-Time Parameter**
- **Location:** `src/camera/camera.cpp:78-86`
- **Principles:** Radical Simplicity
- **Severity:** Low
- **Type:** Dead parameter, Unclear intent
- **Description:** `set_follow_target()` takes `dt` parameter but never uses it. Obscures whether follow mode is intentionally instantaneous or missing time-based smoothing.
- **Fix:** Delete - Remove unused parameter. OR Document - Add comment explaining why instantaneous is correct.
- **Impact:** Code clarity, cognitive load
- **Audit Source:** **Codex**

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

## Audit Checklist

**Run periodically to discover new violations:**

### Code Smells
- [ ] Files >500 lines
- [ ] Functions >50 lines
- [ ] High cyclomatic complexity
- [ ] Deep nesting (>3 levels)
- [ ] Long parameter lists (>5 params)

### Principle Violations
- [ ] Unexplained constants (magic numbers)
- [ ] Entity-type checks (special cases)
- [ ] Player-specific logic
- [ ] Hard-coded sequences
- [ ] Copy-pasted code
- [ ] Animation blocking physics
- [ ] Non-deterministic behavior
- [ ] Frame-rate dependent behavior

### Documentation Gaps
- [ ] Emergent behaviors not in EMERGENCE.md
- [ ] Undocumented design decisions
- [ ] Missing derivations for tuned values
- [ ] TODO/FIXME comments

**Last Audit:** 2025-10-18 (**Dual Analysis:** Gemini + Codex convergence)
**Next Audit:** After resolving high-priority violations

---

## Priority Order

**Foundation at 97%+ ✅ - 6 violations found (0 CRITICAL, 0 high, 3 medium, 3 low)**

**Audit Result:**
- **Convergence**: Excellent - Gemini (broad systematic) + Codex (deep mathematical)
- **High Priority**: ~~GUI coupling (#1)~~ ✅ FIXED, ~~Collision responsibilities (#3)~~ ✅ FIXED
- **Pattern**: All high-priority violations resolved - foundation solid

**Path to maintain 97%+ Layer 3:**
1. Continue medium-priority refinements (#5-#7)
2. OR **Build Layer 4 systems** - Foundation stable at 97%+
3. Address low-priority violations opportunistically (#9-#12)

**Pattern Discovery:**
- **Duplication pattern** - ~~Redundant storage~~ ✅ FIXED, ~~per-frame allocation~~ ✅ FIXED
- **State management pattern** - ~~Multiple sources of truth~~ ✅ FIXED, ~~bidirectional flow~~ ✅ FIXED
- **Documentation debt** - Unjustified decisions (#4), magic numbers (#5)
- **Composability gaps** - Special cases (#3), mixed concerns (#6)

---

## Recent Activity

**Coyote Time Rationale Documented (2025-10-18)**
- Documented principle trade-off: Prime Directive > Consistency
- Controller: Principled Development 8→9 (+1)
- Layer 1: 96% → 96.5% (+0.5%)
- Foundation: 97%+ maintained
- 6 violations remaining (0 critical, 0 high, 3 medium, 3 low)

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
