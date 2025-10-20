# SELECT - Choose System to Build

**Build from certainty toward uncertainty. Choose what the foundation can support.**

---

## Purpose

Select a system from TASKS/BACKLOG_SYSTEMS.md based on dependency readiness.

**This is not about priority or excitement. This is about what we can build with what exists.**

---

## Prerequisites

- `DEPENDENCY_STACK.md` populated with current capabilities
- `BACKLOG_SYSTEMS.md` populated
- No system currently in progress

---

## Process

### 1. Review Capabilities

Open `DEPENDENCY_STACK.md` - understand what exists at each layer and what it enables

---

### 2. Select System

Open `BACKLOG_SYSTEMS.md`. Find systems at ready layer.

**For each candidate, verify:**
- Dependencies exist? (check DEPENDENCY_STACK.md)
- Core identified? (irreducible minimum)
- Mathematically provable? (can verify correctness)
- Enables emergence? (not just features)

**When multiple systems available, prioritize by:**
1. **Lowest layer** - strengthen foundation first
2. **Fewest dependencies** - reduce cascade risk
3. **Clearest core** - easiest to validate

**Boring and fundamental beats exciting and derivative.**

---

### 3. Gather References

**FULL STOP: Ask the user what they actually want.**

Before classifying complexity or creating the plan, collect context that informs the implementation:

**Prompt the user:**
- Technical documentation? (APIs, papers, articles, specs)
- Visual references? (screenshots, videos, diagrams, mockups)
- Inspiration/prior art? (how other games/systems solved this)
- Research findings? (what you've already investigated)
- Constraints or requirements? (specific behavior expectations)

**Document everything provided.** This section informs complexity classification and graybox implementation.

---

### 4. Classify Complexity

**References inform this assessment. A well-documented reference may simplify. An unknown may complicate.**

**Small (1-3 pts):** Single primitive, clear math, minimal integration
- Examples: New camera mode, simple collision shape, basic particle effect

**Medium (4-8 pts):** Multiple components, moderate integration, some unknowns
- Examples: Animation blending system, terrain collision, AI state machine

**Large (>8 pts):** Major subsystem, deep integration, significant unknowns
- Examples: Skeletal animation, networked physics, procedural generation

---

### 5. Create Plan

Create `TASKS/PLANS/<name>_SYSTEM.md`:

```markdown
# System: <Name>

**One-line description of what this enables**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** [Today]
**Layer:** [N]
**Complexity:** Small | Medium | Large
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**
- [APIs, papers, articles, specs]

**Visual references:**
- [Screenshots, videos, diagrams, mockups]

**Inspiration/Prior art:**
- [How other games/systems solved this]
- [What worked, what didn't]

**Research findings:**
- [Key insights from investigation]

**Constraints/Requirements:**
- [Specific behavior expectations]
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- System A (exists in Layer X)
- System B (exists in Layer Y)

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
- [ ] Principles upheld
<!-- END: SELECT/SUCCESS -->
```

---

### 6. Update Current Plan

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
**Complexity:** Small | Medium | Large
**Next:** GRAYBOX

**Dependencies:**
- System A (Layer X)
- System B (Layer Y)
```

**CRITICAL:** `@` reference ensures plan loads automatically via `CLAUDE.md`.

---

### 7. Create Branch

```bash
git checkout -b system/<name>
git add TASKS/PLANS/<name>_SYSTEM.md TASKS/CURRENT_PLAN.md
git commit -m "select: <name> system for layer <N>

Dependencies: <list>
Complexity: Small | Medium | Large
Core: <one-line>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Outputs

- [ ] System selected
- [ ] References gathered from user
- [ ] Complexity classified (informed by references)
- [ ] `PLANS/<name>_SYSTEM.md` created
- [ ] `CURRENT_PLAN.md` updated
- [ ] Branch created
- [ ] Core identified
- [ ] Dependencies verified

---

## Next

**Always → GRAYBOX** (build simplest possible version)

**Defer if:**
- Dependencies don't actually exist
- Core unclear or too complex
- Better choice available

---

**Build from what exists. Choose by dependencies. Enable emergence.**
