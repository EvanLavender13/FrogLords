# Review Plan

**Validate that the system enables emergence, not prescribes outcomes.**

---

## The Purpose

Verify the plan upholds all six principles before allowing implementation. A review is not approval to proceed—it's validation that we should proceed.

The best review rejects the plan. The second best demands simplification. Approval is last resort.

---

Read `PRINCIPLES.md` - internalize the Six Pillars

## Phase 1: Principle Validation

### Test Each Principle

#### 1. Radical Simplicity
- Is this the minimal version?
- Can anything be removed?
- Are we building the irreducible primitive?
- **Red flag:** "We might need this later"

#### 2. Fundamental Composable Functions
- Will it compose without special cases?
- Is it orthogonal to existing systems?
- Does it do exactly one thing?
- **Red flag:** "It also handles..."

#### 3. Solid Mathematical Foundations
- Is the math validated in isolation?
- Can we prove correctness?
- Is debug visualization planned?
- **Red flag:** "It seems to work"

#### 4. Emergent Behavior
- Does it enable more than intended?
- Are outcomes discovered, not designed?
- Will it surprise us?
- **Red flag:** "It will do exactly..."

#### 5. Consistency
- Is behavior predictable?
- Does physics drive animation?
- Is player control preserved?
- **Red flag:** "Except when..."

#### 6. Principled Development
- Can we trace every decision?
- Is it built bottom-up?
- Are stages independently validated?
- **Red flag:** "Trust me"

**If any principle fails, reject or revise.**

---

## Phase 2: Plan Quality Assessment

### Stage Validation
Each stage must:
- [ ] Have single, clear purpose
- [ ] Be mathematically validatable
- [ ] Include debug visualization
- [ ] Be independently reversible
- [ ] Build on validated foundation

### Dependency Check
- [ ] Foundation is ≥90% certain
- [ ] No circular dependencies
- [ ] Clear composition points
- [ ] Bottom-up construction

### Emergence Potential
- [ ] System enables multiple behaviors
- [ ] Not prescribing specific outcomes
- [ ] Allows for surprises
- [ ] Multiplies possibilities

---

## Phase 3: Risk Analysis

### Mathematical Risks
- Unvalidated formulas
- Numerical instabilities
- Coordinate confusions
- Accumulated errors

### Integration Risks
- Hidden dependencies
- State corruption
- Performance impacts
- Composition failures

### Principle Risks
- Complexity creep
- Feature prescription
- Control betrayal
- Debug blindness

**For each risk:** Can we mitigate? If not, reject.

---

## Phase 4: Simplification Opportunities

Before approving, identify:

### What Can Be Removed?
- Unnecessary stages
- Redundant validation
- Over-specified behavior
- Premature optimization

### What Can Be Deferred?
- Polish elements
- Edge cases
- Performance tuning
- Nice-to-haves

### What Can Emerge?
- Behaviors we're prescribing
- Systems we're duplicating
- Features that could compose

**If simplification is possible, demand it.**

---

## Phase 5: Document Review

Create `PLANS/<feature>_PLAN_REVIEW.md`:

```markdown
# Plan Review: [System Name]

**Date:** [YYYY-MM-DD]
**Decision:** APPROVED | REVISE | REJECT

## Principle Validation
1. Radical Simplicity: [PASS/FAIL] - [Reason]
2. Composable Functions: [PASS/FAIL] - [Reason]
3. Mathematical Foundations: [PASS/FAIL] - [Reason]
4. Emergent Behavior: [PASS/FAIL] - [Reason]
5. Consistency: [PASS/FAIL] - [Reason]
6. Principled Development: [PASS/FAIL] - [Reason]

## Critical Issues
[Principle violations that must be fixed]

## Simplification Required
[What must be removed or deferred]

## Mathematical Validation
[How math will be proven correct]

## Emergence Assessment
[What behaviors will emerge vs prescribed]

## Risk Analysis
**Mathematical:** [Issues and mitigation]
**Integration:** [Issues and mitigation]
**Principle:** [Issues and mitigation]

## Recommendation

### If APPROVED:
Proceed with [N] stages, validating [principles] at each stage.

### If REVISE:
Fix [violations] before resubmitting.

### If REJECT:
[Why this shouldn't be built]

## The Test
Can we say: "This [system] will [do one thing] through [mathematical validation],
enabling [emergent behaviors] while maintaining [all six principles]"?

Answer: [YES/NO] - [Explanation]
```

---

## Decision Criteria

### APPROVE only if:
- All six principles validated
- Math can be proven correct
- Stages are reversible
- Debug visualization included
- Enables emergence
- Nothing can be removed

### REVISE if:
- Minor principle violations
- Simplification possible
- Math needs validation strategy
- Stages too complex
- Missing debug visibility

### REJECT if:
- Major principle violations
- Cannot validate mathematically
- Prescribes instead of enables
- Adds unnecessary complexity
- Foundation uncertain (<90%)
- Violates player control

---

## The Review Questions

For every stage in the plan:

1. **Can it be simpler?**
2. **Can it be validated?**
3. **Can it be seen?**
4. **Can it be reversed?**
5. **Does it compose?**

If any answer is "no," that stage must be revised.

---

## Anti-Patterns to Catch

### The Kitchen Sink Plan
Too many features in one system. Demand decomposition.

### The Black Box Plan
No debug visualization. Demand visibility.

### The Prescriptive Plan
Specifies exact outcomes. Demand emergence.

### The Top-Down Plan
Builds before foundation. Demand bottom-up.

### The Faith-Based Plan
Cannot be validated. Demand proof.

---

## The Commitment

Reviews must:
- Protect principles above features
- Demand simplification
- Require validation
- Ensure reversibility
- Enable emergence

Never approve plans that:
- Violate principles
- Cannot be validated
- Hide complexity
- Prescribe outcomes
- Build on uncertainty

**The best review prevents bad systems from being built.**

**This is the way.**