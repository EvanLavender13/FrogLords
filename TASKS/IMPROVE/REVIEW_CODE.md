# Review Code

**Verify simplification achieved and principles restored.**

---

## The Purpose

Confirm the improvement made the codebase simpler, not just different. Validate that violated principles were restored, not just that the code still works.

A review that approves added complexity has failed its purpose.

---

## Prerequisites

- Improvement execution complete
- Complexity metrics documented
- Principle violations addressed
- Build and validation passed

---

## Phase 1: Simplification Verification

### Measure the Reduction

Compare before and after:

#### Line Count
```markdown
Before: [N lines]
After: [N lines]
Reduction: [N lines] (MUST be negative)
```

#### Complexity Metrics
```markdown
Cyclomatic Complexity:
- Before: [N]
- After: [N]
- Reduction: [N] (MUST be negative)

Dependencies:
- Before: [N]
- After: [N]
- Reduction: [N] (MUST be ≤0)

Special Cases:
- Before: [N]
- After: [N]
- Eliminated: [N] (MUST be >0 if any existed)
```

**If complexity increased, REJECT immediately.**

### The Three Questions

For every change made:

1. **Did we delete first?**
   - Was removal attempted?
   - What prevented deletion?
   - Is the reason valid?

2. **Did we simplify second?**
   - Are there fewer branches?
   - Are there fewer parameters?
   - Are responsibilities clearer?

3. **Did we document only as last resort?**
   - Is remaining complexity essential?
   - Are magic numbers derived?
   - Is intent now clear?

---

## Phase 2: Principle Restoration

### Verify Violation Fixed

From `PLANS/IMPROVE_<name>.md`, confirm:

#### The Original Violation
- **Principle:** [Which was violated]
- **Evidence:** [What showed violation]
- **Status:** [FIXED/PARTIALLY FIXED/UNFIXED]

#### How It Was Restored

For the violated principle, verify:

1. **Radical Simplicity**
   - Complexity removed, not moved
   - No new abstractions added
   - Fewer lines, not more

2. **Fundamental Composable Functions**
   - Responsibilities separated
   - Dependencies reduced
   - Orthogonality improved

3. **Solid Mathematical Foundations**
   - Formulas validated
   - Magic numbers derived
   - Debug visualization added

4. **Emergent Behavior**
   - Prescriptions removed
   - Possibilities opened
   - Flexibility increased

5. **Consistency**
   - Special cases eliminated
   - Predictability improved
   - Control preserved

6. **Principled Development**
   - Decisions now justified
   - Code now explainable
   - Intent now clear

**The violated principle MUST show improvement.**

---

## Phase 3: Side Effect Analysis

### Check for New Violations

While fixing one principle, did we violate another?

For each of the Six Pillars:
- [ ] Not made worse
- [ ] No new violations introduced
- [ ] Net improvement positive

### Check for Scope Creep

Did we stay focused?
- [ ] Only planned changes made
- [ ] No "while we're here" additions
- [ ] No feature additions
- [ ] No unrelated fixes

### Check for Regressions

Did we break anything?
- [ ] Existing functionality preserved
- [ ] Performance not degraded
- [ ] No new bugs introduced
- [ ] Player control maintained

---

## Phase 4: Code Quality Validation

### Technical Standards
- [ ] snake_case naming throughout
- [ ] Proper indentation (4 spaces)
- [ ] Dependency flow correct
- [ ] No circular includes

### Cleanliness
- [ ] No commented-out code
- [ ] No debug prints remaining
- [ ] No TODO comments (address or remove)
- [ ] No experimental code

### Documentation
- [ ] Complex logic explained
- [ ] Derivations documented
- [ ] Decisions justified
- [ ] Intent clear

---

## Phase 5: The Simplification Test

For the entire improvement, answer:

1. **Does less code exist?** [YES/NO]
2. **Is complexity reduced?** [YES/NO]
3. **Are special cases removed?** [YES/NO/N/A]
4. **Are dependencies fewer?** [YES/NO]
5. **Is intent clearer?** [YES/NO]

