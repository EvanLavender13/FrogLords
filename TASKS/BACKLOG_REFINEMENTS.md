# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Foundation:** 97%+ ✅ TARGET EXCEEDED
**Last Updated:** 2025-10-18
**Last Audit:** 2025-10-18 (Gemini + Codex dual analysis)

---

## Active Violations

### High Priority

**#3: Collision Resolution Mixed Responsibilities**
- **Location:** `src/foundation/collision.cpp:resolve_box_collisions`
- **Principles:** Composable Functions
- **Severity:** High
- **Type:** Mixed concerns, Non-orthogonal
- **Description:** Function both resolves collision (corrects position/velocity) AND determines grounded state by interpreting collision normal. Two orthogonal responsibilities coupled in one function.
- **Fix:** Simplify - Split responsibilities. Return detailed contact information (normals, penetration, etc.). Let controller interpret contact data to update its own grounded state. Collision system becomes pure primitive.
- **Impact:** System composability, testability
- **Audit Source:** Gemini + Codex convergence

### Medium Priority

**#4: Controller Hardwired to Input Polling**
- **Location:** `src/character/controller.cpp:48-88`
- **Principles:** Composable Functions
- **Severity:** Medium
- **Type:** Tight coupling, Non-reusable
- **Description:** `apply_input()` accepts camera-relative intent struct but still reaches out to global `input::` state for WASD/jump polling. Makes movement system impossible to reuse or test with synthetic input, undermines orthogonal primitives.
- **Fix:** Simplify - Route all player intent through explicit `controller_input` data structure. Controller becomes pure: takes input struct, produces new state. No global dependencies.
- **Impact:** Testability, composability, system reuse
- **Audit Source:** **Codex** (composability analysis)

**#5: Special Casing for Camera Modes**
- **Location:** `src/camera/camera.cpp:50-58`
- **Principles:** Composable Functions
- **Severity:** Medium
- **Type:** Special case, Overlapping responsibilities
- **Description:** `zoom()` has if/else for FOLLOW vs ORBIT modes using separate state (`follow_distance` vs `distance`). Modes not cleanly composed.
- **Fix:** Simplify - Refactor. Base camera handles view/projection, separate `Follower` component updates target position. Orthogonal systems.
- **Impact:** Camera system architecture

**#6: Unjustified Friction Model**
- **Location:** `src/character/tuning.cpp:8-17, 25-27`
- **Principles:** Mathematical Foundations, Principled Development
- **Severity:** Medium
- **Type:** Unjustified decision, Dimensionally strange
- **Description:** Friction based on `FRICTION_RATIO` percentage, not standard physics (F=μN). Formula `c.friction = (c.ground_accel * FRICTION_RATIO) / gravity_mag` lacks physical rationale.
- **Fix:** Document - Explain why this model chosen for game feel over physics-based. OR Replace with standard kinetic friction for principled, predictable system.
- **Impact:** Character movement feel, tuning predictability

**#7: Magic Numbers in Debug Visualization**
- **Location:** `src/app/debug_generation.cpp:170-180` (generate_collision_state_primitives)
- **Principles:** Radical Simplicity, Mathematical Foundations, Consistency
- **Severity:** Medium
- **Type:** Magic numbers, Arbitrary logic
- **Description:** Color logic uses if/else with unexplained constants (0.4f, 1.2f, 1.0f, 0.3f). No mathematical or physical justification. Inconsistent color rule application.
- **Fix:** Document - Define named constants explaining semantic meaning. OR Simplify - Use continuous gradient function instead of branching.
- **Impact:** Code clarity, debug visualization consistency
- **Audit Source:** Gemini + Codex convergence

