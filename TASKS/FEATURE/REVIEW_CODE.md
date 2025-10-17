# Review Code

**Validate that implementation upholds principles and enables emergence.**

---

## The Purpose

Verify the executed code embodies the Six Pillars, not just that it works. Code that functions but violates principles is technical debt incarnate.

A review is not approval—it's philosophical validation that the code deserves to exist.

---

Read `AGENTS.md` - internalize the Six Pillars

## Prerequisites

- All plan stages executed and marked complete
- Mathematical validation performed
- Debug visualization functional
- Emergence observed (or lack thereof noted)

---

## Phase 1: Principle Validation

### The Six Lenses of Review

#### 1. Radical Simplicity
**Is this the simplest possible implementation?**

Review for:
- Unnecessary abstractions
- Over-engineered solutions
- Code that could be deleted
- Features that could emerge instead

Red flags:
- "Manager" classes
- Abstract base classes
- Template metaprogramming
- "Just in case" code

#### 2. Fundamental Composable Functions
**Does each piece do exactly one thing perfectly?**

Review for:
- Mixed responsibilities
- Hidden dependencies
- Special cases for composition
- Orthogonality violations

Red flags:
- Functions with "and" in the name
- Circular dependencies
- Friend classes
- Global state

#### 3. Solid Mathematical Foundations
**Is the math correct and validated?**

Review for:
- Unvalidated formulas
- Magic numbers without derivation
- Coordinate system confusion
- Numerical instabilities

Red flags:
- "Seems to work" comments
- Undocumented constants
- No debug visualization
- Missing unit verification

#### 4. Emergent Behavior
**Does it enable more than intended?**

Review for:
- Prescribed behaviors
- Hard-coded outcomes
- Artificial limitations
- Missed emergence opportunities

Red flags:
- Switch statements for behavior
- Explicit state machines
- "Only works for" comments
- Feature-specific code

#### 5. Consistency
**Is behavior predictable and control preserved?**

Review for:
- Special cases
- Conditional behavior
- Input delays or buffering
- Animation overriding physics

Red flags:
- "Except when" logic
- Mode-dependent responses
- Frame-dependent behavior
- Lost player control

#### 6. Principled Development
**Can every decision be justified?**

Review for:
- Unjustified complexity
- Unexplained choices
- Missing documentation
- Unclear intent

Red flags:
- "Don't know why this works"
- Copy-pasted code
- No comments on complex logic
- Inconsistent patterns

---

## Phase 2: Technical Validation

### Code Quality Standards

Beyond principles, verify basics:

#### Naming and Style
- [ ] snake_case throughout
- [ ] 4-space indentation
- [ ] Same-line braces
- [ ] Meaningful names

#### Architecture
- [ ] Foundation → Character → Rendering → App flow
- [ ] No sideways dependencies
- [ ] No circular includes
- [ ] Proper file placement

#### Cleanliness
- [ ] No dead code
- [ ] No commented experiments
- [ ] No debug prints left in
- [ ] No TODO comments (fix or remove)

---

## Phase 3: Emergence Assessment

### What Emerged?

Document unexpected behaviors discovered during implementation:

```markdown
## Emergent Discoveries
- [Behavior that surprised us]
- [System interaction we didn't plan]
- [Player possibility we didn't foresee]
```

### What Was Prescribed?

Identify where we authored instead of enabled:

```markdown
## Prescribed Behaviors (Issues)
- [Specific outcome we forced]
- [Limitation we imposed]
- [Possibility we prevented]
```

**Prescribed behaviors must be justified or removed.**

---

## Phase 4: Mathematical Verification

### Formula Review
For each mathematical implementation:
- [ ] Formula documented with derivation
- [ ] Units verified for consistency
- [ ] Numerical stability analyzed
- [ ] Edge cases tested
- [ ] Debug visualization shows correctness

### Coordinate System Verification
- [ ] Consistent use of Y-up, Z-forward
- [ ] No confusion between spaces
- [ ] Transformations validated
- [ ] Vectors normalized where needed

---

## Phase 5: Simplification Opportunities

Before approving, identify what could be simpler:

### Could Be Removed
- Unnecessary validation
- Redundant state
- Unused parameters
- Dead code paths