**Need at least 4 YES answers to approve.**

---

## Phase 6: Document Review

Append to `PLANS/IMPROVE_<name>.md`:

```markdown
## CODE REVIEW

**Date:** [YYYY-MM-DD]
**Decision:** APPROVED | REVISE | REJECT

## Simplification Achieved
- Lines removed: [N]
- Complexity reduced: [N points]
- Special cases eliminated: [N]
- Dependencies removed: [N]

**Net Simplification:** [YES/NO]

## Principle Restoration
**Violated Principle:** [Which one]
**Status:** RESTORED | PARTIALLY RESTORED | NOT RESTORED
**Evidence:** [How it's better]

## Other Principles
1. Simplicity: [Improved/Maintained/Degraded]
2. Composability: [Improved/Maintained/Degraded]
3. Mathematics: [Improved/Maintained/Degraded]
4. Emergence: [Improved/Maintained/Degraded]
5. Consistency: [Improved/Maintained/Degraded]
6. Development: [Improved/Maintained/Degraded]

**Net Impact:** [Positive/Neutral/Negative]

## Quality Checks
- Naming: ✓ snake_case
- Dependencies: ✓ Proper flow
- Side effects: ✓ None
- Regressions: ✓ None

## The Test
Can we say: "This improvement removed [N lines] by [method],
fixing [principle violation] with net simplification of [measurement]"?

Answer: [YES/NO]

## Recommendation

### If APPROVED:
The codebase is measurably simpler.
Principle [X] has been restored.
Proceed to FINALIZE.

### If REVISE:
[Specific issues to fix]
- Simplification insufficient
- New violations introduced
- Scope creep detected

### If REJECT:
The improvement made things worse.
Rollback all changes.
[Explanation of failure]
```

---

## Decision Criteria

### APPROVE only if:
- Net complexity reduced (measurable)
- Original violation fixed
- No new violations introduced
- No scope creep
- Build and tests pass
- Code cleaner than before

### REVISE if:
- Minor issues found
- Small scope creep
- Documentation missing
- Standards not met
- Opportunity for more deletion

### REJECT if:
- Complexity increased
- Violation not fixed
- New violations created
- Major scope creep
- Functionality broken
- Made things worse

---

## Anti-Patterns to Reject

### The Lateral Move
Complexity moved, not removed. Same problems, different place.

### The Abstraction Addition
"Simplified" by adding layers. More code pretending to be less.

### The Refactor Disguise
Reorganization without simplification. Different but not simpler.

### The Feature Sneak
Improvements that add capabilities. Scope creep in disguise.

### The Over-Engineering
Solution more complex than the problem. Cure worse than disease.

---

## Special Cases

### For Trivial Improvements (Path A)
Focus on:
- Change matches plan exactly
- No scope expansion
- Measurable simplification
- Quick approval or rejection

### For Mathematical Fixes
Verify:
- Formula now correct
- Derivation documented
- Validation tests added
- Debug visualization present

### For Dependency Untangling
Confirm:
- Coupling actually reduced
- Systems more orthogonal
- No new dependencies added
- Cleaner interfaces

### For Magic Number Documentation
Check:
- Numbers properly derived
- Constants well-named
- Derivation documented
- Or better: deleted entirely

---

## The Philosophy

### Deletion Is Success
Every line removed is a victory. Celebrate the code that no longer exists.

### Simplification Is Clarity
When code becomes simple, bugs become obvious, maintenance becomes trivial.

### Complexity Is Debt
Every line added must pay for itself. Most can't afford the interest.

---

## The Commitment

Approve improvements that:
- Make measurable reductions
- Restore violated principles
- Leave code simpler
- Can justify their changes
- Focus on one problem

Never approve improvements that:
- Add more than they remove
- Create new violations
- Include scope creep
- Hide complexity elsewhere
- Pretend reorganization is simplification

**The best review celebrates deletion. The second best demands more removal. Approval confirms true simplification.**

**This is the way.**