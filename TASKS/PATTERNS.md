# Pattern Detection Guide

**Code and process anti-patterns. Updated after retrospectives.**

---

## Process Anti-Patterns

**Pattern: Contract/Implementation Mismatch**
- **Detection:** Read contract claims, trace through implementation to verify
- **Root cause:** Documentation written before understanding, not updated after learning
- **Fix:** Align contract with actual behavior or fix implementation
- **Prevention:** External review validates contracts against code

**Pattern: Stale Documentation After Changes**
- **Detection:** Grep for related values throughout codebase after changing constants
- **Root cause:** Local changes don't trigger documentation sweep
- **Fix:** Update all references when values change
- **Prevention:** Explicit documentation sweep step in workflow

**Pattern: Plan Section Divergence**
- **Detection:** Compare approach described in different plan sections
- **Root cause:** Sections written at different times without reconciliation
- **Fix:** Align all sections to single approach before starting
- **Prevention:** External review before implementation

**Pattern: Missing Invariant Enforcement**
- **Detection:** Search for bounds, constraints, min/max pairs without enforcement code
- **Root cause:** Contract specifies invariant but implementation forgets to maintain it
- **Fix:** Add assertions and enforcement at state mutation points
- **Prevention:** Pre-implementation review checks enforcement locations

**Pattern: Process Overhead for Simple Changes**
- **Detection:** Documentation substantially larger than code change
- **Root cause:** No lightweight path exists for trivial fixes
- **Fix:** Use lightweight process for pure deletions and simple fixes
- **Prevention:** Workflow distinguishes trivial from complex changes

---

## Code Anti-Patterns

---

## Pattern: Special Cases for Entities
- **Detection:** Search for `if (entity == ...)` or entity-type checks
- **Root cause:** Design trying to handle exceptions instead of general rules
- **Fix:** Make behavior uniform, remove special-case logic
- **Prevention:** Code review flag entity-specific branches

## Pattern: Custom Smoothing Instead of Primitives
- **Detection:** Search for `std::exp`, custom `lerp` with time, manual smoothing
- **Root cause:** Unaware of spring-damper or thinking custom is "simpler"
- **Fix:** Replace with `spring_damper` primitive
- **Prevention:** Default to spring-damper for all smoothing needs

## Pattern: Principle Without Exception Documentation
- **Detection:** Absolute principle statements that have legitimate exceptions
- **Root cause:** Principles defined without documenting edge cases
- **Fix:** Add exception clauses to principles at definition time
- **Prevention:** When defining principles, ask "are there legitimate exceptions?"

## Pattern: Magic Numbers
- **Detection:** Unexplained numeric constants
- **Root cause:** Lack of documentation discipline
- **Fix:** Document with derivation, units, and classification tag
- **Prevention:** Require comment on every constant declaration

## Pattern: Division by Zero
- **Detection:** Normalization without epsilon check, `length = sqrt(x); result = vec / length`
- **Root cause:** Unchecked division when vector magnitude could be zero
- **Fix:** Always use `safe_normalize` for vector normalization—never divide by magnitude without epsilon check
- **Prevention:** Use math primitives (safe_normalize) instead of manual division

## Pattern: Convenient Coupling
- **Detection:** Struct handles multiple unrelated concerns; "just add a member" changes
- **Root cause:** Feature accretion without architectural discipline
- **Fix:** Extract subsystems to separate types with unidirectional data flow
- **Prevention:** Default to separation; challenge convenience; require test isolation

## Pattern: Per-Frame Allocation
- **Detection:** Creating/destroying resources every frame instead of reusing
- **Root cause:** Unfamiliarity with graphics APIs, quick implementation
- **Fix:** Use persistent buffers, dynamic updates, proper resource management
- **Prevention:** Review resource lifecycle in rendering code

## Pattern: Mode-Specific Branching
- **Detection:** If/else blocks for different modes using separate state variables
- **Root cause:** Modes not designed as composable components
- **Fix:** Refactor to orthogonal systems, base + mode-specific components
- **Prevention:** Design modes as composable pieces, not special cases

## Pattern: Input Modification Before System Processing
- **Detection:** Input values scaled, filtered, or modified before reaching target system
- **Root cause:** Convenience of modifying input at source instead of proper system parameters
- **Fix:** Pass raw input to systems, let systems apply their own parameters
- **Prevention:** Input is intent - never modify before it reaches the system that needs it

## Pattern: Arbitrary Fallback Constants
- **Detection:** Functions returning arbitrary constants when unable to compute correct value
- **Root cause:** Avoiding degenerate case handling with default constant
- **Fix:** Compute correct answer from geometry or use safe primitives with meaningful fallback
- **Prevention:** Never return arbitrary constants - derive from mathematics or assert if invalid

## Pattern: Input Loss Through Overwriting
- **Detection:** Event handlers overwriting accumulated state instead of accumulating
- **Root cause:** Single-frame assumptions when multiple events can occur per frame
- **Fix:** Accumulate input events or process immediately
- **Prevention:** All input must accumulate or be processed immediately - never overwrite

---

**When a pattern repeats, add it here. When adding code, check against these first.**
