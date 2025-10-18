# REFINE - Execute the Fix

**Delete. Simplify. Document. In that order.**

---

## Purpose

Execute the planned refinement—removing violation, restoring principle, and reducing complexity.

**Most refinements should delete code. The best fix is no code at all.**

---

## Prerequisites

- [ ] `TASKS/PLANS/REFINE_<name>.md` exists
- [ ] Complexity classified (Trivial or Standard)
- [ ] Fix approach identified
- [ ] Branch `refine/<name>` active
- [ ] All tests currently passing

---

## Process (Path A - Trivial)

**For trivial refinements (<1 hour, obvious fix):**

### 1. Make the Change

**Execute the fix:**
- Delete the code
- Simplify the logic
- Document the justification

**Keep it minimal:**
- Don't add features
- Don't refactor surrounding code
- Focus on the violation only

---

### 2. Run Tests

```bash
# Run full test suite
[your test command]
```

**All tests must pass.**

**If tests fail:**
- Fix breakage OR
- Revert change OR
- Update tests (if behavior intentionally changed)

---

### 3. Update Document

In `TASKS/PLANS/REFINE_<name>.md`:

```markdown
## Completed (Trivial Path)

**Change made:**
- Deleted: [what was removed]
- Simplified: [what was simplified]
- Documented: [what was documented]

**Tests:**
- [x] All passing
- Updated: [list if any]

**Metrics:**
- LOC before: __ → after: __ (-__)
- Principle score before: __/10 → after: __/10

**Result:** ✓ Violation removed
```

---

### 4. Commit

```bash
git add .
git commit -m "refine: <name> - <what was removed/simplified>

Removed: <LOC> lines
Restored: <principle>
Tests: All passing

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

**→ Skip to MEASURE**

---

## Process (Path B - Standard)

**For standard refinements (requires planning and validation):**

### 1. Plan the Refinement

In `TASKS/PLANS/REFINE_<name>.md`, add:

```markdown
## Refinement Plan

**Steps:**
1. [First change]
2. [Second change]
3. [Third change]

**For each step:**
- What changes
- Which files affected
- Which tests update
- Validation criteria

---

### Step 1: [Name]

**Changes:**
- File: `<path>`
- From: [current state]
- To: [new state]

**Tests to update:**
-

**Validation:**
- [ ] Tests pass
- [ ] No regressions
- [ ] Behavior same (or intentionally different)

---

[Repeat for each step]

---

## Rollback Plan

**If this breaks things:**
1. `git reset --hard HEAD`
2. Or: [specific revert steps]

**Safe because:**
- On feature branch
- Tests guard regressions
- Small, focused changes
```

---

### 2. Execute Each Step

**For each planned step:**

**A. Make the change**
```bash
# Edit files
# Keep changes minimal
# One step at a time
```

**B. Run tests**
```bash
[test command]
```

**C. Verify**
- [ ] Tests passing?
- [ ] Behavior correct?
- [ ] No regressions?

**D. Commit the step**
```bash
git add .
git commit -m "refine: <name> step N - <what changed>

<specific change>
Tests: Passing

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

**E. Update plan document**
Mark step complete, note any surprises

---

### 3. Handle Blockers

**If you discover:**

**Can't be fixed as planned:**
- Document blocker in `TASKS/PLANS/REFINE_<name>.md`
- Explain why approach won't work
- Suggest alternative or defer
- Commit documentation
- Move to MEASURE (document learnings)

**Bigger than expected:**
- If now >1 day, might be Complex
- Consider deferring to system build
- Document scope creep
- Commit what's done
- Defer rest

**Breaking changes required:**
- Document what breaks and why
- Update affected systems
- Expand test coverage
- Proceed carefully

---

### 4. Validate Principle Restoration

**After all steps complete:**

**Re-score the principle that was violated:**

In `TASKS/PLANS/REFINE_<name>.md`:
```markdown
## Principle Validation

**Principle:** <name>

**Before:**
- Score: __/10
- Violations: [list]

**After:**
- Score: __/10
- Violations: [remaining, if any]

**Improvement:** +__ points

**Evidence:**
- Removed: [what violations fixed]
- Simplified: [what complexity reduced]
- Generalized: [what special cases removed]

**Verdict:** ✓ Principle restored
```

**If score didn't improve, something went wrong.**

---

## Outputs

- [ ] Violation fixed (deleted, simplified, or documented)
- [ ] All tests passing
- [ ] No regressions introduced
- [ ] Principle score improved
- [ ] Each step committed
- [ ] Documentation updated

---

## Exit Criteria

**Proceed to MEASURE when:**
- Fix complete (all steps done)
- Tests passing
- Principle score improved
- Ready to calculate metrics

**Defer if:**
- Blocker discovered
- Scope too large
- Breaking changes unacceptable
- Better to try different approach

---

## Next Step

**→ MEASURE** (calculate before/after metrics)

---

## Refinement Philosophy

### Delete is Best

**The code you don't have:**
- Never has bugs
- Needs no maintenance
- Can't violate principles
- Costs nothing to run
- Takes no time to understand

**Most refinements should reduce LOC.**

### Small Steps, Frequent Commits

**Why:**
- Easy to revert if broken
- Clear history of changes
- Validates at each step
- Reduces risk

**Each commit should:**
- Make one clear change
- Pass all tests
- Be independently revertable

### No Feature Creep

**Refinement is not:**
- Adding features
- Optimizing performance
- Refactoring everything nearby
- "While we're here" changes

**Refinement is:**
- Removing violation
- Restoring principle
- Reducing complexity
- Nothing else

**Stay focused. Fix the violation. Stop.**

### Tests Guard Principles

**Before refactoring:**
- All tests pass → Behavior is captured

**After refactoring:**
- All tests pass → Behavior preserved

**If tests fail:**
- Intentional change → Update tests + document
- Unintentional → Fix code or revert

**No green tests = no merge.**

### Document Learning

**Every refinement teaches:**
- Why violation appeared
- How to prevent recurrence
- What patterns to watch for
- What principles to emphasize

**Capture lessons for future audits.**

---

**Delete first. Simplify second. Document last. Test always.**
