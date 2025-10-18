# FrogLords: Development Workflow

**Build from certainty. Iterate toward truth. Remove what doesn't serve.**

---

## The Foundation

This workflow serves the Six Pillars (@PRINCIPLES.md). Every decision, every cycle, every line of code must trace back to:

1. Radical Simplicity
2. Fundamental Composable Functions
3. Solid Mathematical Foundations
4. Emergent Behavior
5. Consistency
6. Principled Development

**When principles conflict, simplicity wins.**

---

## The Philosophy

### We Don't Plan Games, We Discover Them

Game designs are hidden in the logic of the universe, obscured by excess. Development is not manufacturing from a blueprint—it's **sculpture**, removing everything that doesn't serve until the form emerges.

### The Dependency Stack Is Truth

Systems depend on other systems. Uncertainty multiplies upward through dependencies. A system five layers up the stack, built on unproven foundations, has near-zero chance of surviving unchanged.

**Build from the bottom up. Start with core. Strengthen foundations before adding layers.**

### The Design Backlog Is Liquid

Most ideas will never be implemented. That's success, not failure. The backlog is a reservoir of possibilities, kept liquid and unordered until foundations are certain enough to support them.

**Ideas are cheap. Implementation is expensive. Wait for certainty.**

### Iteration Replaces Prediction

We cannot predict how a system will play until we build and test it. Deep planning on uncertain foundations is theater—comforting but delusional.

**Plan to the horizon of certainty. Test. Learn. Repeat.**

### Graybox Everything

Full art on unproven mechanics is premature production. Build cheap, test thoroughly, prove the system works before investing in polish.

**Validate with cubes. Polish when certain.**

---

## The Two Backlogs

### BACKLOG_SYSTEMS.md
**Purpose:** Systems awaiting foundation certainty

**Contents:**
- Systems organized by dependency level
- Core gameplay at bottom, polish at top
- Certainty required for each (%)
- Dependencies mapped

**Philosophy:** The stack visualizes what we can build now vs. what needs a stronger foundation.

**Selection Criteria:** Foundation ≥90% certain for that dependency level

---

### BACKLOG_REFINEMENTS.md
**Purpose:** Complexity to remove, principles to restore

**Contents:**
- Violations organized by principle
- Magic numbers to derive
- Special cases to eliminate
- Emergent behaviors to document

**Philosophy:** Systems accumulate violations. Audits make them visible. Refinement restores truth.

**Selection Criteria:** Foundation ≥70% stable, between system builds

---

## The System-Building Cycle

**When:** Foundation certain enough (≥90%) for next layer

### 1. SELECT
**File:** `PLANS/<name>_SYSTEM.md`

- Choose from BACKLOG_SYSTEMS based on dependency readiness
- Create feature branch: `system/<name>`
- Document: What is the irreducible core? What dependencies does it require?

**Exit Criteria:** Dependencies satisfied, core identified

---

### 2. GRAYBOX
**Update:** `PLANS/<name>_SYSTEM.md`

- Build in simplest possible form (cubes, primitives, debug visuals)
- Implement only the core mechanic
- Create validation harness
- Test continuously (self-test → over-shoulder → patterns emerge)

**Validation:**
- Mathematical correctness proven
- Debug visualization functional
- Core mechanic feels right

**Exit Criteria:** Core works in graybox OR discovered we shouldn't build this

**Deferral is success here.** Most systems fail in graybox. That's why we graybox.

---

### 3. ITERATE
**Update:** `PLANS/<name>_SYSTEM.md`

- Short planning horizon (hours to days)
- Small changes, frequent tests
- Watch for emergence
- Let serendipity guide (but verify mathematically)
- Stop when patterns repeat in playtests

**Questions at each iteration:**
1. Does this serve player control?
2. Can this emerge from simpler rules?
3. Is this mathematically correct?
4. Will this behave consistently?
5. Can anything be removed?

**Exit Criteria:** System stable, emergent behaviors documented OR blocker discovered

---

### 4. VALIDATE
**File:** `PLANS/<name>_REVIEW.md`

