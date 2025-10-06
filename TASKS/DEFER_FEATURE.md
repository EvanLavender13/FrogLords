# Defer Feature

Execute when principle review reveals a feature is premature, unnecessary, or violates core principles. Cleanly back out of planned work, capture learnings, and return to stable state.

**Use this task when:**
-   `REVIEW_PLAN.md` identifies violations or significant misalignments
-   Feature's value hypothesis lacks supporting evidence
-   Foundation dependency is less certain than initially assessed
-   Better alternative emerges during planning phase
-   "May not be necessary" intuition gains supporting analysis

**Critical:** This task is for the *planning phase only* (before implementation begins). If code already exists, use a different workflow to evaluate keeping vs. reverting changes.

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

### 6. Archive Planning Documents

**Do NOT delete planning documents.** They contain valuable knowledge about *why* something was deferred.

1.  Add `## Deferral` section to end of `PLANS/iteration_<feature_name>.md`
2.  Add `## Deferral` section to end of `PLANS/implementation_<feature_name>.md` (if exists)
3.  Include: date, reason (1-2 sentences), evidence gap, reconsideration criteria, link to review document
4.  Run `TASKS/ARCHIVE_ITERATION.md` to move planning documents to `PLANS/ARCHIVE/` with timestamp prefix

### 7. Capture Learnings

1.  Open `PLANS/DEFERRAL_LOG.md`
2.  Add entry for this feature with: date, reason, key learning (1-3 sentences), principle reinforced, link to review document
3.  Purpose: Build institutional memory about *why* ideas get deferred; prevents repeated investigation of same dead ends

### 8. Propose Next Steps

Recommend running `TASKS/NEXT_FEATURE.md` to identify next work from backlog based on current dependency stack state, highest certainty foundations, and test-driven needs (not speculation).

### Tone & Constraints

-   Concise and systematic
-   Deferral is cheap; celebrate avoiding expensive mistakes
-   Preserve knowledge (don't delete documents)
-   Clean branches prevent clutter
-   Quick return to productive work on higher-value features

---

### Example Deferral Section

```markdown
## Deferral

**Date:** 2025-10-06

**Reason:** Current constant magnitude feels good; no evidence of problem requiring speed scaling.

**Evidence Gap:** No documented gameplay moments where constant tilt magnitude fails (e.g., "tilt too subtle at max speed").

**Reconsideration Criteria:** After playtesting identifies specific moments where tilt magnitude is inadequate at high speeds or distracting at low speeds.

**Review:** See `PLANS/plan_review_speed_animation_scaling.md` for full analysis
```

---

### Example Deferral Log Entry

```markdown
## Speed-Based Animation Scaling (2025-10-06)

**Reason:** Premature feature without evidence of need

**Key Learning:** Test-first principle applies to features, not just bugs. "May not be necessary" intuitions deserve investigation before planning entire iterations. Current system working well is evidence to preserve, not improve speculatively.

**Principle Reinforced:** "Paradox of quality: accept temporary roughness early to maximize iteration count" and "Test before building"

**Review Document:** `PLANS/plan_review_speed_animation_scaling.md`
```
