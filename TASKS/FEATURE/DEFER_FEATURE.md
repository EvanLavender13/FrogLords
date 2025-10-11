# Defer Feature

Execute when feature is premature, unnecessary, or violates principles. Cleanly back out, capture learnings, return to stable state.

**Use when:**
- REVIEW_PLAN identifies violations or misalignments
- Value hypothesis lacks supporting evidence
- Foundation dependency less certain than assessed
- Better alternative emerges
- "May not be necessary" intuition gains analysis support
- Implementation reveals missing prerequisites or fundamental flaws
- Multiple approaches fail to produce expected behavior

**Applies to:**
- **Planning phase:** Prevents wasted work
- **Implementation phase:** After discovering blockers or invalidated assumptions

**When code exists:** May need to revert or leave stubs. Document attempts and failures.

### 1. Review Development Principles

Read `AGENTS.md` to synthesize deferral principles:
- Test before building—features need evidence
- Paradox of quality—maximize iteration count on valuable work
- Liquid backlog—ideas stay fluid until needed
- Short planning horizon—avoid planning uncertain futures
- Decision cost—quick deferrals cheap; building wrong things expensive

### 2. Identify Target Feature

Extract feature name from branch (`feature/<feature_name>`).

### 3. Verify Deferral Justification

1. Confirm `PLANS/plan_review_<feature_name>.md` exists with clear violations/misalignments
2. Verify specific principle citations from AGENTS.md
3. Confirm recommendations support deferral
4. Verify evidence gap documented

If review missing/incomplete, run REVIEW_PLAN first.

### 4. Update Design Backlog

1. Open `PLANS/DESIGN_BACKLOG.md`, locate entry
2. Modify to include deferral:
   - Add `*Status:* **DEFERRED**` with date and reason
   - Lower certainty score to reflect uncertainty
   - Document reconsideration requirements
   - Link to review document

### 5. Update Dependency Stack

1. Open `PLANS/DEPENDENCY_STACK.md`, locate feature (if in active layers)
2. Remove from active layers
3. Return to `DESIGN BACKLOG` (liquid) section
4. Update commentary if needed
5. Do NOT mark complete (✅) - deferral ≠ completion

### 6. Document Implementation Attempts (If Applicable)

If implementation attempted:
1. Add `### Implementation Attempts` to `## Deferral` section in `PLANS/implementation_<feature_name>.md`
2. Document each approach:
   - Brief description
   - What went wrong / why failed
   - Key technical insight or blocker
3. Include code artifacts left (stubs, partial implementations) and state

Prevents repeating failed approaches.

### 7. Archive Planning Documents

**Do NOT delete.** Contains valuable knowledge about *why* deferred.

1. Add `## Deferral` section to `PLANS/feature_<feature_name>.md`
2. Include: date, reason, evidence gap, reconsideration criteria, prerequisites, review link
3. For implementation-phase: add `### Implementation Attempts` subsection
4. Use archive script to move to `PLANS/ARCHIVE/` with timestamp prefix

### 8. Capture Learnings

1. Open `PLANS/DEFERRAL_LOG.md`
2. Add entry: date, reason, key learning, principle reinforced, prerequisites, review link
3. For implementation-phase: emphasize technical blockers/discoveries

**Categorize for retrospectives:**
- **Phase:** Planning | Implementation
- **Category:** Premature | Missing Prerequisites | Principle Violation | Blocker | Better Alternative | Invalidated Assumption

Helps identify patterns and improve planning.

### 9. Handle Code Changes (If Attempted)

**Option A - Revert:**
- If unstable, incomplete, or violates principles
- Revert commits or discard branch
- Return to clean main branch

**Option B - Keep Stubs:**
- If foundational work sound but feature premature
- Document stubs, disable non-functional code
- Commit: "feat(deferred): Add infrastructure for <feature> (disabled, pending prerequisites)"

**Criteria:** Revert unless infrastructure has independent value or low maintenance burden.

