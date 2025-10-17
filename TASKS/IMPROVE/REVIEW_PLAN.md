# Review Plan

**Validate that the improvement simplifies, not complicates.**

---

## The Purpose

Verify the plan truly fixes a principle violation through simplification. A review is not permission to refactor—it's validation that simplification is achieved.

The best review rejects unnecessary changes. The second best demands more deletion. Approval means true simplification.

---

## Phase 1: Violation Verification

### Confirm the Violation Exists
- Which principle is actually violated?
- Can we measure the violation?
- Is this a real problem or opinion?
- Does it affect gameplay?

**If no clear violation, reject immediately.**

### Validate the Measurement
- Current complexity: [Quantified]
- Target simplicity: [Quantified]
- Net reduction: [Must be negative]

**If complexity increases, reject immediately.**

---

## Phase 2: Strategy Validation

### The Three Approaches (in order)

#### 1. Deletion Assessment
- Was deletion attempted first?
- What prevents complete removal?
- Can something else handle this?
- **If not tried:** Demand deletion attempt

#### 2. Simplification Verification
- Does it actually simplify?
- Are special cases removed?
- Are dependencies reduced?
- **If adds complexity:** Reject

#### 3. Documentation Justification
- Are magic numbers derived?
- Is complexity now justified?
- Is intent clarified?
- **If still unclear:** Revise

---

## Phase 3: Stage Analysis

### Each Stage Must:
- [ ] Fix one specific violation
- [ ] Be measurably simpler
- [ ] Be independently reversible
- [ ] Build confidence for next stage
- [ ] Not introduce new complexity

### Red Flags in Stages:
- "While we're here..." additions
- Abstraction layer creation
- "Clever" solutions
- Complexity migration
- Feature additions

**Any red flag = automatic revision.**

---

## Phase 4: Principle Impact

### Test Against All Six

#### 1. Radical Simplicity
- Is the result simpler?
- Was everything unnecessary removed?
- Is this the minimal fix?

#### 2. Fundamental Composable Functions
- Are responsibilities clearer?
- Is coupling reduced?
- Are systems more orthogonal?

#### 3. Solid Mathematical Foundations
- Is math now validated?
- Are formulas documented?
- Is correctness provable?

#### 4. Emergent Behavior
- Are prescribed behaviors removed?
- Is the system more open?
- Can more behaviors emerge?

#### 5. Consistency
- Is behavior more predictable?
- Are special cases eliminated?
- Is control preserved?

#### 6. Principled Development
- Can we now trace decisions?
- Is the fix principled?
- Is validation improved?

**Net principle improvement must be positive.**

---

## Phase 5: Risk Assessment

### Deletion Risks
- What might break?
- Hidden dependencies?
- Undocumented uses?

### Simplification Risks
- Performance impacts?
- Behavioral changes?
- Integration issues?

### Documentation Risks
- Still unclear?
- Wrong derivation?
- Incomplete explanation?

**For each risk:** Is the simplification worth it? If not, defer.

---

## Phase 6: Document Review

Append to `PLANS/IMPROVE_<name>.md`:

```markdown
---
## PLAN REVIEW

**Date:** [YYYY-MM-DD]
**Decision:** APPROVED | REVISE | REJECT | DEFER

## Violation Confirmation
**Principle Violated:** [Which one]
**Measured:** [Current complexity]
**Target:** [Desired simplicity]
**Valid:** [YES/NO]

## Strategy Assessment
**Approach:** [Delete | Simplify | Document]
**Deletion Attempted:** [YES/NO]
**Net Simplification:** [Positive/Negative]

## Stage Quality
Each stage:
- [ ] Fixes specific violation
- [ ] Measurably simpler
- [ ] Independently reversible
- [ ] No new complexity

## Principle Impact
1. Simplicity: [Improved/Same/Worse]
2. Composability: [Improved/Same/Worse]
3. Mathematics: [Improved/Same/Worse]
4. Emergence: [Improved/Same/Worse]
5. Consistency: [Improved/Same/Worse]
6. Development: [Improved/Same/Worse]

**Net Impact:** [Must be positive]

## Risk Analysis
**Risk Level:** [Low/Medium/High]
**Worth It:** [YES/NO]
**Mitigation:** [Strategy if yes]

## Recommendation

### If APPROVED:
Proceed with [N] stages of simplification.

### If REVISE:
- [ ] Try deletion first
- [ ] Remove new complexity
- [ ] Simplify stages further

### If REJECT:
[Why this doesn't simplify]

### If DEFER:
[What must happen first]

## The Test
Can we say: "This removes [complexity] by [method],
fixing [principle violation] through [N] reversible stages,
with net simplification of [measurement]"?

Answer: [YES/NO]
```

---

## Decision Criteria

### APPROVE only if:
- Real violation exists
- Net complexity reduced
- Deletion attempted
- All stages reversible
- No new violations
- Measurable simplification

### REVISE if:
- Deletion not attempted
- Stages too complex
- New complexity added
- Unclear simplification
- Missing measurements

### REJECT if:
- No real violation
- Complexity increases
- Abstraction added
- Principles worsened
- Can't measure improvement

### DEFER if:
- Foundation unstable (<85%)
- Higher priority violations exist
- Dependencies unclear
- Risk exceeds reward

---

## The Simplification Test

For every change, ask:

1. **Does this remove code?**
2. **Does this remove complexity?**
3. **Does this remove special cases?**
4. **Does this remove dependencies?**
5. **Does this remove confusion?**

Count the "yes" answers. Need at least 3 to proceed.

---

## Anti-Patterns to Catch

### The Grand Refactor
Changing everything "while we're at it." Reject scope creep.

### The Abstraction Addition
"Simplifying" by adding layers. Reject complexity hiding.

### The Lateral Move
Moving complexity elsewhere. Demand actual removal.

### The Perfect Solution
Over-engineering the fix. Demand minimal change.

### The Feature Sneak
Adding capabilities during cleanup. Reject immediately.

---

## Special Considerations

### For Mathematical Fixes
- Is isolation testing included?
- Is formula derivation shown?
- Is numerical validation planned?
- Is debug visualization added?

### For Dependency Untangling
- Is each cut independent?
- Are stages truly reversible?
- Is orthogonality achieved?
- Are systems simpler after?

### For Documentation
- Are magic numbers explained?
- Are formulas derived?
- Is complexity justified?
- Or should we just delete it?

---

## The Ultimate Question

After reviewing the entire plan:

> "Is the codebase simpler after this change?"

If you cannot answer "yes" with certainty, do not approve.

---

## The Commitment

Reviews must:
- Protect simplicity
- Demand deletion
- Measure improvement
- Ensure reversibility
- Prevent complexity

Never approve plans that:
- Add abstraction
- Hide complexity
- Increase line count
- Create dependencies
- Serve hypothetical needs

**The best review prevents unnecessary changes. The second best demands more deletion.**

**This is the way.**