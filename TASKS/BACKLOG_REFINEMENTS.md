# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Foundation:** 95.5% ✅ TARGET EXCEEDED
**Last Updated:** 2025-10-18

---

## Active Violations

### High Priority

**#1: Inconsistent Character State Management**
- **Location:** `src/gui/character_panel.cpp:13-20`
- **Principles:** Consistency
- **Severity:** High
- **Type:** Bidirectional Data Flow, Multiple Truth Sources
- **Description:** GUI modifies both `controller` directly (e.g. `max_speed`) and `tuning_params` struct, creating two paths for state changes. Changing one makes the other incorrect.
- **Fix:** Simplify - GUI modifies only `tuning_params`. The `apply_to()` function is single source of truth for updates.
- **Impact:** Affects tuning, GUI, character controller

### Medium Priority

**#2: Inefficient Per-Draw Buffer Creation**
- **Location:** `src/rendering/renderer.cpp:61-71`
- **Principles:** Radical Simplicity
- **Severity:** Medium
- **Type:** Complexity, Performance
- **Description:** Creates and destroys new vertex/index buffers for every mesh drawn per frame. Highly inefficient even if driver defers operations.
- **Fix:** Simplify - Use single dynamic vertex buffer for entire frame. Append all vertex data, issue draws with offsets.
- **Impact:** Rendering performance, GPU memory management

**#3: Special Casing for Camera Modes**
- **Location:** `src/camera/camera.cpp:50-58`
- **Principles:** Composable Functions
- **Severity:** Medium
- **Type:** Special case, Overlapping responsibilities
- **Description:** `zoom()` has if/else for FOLLOW vs ORBIT modes using separate state (`follow_distance` vs `distance`). Modes not cleanly composed.
- **Fix:** Simplify - Refactor. Base camera handles view/projection, separate `Follower` component updates target position. Orthogonal systems.
- **Impact:** Camera system architecture

**#4: Unjustified Friction Model**
- **Location:** `src/character/tuning.cpp:8-17, 25-27`
- **Principles:** Mathematical Foundations, Principled Development
- **Severity:** Medium
- **Type:** Unjustified decision, Dimensionally strange
- **Description:** Friction based on `FRICTION_RATIO` percentage, not standard physics (F=μN). Formula `c.friction = (c.ground_accel * FRICTION_RATIO) / gravity_mag` lacks physical rationale.
- **Fix:** Document - Explain why this model chosen for game feel over physics-based. OR Replace with standard kinetic friction for principled, predictable system.
- **Impact:** Character movement feel, tuning predictability

### Low Priority

**#5: Magic Number for Collision Passes**
- **Location:** `src/foundation/collision.cpp:41`
- **Principles:** Mathematical Foundations
- **Severity:** Low
- **Type:** Magic number
- **Description:** Hard-coded `for (int pass = 0; pass < 3; ++pass)` for collision resolution. Number `3` is arbitrary, lacks mathematical guarantee.
- **Fix:** Document - Define as named constant `COLLISION_RESOLUTION_PASSES = 3` with comment explaining empirical choice balancing performance/accuracy.
- **Impact:** Code clarity

**#6: Embedded Test Suite in Runtime**
- **Location:** `src/app/runtime.cpp:20-25`
- **Principles:** Principled Development, Composable Functions
- **Severity:** Low
- **Type:** Workaround, Mixed concerns
- **Description:** `#ifdef` runs quaternion test suite in runtime initialization. Mixes test with application code despite existing `/tests` directory.
- **Fix:** Delete - Move tests to proper test file in `/tests` directory using existing infrastructure.
- **Impact:** Code organization

**#7: Discontinuous Color Gradient Logic**
- **Location:** `src/app/debug_generation.cpp:49-59`
- **Principles:** Mathematical Foundations
- **Severity:** Low
- **Type:** Unjustified implementation, Discontinuous
- **Description:** Speed gradient uses if/else with hard-coded thresholds (0.33, 0.66) creating sharp jumps instead of smooth gradient.
- **Fix:** Simplify - Replace conditionals with continuous interpolation (`glm::mix`) for smooth color blend across speed range.
- **Impact:** Debug visualization quality

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

### Pattern: Multiple Truth Sources (Discovered 2025-10-18)
- **Detection:** Different code paths modify same logical state through different variables
- **Root cause:** Unclear ownership, convenience over correctness
- **Fix:** Single path for modification, one authoritative update function
- **Prevention:** Enforce unidirectional data flow, reject shortcuts
- **Related fixes:** Character state management (#2)

### Pattern: Per-Frame Allocation (Discovered 2025-10-18)
- **Detection:** Creating/destroying resources every frame instead of reusing
- **Root cause:** Unfamiliarity with graphics APIs, quick implementation
- **Fix:** Use persistent buffers, dynamic updates, proper resource management
- **Prevention:** Review resource lifecycle in rendering code
- **Related fixes:** Renderer buffers (#3)

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

**Last Audit:** 2025-10-18 (Gemini comprehensive review)
**Next Audit:** After completing high-priority refinements

---

## Priority Order

**Foundation at 95.5% ✅ - 7 violations remaining**

**Recommended path:**
1. Fix high-priority violation (#1) - Foundation impact
2. Assess: Build Layer 4 OR continue medium-priority refinements
3. Address low-priority violations opportunistically

**Pattern Discovery:**
- **Duplication pattern** - ~~Redundant storage~~ ✅ FIXED, per-frame allocation (#2)
- **State management pattern** - Multiple sources of truth (#1), bidirectional flow (existing pattern)
- **Documentation debt** - Unjustified decisions (#4), magic numbers (#5)
- **Composability gaps** - Special cases (#3), mixed concerns (#6)

---

## Recent Activity

**World Geometry Duplication Fixed (2025-10-18)**
- Redundant storage violation removed - scene storage was dead code
- Foundation: 95% → 95.5% (+0.5%)
- Layer 3: 95% → 96% (+1%)
- 7 violations remaining (1 high, 3 medium, 3 low)

**See individual plan files for detailed metrics:**
- `PLANS/REFINE_world_geometry_duplication.md` (2025-10-18) - Radical Simplicity +2.0
- `PLANS/REFINE_accumulated_state_pattern.md` (2025-10-18) - Principled Development +1.0
- `PLANS/REFINE_controller_mixed_concerns.md` (2025-10-18) - Composable Functions +1.0
- `PLANS/REFINE_orientation_dual_reference.md` (2025-10-17) - Composability +2.5

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
