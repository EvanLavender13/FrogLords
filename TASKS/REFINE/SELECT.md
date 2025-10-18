# SELECT - Choose Violation to Fix

**Delete first. Simplify second. Document last.**

---

## Purpose

Choose the highest-impact violation from TASKS/BACKLOG_REFINEMENTS.md and classify its complexity to determine the refinement path.

**Not all violations require the same rigor. Choose the right path.**

---

## Prerequisites

- [ ] Foundation ≥70% stable
- [ ] `TASKS/BACKLOG_REFINEMENTS.md` populated with violations
- [ ] No system builds in progress
- [ ] Between features (good time to refine)

---

## Process

### 1. Check Foundation Stability

Open `TASKS/DEPENDENCY_STACK.md`:
- Overall foundation: ___%
- Recent changes: ___
- Active systems: ___

**Foundation requirements:**
- **≥70%**: Safe to refine
- **<70%**: Strengthen foundation first (build or iterate core systems)

**If foundation unstable, defer refinement.**

---

### 2. Review Available Violations

Open `TASKS/BACKLOG_REFINEMENTS.md` and scan:

**By severity:**
- Critical violations (foundation threatening)
- High violations (actively harmful)
- Medium violations (technical debt)
- Low violations (cosmetic)

**By principle:**
- Which principle most violated?
- Which violations block future work?
- Which violations cascade?

---

### 3. Select by Priority

**Choose violation by:**

**Priority order:**
1. **Severity** (Critical > High > Medium > Low)
2. **Foundation impact** (Core systems > Leaf systems)
3. **Cascade risk** (Many dependents > Few dependents)
4. **Fix approach** (Delete > Simplify > Document)

**Prefer:**
- Violations that can be deleted entirely
- Violations in core/primitive layers
- Violations blocking future work
- Violations with clear fix path

**Avoid:**
- Violations requiring new systems
- Violations with unclear scope
- Violations blocked by dependencies

---

### 4. Classify Complexity

**For the selected violation, estimate:**

**Trivial (1-2 complexity points):**
- Obvious fix
- Low risk
- Single file
- <1 hour
- No tests to update
- Can skip planning

**Examples:**
- Delete dead code
- Remove commented-out code
- Fix obvious duplication
- Document magic number

**Standard (3-8 complexity points):**
- Requires thought
- Moderate risk
- Multiple files or complex logic
- <1 day
- Tests need updates
- Needs planning

**Examples:**
- Derive magic number from first principles
- Remove special case (make rule general)
- Simplify complex function
- Extract composable primitives

**Complex (>8 complexity points):**
- Major refactor
- High risk
- System-wide impact
- >1 day
- Extensive testing
- Should be a new system build

**Examples:**
- Redesign entire system
- Change fundamental architecture
- Replace prescribed with emergent
- Major performance rewrite

**If complex, defer to system-building cycle instead.**

---

### 5. Create Refinement Document

Create `TASKS/PLANS/REFINE_<name>.md`:

```markdown
# Refinement: <Name>

**One-line description of what we're removing/simplifying**

---

## Selected

**Date:** [Today]
**Complexity:** Trivial | Standard | Complex
**Path:** A (trivial) | B (standard)

---

## Violation Details

**Location:**
- File: `<file path>`
- Line: <number> (if applicable)
- System: <which system>

**Principle Violated:**
- Primary: <principle name>
- Score before: __/10

**Severity:** Critical | High | Medium | Low

**Type:** Magic number | Special case | Duplication | Complexity | Undocumented | Other

---

## Current State

**What exists now:**
```
[Code snippet or description of current violation]
```

**Why this violates principles:**
-
-

**Impact:**
- Blocks: [Future work blocked]
- Cascades to: [Systems affected]
- Foundation impact: [%]

---

## Fix Approach

**Primary approach:** Delete | Simplify | Document

**If DELETE:**
- Remove: [What gets deleted]
- Impact: [What breaks, how to fix]
- Tests: [Which tests to update/remove]

**If SIMPLIFY:**
- From: [Current complex state]
- To: [Simpler state]
- Derive: [If magic number, derive from what?]
- Generalize: [If special case, what's the general rule?]

**If DOCUMENT:**
- Why we must keep it: [Justification]
- Derivation: [Mathematical/logical basis]
- Trade-off: [What we're accepting]

---

## Success Criteria

**This refinement succeeds when:**
- [ ] Violation removed/simplified/documented
- [ ] Principle score improved
- [ ] All tests passing
- [ ] No regressions
- [ ] Complexity reduced (if simplify)
- [ ] Completely gone (if delete)

---

## Estimated Metrics

**Before:**
- LOC: __
- Principle score: __/10
- Special cases: __
- Cyclomatic complexity: __

**After (estimated):**
- LOC: __ (-__)
- Principle score: __/10 (+__)
- Special cases: __ (-__)
- Cyclomatic complexity: __ (-__)

---

## Risk Assessment

**What could go wrong:**
-
-

**Mitigation:**
-
-

**Rollback plan:**
[How to revert if this breaks things]
```

