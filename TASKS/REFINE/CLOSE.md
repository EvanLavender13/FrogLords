# CLOSE - Archive the Refinement

**Measurement complete. Update state. Close the loop.**

---

## Purpose

Update backlogs to current state. Archive detailed metrics in plan file. Prepare for next cycle.

**Backlogs show what's now. Plan files hold what happened.**

---

## Prerequisites

- Read @TASKS/CURRENT_PLAN.md
- MEASURE complete (metrics in plan file)

---

## Process

### 1. Update Backlog

**In `TASKS/BACKLOG_REFINEMENTS.md`:**

Update header:
```markdown
**Last Updated:** YYYY-MM-DD
```

Remove fixed violation from "Active Violations"

Add new pattern if discovered - document how to detect, understand, fix, and prevent it.

Update priority order:
```markdown
## Priority Order

1. **Next violation** ← NEXT
2. **Another violation**
```

---

### 2. Update Current Plan

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

**See:**
- `BACKLOG_REFINEMENTS.md` - Current violations
- `BACKLOG_SYSTEMS.md` - Systems to build
- `DEPENDENCY_STACK.md` - Current capabilities
```

---

### 3. Create Final Commit

```bash
git add TASKS/BACKLOG_REFINEMENTS.md TASKS/CURRENT_PLAN.md TASKS/PLANS/REFINE_<name>.md

git commit -m "close: <name> refinement

[concise description]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

### 4. Merge to Main

```bash
# Ensure on main
git checkout main

# Merge with no-fast-forward
git merge --no-ff refine/<name> -m "Merge refine/<name>

[concise description]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"

# Delete refinement branch
git branch -d refine/<name>

# Verify
git log --oneline -3
```

---

### 5. Check Retrospective Threshold

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
