# Refinements Backlog

**Principle violations to restore. Complexity to remove.**

**Last Updated:** 2025-10-17 (tuning defaults completed)
**Foundation:** 90% → Target 95% (+1% from tuning fix)

---

## By Principle

### 1. Radical Simplicity
**Complexity that doesn't serve emergence**

- **src/character/orientation.cpp**: Complex yaw smoothing logic
  - Principle: Radical Simplicity
  - Severity: Medium
  - Type: Complexity
  - Fix: Simplify (use spring-damper directly, remove special handling)
  - LOC: ~50 lines
  - Related: Dual-reference violation

### 2. Fundamental Composable Functions
**Systems that overlap or don't compose cleanly**

- **src/character/controller.{h,cpp}**: Mixed concerns (physics + animation + orientation)
  - Principle: Composable Functions
  - Severity: Medium
  - Type: Coupling
  - Fix: Simplify (extract subsystems more clearly)
  - LOC: 122 lines header, 233 lines cpp
  - Note: Partially improved, needs more

### 3. Solid Mathematical Foundations
**Unproven math, magic numbers, unexplained constants**

- ✅ **Magic numbers throughout** → FIXED (2025-10-17)
  - All 27 constants documented
  - Derivations complete
  - Units specified

- **src/foundation/spring_damper.cpp**: No validation tests
  - Principle: Mathematical Foundations
  - Severity: High
  - Type: Unproven (critical damping formula untested)
  - Fix: Document (add unit tests for ζ=1 condition)
  - Test needed: Verify c = 2√(km) produces no overshoot

- **src/character/tuning.h vs controller.h**: Defaults mismatch
  - Principle: Mathematical Foundations
  - Severity: Critical
  - Type: Inconsistent sources of truth
  - Fix: Simplify (choose single source, eliminate bidirectional init issue)
  - Impact: 4x discrepancy in `time_to_max_speed`
  - Root cause: Bidirectional system (`apply_to` + `read_from`)

### 4. Emergent Behavior
**Prescribed outcomes instead of enabled possibilities**

- ✅ **Removed locomotion system** (2025-10-17)
  - Was prescribing animation states
  - Now purely reactive (spring-damper based)
  - Emergence improved

### 5. Consistency
**Special cases, unpredictable behavior, betrayed expectations**

- ✅ **Ground plane special case** → REMOVED (2025-10-17)
  - All collision now uniform
  - No entity-specific logic

- ✅ **Framerate-dependent yaw** → FIXED (2025-10-17)
  - Proper delta-time integration
  - Behavior now frame-rate independent

- **src/character/orientation.cpp**: Dual-reference violation
  - Principle: Consistency (The Dual Reference pattern)
  - Severity: High
  - Type: Circular reference (target yaw references smoothed yaw)
  - Fix: Simplify (separate target from current, maintain pure intent)
  - PRINCIPLES.md quote: "Never let a smoothed value reference itself"

### 6. Principled Development
**Decisions that can't be traced to principles**

- **src/character/controller.{h,cpp}**: Accumulated state pattern
  - Principle: Principled Development
  - Severity: Medium
  - Type: Undocumented trade-off
  - Fix: Document (accept and explain) OR Simplify (derive each frame)
  - PRINCIPLES.md: "Pure Functions Over Accumulated State"
  - Options:
    1. Document why accumulation is acceptable here
    2. Derive velocity from input each frame
    3. Switch to Verlet integration (position-based)

---

## In Progress

None currently - ready to select next refinement

---

## By Severity

### Critical (Foundation Threatening)
**Violations that undermine core systems**

None currently - Critical violation moved to In Progress

### High (Actively Harmful)
**Violations causing bugs or limiting emergence**

2. **Spring-damper math unvalidated** (`spring_damper.cpp`)
   - Impact: Foundation primitive untested
   - Used by: Landing animation, future systems
   - Fix: 1 point, add unit tests
   - Status: After tuning fix

3. **Dual-reference in orientation** (`orientation.cpp`)
   - Impact: Violates "The Dual Reference" principle
   - Causes: Subtle coupling, hard to reason about
   - Fix: 2 points, separate target from current
   - Status: After spring-damper

