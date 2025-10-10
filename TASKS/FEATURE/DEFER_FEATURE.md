# Defer Feature

Execute when principle review reveals a feature is premature, unnecessary, or violates core principles. Cleanly back out of planned work, capture learnings, and return to stable state.

**Use this task when:**
-   `REVIEW_PLAN.md` identifies violations or significant misalignments
-   Feature's value hypothesis lacks supporting evidence
-   Foundation dependency is less certain than initially assessed
-   Better alternative emerges during planning phase
-   "May not be necessary" intuition gains supporting analysis
-   **Implementation attempts reveal missing prerequisites or fundamental design flaws**
-   **Multiple implementation approaches fail to produce expected behavior**

**Applies to both:**
-   **Planning phase** (before implementation): Deferral prevents wasted work
-   **Implementation phase** (during development): Deferral after discovering blockers or invalidated assumptions

**When code exists:** You may need to revert changes or leave stubs. Document what was attempted and why it failed.

### 1. Review Development Principles

Read `AGENTS.md` to synthesize deferral principles. Essential for:
-   Test before building—features need evidence of problems
-   Paradox of quality—maximize iteration count on valuable work
-   Liquid backlog—most ideas stay fluid until needed
-   Short planning horizon—avoid planning uncertain futures
-   Decision cost—quick deferral decisions are cheap; building wrong things is expensive

### 2. Identify Target Feature

Extract the feature name from the current git branch name (format: `iteration/<feature_name>`).

### 3. Verify Deferral Justification

1.  Confirm `PLANS/plan_review_<feature_name>.md` exists with clear identification of violations or misalignments
2.  Verify specific principle citations from `AGENTS.md` are included
3.  Confirm actionable recommendations support deferral
4.  Verify evidence gap is documented (what's missing to justify proceeding)

If review document doesn't exist or is incomplete, run `TASKS/REVIEW_PLAN.md` first.

### 4. Update Design Backlog

1.  Open `PLANS/DESIGN_BACKLOG.md` and locate the feature's entry
2.  Modify the entry to include deferral status:
    -   Add `*Status:* **DEFERRED**` with date and concise reason
    -   Lower certainty score to reflect actual uncertainty
    -   Document requirements to reconsider (e.g., "Needs test-first evidence" or "Blocked by unstable dependency")
    -   Link to principle review document

### 5. Update Dependency Stack

1.  Open `PLANS/DEPENDENCY_STACK.md` and locate the feature (if present in active stack layers)
2.  Remove feature from active stack layers
3.  Return feature to `DESIGN BACKLOG` (liquid) section
4.  Update commentary if needed (e.g., remove "next candidate" language)
5.  Do NOT mark as complete (✅) - deferral ≠ completion

### 6. Document Implementation Attempts (if applicable)

If implementation was attempted before deferral:

1.  In `## Deferral` section of `PLANS/implementation_<feature_name>.md`, add subsection: `### Implementation Attempts`
2.  Document each approach tried with:
    -   Brief description of approach
    -   What went wrong / why it failed
    -   Key technical insight or blocker discovered
3.  This prevents future attempts from repeating failed approaches
4.  Include any code artifacts left in place (stubs, partial implementations) and their state

### 7. Archive Planning Documents

**Do NOT delete planning documents.** They contain valuable knowledge about *why* something was deferred.

1.  Add `## Deferral` section to end of `PLANS/feature_<feature_name>.md`
2.  Include: date, reason (1-2 sentences), evidence gap, reconsideration criteria, discovered prerequisites (if any), link to review document (if exists)
3.  For implementation-phase deferrals: add `### Implementation Attempts` subsection documenting each approach tried, what failed, and key technical insights
4.  Run `TASKS/ARCHIVE_ITERATION.md` to move planning documents to `PLANS/ARCHIVE/` with timestamp prefix

### 8. Capture Learnings

1.  Open `PLANS/DEFERRAL_LOG.md`
2.  Add entry for this feature with: date, reason, key learning (1-3 sentences), principle reinforced, missing prerequisites (if discovered), link to review document (if exists)
3.  Purpose: Build institutional memory about *why* ideas get deferred; prevents repeated investigation of same dead ends
4.  For implementation-phase deferrals: emphasize technical blockers or architectural discoveries

**NOTE FOR RETROSPECTIVES:** Categorize deferrals to help identify patterns:
- **Deferral Phase:** Planning Phase | Implementation Phase
- **Deferral Category:** Premature Feature | Missing Prerequisites | Principle Violation | Discovered Blocker | Better Alternative | Invalidated Assumption
- This categorization helps RETROSPECTIVE task identify why features are being deferred and whether planning phase could catch issues earlier

### 9. Handle Code Changes (if implementation was attempted)

1.  **Option A - Revert Changes:**
    -   If changes are unstable, incomplete, or violate principles
    -   Revert commits or discard branch entirely
    -   Return to clean state on main branch
2.  **Option B - Keep Stubs/Infrastructure:**
    -   If foundational work is sound but feature is premature
    -   Ensure stubs are documented and non-functional code is disabled
    -   Commit with clear message: "feat(deferred): Add infrastructure for <feature> (disabled, pending prerequisites)"
3.  **Decision criteria:** Revert unless infrastructure has independent value or low maintenance burden

### 10. Propose Next Steps

Recommend running `TASKS/NEXT_FEATURE.md` to identify next work from backlog based on current dependency stack state, highest certainty foundations, and test-driven needs (not speculation).

### Tone & Constraints

-   Concise and systematic
-   Deferral is cheap; celebrate avoiding expensive mistakes
-   Preserve knowledge (don't delete documents)
-   Clean branches prevent clutter
-   Quick return to productive work on higher-value features

---

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