**#8: Coyote Time / Jump Buffering Lacks Rationale**
- **Location:** `src/character/controller.cpp:apply_input, update`
- **Principles:** Consistency, Principled Development
- **Severity:** Medium
- **Type:** Undocumented principle violation
- **Description:** Special-case mechanics create intentional inconsistency - "jumping only when grounded" rule has exceptions for game feel. Implementation exists but lacks explicit comment explaining *why* this trade-off was made (Prime Directive > Consistency).
- **Fix:** Document - Add comment: "Violates Consistency principle for game feel - forgives near-miss jumps per Prime Directive (Do No Harm to Gameplay)."
- **Impact:** Future developer understanding, principle communication
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

**Use these to detect violations during development:**

### Pattern: Special Cases for Entities
- **Detection:** Search for `if (entity == ...)` or entity-type checks
- **Root cause:** Design trying to handle exceptions instead of general rules
- **Fix:** Make behavior uniform, remove special-case logic
- **Prevention:** Code review flag entity-specific branches

### Pattern: Frame-Rate Dependent Behavior
- **Detection:** Behavior changes at different frame rates
- **Root cause:** Missing delta-time multiplication for rates
- **Fix:** Always multiply rates by `dt`
- **Prevention:** Unit analysis - rates should have `/time` dimension

### Pattern: Custom Smoothing Instead of Primitives
- **Detection:** Search for `std::exp`, custom `lerp` with time, manual smoothing
- **Root cause:** Unaware of spring-damper or thinking custom is "simpler"
- **Fix:** Replace with `spring_damper` primitive
- **Prevention:** Default to spring-damper for all smoothing needs
- **Related fixes:** Orientation (2025-10-17)

### Pattern: Principle Without Exception Documentation
- **Detection:** Absolute principle statements that have legitimate exceptions
- **Root cause:** Principles defined without documenting edge cases
- **Fix:** Add exception clauses to principles at definition time
- **Prevention:** When defining principles, ask "are there legitimate exceptions?"
- **Related fixes:** Accumulated state pattern (2025-10-18)

### Pattern: Dual-Reference (Smoothed referencing itself)
- **Detection:** Target value calculated from smoothed value
- **Root cause:** Confusing intent (target) with state (current)
- **Fix:** Keep target and current separate
- **Prevention:** "Never let a smoothed value reference itself" (PRINCIPLES.md)
- **Related fixes:** None yet (pattern identified, not yet observed)

### Pattern: Magic Numbers
- **Detection:** Unexplained numeric constants
- **Root cause:** Lack of documentation discipline
- **Fix:** Document with derivation, units, and classification tag
- **Prevention:** Require comment on every constant declaration
- **Related fixes:** All constants (2025-10-17)

### Pattern: Division by Zero
- **Detection:** Normalization without epsilon check, `length = sqrt(x); result = vec / length`
- **Root cause:** Unchecked division when vector magnitude could be zero
- **Fix:** Always use `safe_normalize` for vector normalization—never divide by magnitude without epsilon check
- **Prevention:** Use math primitives (safe_normalize) instead of manual division
- **Related fixes:** Collision detection (2025-10-18)

### Pattern: Bidirectional Data Flow
- **Detection:** Systems with both `apply_to()` and `read_from()`
- **Root cause:** Unclear ownership of truth
- **Fix:** Establish single source of truth, unidirectional flow
- **Prevention:** Prefer one-way data flow
- **Related fixes:** Tuning defaults (2025-10-17)

### Pattern: Convenient Coupling
- **Detection:** Struct handles multiple unrelated concerns; "just add a member" changes
- **Root cause:** Feature accretion without architectural discipline
- **Fix:** Extract subsystems to separate types with unidirectional data flow
- **Prevention:** Default to separation; challenge convenience; require test isolation
- **Related fixes:** Controller (2025-10-18)

### Pattern: Redundant Storage
- **Detection:** Same data stored in multiple places requiring manual synchronization
- **Root cause:** Separation of visual/physical or multiple system representations
- **Fix:** Create single source of truth, systems read from unified representation
- **Prevention:** Question any data duplication. Prefer single authoritative storage.
- **Related fixes:** World geometry (2025-10-18)