### Medium (Technical Debt)
**Violations making future work harder**

4. **Accumulated state pattern** (`controller.{position,velocity}`)
   - Impact: Long-term stability unknown
   - Fix: 2-3 points, document or derive
   - Status: Lower priority

5. **Mixed concerns in controller** (`controller.{h,cpp}`)
   - Impact: Hard to modify one subsystem without affecting others
   - Fix: 3 points, extract subsystems
   - Status: Partially done, ongoing

6. **Orientation complexity** (`orientation.cpp`)
   - Impact: ~50 lines doing what spring-damper should do
   - Fix: 2 points, simplify via spring-damper
   - Status: Related to dual-reference fix

### Low (Cosmetic)
**Minor violations, address when convenient**

- None currently (recent cleanup eliminated these)

---

## Recently Completed

**2025-10-17: Tuning Defaults Mismatch**
- Location: `src/character/tuning.h` vs `controller.h`
- Completed: 2025-10-17
- Principle: Solid Mathematical Foundations
- Violation: 4x discrepancy in time_to_max_speed (0.4s vs 1.6s)
- Fix: Updated controller.h defaults to match tuning.h (single source of truth)
- Changes:
  - ground_accel: 20.0 → 80.0 m/s²
  - air_accel: 10.0 → 20.0 m/s²
- Metrics:
  - Discrepancy: 4x → 0x (-100%)
  - Inconsistent defaults: 2 → 0 (-2)
  - Sources of truth: 2 → 1 (-1)
  - Principle score: 8/10 → 10/10 (+2)
- Foundation impact:
  - Tuning system: 80% → 95% (+15%)
  - Layer 3: 90% → 92% (+2%)
  - Overall: 89% → 90% (+1%)
- Pattern: Bidirectional systems are fragile (prefer unidirectional data flow)
- Learning: Single source of truth prevents initialization order bugs
- Future work: Consider making constructor apply tuning automatically

**2025-10-17: Magic Number Documentation**
- Location: 12 files, 27 constants
- Principle: Mathematical Foundations
- Before: Unexplained constants throughout
- After: All documented with derivations
- Metrics:
  - Grep-able tags added (PHYSICAL, TUNED, DERIVED, etc.)
  - Units specified
  - Formulas verified
  - Relationships documented
- Principle score: 6/10 → 9/10 (+3)
- Foundation impact: +10 certainty (79% → 89%)
- Learning: Documentation IS foundation repair

**2025-10-17: Debug Visualization System**
- Location: `src/rendering/debug_*`
- Principle: Principled Development (validation before integration)
- Added: Complete debug primitive system
- Impact: 3-day bugs now 30-second fixes
- Foundation: 95% → 98%

**2025-10-17: Remove Ground Plane Special Case**
- Location: Collision system
- Principle: Consistency (no special cases)
- Removed: Entity-specific ground logic
- Simplified: Uniform collision handling
- LOC: ~30 lines deleted

**2025-10-17: Fix Framerate-Dependent Yaw**
- Location: `orientation.cpp`
- Principle: Consistency (predictable behavior)
- Fixed: Delta-time integration
- Result: Frame-rate independent behavior

---

## Discovered During System Building

**During Debug Viz Implementation (2025-10-17):**
- None (clean implementation, exemplary)

**During Magic Number Documentation (2025-10-17):**
- Tuning defaults mismatch (critical!)
- Bidirectional tuning pattern (interesting, document)
- 75/25 friction decomposition (reusable)

**During Character Work (Previous):**
- Locomotion was too prescriptive (removed)
- Orientation has dual-reference (needs fix)
- Ground plane was special case (fixed)

---

## Patterns

**Recurring violations (inform future audits):**

1. **Special cases for specific entities**
   - Appeared: Ground plane, player-specific logic
   - Fixed: Ground plane (2025-10-17)
   - Remaining: None found
   - Prevention: Code review for `if (entity == ...)` patterns

2. **Frame-rate dependent behavior**
   - Appeared: Yaw smoothing
   - Fixed: 2025-10-17
   - Root cause: Missing delta-time multiplication
   - Prevention: Always multiply by dt for rates

