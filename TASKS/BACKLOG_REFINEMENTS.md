# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Foundation:** 98%+ ✅ TARGET EXCEEDED
**Last Updated:** 2025-10-19
**Last Audit:** 2025-10-19 (Gemini + Manual dual analysis)

---

## Active Violations

### High Priority

**#13: GUI Direct State Modification - Camera Panel**
- **Location:** `src/gui/camera_panel.cpp:31-46` (draw_camera_panel)
- **Principles:** Fundamental Composable Functions, Consistency
- **Severity:** High
- **Type:** Tight coupling, Unidirectional data flow violation
- **Description:** camera_panel uses ImGui sliders that directly modify camera_follow state (distance, height_offset, min_distance, max_distance). This creates tight coupling between GUI and camera system. Inconsistent with character_panel.cpp which correctly uses command pattern (returns vector of commands). Violates unidirectional data flow principle.
- **Fix:** Refactor to command pattern - return vector of camera_command, apply in game_world similar to character_panel
- **Impact:** GUI-system coupling prevents composability, inconsistent architecture between panels
- **Audit Source:** Gemini + Manual convergence

### Medium Priority

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

**Foundation at 95%+ ✅ - 5 violations found (0 CRITICAL, 1 high, 1 medium, 3 low)**

**Audit Result:**
- **Convergence**: Excellent - Gemini (broad systematic) + Manual (targeted deep-dive)
- **New High Priority**: GUI direct state modification (#13) - camera_panel inconsistent with character_panel
- **Pattern**: GUI architecture inconsistency - character_panel uses commands, camera_panel directly modifies

**Path to restore 98%+ Layer 3:**
1. **PRIORITY: Fix #13** - Refactor camera_panel to command pattern (restore consistency)
2. Continue medium-priority refinements (#7)
3. OR **Build Layer 4 systems** after fixing #13
4. Address low-priority violations opportunistically (#9, #11-#12)

**Pattern Discovery:**
- **Duplication pattern** - ~~Redundant storage~~ ✅ FIXED, ~~per-frame allocation~~ ✅ FIXED
- **State management pattern** - ~~Multiple sources of truth~~ ✅ FIXED, ~~bidirectional flow~~ ✅ FIXED
- **Documentation debt** - Unjustified decisions (#4), magic numbers (#5)
- **Composability gaps** - Special cases (#3), mixed concerns (#6)

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
