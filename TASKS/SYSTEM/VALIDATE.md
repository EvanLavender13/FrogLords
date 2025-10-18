# VALIDATE - Principle Check and Scoring

**The best validation rejects the system. The second best demands simplification.**

---

## Purpose

Validate the iterated system against all Six Pillars. This is philosophical gatekeeping—ensuring principles are maintained before integration.

**A system that works but violates principles is technical debt incarnate.**

---

## Prerequisites

- [ ] Iteration complete (patterns emerged)
- [ ] Emergent behaviors documented
- [ ] Mathematical validation complete
- [ ] `TASKS/PLANS/<name>_SYSTEM.md` updated with iteration results
- [ ] System stable in graybox

---

## Process

### 1. Create Validation Document

Create `TASKS/PLANS/<name>_VALIDATE.md`:

```markdown
# Validation: <Name>

**Date:** [Today]
**System Status:** Awaiting principle validation

---

## The Six Pillars

### 1. Radical Simplicity
**Can anything be removed?**

**Scoring (1-10):**
- 10: Irreducible. Removing anything breaks it.
- 7-9: Minimal. A few things could simplify.
- 4-6: Moderate. Significant simplification possible.
- 1-3: Complex. Needs major simplification.

**Score:** __/10

**Evidence:**
- Irreducible core:
- Could remove:
- Complexity justified by:

**Verdict:** PASS | SIMPLIFY | REJECT

---

### 2. Fundamental Composable Functions
**Does this compose cleanly with other systems?**

**Scoring (1-10):**
- 10: Perfectly orthogonal. No overlap or special cases.
- 7-9: Minimal overlap. Composes with minor consideration.
- 4-6: Some overlap. Requires special handling.
- 1-3: Significant overlap. Doesn't compose cleanly.

**Score:** __/10

**Evidence:**
- Orthogonal to:
- Overlaps with:
- Special cases:
- Composes by:

**Verdict:** PASS | REVISE | REJECT

---

### 3. Solid Mathematical Foundations
**Is every behavior mathematically proven?**

**Scoring (1-10):**
- 10: Every property formally proven.
- 7-9: Core properties proven, edge cases tested.
- 4-6: Some math proven, some guessed.
- 1-3: Mostly guesswork, little proof.

**Score:** __/10

**Evidence:**
- Proven properties:
- Tested edge cases:
- Mathematical model:
- Unproven areas:

**Verdict:** PASS | PROVE | REJECT

---

### 4. Emergent Behavior
**Does this enable outcomes, not prescribe them?**

**Scoring (1-10):**
- 10: Rich emergence, surprises creator.
- 7-9: Clear emergence, some surprises.
- 4-6: Limited emergence, mostly predicted.
- 1-3: Prescriptive, no emergence.

**Score:** __/10

**Evidence:**
- Emergent behaviors documented:
- Serendipitous discoveries:
- Player strategies enabled:
- Prescribed outcomes:

**Verdict:** PASS | ENABLE_MORE | REJECT

---

### 5. Consistency
**Does this behave predictably? Any special cases?**

**Scoring (1-10):**
- 10: Perfectly predictable, zero special cases.
- 7-9: Highly consistent, minimal exceptions.
- 4-6: Mostly consistent, some special cases.
- 1-3: Unpredictable, many special cases.

**Score:** __/10

**Evidence:**
- Deterministic:
- Predictable:
- Special cases:
- Edge case behavior:

**Verdict:** PASS | FIX_SPECIAL_CASES | REJECT

---

### 6. Principled Development
**Can every decision be traced to a principle?**

**Scoring (1-10):**
- 10: Every decision explicitly justified.
- 7-9: Most decisions justified.
- 4-6: Some decisions justified, some arbitrary.
- 1-3: Mostly arbitrary, no clear justification.

**Score:** __/10

**Evidence:**
- Decisions made:
- Justified by:
- Arbitrary choices:
- Trade-offs:

**Verdict:** PASS | JUSTIFY | REJECT

---

## Overall Assessment

**Average Score:** __/10

**Decision:**
- [ ] **APPROVED** (avg ≥7.0, all individual ≥6)
- [ ] **REVISE** (avg ≥5.0, fixable issues)
- [ ] **REJECT** (avg <5.0, fundamental problems)
- [ ] **DEFER** (good system, wrong time)

---

## Required Changes

**If REVISE, list required changes:**
1.
2.
3.

**If REJECT, document why:**
- Principle violations:
- Cannot be fixed because:
- Learning:

**If DEFER, document why:**
- Good system but:
- Should build when:
- Dependencies needed:

---

## Emergence Documentation

**Unexpected behaviors that emerged:**
1.
2.
3.

**Player strategies enabled (not prescribed):**
1.
2.
3.

**Serendipitous discoveries:**
1.
2.
3.

**What this system enables (future possibilities):**
-
-
-

---

## Validation Sign-Off

**Validated by:** [Name]
**Date:** [Date]
**Time spent on system:** [hours/days]
**Iteration count:** [cycles]

**Notes:**
[Reflections, concerns, celebrations]
```

---

### 2. Score Each Principle

**Be honest. Be harsh.**

