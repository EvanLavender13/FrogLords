# SELECT - Choose Violation to Fix

**Delete first. Simplify second. Document last.**

---

## Purpose

Choose highest-impact violation from backlog. Classify complexity. Determine path.

---

## Prerequisites

- Foundation ≥70% (`TASKS/DEPENDENCY_STACK.md`)
- `TASKS/BACKLOG_REFINEMENTS.md` populated
- Between features (not during builds)

---

## Process

### 1. Check Foundation

Open `TASKS/DEPENDENCY_STACK.md`:
- Overall: ___%
- **≥70%**: Proceed
- **<70%**: Defer—strengthen foundation first

---

### 2. Select Violation

Open `TASKS/BACKLOG_REFINEMENTS.md`.

Choose the highest-impact violation. Apply the principles.

---

### 3. Classify Complexity

**Trivial (1-2 pts):** Single concern, isolated to one location. Fix path obvious. Deletion or documentation. No behavior changes. Zero cascade risk.

**Standard (3-8 pts):** Crosses file boundaries. Requires deriving from first principles or extracting primitives. Changes behavior. Moderate cascade risk. Must validate existing systems remain unaffected.

**Complex (>8 pts):** System-wide implications. Requires new primitives or fundamental restructuring. High cascade risk across multiple layers. **Don't refine—build new system instead.**

---

### 4. Create Plan

Create `TASKS/PLANS/REFINE_<name>.md`:

```markdown
# Refinement: <Name>

**One-line description**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** [Today]
**Complexity:** Trivial | Standard
**Path:** A (trivial) | B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `<file>:<line>` | System: <name>
**Principle:** <violated> | Score: __/10
**Severity:** Critical | High | Medium | Low
**Type:** <describe the nature of the violation>

**Current state:**
```
[code/description]
```

**Why violation:**
-
-

**Impact:**
- Blocks:
- Cascades to:
- Foundation: __%
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Delete | Simplify | Document

**CRITICAL:** Describe the approach conceptually. Use pseudocode and high-level explanations. Do NOT write actual source code—that happens during REFINE.

**DELETE:**
- Remove:
- Impact:
- Tests:

**SIMPLIFY:**
- From:
- To:
- Derive/Generalize:
- Check duplicates:

**DOCUMENT:**
- Why keep:
- Derivation:
- Trade-off:
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved
- [ ] Principle score improved
- [ ] Tests passing
- [ ] No regressions

**Metrics:**
- Before: LOC __, Principle __/10, Complexity __
- After: LOC __ (-__), Principle __/10 (+__), Complexity __ (-__)
<!-- END: SELECT/SUCCESS -->
```

---

### 5. Determine Path

Record complexity classification in the plan:

**Path A (Trivial):** Complexity 1-2 pts
**Path B (Standard):** Complexity 3-8 pts
**Path C (Complex):** Complexity >8 pts - defer to SYSTEM/SELECT

---

### 6. Update Current Plan

Update `TASKS/CURRENT_PLAN.md`:

```markdown
# Current Plan

**Status:** 🔧 Refinement - <Name>
**Started:** [Today]
**Branch:** refine/<name>
**Path:** A | B
**Phase:** SELECT → REFINE

---

## Current Work

**Refinement:** <description>
**Location:** @TASKS/PLANS/REFINE_<name>.md
**Principle:** <which>
**Approach:** Delete | Simplify | Document
**Next:** REFINE

**Target Impact:**
- Layer X: __% → __% (+__)
- Overall: __% → __% (+__)
```

**CRITICAL:** `@` reference ensures plan loads automatically via `CLAUDE.md`.

---

### 7. Create Branch

```bash
git checkout -b refine/<name>
git add TASKS/PLANS/REFINE_<name>.md TASKS/CURRENT_PLAN.md
git commit -m "select: refinement of <violation>

Violation: <description>
Principle: <which>
Severity: <level>
Complexity: Trivial | Standard
Path: A | B

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Outputs

- [ ] Violation selected
- [ ] `TASKS/PLANS/REFINE_<name>.md` created
- [ ] Complexity classified
- [ ] Path determined
- [ ] `TASKS/CURRENT_PLAN.md` updated
- [ ] Branch created

---

## Next

**Always → REFINE** (unless Path C - Complex)

**Path C (Complex):** → SYSTEM/SELECT (build system instead)

---

**Choose by impact. Classify by risk. Delete when possible.**
