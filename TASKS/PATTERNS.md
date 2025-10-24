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

## Pattern: Derive, Don't Accumulate
- **Detection:** State accumulating over time (`+=`, timeline building, frame counters) instead of being derived; frame-rate dependent behavior
- **Root cause:** Easier to accumulate than derive; missing single source of truth; using time as proxy for distance/events
- **Fix:** Identify canonical state, derive dependent values; use frame-independent measures (distance, events, phase)
- **Prevention:** Ask "what is the single source of truth?" before accumulating; test at different frame rates
- **Examples:** Surveyor wheel distance_traveled, velocity trail timestamps/time sampling, frame-dependent dt accumulation

## Pattern: Use Primitives
- **Detection:** Custom smoothing (`std::exp`, manual lerp with time); normalization without epsilon check; reinventing foundation utilities
- **Root cause:** Unaware of primitives or thinking custom is simpler; unchecked division
- **Fix:** Use `spring_damper` for smoothing, `safe_normalize` for normalization, existing primitives for standard operations
- **Prevention:** Check foundation layer before implementing; never divide by magnitude without epsilon check
- **Examples:** Custom exponential smoothing vs spring_damper, manual `vec/length` vs safe_normalize

## Pattern: Single Source for Configuration
- **Detection:** Unexplained numeric constants; parameters defined multiple times; mismatched dimensions; split control logic
- **Root cause:** Taking shortcuts during initial implementation; lack of documentation discipline
- **Fix:** Centralize in tuning/config system, derive dependent values; document with derivation, units, and classification
- **Prevention:** Configuration lives in dedicated structures; require comment on every constant; grep for duplication
- **Examples:** Spring stiffness in constructors, ground geometry visual vs collision, magic numbers, camera control direction split

## Pattern: No Special Cases
- **Detection:** `if (entity == ...)` checks; mode-specific if/else blocks; type-based branching
- **Root cause:** Design trying to handle exceptions instead of general rules; modes not designed as composable components
- **Fix:** Make behavior uniform; refactor to orthogonal systems with base + mode-specific components
- **Prevention:** Code review flag entity-specific branches; design modes as composable pieces
- **Examples:** Entity-type checks, mode branching with separate state variables

## Pattern: Input Integrity
- **Detection:** Input values modified before reaching target system; event handlers overwriting accumulated input
- **Root cause:** Convenience of modifying at source; single-frame assumptions when multiple events can occur
- **Fix:** Pass raw input to systems, let systems apply parameters; accumulate input events or process immediately
- **Prevention:** Input is intent - treat as immutable; never overwrite, only accumulate or process
- **Examples:** Scaling input before system receives it, overwriting mouse delta instead of accumulating

## Pattern: Dead State
- **Detection:** Struct members never read or flags never mutated
- **Root cause:** Adding state "just in case" or forgetting to wire up UI/consumers
- **Fix:** Delete unused state immediately
- **Prevention:** Before adding state, identify specific consumer; grep for reads/writes after implementation
- **Examples:** contact_debug_info unused fields, debug_text::font_size, show_velocity_trail flag

## Pattern: Principle Without Exception Documentation
- **Detection:** Absolute principle statements that have legitimate exceptions
- **Root cause:** Principles defined without documenting edge cases
- **Fix:** Add exception clauses to principles at definition time
- **Prevention:** When defining principles, ask "are there legitimate exceptions?"

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

## Pattern: Arbitrary Fallback Constants
- **Detection:** Functions returning arbitrary constants when unable to compute correct value
- **Root cause:** Avoiding degenerate case handling with default constant
- **Fix:** Compute correct answer from geometry or use safe primitives with meaningful fallback
- **Prevention:** Never return arbitrary constants - derive from mathematics or assert if invalid

## Pattern: Missing Validation at Foundation
- **Detection:** Foundation primitives accepting inputs without validation (delta_time, vectors, etc.)
- **Root cause:** Assuming callers will always provide valid inputs
- **Fix:** Add precondition assertions at primitive boundaries
- **Prevention:** All foundation functions validate contracts with debug assertions
- **Examples:** spring_damper::update missing delta_time validation

---

**When a pattern repeats, add it here. When adding code, check against these first.**
