# SELECT - Choose System to Build

**Build from certainty toward uncertainty. Choose what the foundation can support.**

---

## Purpose

Select a system from TASKS/BACKLOG_SYSTEMS.md based on dependency readiness and foundation certainty.

**This is not about priority or excitement. This is about what we can build without building on sand.**

---

## Prerequisites

- Foundation ≥90% (`DEPENDENCY_STACK.md`)
- `BACKLOG_SYSTEMS.md` populated
- No system currently in progress

---

## Process

### 1. Check Foundation

Open `DEPENDENCY_STACK.md`:
- Current certainty: ___%
- **≥90%**: Proceed to layer selection
- **<90%**: Defer—strengthen foundation first

**Ready layer:** Layer __ (≥90%)

---

### 2. Select System

Open `BACKLOG_SYSTEMS.md`. Find systems at ready layer.

**For each candidate, verify:**
- Dependencies ≥90%? (all prerequisites)
- Core identified? (irreducible minimum)
- Mathematically provable? (can verify correctness)
- Enables emergence? (not just features)

---

### 3. Prioritize

When multiple systems available, choose by:
1. **Lowest layer** - strengthen foundation first
2. **Fewest dependencies** - reduce cascade risk
3. **Clearest core** - easiest to validate

**Boring and fundamental beats exciting and derivative.**

---

### 4. Create Plan

Create `TASKS/PLANS/<name>_SYSTEM.md`:

```markdown
# System: <Name>

**One-line description of what this enables**

---

## Selected

**Date:** [Today]
**Layer:** [N]
**Foundation:** [%]

---

## Dependencies

**Required (≥90%):**
- System A: __%
- System B: __%

**Enables (will become buildable):**
- System X
- System Y

---

## Core

**Irreducible minimum:**
[2-3 sentences: what cannot be removed?]

**Why a system, not a feature:**
[How does this enable emergence?]

---

## Mathematical Foundation

**Needs proof:**
-
-

**Known uncertainties:**
-
-

---

## Graybox

**Simplest implementation:**
[Cubes? Primitives? Debug visuals?]

**Validation:**
[How to test continuously?]

---

## Success

- [ ] Core functional in graybox
- [ ] Mathematical correctness proven
- [ ] Debug visualization shows behavior
- [ ] Emergent behaviors appear
- [ ] Principle scores ≥7.0 average
```

---

### 5. Update Current Plan

Update `TASKS/CURRENT_PLAN.md`:

```markdown
# Current Plan

**Status:** 🏗️ System - <Name>
**Started:** [Today]
**Branch:** system/<name>
**Phase:** SELECT → GRAYBOX

---

## Current Work

**System:** <description>
**Location:** @TASKS/PLANS/<name>_SYSTEM.md
**Layer:** [N]
**Foundation:** [%]
**Next:** GRAYBOX

**Dependencies:**
- System A: __%
- System B: __%

**Target Impact:**
- Layer [N]: __% → __% (+__)
- Overall: __% → __% (+__)
```

**CRITICAL:** `@` reference ensures plan loads automatically via `CLAUDE.md`.

---

### 6. Create Branch

```bash
git checkout -b system/<name>
git add TASKS/PLANS/<name>_SYSTEM.md TASKS/CURRENT_PLAN.md
git commit -m "select: <name> system for layer <N>

Dependencies: <list>
Foundation: <%>
Core: <one-line>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Outputs

- [ ] System selected
- [ ] `PLANS/<name>_SYSTEM.md` created
- [ ] `CURRENT_PLAN.md` updated
- [ ] Branch created
- [ ] Core identified
- [ ] Dependencies verified

---

## Next

**Always → GRAYBOX** (build simplest possible version)

**Defer if:**
- Dependencies not actually ≥90%
- Core unclear or too complex
- Foundation not ready
- Better choice available

---

**Build from certainty. Choose by foundation. Enable emergence.**
