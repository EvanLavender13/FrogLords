# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Foundation:** 92% → Target: 95%
**Last Updated:** 2025-10-17

---

## Active Violations

### High Priority

**1. Controller Mixed Concerns** (`controller.{h,cpp}`)
- Principle: Composable Functions
- Type: Coupling (physics + animation + orientation)
- Impact: Hard to modify subsystems independently
- Fix: Extract subsystems more clearly
- Effort: 3 points

**2. Accumulated State Pattern** (`controller.{position,velocity}`)
- Principle: Principled Development
- Type: Undocumented trade-off
- Impact: Long-term stability unknown
- Fix: Document rationale OR derive from inputs
- Effort: 2-3 points
- Options:
  - Document why accumulation is acceptable
  - Derive velocity from input each frame
  - Switch to Verlet integration

### Medium Priority

None currently blocking progress to 95%

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

**Path to 95% foundation:**

1. **Controller mixed concerns** (3 pts) ← NEXT
2. **Accumulated state** (2-3 pts)

**Estimated:** ~2 refinements remaining

---

## Recent Activity

**See individual plan files for detailed metrics:**
- `PLANS/REFINE_orientation_dual_reference.md` (2025-10-17) - Composability +2.5
- `PLANS/REFINE_tuning_defaults.md` (2025-10-17) - Math foundations +2
- `PLANS/REFINE_spring_damper_validation.md` (2025-10-17) - Layer 2 → 100%

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
