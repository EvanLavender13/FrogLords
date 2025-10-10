# Select Refactoring Opportunity

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles. Essential for evaluating refactor opportunities:
- "Abstract repeated patterns into systems; prefer parameters over assets"
- "Clarity over cleverness"
- "Simplicity over sophistication"
- "Wait for third use" (rule of three before abstracting)

### 2. Review Refactor Backlog

1. Open `PLANS/REFACTOR_BACKLOG.md`
2. Scan sections in priority order: Critical → High → Medium → Low
3. Look for items that meet selection criteria (see below)

### 3. Apply Selection Criteria

Choose **one item** that satisfies ALL of the following:

#### Must-Have Criteria
- **Priority:** Higher priority first (Critical > High > Medium > Low)
- **Stability Gate:** System must be at ≥70% certainty in `DEPENDENCY_STACK.md`
- **Rule of Three Verified:** Pattern appears 3+ times (or architectural flaw is Critical)
- **Clear Definition:** "Current State" and "Proposed Change" sections are concrete
- **Testable:** Can verify behavior preservation after refactor

#### Nice-to-Have Criteria (Tie-breakers)
- **Impact:** Affects multiple systems or unblocks future work
- **Risk:** Lower risk preferred (check "Risk" field: Low > Medium > High)
- **Prerequisites:** No dependencies on incomplete refactors or features
- **Isolation:** Can be completed independently without touching unstable systems

### 4. Verify System Stability

For the candidate refactor:

1. Read `PLANS/DEPENDENCY_STACK.md`
2. Find certainty scores for all affected systems
3. Verify all systems are ≥70% certainty
4. Check if any systems are currently under active feature development (check git branch)

**If stability gate fails:** Skip to next candidate or defer selection until systems stabilize.

### 5. Verify Rule of Three

For pattern extraction refactors:

1. Read the files mentioned in the refactor item
2. Confirm the pattern appears 3+ times
3. Verify the pattern is truly identical (not just similar)

**If rule of three fails:** Mark item as deferred with reason "Only N uses found" and select next candidate.

### 6. Check for Blockers

Verify the refactor is not blocked by:
- Missing prerequisite refactors (check "Prerequisites" in backlog item)
- Active feature work on the same systems (check current git branch and feature plans)
- Uncertain or evolving requirements (system may change soon)

### 7. Document Selection

State selected item clearly:
- Item name/description
- Priority level
- Category (Pattern Extraction, Simplification, System Design, Utilities, API Design)
- Affected systems and files
- Certainty scores of affected systems (from DEPENDENCY_STACK.md)
- Expected duration (hours to 1-2 days)

### 8. Proceed to Analysis

Once selected, proceed directly to `ANALYZE_IMPACT.md` to perform deep dive into affected systems.

### Tone & Constraints

- Concise and direct
- Favor high-priority items that meet stability requirements
- Avoid touching uncertain or actively-changing systems
- Respect "wait for third use" discipline (no premature abstraction)
- Select only one item at a time
- Skip items that require architectural decisions still under discussion

## When Nothing Qualifies

If no items meet the selection criteria:
1. Document why (stability gates, rule of three violations, etc.)
2. Suggest conditions needed for items to become viable
3. Consider running IDENTIFY_REFACTORS to populate backlog with new opportunities
4. Return to other workflows (FEATURE or MAINTENANCE)