**Against each principle:**
1. **Radical Simplicity** - Can anything be removed? (score 1-10)
2. **Composable Functions** - Orthogonal to existing systems? (score 1-10)
3. **Mathematical Foundations** - Mathematically proven? (score 1-10)
4. **Emergent Behavior** - Generates unpredicted outcomes? (score 1-10)
5. **Consistency** - Predictable, no special cases? (score 1-10)
6. **Principled Development** - Decision traceable to principles? (score 1-10)

**Average score < 7.0:** Defer or revise
**Average score ≥ 7.0:** Proceed

**Document:**
- What emerged that wasn't planned?
- What surprised even the creator?
- What violations were discovered?

---

### 5. INTEGRATE
**Update:** `PLANS/<name>_SYSTEM.md`

- Remove graybox, add final implementation (if needed—graybox often sufficient)
- Verify no regressions in dependent systems
- Update debug visualizations for production
- Document emergent properties

**Exit Criteria:** System integrated, all tests pass

---

### 6. STRENGTHEN
**Update:** `BACKLOG_SYSTEMS.md`, `DEPENDENCY_STACK.md`

- Update foundation certainty (this layer now ≥90%)
- Note what systems are now buildable
- Add discovered opportunities to backlog
- Add discovered violations to BACKLOG_REFINEMENTS

**Commit with reflection:**
```
system: add <name> to enable <emergence>

Built from: <dependencies>
Emerged: <unexpected behaviors>
Enables: <future possibilities>
Certainty: <foundation level>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>
```

**Return to:** SELECT (next system)

---

## The Refinement Cycle

**When:** Between systems, foundation ≥70% stable, violations accumulating

### 1. AUDIT
**File:** `BACKLOG_REFINEMENTS.md`

Search for violations:
- Magic numbers without derivation
- Special cases (if/else on entity type, etc.)
- Duplicated logic
- Complexity without emergence
- Undocumented emergent behaviors
- Lost responsiveness (animation over physics)
- Behavioral drift (accumulated state)

**Document each:** Principle violated, severity, location

---

### 2. SELECT
**File:** `PLANS/REFINE_<name>.md`

Choose violation by:
- Severity (higher first)
- Foundation impact (core violations first)
- Simplicity of fix (sometimes)

**Classify:**
- **Trivial:** Obvious fix, low risk (<1 hour)
- **Standard:** Requires care, moderate risk (<1 day)
- **Complex:** Needs planning, high risk (>1 day)

---

### 3. REFINE
**Update:** `PLANS/REFINE_<name>.md`

**Trivial:** Just fix it
**Standard:** Plan → Execute → Validate
**Complex:** Defer to system-building cycle (might be new system)

**Three paths in order:**
1. **Delete** - Can we remove this entirely? (preferred)
2. **Simplify** - Can we derive this from something simpler?
3. **Document** - If we must keep it, why does it exist?

**Validation:**
- Principle score improved?
- Complexity reduced (LOC, dependencies)?
- No regressions?

---

### 4. MEASURE
**Update:** `PLANS/REFINE_<name>.md`

**Before/After:**
- Lines of code
- Cyclomatic complexity
- Number of special cases
- Principle scores (1-10 each)

**Document:**
- What was removed/simplified?
- What principle was restored?
- What pattern emerged? (for future audits)

**Commit with metrics:**
```
refine: remove <complexity> from <system>

Restored: <principle>
Removed: <LOC> lines, <N> special cases
Before: <principle scores>
After: <principle scores>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>
```

**Return to:** SELECT (next refinement) OR system-building cycle

---

## The Dependency Stack

**File:** `DEPENDENCY_STACK.md`

Visualize the dependency hierarchy:

```
Layer 5: POLISH          [20% certain] ← backlog only
    ↓
Layer 4: VARIATION       [40% certain] ← backlog only
    ↓
Layer 3: SYSTEMS         [70% certain] ← buildable, risky
    ↓
Layer 2: PRIMITIVES      [90% certain] ← buildable, safe
    ↓
Layer 1: CORE            [95% certain] ← proven, solid
```

**Update after every system build and refinement.**

**Rules:**
- Never build on foundations <90% certain
- Uncertainty multiplies upward: 0.9 × 0.9 × 0.9 = 0.73
- Five uncertain layers = ~60% chance of total redesign
- Build bottom-up, test at every layer

