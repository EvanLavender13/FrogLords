# CLOSE - Archive the Refinement

**Measurement complete. Update state. Close the loop.**

---

## Purpose

Update backlogs to current state. Archive detailed metrics in plan file. Prepare for next cycle.

**Backlogs show what's now. Plan files hold what happened.**

---

## Prerequisites

- [ ] Refinement complete
- [ ] MEASURE complete (metrics in plan file)
- [ ] `TASKS/PLANS/REFINE_<name>.md` fully documented
- [ ] All changes committed to refinement branch

---

## Process

### 1. Update Backlog

**In `TASKS/BACKLOG_REFINEMENTS.md`:**

Update header:
```markdown
**Foundation:** NEW_% → Target: 95%
**Last Updated:** YYYY-MM-DD
```

Remove or update fixed violation in "Active Violations"

Add new pattern if discovered:
```markdown
### Pattern: <Pattern Name>
- **Detection:** How to spot it
- **Root cause:** Why it happens
- **Fix:** How to resolve
- **Prevention:** How to avoid
- **Related fixes:** <System> (YYYY-MM-DD)
```

Update priority order:
```markdown
## Priority Order

**Path to 95% foundation:**

1. **Next violation** (X pts) ← NEXT
2. **Another violation** (Y pts)

**Estimated:** ~X refinements remaining
```

---

### 2. Update Dependency Stack

**In `TASKS/DEPENDENCY_STACK.md`:**

Update header, layer diagram, and cascade:
```markdown
**Overall Foundation:** NEW_%
**Last Updated:** YYYY-MM-DD

Layer 3: SYSTEMS         [NEW_%]

**Cascade:** 0.XX × 0.XX × 0.XX = **NEW_% survival**
```

Update affected layer table and status:
```markdown
| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| <System> | NEW_% | ✅ Refined | `path/to/file` |
```

Update blocking issues and next actions:
```markdown
**Blocking:**
1. ~~Fixed issue~~ ✅ COMPLETE
2. Remaining issue (priority)

## Next Actions

**To reach 95% Layer 3:**
1. Next refinement (X points)

**Estimated:** ~X refinements remaining
```

---

### 3. Update Current Plan

**In `TASKS/CURRENT_PLAN.md`:**

```markdown
# Current Plan

**Status:** ✅ Refinement Complete
**Last Completed:** YYYY-MM-DD - <Refinement Name>

---

## Ready for Next Task

**Choose next action:**
- `/REFINE/SELECT` - Pick next violation to fix
- `/SYSTEM/SELECT` - Pick next system to build

---

## Quick Reference

**Foundation:** NEW_%
**Target:** 95% (Layer 3)

**Layer Status:**
- Layer 1 (Core): __% - Status
- Layer 2 (Primitives): 100% ✅ - Complete
- Layer 3 (Systems): NEW_% - X refinements to target

**Next Priority:** <Next violation>
**Estimated Path:** ~X refinements to 95%

---

**See:**
- `BACKLOG_REFINEMENTS.md` - Current violations
- `BACKLOG_SYSTEMS.md` - Systems to build
- `DEPENDENCY_STACK.md` - Foundation status
```

---

### 4. Create Final Commit

```bash
git add TASKS/BACKLOG_REFINEMENTS.md TASKS/DEPENDENCY_STACK.md TASKS/CURRENT_PLAN.md TASKS/PLANS/REFINE_<name>.md

git commit -m "close: <name> refinement

Foundation: OLD_% → NEW_% (+__%)
Layer 3: OLD_% → NEW_% (+__%)

Next: <Next refinement or milestone>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

### 5. Merge to Main

```bash
# Ensure on main
git checkout main

# Merge with no-fast-forward
git merge --no-ff refine/<name> -m "Merge refine/<name>

<Principle> +X.X points
Foundation +__% (OLD% → NEW_%)

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"

# Delete refinement branch
git branch -d refine/<name>

# Verify
git log --oneline -3
```

---

### 6. Verify System State

```bash
# Verify main is clean
git status

# Verify foundation updated
grep "Overall Foundation" TASKS/DEPENDENCY_STACK.md

# Verify active violations updated
grep -A10 "Active Violations" TASKS/BACKLOG_REFINEMENTS.md

# Verify current plan cleared
head -10 TASKS/CURRENT_PLAN.md
```

---

### 7. Check Retrospective Threshold

```bash
# Count completed plans (REFINE_* and *_SYSTEM only - NOT *_ITERATION_*)
ls TASKS/PLANS/REFINE_*.md TASKS/PLANS/*_SYSTEM.md 2>/dev/null | wc -l
```

**If count is ≥5**, add to `CURRENT_PLAN.md`:

```markdown
## Recommended Next Action

**📊 RETRO recommended** - <N> plans completed since start
**Run:** Review TASKS/RETRO.md and extract patterns

**Or proceed with:**
- `/REFINE/SELECT` - Pick next violation to fix
- `/SYSTEM/SELECT` - Pick next system to build
```

---

## Outputs

- [ ] BACKLOG_REFINEMENTS.md updated (current state only)
- [ ] DEPENDENCY_STACK.md updated (current state only)
- [ ] CURRENT_PLAN.md updated
- [ ] Final commit created
- [ ] Merged to main
- [ ] Branch deleted
- [ ] System state verified
- [ ] Retrospective threshold checked

---

## Exit Criteria

**Complete when:**
- Backlogs show current state (not history)
- Stack shows what's possible now (not past)
- Detailed metrics archived in plan file
- Changes merged to main
- Ready to select next task

**→ Next: REFINE/SELECT or SYSTEM/SELECT**

---

## Core Rule

**Backlogs are decision tools, not archives.**

Show what's broken now. Show what's next. Point to plan files for history.

Details live in `PLANS/REFINE_<name>.md`. Backlogs stay lean.

---

**Archive in plan files. Track current state in backlogs. Keep both truthful.**
