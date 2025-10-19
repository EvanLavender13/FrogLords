# Refinement: Coyote Time / Jump Buffering Rationale

**Document why special-case mechanics violate Consistency for Prime Directive**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-18
**Complexity:** Trivial (1 pt)
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/character/controller.cpp:apply_input, update`
**Principle:** Consistency, Principled Development | Score: 8/10
**Severity:** Medium
**Type:** Undocumented principle violation

**Current state:**
```cpp
// Coyote time and jump buffering implementation exists
// but lacks explicit comment explaining WHY this trade-off was made
```

**Why violation:**
- Special-case mechanics create intentional inconsistency
- "Jumping only when grounded" rule has exceptions for game feel
- Implementation exists but principle trade-off not documented
- Future developers won't understand *why* Consistency was violated
- Violates Principled Development: "Every decision must trace back to principles"

**Impact:**
- Blocks: Future developer understanding of principle hierarchy
- Cascades to: May inspire other undocumented violations
- Foundation: Layer 1 (Controller) at 96%
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Document

**DOCUMENT:**
- **Why keep:** Coyote time/jump buffering are proven game feel mechanics. They forgive near-miss inputs (jumping just after leaving ledge, pressing jump just before landing). This improves responsiveness and reduces player frustration.

- **Principle trade-off:** Violates **Consistency** principle (jumping allowed when not grounded) in service of **Prime Directive** (Do No Harm to Gameplay - preserve player intent).

- **Derivation:** When principles conflict, the hierarchy applies:
  1. Prime Directive (sacred)
  2. Radical Simplicity
  3. Other principles

  Player control > behavioral consistency. Forgiveness mechanics preserve intent, which is the higher truth.

- **Documentation location:** Add inline comment above coyote time logic and jump buffer logic explaining:
  - What: Brief description of mechanic
  - Why: Prime Directive > Consistency
  - Trade-off: Accepting behavioral inconsistency for better game feel

**Example comment structure:**
```cpp
// PRINCIPLE TRADE-OFF: Coyote time violates Consistency for Prime Directive.
// Allows jumping for brief period after leaving ground to forgive near-miss inputs.
// Player intent (jump at edge) > strict physics (must be grounded).
// Prime Directive: Do No Harm to Gameplay.
```
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [x] Violation resolved
- [x] Principle score improved (8/10 → 9/10)
- [x] Tests passing (no changes required)
- [x] No regressions

**Metrics:**
- Before: LOC ~500, Principle 8/10, Undocumented trade-off
- After: LOC ~522 (+22 comment lines), Principle 9/10 (+1), Trade-off explicit
<!-- END: SELECT/SUCCESS -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Date:** 2025-10-18

**Changes:**
- Added comprehensive documentation block explaining coyote time and jump buffering principle trade-off
- Location: `src/character/controller.cpp:apply_input` (main documentation block)
- Cross-reference comment at jump buffer execution site

**Documentation added:**
- Principle conflict: Consistency vs Prime Directive
- Rationale: Player intent preservation > strict behavioral rules
- Specific mechanics explained: coyote time (post-edge jumping) and jump buffering (pre-land input)
- Principle hierarchy established: Prime Directive > Consistency
- Design reference: Super Mario Bros (1985) precedent from NOTES/DesigningGames/DG_Interface.md
- Principle reference: PRINCIPLES.md - Prime Directive

**Tests:** All passing (build successful, documentation-only change)

**Result:** ✓ Violation removed
- Future developers now understand *why* this intentional inconsistency exists
- Principle hierarchy explicitly documented
- Satisfies Principled Development: "Every decision must trace back to principles"
<!-- END: REFINE/COMPLETED -->