**Core Gameplay:**
The irreducible minimum. Remove everything possible while keeping the experience meaningful. This is Layer 1.

---

## Key Operational Principles

### Build From Certainty
Foundation ≥90% → Safe to build
Foundation <90% → Strengthen foundation first

### Iterate to Truth
Planning horizon = certainty horizon
Uncertain system → short iterations (hours)
Proven system → longer iterations (days)

### Validate Continuously
- Mathematical correctness proven (not guessed)
- Debug visualization always functional
- Playtests until patterns repeat
- Principle scores at every gate

### Exit Freely
Deferral is learning, not failure
Most ideas fail in graybox—that's success
Document what you learned, strengthen backlog

### Remove Aggressively
Before adding, ask what can be removed
Delete > Simplify > Document
Special cases are warnings

### Measure Everything
Foundation certainty (%)
Principle scores (1-10)
Complexity (LOC, dependencies, special cases)
Emergence (count unexpected behaviors)

---

## Warning Signs

**Lost Responsiveness:** Animation has seized control from physics
**Behavioral Drift:** Accumulated state departed from truth
**Combinatorial Explosion:** Systems not orthogonal
**Debugging Blindness:** Complex behavior lacks visualization
**Special Cases:** Consistency sacrificed for convenience
**Surprising Instability:** Mathematical foundations not validated
**Premature Production:** Polish on unproven mechanics
**Therapeutic Planning:** Deep plans to relieve anxiety, not coordinate work
**Zero Deferrals:** Not being critical enough

---

## The Commitment

**This workflow serves the principles, not productivity theater.**

We commit to:
- Build from certainty, never from ambition
- Iterate toward truth, not toward deadlines
- Validate continuously, trust nothing
- Remove complexity relentlessly
- Document emergence, celebrate surprise
- Defer freely, learn constantly

We refuse to:
- Build on uncertain foundations (<90%)
- Skip validation to "ship faster"
- Add complexity without justification
- Ignore principle violations
- Prescribe outcomes over enabling emergence
- Maintain failing approaches from stubbornness

---

## The Process Emerges

This workflow itself follows the principles:

**Radical Simplicity:** Two cycles. Two backlogs. Clear gates.
**Composable Functions:** Cycles are orthogonal. Can run independently.
**Mathematical Foundations:** Certainty thresholds are measurable.
**Emergent Behavior:** Process allows serendipity, doesn't prescribe outcomes.
**Consistency:** Same process every time, no exceptions.
**Principled Development:** Every step traces to PRINCIPLES.md.

**The workflow is a system that generates development, not a script that prescribes it.**

---

## Getting Started

### First System (Core Gameplay)
1. Create `BACKLOG_SYSTEMS.md` with initial ideas
2. Identify core gameplay (the irreducible minimum)
3. Start system-building cycle on core
4. Create `DEPENDENCY_STACK.md` as you go

### First Refinement
1. Run audit on existing code
2. Create `BACKLOG_REFINEMENTS.md`
3. Select highest-severity violation
4. Execute refinement cycle

### Steady State
Alternate between system-building (when foundation ready) and refinement (between systems). The codebase breathes: grow, refine, grow, refine.

---

## File Structure

```
/
├── PRINCIPLES.md                    # The Six Pillars
├── WORKFLOW.md                      # This document
├── BACKLOG_SYSTEMS.md              # Systems to build
├── BACKLOG_REFINEMENTS.md          # Violations to fix
├── DEPENDENCY_STACK.md             # Foundation state
│
└── PLANS/
    ├── <name>_SYSTEM.md            # System description + reflections
    ├── <name>_REVIEW.md            # Principle validation
    └── REFINE_<name>.md            # Refinement with metrics
```

**Lean. Essential. Traceable.**

---

## The Truth

We are building engines of experience.

Every system we build enables player intent.
Every refinement we make removes barriers to that intent.
Every principle we uphold serves that purpose.

The workflow exists to protect the principles.
The principles exist to serve the players.
The players exist to experience emergence.

**Build systems, not features.**
**Remove complexity, not just bugs.**
**Serve principles, not schedules.**

**This is the way.**