---

### 6. Determine Path

**Based on complexity:**

**Path A (Trivial):**
- Skip PLAN step
- Go directly to REFINE
- Minimal documentation
- Fast cycle

**Path B (Standard):**
- Full workflow
- Plan → Review → Execute → Measure
- Complete documentation
- Careful validation

**Path C (Complex):**
- Don't refine, create new system
- Use system-building cycle instead
- Full graybox → iterate → validate flow

---

### 7. Update Current Plan

Update `TASKS/CURRENT_PLAN.md` with high-level tracking:

```markdown
# Current Plan

**Status:** 🔧 Refinement - <Name>
**Started:** [Today]
**Branch:** refine/<name>
**Path:** A (trivial) | B (standard)
**Phase:** SELECT → [REFINE/PLAN]

---

## Current Work

**Refinement:** <one-line description>

**Location:** @TASKS/PLANS/REFINE_<name>.md

**Principle:** <which principle>

**Approach:** Delete | Simplify | Document

**Next Step:** [REFINE | PLAN]

**Target Impact:**
- Layer X: __% → __% (+__%)
- Overall: __% → __% (+__%)

---
```

**CRITICAL:** The `@TASKS/PLANS/REFINE_<name>.md` reference in Location ensures the detailed plan is ALWAYS in context when CURRENT_PLAN is loaded by CLAUDE.md.

**This file is imported by CLAUDE.md** - updating it injects current context automatically.

---

### 8. Create Branch & Commit

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

- [ ] Violation selected from backlog
- [ ] `TASKS/PLANS/REFINE_<name>.md` created
- [ ] Complexity classified (Trivial | Standard | Complex)
- [ ] Path determined (A | B | System build)
- [ ] `TASKS/CURRENT_PLAN.md` updated (injects context via CLAUDE.md)
- [ ] Branch created with initial commit

---

## Exit Criteria

**Proceed to REFINE when:**
- Path A (Trivial) selected
- Clear fix approach
- Low risk

**Proceed to PLAN when:**
- Path B (Standard) selected
- Needs careful thought
- Moderate risk

**Defer to system-building when:**
- Path C (Complex) identified
- Major refactor needed
- Should be treated as new system

---

## Next Step

**→ REFINE** (if Path A - Trivial)
**→ REFINE/PLAN** (if Path B - Standard)
**→ SYSTEM/SELECT** (if Path C - Complex, create new system)

---

## Selection Philosophy

### Delete > Simplify > Document

**Preference order matters:**

1. **Delete entirely** (best)
   - Removes complexity
   - No maintenance burden
   - Can't violate principles if doesn't exist

2. **Simplify to general rule** (good)
   - Reduces complexity
   - Enables composition
   - Restores principles

3. **Document justification** (last resort)
   - Acknowledges violation
   - Explains trade-off
   - Prevents future confusion

**Most refinements should DELETE code.**

### Not All Violations Equal

**Critical in core** > **Low in leaf**

Priority factors:
- Severity
- Foundation layer
- Dependency cascade
- Fix difficulty

**Fix what matters most, not what's easiest.**

### Complexity Determines Path

**Trivial** → Quick fix, minimal docs
**Standard** → Careful process, full docs
**Complex** → Wrong tool, use system build

**Match rigor to risk.**

### Between Features, Not During

**Refine when:**
- Between system builds
- Foundation stable
- No active feature work

**Don't refine during:**
- Active system building
- Foundation unstable
- Critical bug fixing

**Refinement requires calm, not crisis.**

---

**Choose by impact. Classify by risk. Delete when possible.**