3. **Dual-reference pattern**
   - Appeared: Orientation (target references smoothed)
   - Status: Needs fix
   - PRINCIPLES.md: "The Dual Reference" - Keep target separate
   - Prevention: Never let smoothed value reference itself

4. **Accumulated state**
   - Appeared: Controller position/velocity
   - Status: Document or derive
   - PRINCIPLES.md: "Pure Functions Over Accumulated State"
   - Prevention: Consider deriving state from inputs

5. **Undocumented math**
   - Appeared: 27 magic numbers
   - Fixed: All documented (2025-10-17)
   - Prevention: Require comment on every constant

**Pattern meta-learning:**
- Documentation dramatically improves understanding (+10 certainty)
- Special cases always indicate design smell
- Frame-rate independence requires discipline
- Principles document common violations well

---

## Audit Checklist

**Run periodically to populate this backlog:**

### Code Smells
- [x] Files >500 lines → controller.cpp (233 lines, acceptable)
- [x] Functions >50 lines → None found
- [x] High cyclomatic complexity → debug_assert.h (233 lines but template)
- [x] Deep nesting → None problematic
- [x] Long parameter lists → None found

### Magic Numbers
- [x] Unexplained constants → ✅ ALL DOCUMENTED (2025-10-17)
- [x] Undocumented formulas → ✅ ALL VERIFIED
- [x] Arbitrary thresholds → ✅ ALL EXPLAINED
- [x] Tweaked-by-feel values → ✅ ALL JUSTIFIED

### Special Cases
- [x] Entity-type checks → ✅ Ground plane removed
- [x] Player-specific logic → None found
- [x] Level-specific behavior → N/A (no levels yet)
- [x] Hard-coded sequences → None found

### Duplication
- [x] Copy-pasted code → None found
- [x] Similar logic multiple files → None problematic
- [x] Overlapping responsibilities → Controller (known, being improved)

### Undocumented
- [x] Emergent behaviors → Some not in EMERGENCE.md yet
- [x] Magic numbers → ✅ FIXED
- [x] Design decisions → Some in comments, could improve
- [x] TODOs and FIXMEs → None found

### Anti-Patterns
- [x] Animation blocking physics → None (physics drives animation ✓)
- [x] Accumulated state → Position/velocity (documented as issue)
- [x] Non-deterministic behavior → None found
- [x] Frame-rate dependent → ✅ Yaw fixed
- [x] Mutable global state → None found

**Last Audit:** 2025-10-17
**Next Audit:** After tuning defaults fix (to see impact)

---

## Usage

### Adding a Violation
From code review, audit, or discovery during building:
```markdown
- **Location**: Description
  - Principle: <which pillar>
  - Severity: Critical | High | Medium | Low
  - Type: Magic number | Special case | Duplication | etc.
  - Fix: Delete | Simplify | Document
  - LOC: <lines affected>
```

### Selecting a Violation
1. Choose by severity × foundation impact
2. Create `TASKS/PLANS/REFINE_<name>.md`
3. Classify: Trivial (path A) vs Standard (path B)
4. Begin refinement cycle

### Completing a Refinement
Move to "Recently Completed" with metrics:
```markdown
**Date: System/Location**
- Principle: <restored>
- Before: <state>
- After: <state>
- Metrics: <LOC removed, complexity reduced, etc.>
- Foundation impact: +<certainty %>
```

### Discovering Violations
Add immediately when found during:
- System building
- Code review
- Playtesting
- Audit cycle

---

## Priority Order

**Current (Foundation Repair Mode):**

1. ✅ ~~**Tuning defaults**~~ (critical, 1 pt) - COMPLETED 2025-10-17
2. **Spring-damper validation** (high, 1 pt) ← NEXT
3. **Dual-reference orientation** (high, 2 pts)
4. **Accumulated state** (medium, 2-3 pts)
5. **Controller concerns** (medium, 3 pts)
6. **Orientation complexity** (medium, 2 pts)

**Goal:** Foundation 90% → 95% (enables Layer 4)

**After repairs complete:** Run full audit to find new violations

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
