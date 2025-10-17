# Clarify Feature

**Question ambiguity, challenge prescription, seek mathematical truth.**

---

## The Purpose

Clarification is not gathering requirements—it's philosophical interrogation. We question what's being asked, challenge vague intentions, and ensure we understand the primitive being requested.

**The best clarification reveals we shouldn't build. The second best transforms features into systems.**

---

## Prerequisites

- Feature request received (from REQUEST)
- Before planning begins
- Time to think deeply

---

## Phase 1: The Fundamental Questions

Before asking about details, ask about essence:

### Is This a System or a Feature?

**Feature:** "Add a double jump"
**System:** "Enable aerial control through force application"

Ask:
```markdown
## Essence Clarification

**As Stated:** [Original request]
**Is This:** Feature (specific outcome) or System (enabling capability)?

### If Feature
Could this emerge from a simpler system?
What's the primitive underneath?
Are we prescribing when we could enable?

### If System
What's the irreducible core?
What behaviors should emerge?
What are we NOT prescribing?
```

**Often, features reveal themselves as compositions of simpler systems.**

---

## Phase 2: Challenge Through Principles

Test the request against each pillar:

### 1. Radical Simplicity
```markdown
## Simplicity Questions

- What's the minimal version?
- What can we remove from the request?
- Could something existing already enable this?
- Is complexity being requested or will it emerge?

**Challenge:** "Do we need this at all?"
```

### 2. Fundamental Composable Functions
```markdown
## Composability Questions

- Does this compose with existing systems?
- Will it require special cases?
- Is it orthogonal or overlapping?
- What systems does it depend on?

**Challenge:** "Could existing systems compose to create this?"
```

### 3. Solid Mathematical Foundations
```markdown
## Mathematical Questions

- What equations govern this?
- How do we prove correctness?
- What coordinate systems are involved?
- What can go wrong numerically?

**Challenge:** "Can we validate this mathematically?"
```

### 4. Emergent Behavior
```markdown
## Emergence Questions

- What outcomes are prescribed in the request?
- What should emerge instead?
- What behaviors might surprise us?
- Are we authoring or enabling?

**Challenge:** "Are we designing the physics or the outcomes?"
```

### 5. Consistency
```markdown
## Consistency Questions

- Will behavior be predictable?
- Is player control preserved?
- Are there hidden special cases?
- Does it work in all contexts?

**Challenge:** "Will this ever betray player expectations?"
```

### 6. Principled Development
```markdown
## Justification Questions

- Why are we building this?
- What principle guides it?
- How will we know it's correct?
- Can we trace every decision?

**Challenge:** "Can we justify this philosophically?"
```

---

## Phase 3: Seek Mathematical Clarity

Vague terms hide mathematical ambiguity.

### Challenge Vague Language

**Vague:** "Make it smooth"
**Clear:** "Spring-damped over 0.3s with critical damping"

**Vague:** "Natural feeling"
**Clear:** "Physically accurate force integration with [specific parameters]"

**Vague:** "Responsive but not too fast"
**Clear:** "Input to acceleration in [N]ms, capped at [M] units/s²"

### Ask Mathematical Questions
```markdown
## Mathematical Specification

- What are the governing equations?
- What are the input parameters?
- What are the output behaviors?
- How do we validate correctness?
- What reference solution exists?

**For each vague term, demand mathematical precision.**
```

---

## Phase 4: Demand Visual Truth

Words lie. Examples reveal.

### Request Visual References
```markdown
## Visual Clarification

- Reference video/GIF showing the behavior?
- Existing game that does this well?
- Sketch of the intended outcome?
- Debug visualization showing what we should see?

**No visual reference = no clarity**
```

### Challenge Descriptions Without Examples

"Like [Game X]" → Request timestamp in video
"Smooth transition" → Request example with measurable parameters
"Natural movement" → Request reference of what "natural" means

---

## Phase 5: Question Validation

If we can't validate, we can't build.

### Ask Validation Questions
```markdown
## Validation Strategy

- How will we know it's correct?
- What debug visualization do we need?
- What edge cases must we test?
- How will we see what's happening?
- What numerical tests prove correctness?

**If validation is unclear, the requirement is unclear.**
```

---

## Phase 6: Seek the Minimal Primitive

Often requests bundle multiple things.

### Decompose the Request
```markdown
## Primitive Identification

**Original Request:** [The full request]

**Decomposed Into:**
1. [Primitive 1] - Core mechanical element
2. [Primitive 2] - Supporting system
3. [Nice-to-have] - Can emerge later

**Minimal Buildable System:** [What's actually needed]

**Can Be Deferred:** [What's not essential]
```

**The best clarification reduces scope.**

---

## Phase 7: Document Questions

Create `PLANS/<feature>_FEATURE.md` with clarification needed:

