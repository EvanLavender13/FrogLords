# Execute Improvement

**Remove complexity, restore principles, validate continuously.**

---

## The Purpose

Transform violations into simplifications through careful, validated execution. Every change must make the system simpler, clearer, or more correct—preferably all three.

Execution is not refactoring—it's the disciplined removal of complexity.

---

Read `AGENTS.md` - internalize the Six Pillars

## Prerequisites

### Path Selection
- **Path A (Trivial - 1-2 points):** Direct from SELECT, obvious fix
- **Path B (Standard - 3-8 points):** From REVIEW_PLAN with staged approach

### Required Foundation
- Violation clearly identified
- Simplification strategy chosen
- AGENTS.md principles internalized
- Rollback strategy prepared

---

## Path A: Trivial Improvements (1-2 points)

For violations so obvious they insult our principles.

### Phase 1: Validate Violation

Read `PLANS/IMPROVE_<name>.md`:
```markdown
## The Violation
**Principle:** [Which is violated]
**Evidence:** [Specific file:line]
**Fix:** [Exact change]
```

Confirm:
- [ ] Violation exists as described
- [ ] Fix is truly trivial
- [ ] No hidden complexity
- [ ] Will actually simplify

**If any doubt, switch to Path B.**

### Phase 2: Execute Deletion/Simplification

The Three Approaches (in order):

1. **Delete It**
   - Remove the entire thing
   - Verify nothing breaks
   - Celebrate the reduction

2. **Simplify It**
   - Remove special cases
   - Eliminate conditions
   - Reduce to essence

3. **Document It**
   - Derive magic numbers
   - Explain unavoidable complexity
   - Justify what remains

### Phase 3: Validate Improvement

Before:
- Lines of code: [N]
- Cyclomatic complexity: [N]
- Dependencies: [N]

After:
- Lines of code: [N] (must be less)
- Cyclomatic complexity: [N] (must be less)
- Dependencies: [N] (must be less or same)

### Phase 4: Principle Check
- [ ] Simpler than before?
- [ ] Principle restored?
- [ ] No new violations?
- [ ] Build succeeds?
- [ ] Format clean?

### Phase 5: Document

Append to `PLANS/IMPROVE_<name>.md`:
```markdown
## EXECUTION

**Path:** Trivial
**Executed:** [Date/Time]
**Change:** [Exact modification]
**Reduction:** [Lines removed, complexity eliminated]
**Principle Restored:** [Which one]

Verified: Build ✓ Format ✓ Behavior ✓
```

→ Proceed to FINALIZE

---

## Path B: Standard Improvements (3-8 points)

For violations requiring staged simplification.

### Phase 1: Pre-Execution Validation

#### Review Plan
Read `PLANS/IMPROVE_<name>.md`:
- All stages and sequence
- Validation per stage
- Risk assessment
- Rollback points

#### Principle Alignment
For each stage, verify:
1. **Simplicity:** Will this stage reduce complexity?
2. **Composability:** Will systems become more orthogonal?
3. **Mathematics:** Is correctness maintained/improved?
4. **Emergence:** Are we removing prescription?
5. **Consistency:** Are we eliminating special cases?
6. **Development:** Can we justify this change?

**If any stage fails these tests, abort.**

### Phase 2: Execute Stages

For each stage:

#### 1. Measure Before
Document current state:
- Line count in affected files
- Cyclomatic complexity
- Number of special cases
- Dependencies

#### 2. Execute Change
Follow the hierarchy:
1. **Try Deletion First**
   - Can we remove entirely?
   - What actually breaks?
   - Is the break acceptable?

2. **Then Simplification**
   - Remove special cases
   - Eliminate branches
   - Reduce parameters
   - Clarify responsibility

3. **Finally Documentation**
   - Only if complexity is essential
   - Derive all magic numbers
   - Explain why it can't be simpler

#### 3. Validate Stage
- [ ] Build succeeds
- [ ] Tests pass (if any)
- [ ] Behavior unchanged (or acceptably changed)
- [ ] Complexity reduced
- [ ] Principle improved

#### 4. Measure After
- Line count: [Must be less]
- Complexity: [Must be less]
- Special cases: [Must be fewer]
- Dependencies: [Must be same or fewer]

