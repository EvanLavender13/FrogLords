# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Foundation:** 95% ✅ TARGET REACHED
**Last Updated:** 2025-10-18

---

## Active Violations

### High Priority

None - Target reached at 95%

### Medium Priority

None currently blocking progress

### Low Priority

None identified

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

**Last Audit:** 2025-10-17
**Next Audit:** After reaching 95% foundation

---

## Priority Order

**Foundation at 95% ✅**

No active refinements blocking progress. Ready to build Layer 4 systems.

**Optional refinements** (future improvements):
- Further Layer 1 improvements (95% → 100%)
- Layer 3 optimizations
- Pattern library expansion

---

## Recent Activity

**See individual plan files for detailed metrics:**
- `PLANS/REFINE_accumulated_state_pattern.md` (2025-10-18) - Principled Development +1.0
- `PLANS/REFINE_controller_mixed_concerns.md` (2025-10-18) - Composable Functions +1.0
- `PLANS/REFINE_orientation_dual_reference.md` (2025-10-17) - Composability +2.5

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
