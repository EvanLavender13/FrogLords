# Select Feature

**Build systems that generate experiences, not experiences themselves.**

---

## The Selection Philosophy

We are not authors—we are system designers. Every feature must be a fundamental system that enables emergence, not a prescribed outcome.

The best feature is the simplest system with the richest possibilities.

---

Read `AGENTS.md` - internalize the Six Pillars

## The Six Questions

Before selecting any feature, ask:

1. **Is this a system or a feature?** (Systems enable, features prescribe)
2. **Can it emerge from existing systems?** (Don't build what can emerge)
3. **Is it mathematically validatable?** (Can we prove it correct?)
4. **Will it compose with other systems?** (Orthogonality)
5. **Is it the simplest version?** (Radical simplicity)
6. **Can we see it working?** (Debug visibility)

If any answer is "no," do not select.

---

## Selection Process

### Phase 1: Understand the Foundation

Review `PLANS/DEPENDENCY_STACK.md`:
- What is our certainty level? (Need ≥90%)
- What systems are stable?
- Where is the frontier?

**If <90% certain:** Fix foundation first.
**If ≥90% certain:** Can build new systems.

### Phase 2: Identify True Systems

Review `PLANS/DESIGN_BACKLOG.md`

**Look for:**
- Systems that enable multiple behaviors
- Primitives that compose
- Rules that generate outcomes
- Tools that multiply possibilities

**Avoid:**
- Specific behaviors
- Prescribed sequences
- Special cases
- Content (levels, assets, scripts)

### Phase 3: Apply the Filters

**The Mathematical Filter**
- Can we validate this in isolation?
- Is the math provable?
- Will it remain stable?

**The Simplicity Filter**
- Is this the irreducible version?
- Can anything be removed?
- Does it do exactly one thing?

**The Emergence Filter**
- What unexpected behaviors might emerge?
- Does it multiply possibilities?
- Will it surprise us?

**The Visibility Filter**
- Can we see it working?
- Can we debug it visually?
- Will we know when it breaks?

---

## Priority Hierarchy

### 1. Debug Visualization (Always First)
Cannot validate what cannot be seen. Every system needs eyes.

### 2. Mathematical Primitives
Physics, collision, transforms. The foundation of truth.

### 3. Reactive Systems
Systems that interpret state without controlling it.

### 4. Emergent Mechanics
Rules that combine to create possibilities.

### 5. Polish Layers
Only after mechanics are proven and stable.

---

## Complexity Classification

### Primitive (1-2 points)
- Single mathematical operation
- Pure function
- Debug visualization
- Parameter exposure

### System (3-4 points)
- Composable mechanic
- Reactive behavior
- State interpreter
- Tool that enables

### Complex (5-8 points)
- Multi-system integration
- Question if truly needed
- Often better as multiple primitives

**Build primitives. Combine into systems. Let complexity emerge.**

---

## Document the Selection

Create `PLANS/<name>_FEATURE.md`:

```markdown
# [System Name]

## The System
**Core Primitive:** [The irreducible mechanic]
**Mathematical Basis:** [The underlying truth]
**Single Responsibility:** [What it does, nothing more]

## The Validation
**Isolation Test:** [How to verify math correctness]
**Visual Debug:** [What we need to see]
**Stability Metric:** [How we know it's working]

## The Composition
**Combines With:** [Other systems it composes with]
**Enables:** [Behaviors that will emerge]
**Doesn't Prescribe:** [What it intentionally doesn't specify]

## The Implementation
**Foundation Required:** [Prerequisites at 90%+ certainty]
**Insertion Point:** [Where in the code]
**Lines Estimate:** [Keep it small]

## The Emergence
**Intended:** [Basic behavior we need]
**Possible:** [What might emerge]
**Surprising:** [What we hope discovers us]

Complexity: [1-8 points]
Certainty: [% confidence]
```

---

## Selection Criteria

### Must Have
- Is a system, not a feature
- Mathematically validatable
- Can be seen/debugged
- Single clear responsibility
- Composes with existing systems

### Must Not Have
- Prescribed outcomes
- Special cases
- Hidden complexity
- Circular dependencies
- Control inversions

---

## The Anti-Patterns

### The Content Trap
Levels, scripts, and sequences are not systems. They're content. Build the system that generates them.

### The Prescriptive Prison
If you know all outcomes beforehand, it's too rigid. Systems should surprise.

### The Kitchen Sink
One system doing many things is many systems pretending to be one. Separate them.

### The Black Box
If you can't see inside it, you can't debug it. Every system needs windows.

---

## Examples of Systems vs Features

### Systems (Build These)
- Spring-damper (primitive for smooth change)
- Collision detection (spatial queries)
- Phase-based animation (motion from distance)
- Debug visualization (see the invisible)

### Features (Don't Build These)
- "Double jump" (emerges from jump system + state)
- "Wall slide" (emerges from collision + friction)
- "Combo attacks" (emerges from input + timing)
- "Difficulty modes" (emerges from parameter tuning)

---

## When Nothing Qualifies

If no systems meet criteria:

1. **Fix foundation** - Is math validated?
2. **Add visibility** - Can we see our systems?
3. **Simplify existing** - Before adding new
4. **Question need** - Maybe we have enough

Sometimes the best feature is no feature.

---

## The Test

After selecting, you should be able to say:

> "We're building a [primitive/system] that [does one thing] based on [mathematical principle],
> which will compose with [existing systems] to enable [emergent behaviors] that we can
> [see and debug]. It's the simplest version that [solves the problem]."

If you can't say this clearly, don't build it.

---

## Output

State selection clearly:

```
Selected: [Name]
Type: [Primitive | System | Tool]
Mathematical Basis: [Core truth]
Composes With: [Systems]
Enables: [Emergent behaviors]
Complexity: [1-8 points]
Certainty: [%]
Debug Plan: [How we'll see it]
```

---

## The Commitment

Select systems that:
- Can be proven correct
- Do exactly one thing
- Compose without special cases
- Can be debugged visually
- Enable more than intended

Never select features that:
- Prescribe specific outcomes
- Hide their behavior
- Break composability
- Can't be validated
- Exist "just in case"

**The best system enables features we never imagined.**

**This is the way.**