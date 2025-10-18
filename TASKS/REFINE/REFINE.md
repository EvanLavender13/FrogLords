# REFINE - Execute the Fix

**Delete. Simplify. Document. In that order.**

---

## Purpose

Execute planned refinement—remove violation, restore principle, reduce complexity.

---

## Prerequisites

- [ ] `TASKS/PLANS/REFINE_<name>.md` exists
- [ ] Complexity classified (Trivial or Standard)
- [ ] Branch `refine/<name>` active
- [ ] All tests passing

---

## Path A - Trivial

**For: <1 hour, obvious fix, single file, no test changes**

### 1. Execute

Delete code, simplify logic, or document justification.

**Constraints:**
- Fix violation only
- No features, no scope creep
- Minimal change

### 2. Validate

```bash
[test command]
```

All tests pass. If not: fix, revert, or update tests with justification.

### 3. Document

In `TASKS/PLANS/REFINE_<name>.md`:

```markdown
<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Change:** [deleted/simplified/documented what]
**Tests:** All passing | Updated: [list]
**Metrics:** LOC __→__ (-__) | Principle __/10→__/10 (+__)
**Result:** ✓ Violation removed
<!-- END: REFINE/COMPLETED -->
```

### 4. Commit

```bash
git add .
git commit -m "refine: <name> - <what was removed/simplified>

Removed: <LOC> lines
Restored: <principle>
Tests: All passing

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

**→ Proceed to MEASURE**

---

## Path B - Standard

**For: Requires planning, multiple files, test updates, <1 day**

### 1. Plan Steps

In `TASKS/PLANS/REFINE_<name>.md`:

```markdown
<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

### Step 1: [Name]
**Changes:** `<file>:<line>` - [from] → [to]
**Tests:** [which update]
**Validation:** Tests pass, no regressions

[Repeat for each step]

## Rollback
`git reset --hard HEAD` or [specific revert steps]
<!-- END: REFINE/PLAN -->
```

### 2. Execute Each Step

**Per step:**

A. Make change (one step only)
B. Run tests
C. Verify: passing, correct behavior, no regressions
D. Commit:

```bash
git add .
git commit -m "refine: <name> step N - <what changed>

<specific change>
Tests: Passing

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

E. Mark step complete in plan

### 3. Handle Blockers

**Blocked:** Document why, suggest alternative, commit findings, proceed to MEASURE

**Scope grew:** If >1 day, consider deferring to system build

**Breaking changes:** Document impact, update affected systems, expand tests

---

## Outputs

- [ ] Violation fixed
- [ ] All tests passing
- [ ] No regressions
- [ ] Principle score improved
- [ ] Steps committed
- [ ] Documentation updated

---

## Exit Criteria

**→ MEASURE when:**
- Fix complete
- Tests passing
- Principle score improved

**Defer if:**
- Blocker discovered
- Scope exceeds 1 day
- Breaking changes unacceptable

---

## Next Step

**→ MEASURE** (calculate before/after metrics)

---

## Core Rules

**Delete first:** Code you don't have can't have bugs, needs no maintenance, violates no principles.

**Small commits:** One change, all tests pass, independently revertable.

**No scope creep:** Fix violation only. Not features, not optimization, not nearby refactoring.

**Tests guard truth:** Green before → Green after = behavior preserved. Red after = fix or revert.

**Document learning:** Why violation appeared, how to prevent recurrence, patterns to watch.

---

**Delete first. Simplify second. Document last. Test always.**