### Pattern: Multiple Truth Sources
- **Detection:** Different code paths modify same logical state through different variables
- **Root cause:** Unclear ownership, convenience over correctness
- **Fix:** Single path for modification, one authoritative update function
- **Prevention:** Enforce unidirectional data flow, reject shortcuts
- **Related fixes:** Character state management (2025-10-18)

### Pattern: Per-Frame Allocation
- **Detection:** Creating/destroying resources every frame instead of reusing
- **Root cause:** Unfamiliarity with graphics APIs, quick implementation
- **Fix:** Use persistent buffers, dynamic updates, proper resource management
- **Prevention:** Review resource lifecycle in rendering code
- **Related fixes:** Renderer buffers (2025-10-18)

### Pattern: Mode-Specific Branching (Discovered 2025-10-18)
- **Detection:** If/else blocks for different modes using separate state variables
- **Root cause:** Modes not designed as composable components
- **Fix:** Refactor to orthogonal systems, base + mode-specific components
- **Prevention:** Design modes as composable pieces, not special cases
- **Related fixes:** Camera zoom (#4)

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

**Foundation at 97%+ ✅ - 9 violations found (0 CRITICAL, 1 high, 5 medium, 3 low)**

**Audit Result:**
- **Convergence**: Excellent - Gemini (broad systematic) + Codex (deep mathematical)
- **High Priority**: ~~GUI coupling (#1)~~ ✅ FIXED, Collision responsibilities (#3)
- **Pattern**: Violations in non-core systems (GUI, debug, tooling) - foundation solid

**Path to maintain 97%+ Layer 3:**
1. **Collision responsibilities** (#3) - Composability issue ← **NEXT**
2. Continue medium-priority refinements (#4-#8)
3. OR **Build Layer 4 systems** - Foundation stable at 97%+
4. Address low-priority violations opportunistically (#9-#11)

**Pattern Discovery:**
- **Duplication pattern** - ~~Redundant storage~~ ✅ FIXED, ~~per-frame allocation~~ ✅ FIXED
- **State management pattern** - ~~Multiple sources of truth~~ ✅ FIXED, ~~bidirectional flow~~ ✅ FIXED
- **Documentation debt** - Unjustified decisions (#4), magic numbers (#5)
- **Composability gaps** - Special cases (#3), mixed concerns (#6)

---

## Recent Activity

**GUI Coupling Fixed (2025-10-18)**
- Implemented command pattern for unidirectional data flow
- GUI: Composable Functions 6→9 (+3), Principled Development restored
- Foundation: 97%+ maintained (architectural integrity)
- 9 violations remaining (0 critical, 1 high, 5 medium, 3 low)

**See individual plan files for detailed metrics:**
- `PLANS/REFINE_gui_coupling.md` (2025-10-18) - Composable Functions +3, Architecture restored
- `PLANS/REFINE_mouse_camera_jump.md` (2025-10-18) - Prime Directive +FIXED, Consistency +FIXED
- `PLANS/REFINE_division_by_zero.md` (2025-10-18) - Mathematical Foundations +CRITICAL FIX
- `PLANS/REFINE_gradient_continuity.md` (2025-10-18) - Mathematical Foundations +2.0
- `PLANS/REFINE_embedded_tests.md` (2025-10-18) - Composable Functions +2.0
- `PLANS/REFINE_buffer_creation.md` (2025-10-18) - Radical Simplicity +3.0
- `PLANS/REFINE_character_state_management.md` (2025-10-18) - Consistency +2.0
- `PLANS/REFINE_world_geometry_duplication.md` (2025-10-18) - Radical Simplicity +2.0
- `PLANS/REFINE_accumulated_state_pattern.md` (2025-10-18) - Principled Development +1.0
- `PLANS/REFINE_controller_mixed_concerns.md` (2025-10-18) - Composable Functions +1.0

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