For each of the Six Pillars:
1. Read the scoring criteria
2. Examine the evidence
3. Assign score (1-10)
4. Mark verdict (PASS | needs work | REJECT)
5. Document reasoning

**The goal is NOT to pass. The goal is TRUTH.**

---

### 3. Calculate Average

**Average all six scores:**
- Add all scores: __ + __ + __ + __ + __ + __ = __
- Divide by 6: __ / 6 = __

**Thresholds:**
- **≥7.0** = APPROVED (strong principle alignment)
- **5.0-6.9** = REVISE (fixable issues)
- **<5.0** = REJECT (fundamental problems)

**Additional requirement:** No individual score below 6

---

### 4. Make Decision

#### If APPROVED (avg ≥7.0, all ≥6)

**Celebrate! This is rare.**

The system:
- Aligns with all principles
- Enables emergence
- Strengthens foundation
- Ready for integration

**Proceed to INTEGRATE.**

---

#### If REVISE (avg 5.0-6.9 or any score <6)

**This is expected. Most systems need revision.**

Identify required changes:
- Which principles scored low?
- What specific issues?
- Can they be fixed without rebuild?

**Options:**
1. **Minor fixes** → Fix and re-validate (same document)
2. **Moderate changes** → Return to ITERATE
3. **Major changes** → Return to GRAYBOX

**Document changes needed, make them, then re-validate.**

---

#### If REJECT (avg <5.0)

**This is success. We learned before shipping.**

The system violates principles fundamentally and cannot be fixed without complete redesign.

**Options:**
1. **Defer** → Document learning, return to backlog with notes
2. **Restart** → Try completely different approach
3. **Abandon** → Maybe we don't need this system

**Update** `TASKS/PLANS/<name>_SYSTEM.md`:
```markdown
## REJECTED

**Reason:** [fundamental principle violations]

**What we learned:**
-
-

**Why we can't fix it:**
-
-

**Future consideration:**
[If we try this again, we should...]
```

**Commit documentation, delete system code, return to SELECT.**

---

#### If DEFER

**Good system, wrong time.**

The system works and aligns with principles, but:
- Foundation not ready for it
- Dependencies changed
- Better opportunities available
- Should wait and learn more

**Document, commit, return to backlog with increased priority.**

---

### 5. Document Emergence

**Regardless of pass/fail, document what emerged:**

In `TASKS/PLANS/<name>_SYSTEM.md`:
```markdown
## Emergence Documented

**Behaviors that surprised us:**
1. [Unexpected behavior and why it's valuable]
2. [Serendipitous discovery]
3. [Player strategy we didn't design]

**What this enables (not prescribes):**
- Future system A could...
- Future system B could...
- Players could discover...

**Principle alignment:**
- This strengthens: [which principles]
- This challenges: [which principles]
- Trade-offs made: [and why]

**Foundation impact:**
- Certainty before: [%]
- Certainty after: [%]
- Enabled systems: [list]
```

---

### 6. Commit Validation

```bash
git add TASKS/PLANS/<name>_VALIDATE.md TASKS/PLANS/<name>_SYSTEM.md
git commit -m "validate: <name> - <decision>

Principle scores: [scores as array]
Average: <score>
Decision: APPROVED | REVISE | REJECT | DEFER

Emergent: <count> behaviors
Enables: <future systems>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

---

## Outputs

- [ ] `TASKS/PLANS/<name>_VALIDATE.md` created
- [ ] All six principles scored
- [ ] Average calculated
- [ ] Decision made (APPROVED | REVISE | REJECT | DEFER)
- [ ] Emergence documented
- [ ] Validation committed

---

## Exit Criteria

**Proceed to INTEGRATE when:**
- Decision = APPROVED
- Average ≥7.0
- All individual scores ≥6
- Emergence documented
- No blocking principle violations

**Return to ITERATE when:**
- Decision = REVISE (moderate changes)
- Fixable issues identified
- Principles can be restored

**Return to GRAYBOX when:**
- Decision = REVISE (major changes)
- Core mechanic needs rework

**Defer when:**
- Decision = REJECT or DEFER
- Document learning
- Return to backlog

---

## Next Step

**→ INTEGRATE** (if approved)
**→ ITERATE** (if revisions needed)
**→ GRAYBOX** (if major rework needed)
**→ Document and defer** (if rejected)

---

## Validation Philosophy

### The Best Review Rejects

**Approval should be rare.**

If every system passes validation, we're not being critical enough. Rejection means we learned before shipping technical debt.

### Principles Over Progress

**"Shipping" is not the goal. Principle alignment is the goal.**

A rejected system strengthens the project by:
- Clarifying what we value
- Teaching what doesn't work
- Preventing technical debt
- Raising standards

### Score Honestly

**Wishful thinking creates technical debt.**

- Don't round up scores
- Don't excuse violations
- Don't rationalize special cases
- Don't pass systems that should fail

**The pain of rejection is temporary. The pain of bad systems is permanent.**

### Document Emergence Always

**Even rejected systems teach.**

Emergent behaviors that appear during iteration are valuable regardless of whether the system ships. Document them.

---

**Truth over convenience. Principles over shipping.**