```markdown
# Feature: [Name]

**Status:** CLARIFICATION NEEDED

## Original Request
[As stated by user]

## Principle Questions

### Essence
- Is this a system or feature?
- What's the irreducible primitive?
- Could this emerge from existing systems?

### Simplicity
- What's the minimal version?
- What can be removed?
- Is this needed at all?

### Mathematical Foundation
- What are the governing equations?
- How do we validate correctness?
- What coordinate systems involved?

### Emergence
- What outcomes are prescribed vs enabled?
- What should surprise us?
- Are we designing physics or outcomes?

### Consistency
- Will behavior be predictable?
- Is player control preserved?
- Any special cases?

### Validation
- How do we know it's correct?
- What visualization needed?
- What numerical tests?

## Specific Clarifications Needed

1. [Vague term or concept requiring mathematical precision]
2. [Request for visual reference]
3. [Question about scope or necessity]
4. [Challenge about complexity]
5. [Validation approach unclear]

## Recommended Simplification

[If applicable: simpler system that might suffice]

**AWAITING CLARIFICATION**
```

---

## Phase 8: Wait for Answers

**STOP.** Do not proceed to planning.

The request is not clear until:
- Mathematical precision achieved
- Visual references provided
- Validation strategy clear
- Primitive identified
- Principles satisfied

---

## Phase 9: Incorporate Answers

When answers received, update the document:

```markdown
---

## CLARIFICATION RESOLVED

**Date:** [YYYY-MM-DD]

### Answers Received
[Document each answer]

### Mathematical Specification
[Precise equations and parameters]

### Visual References
[Links, timestamps, examples]

### Validation Approach
[How we'll know it's correct]

### Simplified Scope
[What we're actually building]

### Principle Alignment
1. Simplicity: [How this is minimal]
2. Composability: [How this fits]
3. Mathematics: [How we validate]
4. Emergence: [What will surprise]
5. Consistency: [How it's predictable]
6. Development: [Why we're building]

**Ready for PLAN**
```

---

## When to Skip Clarification

Skip only when:

- **Mathematically Precise:** Request includes exact equations and parameters
- **Visually Clear:** Reference examples provided
- **Validation Obvious:** Success criteria unambiguous
- **Primitive Identified:** Core system already clear
- **Principles Aligned:** Obviously upholds all six

**When in doubt, clarify. Ambiguity compounds into chaos.**

---

## Examples

### Example 1: Transforming Feature to System

**Original:** "Add wall running"

**Clarification:**
```markdown
## Essence Questions
- Is this prescribing a specific mechanic or requesting enhanced movement capabilities?
- Could this emerge from: surface contact detection + force application system?
- What's the primitive: "wall running" or "maintaining contact with non-ground surfaces"?

## Simplification Proposal
Instead of "wall running" (prescribed), build:
- Surface normal detection (primitive)
- Force application relative to surface (primitive)
- Friction model for various surfaces (primitive)

Wall running emerges from these primitives.
```

**Result:** System that enables wall running AND other emergent behaviors.

### Example 2: Demanding Mathematical Precision

**Original:** "Make jumping feel better"

**Clarification:**
```markdown
## Mathematical Questions
"Better" is not measurable. What specifically:

1. Initial velocity (current: X m/s, desired: ?)
2. Gravity modifier (current: Y m/s², desired: ?)
3. Air control force (current: Z N, desired: ?)
4. Apex hang time (current: W s, desired: ?)

Visual reference of "better" jump?
Game example with timestamp?

**Challenge:** Can we quantify "better" or is this unsolvable?
```

**Result:** Specific parameters or realization that request is unclear.

### Example 3: Challenging Necessity

**Original:** "Add combat system"

**Clarification:**
```markdown
## Simplicity Challenge
- What problem does combat solve?
- Could interaction emerge from physics instead?
- Is this the minimal mechanic needed?
- What if we just enabled: force application + health system?

**Philosophical Question:**
Are we building "combat" or "physical interaction with consequences"?

The second is simpler and enables more emergence.
```

**Result:** Reframe from complex to simple.

---

## The Philosophy of Clarification

### Ambiguity Is Technical Debt

Every vague term becomes a decision made under pressure during implementation. Make decisions now, with thought, not later under deadline.

### Words Hide Assumptions

"Natural," "smooth," "responsive"—these words mean nothing. Mathematics means everything. Demand precision.

### Features Prescribe, Systems Enable

When users request features, look for the system underneath. Build systems. Features emerge.

### The Best Clarification Simplifies

If clarification reveals we need less than requested, celebrate. Less is more.

---

## The Questions of Clarity

Before accepting any request, ask:

1. **What's the mathematical truth?**
   - Equations, parameters, validation

2. **What's the visual truth?**
   - Examples, references, proof

3. **What's the essential primitive?**
   - Irreducible core underneath

4. **Could this emerge instead?**
   - Simpler systems that compose

5. **How do we validate?**
   - Proof of correctness

---

## The Commitment

Clarify requests that:
- Transform features into systems
- Demand mathematical precision
- Challenge necessity
- Reduce scope
- Enable emergence

Never accept requests that:
- Remain vague after questioning
- Prescribe without justification
- Can't be validated
- Add complexity without cause
- Conflict with principles

**The best clarification prevents building. The second best simplifies what's built. Both serve the principles.**

**This is the way.**