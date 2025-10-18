# Anti-Pattern Detection Guide

**Use these to detect violations during development.**

Patterns observed in real code, documented for prevention. Each pattern includes detection heuristics, root cause analysis, fix strategy, and prevention measures.

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

---

**When a pattern repeats, add it here. When adding code, check against these first.**
