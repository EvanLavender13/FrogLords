# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Foundation:** 99%+ ✅ TARGET EXCEEDED
**Last Updated:** 2025-10-19
**Last Audit:** 2025-10-19 (Gemini + Manual dual analysis)

---

## Active Violations

### High Priority

None - All high-priority violations resolved ✅

### Medium Priority

**#14: Semantic Collision Types for Debug Visualization**
- **Location:** `src/foundation/collision.h`, `src/app/debug_generation.cpp:207-225`
- **Principles:** Radical Simplicity, Fundamental Composable Functions, Consistency
- **Severity:** Medium
- **Type:** Missing semantic information, Coupling to geometry
- **Description:** Collision boxes lack semantic type information (wall/floor/platform). Debug visualization cannot reliably categorize boxes based on dimensions alone. Attempted heuristics (height < 0.4f, etc.) fail because geometry doesn't encode intent.
- **Fix:** Add optional semantic type enum to collision_box (wall/floor/platform/dynamic). Update test arena to specify types. Debug visualization uses types directly rather than inferring from dimensions.
- **Impact:** Debug visualization clarity, future gameplay systems (wall-run, ledge-grab, etc.)
- **Audit Source:** Manual verification during debug_viz_colors refinement


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

**Last Audit:** 2025-10-19 (**Dual Analysis:** Gemini + Manual convergence)
**Next Audit:** After resolving high-priority violations

---

## Priority Order

**Foundation at 99%+ ✅ - 4 violations found (0 CRITICAL, 0 high, 1 medium, 3 low)**

**Audit Result:**
- **Convergence**: Excellent - Gemini (broad systematic) + Manual (targeted deep-dive)
- ~~**New High Priority**: GUI direct state modification (#13)~~ ✅ FIXED (2025-10-19)
- **Pattern**: GUI architectural consistency restored

**Next Actions:**
1. Consider medium-priority refinement (#14 - semantic collision types)
2. OR **Build Layer 4 systems** (foundation stable at 99%+)
3. Address low-priority violations opportunistically (#9, #11-#12)

**Pattern Discovery:**
- **Duplication pattern** - ~~Redundant storage~~ ✅ FIXED, ~~per-frame allocation~~ ✅ FIXED
- **State management pattern** - ~~Multiple sources of truth~~ ✅ FIXED, ~~bidirectional flow~~ ✅ FIXED
- **Documentation debt** - Unjustified decisions (#4), magic numbers (#5)
- **Composability gaps** - Special cases (#3), mixed concerns (#6)

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
