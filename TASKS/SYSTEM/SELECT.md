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

**When multiple systems available, prioritize by:**
1. **Lowest layer** - strengthen foundation first
2. **Fewest dependencies** - reduce cascade risk
3. **Clearest core** - easiest to validate

**Boring and fundamental beats exciting and derivative.**

---

### 3. Classify Complexity

**Small (1-3 pts):** Single primitive, clear math, minimal integration
- Examples: New camera mode, simple collision shape, basic particle effect

**Medium (4-8 pts):** Multiple components, moderate integration, some unknowns
- Examples: Animation blending system, terrain collision, AI state machine

**Large (>8 pts):** Major subsystem, deep integration, significant unknowns
- Examples: Skeletal animation, networked physics, procedural generation

---

### 4. Create Plan

Create `TASKS/PLANS/<name>_SYSTEM.md`:

```markdown
# System: <Name>

**One-line description of what this enables**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** [Today]
**Layer:** [N]
**Foundation:** [%]
**Complexity:** Small | Medium | Large
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required (≥90%):**
- System A: __%
- System B: __%

**Enables (will become buildable):**
- System X
- System Y
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
[2-3 sentences: what cannot be removed?]

**Why a system, not a feature:**
[How does this enable emergence?]
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
-
-

**Known uncertainties:**
-
-
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
[Cubes? Primitives? Debug visuals?]

**Validation:**
[How to test continuously?]
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Core functional in graybox
- [ ] Mathematical correctness proven
- [ ] Debug visualization shows behavior
- [ ] Emergent behaviors appear
- [ ] Principle scores ≥7.0 average
<!-- END: SELECT/SUCCESS -->
```

---

### 5. Update Current Plan

Update `TASKS/CURRENT_PLAN.md`:

```markdown
# Current Plan

**Status:** 🏗️ System - <Name>
**Started:** [Today]
**Branch:** system/<name>
**Complexity:** Small | Medium | Large
**Phase:** SELECT → GRAYBOX

---

## Current Work

**System:** <description>
**Location:** @TASKS/PLANS/<name>_SYSTEM.md
**Layer:** [N]
**Foundation:** [%]
**Complexity:** Small | Medium | Large
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
Complexity: Small | Medium | Large
Core: <one-line>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Outputs

- [ ] System selected
- [ ] Complexity classified
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
