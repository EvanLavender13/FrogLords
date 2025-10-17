# Plan Improvement

**Simplify what exists, don't complicate what works.**

---

## The Purpose

Transform a violation into stages of simplification. Every stage must make the code simpler, clearer, or more correct.

A plan is not permission to refactor—it's proof that simplification is possible.

---

Read `AGENTS.md` - internalize the Six Pillars

## Phase 1: Violation Analysis

### Identify the Principle Violated
Which of the six is broken?
1. **Simplicity:** Unnecessary complexity exists
2. **Composability:** Systems are entangled
3. **Mathematics:** Correctness is unproven
4. **Emergence:** Behavior is over-prescribed
5. **Consistency:** Responses are unpredictable
6. **Development:** Decisions lack justification

### Measure the Violation
- **Current complexity:** [Lines, dependencies, special cases]
- **Ideal simplicity:** [What it should be]
- **Delta:** [What can be removed]

**If no clear violation, do not plan.**

---

## Phase 2: Simplification Strategy

### The Three Approaches

#### 1. Deletion (Best)
Can we remove it entirely?
- What breaks if deleted?
- What still works without it?
- Can something else handle this?

#### 2. Simplification (Good)
Can we make it simpler?
- Remove special cases
- Reduce dependencies
- Clarify responsibilities
- Eliminate state

#### 3. Documentation (Acceptable)
Can we at least explain it?
- Derive the magic numbers
- Justify the complexity
- Clarify the intent

**Always try deletion first.**

---

## Phase 3: Risk Assessment

### What Could Break?
- Systems depending on this
- Hidden assumptions
- Undocumented behaviors
- Performance implications

### How to Validate?
- Mathematical proofs
- Visual confirmation
- Behavioral tests
- Performance metrics

---

## Phase 4: Staged Simplification

### Stage Principles
Each stage must:
- **Simplify one thing** (single focus)
- **Be verifiable** (can prove correctness)
- **Be reversible** (can undo cleanly)
- **Build confidence** (each success enables next)

### Stage Template
```markdown
### Stage [N]: [Single Simplification]

**Violation Fixed:** [Which principle]
**Current:** [What exists]
**Simplified:** [What remains]
**Removed:** [What disappears]

Changes:
- `file.cpp:line`: [Specific change]

Validation:
- [ ] Still works correctly
- [ ] Simpler than before
- [ ] No regressions
- [ ] Principle restored
```

---

## Phase 5: Document Plan

Append to `PLANS/IMPROVE_<name>.md`:

```markdown
---
## PLAN

## The Violation
**Principle:** [Which was violated]
**Measurement:** [Current complexity]
**Target:** [Desired simplicity]

## The Strategy
**Approach:** [Delete | Simplify | Document]
**Risk:** [Low | Medium | High]
**Validation:** [How we'll verify]

## The Stages

### Stage 1: [First Simplification]
**Remove:** [What goes away]
**Verify:** [How to test]

### Stage 2: [Next Simplification]
**Simplify:** [What becomes cleaner]
**Verify:** [How to test]

## Success Criteria
- [ ] Principle violation resolved
- [ ] Code is simpler
- [ ] No functionality lost
- [ ] No new complexity added

Complexity: [1-8 points]
Confidence: [% certain of success]
```

---

## Quality Gates

Before each stage:

### Must Verify
- [ ] Current behavior documented
- [ ] Simplification identified
- [ ] Validation method clear
- [ ] No new complexity introduced

### Red Flags
- Adding abstraction layers
- Creating new dependencies
- Introducing special cases
- Hiding complexity elsewhere
- Making code "clever"

---

## Anti-Patterns to Avoid

### The Grand Refactor
Changing everything at once. Instead: small, verified stages.

### The Abstraction Trap
Adding layers to "simplify." Instead: remove layers.

### The Clever Solution
Complex but "elegant" code. Instead: simple and obvious.

### The Migration
Moving problems elsewhere. Instead: eliminate them.

### The Perfect Fix
Over-engineering the solution. Instead: minimum viable simplification.

---

## When Not to Plan

Do not create a plan if:

1. **No violation exists** - Don't fix what works
2. **Deletion isn't tried** - Always attempt removal first
3. **Complexity increases** - That's not improvement
4. **Dependencies multiply** - That's not simplification
5. **Can't measure success** - That's not verifiable

Sometimes the best improvement is no improvement.

---

## The Test

After planning, you should be able to say:

> "We will fix the [principle] violation by [deleting/simplifying] in [N] stages,
> reducing complexity from [current] to [target], validated by [method],
> with each stage independently reversible."

If you can't say this clearly, don't proceed.

---

## Special Cases

### Mathematical Corrections
When fixing math violations:
1. Isolate the mathematical operation
2. Validate correct formula
3. Build visualization first
4. Replace incrementally
5. Verify numerically

### Debug Visualization
When adding missing visualization:
1. Identify what needs to be seen
2. Choose minimal representation
3. Add controls for debugging
4. Ensure no performance impact
5. Document what it shows

### Dependency Untangling
When separating coupled systems:
1. Map all connections
2. Find the weakest link
3. Break one connection per stage
4. Verify functionality preserved
5. Continue until orthogonal

---

## Output

After planning, state:

```
Improvement Plan: [Name]
Violation: [Which principle]
Strategy: [Delete | Simplify | Document]
Stages: [N]
Complexity Reduction: [Current → Target]
Risk: [Low | Medium | High]
Confidence: [%]
```

---

## The Commitment

Plan improvements that:
- Make code disappear
- Reduce total complexity
- Fix principle violations
- Can be validated
- Can be reversed

Never plan improvements that:
- Add abstraction layers
- Increase line count
- Create dependencies
- Hide complexity
- Serve hypothetical needs

**The best plan removes code. The second best simplifies it. Adding is last resort.**

**This is the way.**