### Could Be Simplified
- Complex conditions
- Nested structures
- Multi-step processes
- Special cases

### Could Emerge
- Hard-coded behaviors
- Explicit configurations
- Prescribed limits
- Feature-specific code

**If significant simplification is possible, request revision.**

---

## Phase 6: Document Review

Create `PLANS/<feature>_CODE_REVIEW.md`:

```markdown
# Code Review: [Feature Name]

**Date:** [YYYY-MM-DD]
**Decision:** APPROVED | REVISE | ROLLBACK

## Principle Validation
1. Radical Simplicity: [PASS/FAIL] - [Details]
2. Composable Functions: [PASS/FAIL] - [Details]
3. Mathematical Foundations: [PASS/FAIL] - [Details]
4. Emergent Behavior: [PASS/FAIL] - [Details]
5. Consistency: [PASS/FAIL] - [Details]
6. Principled Development: [PASS/FAIL] - [Details]

## Mathematical Verification
**Formulas:** [Validated/Issues]
**Coordinates:** [Consistent/Confused]
**Stability:** [Verified/Uncertain]
**Debug Vis:** [Adequate/Missing]

## Emergent Discoveries
[Unexpected behaviors that emerged]

## Prescribed Behaviors
[Outcomes we forced instead of enabled]

## Simplification Opportunities
[What could be removed or simplified]

## Technical Quality
- Naming: [✓/✗] snake_case throughout
- Architecture: [✓/✗] Proper dependencies
- Cleanliness: [✓/✗] No dead code
- Documentation: [✓/✗] Decisions explained

## Critical Issues
[Violations that must be fixed]

## Recommendation

### If APPROVED:
The implementation upholds all principles.
Proceed to FINALIZE.

### If REVISE:
Fix [specific violations] before approval.
[Detailed fixes required]

### If ROLLBACK:
Fundamental principle violations require fresh approach.
[Why implementation failed]

## The Test
Can we say: "This code enables [behavior] through [simple rules],
validated by [mathematics], maintaining [all six principles]"?

Answer: [YES/NO] - [Explanation]
```

---

## Decision Criteria

### APPROVE only if:
- All six principles validated
- Mathematics verified correct
- Debug visualization adequate
- Enables more than intended
- No simpler implementation exists
- Every line justified

### REVISE if:
- Minor principle violations
- Missing documentation
- Simplification opportunities
- Mathematical uncertainty
- Debug visibility gaps

### ROLLBACK if:
- Major principle violations
- Mathematical incorrectness
- Prescribes instead of enables
- Adds unjustified complexity
- Breaks player control
- Cannot be debugged

---

## Review Questions

For every file changed:

1. **Could this be simpler?**
2. **Is the math validated?**
3. **Can we see what it does?**
4. **Does it enable emergence?**
5. **Is control preserved?**
6. **Can we justify every line?**

Count the "yes" answers. Need 6 to approve.

---

## Anti-Patterns to Reject

### The Clever Solution
Code that's "elegant" but not simple. Reject complexity disguised as sophistication.

### The Framework
Abstraction for hypothetical reuse. Reject generalization without need.

### The Black Box
Code without debug visibility. Reject what cannot be seen.

### The Prescriptor
Code that dictates outcomes. Reject authorship of behavior.

### The Betrayer
Code that delays or modifies input. Reject violations of player trust.

---

## Special Considerations

### For Physics Systems
- Verify forces are visible
- Confirm conservation laws
- Check numerical stability
- Validate coordinate systems

### For Control Systems
- Confirm immediate response
- Verify no input buffering
- Check physics drives animation
- Validate player intent preserved

### For Mathematical Systems
- Review formula derivations
- Check unit consistency
- Verify edge case handling
- Confirm debug visualization

---

## The Commitment

Approve code that:
- Embodies all six principles
- Enables unexpected emergence
- Can be mathematically validated
- Can be visually debugged
- Preserves player control

Never approve code that:
- Violates principles for convenience
- Hides complexity in abstraction
- Cannot be validated or seen
- Prescribes specific outcomes
- Betrays player trust

**The best review rejects code that works but violates principles. The second best demands simplification. Approval means the code deserves to exist.**

**This is the way.**