#### 5. Document Stage
```markdown
### Stage [N]: [Title] ✓
**Completed:** [Time]
**Approach:** [Delete|Simplify|Document]
**Changes:** [What was modified]
**Reduction:** [Measurable improvement]
**Validation:** Build ✓ Behavior ✓ Principle ✓
```

**If stage fails, STOP and rollback.**

### Phase 3: Full System Validation

After all stages:

#### Complexity Metrics
- Total lines removed: [N]
- Total lines added: [N]
- Net reduction: [Must be negative]
- Special cases eliminated: [N]
- Dependencies removed: [N]

#### Principle Validation
For each of the Six Pillars:
- [ ] Improved or maintained?
- [ ] No new violations?
- [ ] Measurable benefit?

#### Behavioral Validation
- [ ] System still functions correctly
- [ ] No performance regression
- [ ] No new edge cases
- [ ] Player control preserved

### Phase 4: Document Completion

Append to `PLANS/IMPROVE_<name>.md`:
```markdown
## EXECUTION

**Path:** Standard
**Duration:** [Total time]
**Stages Completed:** [N of N]

### Complexity Reduction
- Lines removed: [N]
- Lines added: [N]
- Net change: [N] (negative is good)
- Special cases removed: [N]
- Dependencies eliminated: [N]

### Principle Restoration
1. Simplicity: [How improved]
2. Composability: [How improved]
3. Mathematics: [How improved]
4. Emergence: [How improved]
5. Consistency: [How improved]
6. Development: [How improved]

### Validation
All stages completed successfully.
No regressions detected.
Principles strengthened.

**The system is simpler.**
```

→ Proceed to REVIEW_CODE

---

## Abort Conditions

Stop immediately and rollback if:

### Complexity Increases
- More lines than removed
- New abstractions added
- Additional special cases
- More dependencies created

### Principles Violated
- Simplicity compromised
- Orthogonality broken
- Mathematics incorrect
- Consistency lost

### System Degraded
- Performance regression
- Functionality broken
- Player control affected
- Debug visibility lost

When aborting:
```markdown
## EXECUTION ABORTED

**Stage:** [Where stopped]
**Reason:** [Specific violation]
**Discovered:** [What went wrong]
**Recommendation:** [Defer|Redesign|Abandon]

The cure was worse than the disease.
```

---

## The Philosophy of Improvement

### Deletion Is Victory
Every line removed is a bug that can't happen, a confusion that can't occur, a maintenance burden lifted. Celebrate deletion.

### Simplification Is Clarity
Complex code hides bugs. Simple code reveals truth. When code becomes simple, bugs become obvious and fixes become trivial.

### Documentation Is Acceptance
Sometimes complexity is essential. When it is, document why. But first, try very hard to make it non-essential.

---

## Anti-Patterns to Avoid

### The Refactor
"Let me reorganize this..." - No. Remove or simplify. Organization is not improvement.

### The Abstraction
"Let me generalize this..." - No. Specificity is clarity. Abstraction is complexity.

### The Optimization
"Let me make this faster..." - No. Make it simpler. Speed follows simplicity.

### The Feature Addition
"While fixing this, let me add..." - Never. Improvement removes. Features add.

---

## Quality Standards

### Every Change Must
- Reduce total complexity
- Restore a principle
- Be independently reversible
- Have measurable improvement
- Leave code simpler

### No Change May
- Add new abstractions
- Create new dependencies
- Introduce special cases
- Hide complexity elsewhere
- Serve hypothetical needs

---

## Special Considerations

### For Mathematical Corrections
1. Validate current formula is wrong
2. Derive correct formula
3. Build test to prove correctness
4. Replace with validation
5. Document the derivation

### For Dependency Untangling
1. Map all connections
2. Find weakest link
3. Break one connection
4. Verify still works
5. Repeat until orthogonal

### For Magic Number Documentation
1. Find the source of the number
2. Derive from first principles
3. Replace with named constant
4. Document the derivation
5. Or delete if possible

---

## The Commitment

Execute improvements that:
- Make code disappear
- Restore violated principles
- Reduce measurable complexity
- Can be validated and reversed
- Leave the system simpler

Never execute improvements that:
- Add abstraction layers
- Increase line count
- Create new dependencies
- Move complexity around
- Complicate the simple

**The best improvement deletes entire files. The second best deletes entire functions. The third best deletes entire branches.**

**This is the way.**