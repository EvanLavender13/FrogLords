# SELECT - Choose Violation to Fix

**Delete first. Simplify second. Document last.**

---

## Purpose

Choose highest-impact violation from backlog. Classify complexity. Determine path.

---

## Prerequisites

- `TASKS/BACKLOG_REFINEMENTS.md` populated
- Between features (not during builds)

---

## Process

### 1. Review Current Capabilities

Open `TASKS/DEPENDENCY_STACK.md` - understand what exists and what the violation affects

---

### 2. Select Violation

Open `TASKS/BACKLOG_REFINEMENTS.md`.

Choose the highest-impact violation. Apply the principles.

---

### 3. Classify Complexity

**Trivial (1-2 pts):** Single concern, isolated to one location. Fix path obvious. Deletion or documentation. No behavior changes. Zero cascade risk.

**Skip plan for immediate fixes:** Single-line changes, obvious deletions, static analyzer fixes with clear semantic validation. Commit directly with descriptive message.

**Create plan when:** Multiple files affected, requires validation approach, or teaches pattern worth documenting.

**Standard (3-8 pts):** Crosses file boundaries. Requires deriving from first principles or extracting primitives. Changes behavior. Moderate cascade risk. Must validate existing systems remain unaffected.

**Complex (>8 pts):** System-wide implications. Requires new primitives or fundamental restructuring. High cascade risk across multiple layers. **Don't refine—build new system instead.**

**Brevity principle:** Documentation should not substantially exceed the work. For naming changes, deletions, and audits, keep plans concise. Delete repetition, state facts once, stop when done.

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
**Principle:** <violated>
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
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Delete | Simplify | Document

**CRITICAL:** State the approach directly in plain language. No code examples, no pseudocode demonstrations. "Directness Over Demonstration"—if approach needs examples to be clear, the approach is unclear.

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