### 10. Propose Next Steps

Recommend SELECT_FEATURE to identify next work based on dependency stack, highest certainty foundations, and test-driven needs.

### Tone & Constraints

- Concise and systematic
- Deferral is cheap; celebrate avoiding expensive mistakes
- Preserve knowledge (don't delete documents)
- Clean branches prevent clutter
- Quick return to productive work

### Example Deferral Section (Planning Phase, in feature_*.md)

```markdown
## Deferral

**Date:** 2025-10-06

**Reason:** Current constant magnitude feels good; no evidence of problem requiring speed scaling.

**Evidence Gap:** No documented gameplay moments where constant tilt magnitude fails (e.g., "tilt too subtle at max speed").

**Reconsideration Criteria:** After playtesting identifies specific moments where tilt magnitude is inadequate at high speeds or distracting at low speeds.

**Review:** See `PLANS/ARCHIVE/20251006_120000_plan_review_speed_animation_scaling.md` for full analysis
```

### Example Deferral Section (Implementation Phase, in feature_*.md)

```markdown
## Deferral

**Date:** 2025-10-06

**Reason:** Feature requires primary skeletal animation system as prerequisite. Current static T-pose skeleton has no base motion for secondary motion to react to.

**Missing Prerequisites:**
-   Locomotion-driven primary limb animation (walk/run arm swing cycles)
-   Distance-phased animation system for synchronized gait
-   Base skeletal animation layer for reactive systems to layer upon

**Reconsideration Criteria:** After implementing primary locomotion animation system (dependency layer below reactive animation).

**Technical Insight:** Secondary motion is a reactive layer—requires stable primary animation beneath it per procedural animation layering principles.

### Implementation Attempts

1.  **Parent rotation lag approach:** Measured joint rotation changes frame-to-frame. Failed because T-pose joints don't rotate—no parent motion to lag behind.
2.  **Acceleration-driven wobble:** Applied character acceleration to joint rotations directly. Produced unstable/wild spinning due to lack of base pose to offset from.
3.  **Slerp-based lag:** Attempted to interpolate between previous and current joint rotations. Failed for same reason—no meaningful rotation changes exist in static T-pose.

**Key Insight:** Multiple implementation attempts confirmed the architectural blocker rather than implementation bugs. Static data structure ≠ animated data structure. Reactive layers need motion sources, not just data structures.

**Review:** N/A (discovered during implementation)
```

---

### Example Deferral Log Entry (Planning Phase)

```markdown
## Speed-Based Animation Scaling (2025-10-06)

**Deferral Phase:** Planning Phase
**Deferral Category:** Premature Feature

**Reason:** Premature feature without evidence of need

**Key Learning:** Test-first principle applies to features, not just bugs. "May not be necessary" intuitions deserve investigation before planning entire iterations. Current system working well is evidence to preserve, not improve speculatively.

**Principle Reinforced:** "Paradox of quality: accept temporary roughness early to maximize iteration count" and "Test before building"

**Review Document:** `PLANS/plan_review_speed_animation_scaling.md`
```

### Example Deferral Log Entry (Implementation Phase)

```markdown
## Secondary Motion (2025-10-06)

**Deferral Phase:** Implementation Phase
**Deferral Category:** Missing Prerequisites

**Reason:** Missing critical prerequisite: primary skeletal animation system

**Key Learning:** Dependency analysis failed to identify that "reactive animation layer" requires a stable "primary animation layer" beneath it. Secondary motion needs base motion to react to—a static T-pose has no motion to add wobble on top of. Multiple implementation attempts confirmed the architectural blocker rather than implementation bugs.

**Missing Prerequisites:** Locomotion-driven skeletal animation (walk/run cycles, arm swing synchronized to gait)

**Principle Reinforced:** "Work bottom-up. Stabilize core gameplay before adding layers" and "Dependency = if A changes, B must change. Uncertainty multiplies up the stack."

**Review Document:** N/A (discovered during implementation)
```
