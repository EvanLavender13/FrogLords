# Plan Feature

**Design systems that emerge, not features that prescribe.**

---

## The Purpose

Transform a selected system into concrete steps that maintain principles. Every step must be validatable, reversible, and simpler than alternatives.

A plan is not a roadmap to build—it's a proof that we should build.

---

Read `AGENTS.md` - internalize the Six Pillars

## Phase 1: Principle Validation

Before any planning, verify the system against principles:

### The Six Tests
1. **Simplicity:** Is this the irreducible version?
2. **Composability:** Will it combine cleanly with existing systems?
3. **Mathematical Truth:** Can we validate the math in isolation?
4. **Emergence:** Will it enable more than we intend?
5. **Consistency:** Will behavior be predictable?
6. **Justification:** Can we trace every decision to a principle?

**If any test fails, do not plan. Return to selection.**

---

## Phase 2: Mathematical Foundation

### Identify Core Mathematics
- What equations govern this system?
- What numerical methods are used?
- What coordinate systems are involved?
- What can go wrong mathematically?

### Plan Validation Strategy
- How will we test in isolation?
- What debug visualization is needed?
- How will we know it's correct?
- What are the failure modes?

**No math, no system. No validation, no implementation.**

---

## Phase 3: System Decomposition

### Find the Primitives
Break the system into irreducible parts:
- Each part does exactly one thing
- Each part can be tested alone
- Parts compose without special cases
- No part knows about others

### Identify Dependencies
- What must exist first?
- What can be built in parallel?
- What might break?
- What stays stable?

### Plan Debug Visibility
Every step needs eyes:
- What needs visualization?
- How will we see state?
- Where might blindness occur?
- What validates correctness?

---

## Phase 4: Implementation Stages

### Stage Structure
Each stage must:
- **Do one thing** (single responsibility)
- **Be reversible** (can undo if wrong)
- **Be testable** (can validate in isolation)
- **Build on truth** (previous stage validated)

### Stage Template
```markdown
### Stage [N]: [Single Purpose]

**Principle:** [Which principle guides this]
**Change:** [Specific modification]
**Validation:** [How we verify correctness]
**Debug:** [What we need to see]

Files:
- `path/file.cpp`: [Specific change]

Tests:
- [ ] Mathematical correctness
- [ ] Visual confirmation
- [ ] No regression
```

---

## Phase 5: Document Plan

Create/Update `PLANS/<feature>_PLAN.md`:

```markdown
# Plan: [System Name]

## The System
**Primitive:** [Core mechanic]
**Mathematics:** [Underlying equations]
**Composes With:** [Other systems]

## The Validation
**Isolation Tests:** [How to verify math]
**Debug Visualization:** [What we'll see]
**Success Criteria:** [How we know it works]

## The Stages

### Stage 1: Mathematical Foundation
[Validate core math in isolation]

### Stage 2: Debug Visualization
[Build ability to see the system]

### Stage 3: Core Implementation
[Minimal working version]

### Stage 4: Composition Tests
[Verify clean interaction]

## The Risks
**Mathematical:** [What could be wrong]
**Integration:** [What might break]
**Mitigation:** [How we'll handle issues]

## The Emergence
**Intended:** [What we're building for]
**Possible:** [What might emerge]

Complexity: [1-8 points]
Certainty: [% confidence]
```

---

## Quality Gates

Before implementation:

### Must Have
- [ ] Mathematical validation strategy
- [ ] Debug visualization plan
- [ ] Each stage independently testable
- [ ] Clear composition points

### Must Not Have
- [ ] Untestable black boxes
- [ ] Circular dependencies
- [ ] Special cases for composition
- [ ] Prescribed behaviors
- [ ] Hidden complexity

---

## Anti-Patterns to Avoid

### The Big Bang
Planning everything at once. Instead: validate each stage before the next.

### The Black Box
Planning without visualization. Instead: every stage must be observable.

### The Assumption
Planning on unvalidated math. Instead: prove correctness first.

### The Monolith
Planning tightly coupled changes. Instead: orthogonal, composable stages.

---

## When Planning Fails

If you cannot create a clear plan:

1. **System too complex** - Find simpler primitives
2. **Math uncertain** - Validate in isolation first
3. **Dependencies unclear** - Map them explicitly
4. **Can't visualize** - Build debug tools first
5. **Stages too large** - Decompose further

Sometimes the best plan is to not build.

---

## The Test

After planning, you should be able to say:

> "We will build [system] in [N] reversible stages, each validating [principle],
> with [debug visualization] confirming [mathematical correctness], enabling
> [emergent behaviors] through [clean composition]."

If you can't say this clearly, the plan isn't ready.

---

## Output

After planning, state:

```
Plan Created: [System Name]
Stages: [N]
Mathematical Validation: [Strategy]
Debug Visualization: [Approach]
Risk Level: [Low/Medium/High]
Certainty: [%]
```

---

## The Commitment

Plan features that:
- Can be built in validated stages
- Have clear mathematical foundations
- Can be debugged visually
- Compose without special cases
- Enable emergence

Never plan features that:
- Require blind faith
- Can't be validated
- Hide their behavior
- Break composability
- Prescribe all outcomes

**The best plan proves we shouldn't build. The second best proves we should.**

**This is the way.**