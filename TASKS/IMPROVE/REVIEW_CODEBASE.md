# Review Codebase

**A systematic search for principle violations and opportunities for simplification.**

---

## The Purpose

Not to find bugs, but to find lies. Not to add features, but to remove complexity. Not to perfect, but to simplify.

Every line of code must trace back to a principle. Every system must justify its existence. Every complexity must earn its keep.

---

Read `AGENTS.md` - internalize the Six Pillars

## The Six Lenses of Review

### 1. Radical Simplicity
**Question:** Can this be removed?

**Look for:**
- Code that doesn't serve immediate gameplay
- Duplicate systems doing similar things
- Complex solutions to simple problems
- Features without clear purpose
- Premature abstractions
- "Just in case" code

**Red flags:**
```cpp
// Multiple ways to do the same thing
// Deep inheritance hierarchies
// Abstractions used once
// Code that "might be useful later"
```

---

### 2. Fundamental Composable Functions
**Question:** Does each system do exactly one thing?

**Look for:**
- Functions with "and" in their purpose
- Systems that know too much about each other
- Non-orthogonal features
- Special case handling
- Circular dependencies
- Mixed responsibilities

**Red flags:**
```cpp
// Functions like: update_physics_and_animation()
// Classes that manage multiple unrelated concerns
// Systems that can't be understood in isolation
```

---

### 3. Solid Mathematical Foundations
**Question:** Is this mathematically correct?

**Look for:**
- Unvalidated mathematical operations
- Accumulated floating point errors
- Division by potentially zero values
- Undocumented coordinate conventions
- Magic numbers without derivation
- Unstable numerical methods

**Red flags:**
```cpp
position += velocity * dt;  // Accumulates error
float epsilon = 0.0001f;    // Why this value?
if (denominator != 0)       // Checking after use
```

---

### 4. Emergent Behavior
**Question:** Are we authoring outcomes or systems?

**Look for:**
- Hardcoded sequences
- Prescribed behaviors
- Special-case reactions
- Over-specified state machines
- Systems that prevent emergence

**Red flags:**
```cpp
if (state == JUMPING && frame == 15) { /* specific behavior */ }
// Hardcoded animation timings
// Prescribed reaction chains
```

---

### 5. Consistency
**Question:** Does this behave predictably?

**Look for:**
- Animation overriding physics
- Input ignored during transitions
- State-dependent control changes
- Inconsistent responses to same input
- Special modes that break rules

**Red flags:**
```cpp
if (animating) { ignore_input = true; }
// Different physics in cutscenes
// Special cases that break expectations
```

---

### 6. Principled Development
**Question:** Why does this exist?

**Look for:**
- Code that can't be justified
- Top-down constructed systems
- Unvalidated components in use
- Features that violate principles "just this once"
- Decisions that can't be traced to principles

**Red flags:**
```cpp
// TODO: Not sure why this works
// HACK: Don't touch this
// Complex workarounds for simple problems
```

---

## The Review Process

### Phase 1: Document the Truth
Before reviewing, understand:
- What coordinate system is used?
- What are the core physics rules?
- What is the dependency flow?
- What can be seen (debug visualization)?

### Phase 2: Find Violations
For each file in `src/`:
1. **Can anything be removed?** (Simplicity)
2. **Does each part do one thing?** (Composability)
3. **Is the math validated?** (Foundations)
4. **Does it enable or prescribe?** (Emergence)
5. **Is behavior predictable?** (Consistency)
6. **Can we explain why it exists?** (Principled)

### Phase 3: Categorize by Truth

**Mathematical Violations** (Fix immediately)
- Wrong math compounds into chaos
- Example: Incorrect coordinate transforms

**Principle Violations** (Fix soon)
- Breaks fundamental philosophy
- Example: Animation controlling physics

**Complexity Violations** (Simplify when touched)
- Works but unnecessarily complex
- Example: Multiple systems doing same thing

**Convention Violations** (Fix in batches)
- Style and naming issues
- Example: Not using snake_case

---

## Recording Findings

### Format for Backlog

```markdown
### [Violation Type]: [Brief Description]
**File:** `src/path/file.cpp:line`
**Principle Violated:** [Which of the six]
**Current:** [What exists]
**Problem:** [Why it violates]
**Solution:** [Simplest fix]
**Complexity:** [1-8 points]
**Certainty:** [How sure of the fix]
```

### Severity Levels

**Critical** - Mathematical incorrectness or gameplay-breaking
**High** - Principle violations that affect player experience
**Medium** - Complexity that impedes understanding
**Low** - Convention and style issues

---

## The Gates

Before adding to backlog, ask:

1. **Is this a real violation?** (Not opinion)
2. **Does fixing serve gameplay?** (Not perfectionism)
3. **Is the solution simpler?** (Not just different)
4. **Is the foundation stable enough?** (≥70% certainty)

If any answer is "no," do not add.

---

## Batching Opportunities

Group similar fixes:
- All magic number documentations
- All naming convention fixes
- All dead code removal
- All similar refactors

**Rule:** If fixing one requires understanding the pattern, fix all instances.

---

## Anti-Patterns to Flag

### The Accumulated State Disease
```cpp
// BAD: State drifts over time
total += increment * dt;

// GOOD: State calculated fresh
total = calculate_from_source();
```

### The Circular Reference Trap
```cpp
// BAD: Self-referential smoothing
current = lerp(current, target, dt);

// GOOD: Dual reference
current = lerp(current, target, dt);
// Where target never depends on current
```

### The Debug Blindness
```cpp
// BAD: Complex behavior without visualization
complex_calculation();
// What does this look like?

// GOOD: Visible at every step
complex_calculation();
debug_draw_result();
```

### The Control Betrayal
```cpp
// BAD: Animation controls movement
if (animation.playing()) {
    physics.velocity = animation.root_motion();
}

// GOOD: Physics controls animation
physics.update();
animation.match(physics.state);
```

---

## Output Summary

After review, report:

```
Principle Violations Found:
- Radical Simplicity: N violations
- Composable Functions: N violations
- Mathematical Foundations: N violations
- Emergent Behavior: N violations
- Consistency: N violations
- Principled Development: N violations

Critical Issues: N (fix immediately)
High Priority: N (fix soon)
Medium Priority: N (fix when touched)
Low Priority: N (batch together)

Backlog updated: PLANS/IMPROVE_BACKLOG.md
```

---

## The Commitment

This review is not about finding problems to fix.
It is about finding complexity to remove.

Every violation found is an opportunity to simplify.
Every simplification is a step toward truth.

**Review not to perfect, but to simplify.**
**Fix not to add, but to remove.**
**Build not features, but systems.**

---

## Quality Checklist

- [ ] Each violation cites specific file:line
- [ ] Each violation identifies principle broken
- [ ] Each solution makes things simpler
- [ ] Batching opportunities identified
- [ ] Only real violations, not opinions
- [ ] Respects stability requirements
- [ ] Focuses on what can be removed

---

## When to Review

- After completing a feature (check for patterns)
- When confidence collapses (find the lies)
- Before major changes (establish truth)
- When something surprises (principle was violated)

**Remember:** The best code is no code. The best fix is deletion. The best system is the simplest one that still works.

**This is the way.**