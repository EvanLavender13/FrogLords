# Select Improvement

**Choose what to remove, not what to add.**

---

## The Selection Philosophy

Every improvement must make the codebase simpler. If it doesn't reduce complexity, it's not an improvement—it's decoration.

The best improvement is deletion. The second best is simplification. Adding is last resort.

---

Read `PRINCIPLES.md` - internalize the Six Pillars

## The Six Questions

Before selecting any improvement, ask:

1. **Does this make things simpler?** (Not just different)
2. **Can we remove something instead?** (Deletion over modification)
3. **Does it serve immediate gameplay?** (Not hypothetical futures)
4. **Is the foundation stable?** (≥85% certainty)
5. **Will this prevent future complexity?** (Systems over patches)
6. **Can we explain why it exists?** (Traceable to principles)

If any answer is "no," do not select.

---

## Selection Process

### Phase 1: Identify the Truth

Review `PLANS/DEPENDENCY_STACK.md`:
- What is the current certainty level?
- Are we in repair mode (<90%)?
- What layer are we working in?

**If in Repair Mode:** Only select fixes for principle violations.
**If stable (≥90%):** Can select complexity reductions.

### Phase 2: Review Violations

Open `PLANS/IMPROVE_BACKLOG.md`

**Priority Order:**
1. **Mathematical Violations** - Wrong math compounds into chaos
2. **Control Violations** - Anything that betrays player intent
3. **Visibility Violations** - Can't debug what can't be seen
4. **Simplicity Violations** - Unnecessary complexity
5. **Convention Violations** - Style issues (batch these)

### Phase 3: Apply the Filters

**The Stability Filter**
- System must be ≥85% certain
- Exception: Mathematical violations (always fix)

**The Simplicity Filter**
- Solution must be simpler than problem
- Prefer deletion over modification
- Prefer documentation over code change

**The Emergence Filter**
- Will this enable new behaviors?
- Or just patch a symptom?

---

## Batching Opportunities

Group only when all items:
- Share the same pattern
- Have the same solution
- Can be understood together
- Are equally simple

**Rule:** If understanding one helps understand all, batch them.

---

## Complexity Classification

### Trivial (1-2 points)
- Documentation only
- Deletion only
- Single constant change
- Naming fixes

### Simple (3-4 points)
- Single-file simplification
- Pattern extraction (3+ uses)
- Mathematical validation
- Debug visualization

### Complex (5-8 points)
- Multi-file refactoring
- Dependency restructuring
- System separation
- Should rarely be selected

**Prefer trivial. Accept simple. Question complex.**

---

## Document the Selection

Create `PLANS/IMPROVE_<name>.md`:

```markdown
# [Improvement Name]

## The Violation
**Principle:** [Which of the six was violated]
**File:** `src/path/file.cpp:line`
**Current:** [What exists]
**Problem:** [Why it violates the principle]

## The Solution
**Approach:** [Delete | Simplify | Document]
**Change:** [Specific modification]
**Result:** [How it becomes simpler]

## The Justification
**Why Now:** [Why this over other improvements]
**Dependencies:** [What must remain stable]
**Risk:** [What could go wrong]

## The Measure
**Before:** [Current complexity metric]
**After:** [Expected simplification]
**Test:** [How to verify improvement]

Complexity: [1-8 points]
Certainty: [% confidence in solution]
```

---

## Selection Criteria

### Must Have
- Makes codebase simpler
- Fixes real violation (not opinion)
- Solution understood completely
- Can be tested in isolation

### Tie Breakers
1. Enables most deletion
2. Fixes mathematical incorrectness
3. Improves debug visibility
4. Removes special cases
5. Reduces line count

---

## When Nothing Qualifies

If no improvements meet criteria:

1. **Check certainty** - Are we stable enough?
2. **Run audit** - Do we know our violations?
3. **Question necessity** - Is the codebase simple enough?
4. **Return to features** - Build something to learn

Sometimes the best improvement is to stop improving.

---

## The Anti-Patterns

### The Perfection Trap
"This could be better" is not a reason. Only "this violates a principle" is.

### The Abstraction Addiction
Wait for the third use. Then wait some more. Abstraction is complexity debt.

### The Refactor Rabbit Hole
One improvement at a time. Complete it. Test it. Then consider the next.

### The Feature in Disguise
If it adds capability, it's not an improvement—it's a feature.

---

## Output

After selection, state clearly:

```
Selected: [Name]
Principle Violated: [Which one]
Current Complexity: [Measure]
Expected Simplicity: [Measure]
Approach: [Delete | Simplify | Document]
Risk: [Low | Medium | High]
```

---

## The Commitment

Select improvements that:
- Make code disappear
- Make systems simpler
- Make principles clearer
- Make debugging easier

Never select improvements that:
- Add abstraction layers
- Create special cases
- Increase line count
- Serve hypothetical needs

**The best improvement makes future improvements unnecessary.**

**This is the way.**