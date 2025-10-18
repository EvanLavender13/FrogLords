# SELECT - Choose System to Build

**Build from certainty toward uncertainty. Choose what the foundation can support.**

---

## Purpose

Select a system from TASKS/BACKLOG_SYSTEMS.md based on dependency readiness and foundation certainty.

**This is not about priority or excitement. This is about what we can build without building on sand.**

---

## Prerequisites

- [ ] TASKS/BACKLOG_SYSTEMS.md populated with systems
- [ ] DEPENDENCY_STACK.md shows current certainty levels
- [ ] No other system currently in progress
- [ ] Foundation stable enough for new work

---

## Process

### 1. Check Foundation State

Open `DEPENDENCY_STACK.md` and review:
- What is the current certainty at each layer?
- Which layer is ready for new systems (≥90%)?
- Are all dependencies for that layer satisfied?

**If no layer ≥90%, strengthen foundation first (run refinements or iterate on existing systems).**

---

### 2. Review Available Systems

Open `TASKS/BACKLOG_SYSTEMS.md` and find systems marked "Buildable Now" at the layer you identified.

For each candidate:
- Dependencies satisfied? (all prerequisite systems at ≥90%)
- Core identified? (can describe irreducible minimum)
- Mathematically approachable? (can prove correctness)
- Aligns with principles? (serves emergence, not features)

---

### 3. Choose the Simplest

When multiple systems are available, choose by:
1. **Lowest layer** (strengthen foundation first)
2. **Fewest dependencies** (reduce cascade risk)
3. **Clearest core** (easiest to validate)

**Boring and fundamental beats exciting and derivative.**

---

### 4. Create System Document

Create `TASKS/PLANS/<name>_SYSTEM.md`:

```markdown
# System: <Name>

**One-line description of what this enables (not what it does)**

---

## Selected

**Date:** [Today]
**Layer:** [1-5]
**Foundation Certainty:** [%]

---

## Dependencies

**Required Systems (must be ≥90%):**
- System A: [certainty %]
- System B: [certainty %]

**Enabled Systems (will become buildable):**
- System X
- System Y

---

## Core

**The irreducible minimum:**
[Describe in 2-3 sentences what cannot be removed while keeping this meaningful]

**What makes this a system, not a feature:**
[How does this enable emergence vs. prescribe outcomes?]

---

## Mathematical Foundation

**What needs mathematical proof:**
-
-

**Known uncertainties:**
-
-

---

## Graybox Approach

**Simplest possible implementation:**
[Cubes? Primitives? Debug visuals?]

**Validation harness:**
[How will we test this continuously?]

---

## Success Criteria

**This system works when:**
- [ ] Core mechanic functional in graybox
- [ ] Mathematical correctness proven
- [ ] Debug visualization shows behavior
- [ ] Emergent behaviors appear
- [ ] Principle scores ≥7.0 average

---

## Notes

[Initial thoughts, concerns, opportunities]
```

---

### 5. Create Feature Branch

```bash
git checkout -b system/<name>
git add TASKS/PLANS/<name>_SYSTEM.md
git commit -m "select: <name> system for layer <N>

Dependencies satisfied: <list>
Foundation certainty: <%>
Core identified: <one-line>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

### 6. Update Backlog

In `TASKS/BACKLOG_SYSTEMS.md`:
- Move selected system from "Buildable Now" to "In Progress"
- Add note: `[WIP: system/<name>]`

---

## Outputs

- [ ] `TASKS/PLANS/<name>_SYSTEM.md` created
- [ ] Feature branch `system/<name>` created
- [ ] Initial commit made
- [ ] Backlog updated
- [ ] Core identified
- [ ] Dependencies verified

---

## Exit Criteria

**Proceed to GRAYBOX when:**
- System document complete
- Core clearly identified
- Dependencies all ≥90% certain
- Mathematical approach clear

**Defer and return to backlog if:**
- Dependencies not actually satisfied
- Core unclear or too complex
- Foundation not ready
- Better choice available

---

## Next Step

**→ GRAYBOX** (build simplest possible version)

---

## Principles Check

**Before proceeding, verify:**

1. **Radical Simplicity** - Is this the simplest system that could enable what we need?
2. **Composable Functions** - Does this compose with existing systems without overlap?
3. **Mathematical Foundations** - Can we prove this will work mathematically?
4. **Emergent Behavior** - Will this enable outcomes, not prescribe them?
5. **Consistency** - Will this behave predictably?
6. **Principled Development** - Can we trace this decision to a principle?

**If any answer is "no," reconsider the selection.**

---

**Build from certainty. Start with core. Strengthen the foundation.**
