# CLOSE - Archive the System

**Update state. Merge work. Close the loop.**

---

## Purpose

Update backlogs to current state. Archive detailed metrics in plan file. Merge to main.

**Backlogs show what's now. Plan files hold what happened.**

---

## Prerequisites

- Read @TASKS/CURRENT_PLAN.md
- `TASKS/PLANS/<name>_ITERATION_<N>.md` APPROVED

---

## Process

### 1. Update Dependency Stack

**In `TASKS/DEPENDENCY_STACK.md`:**

Update header:
```markdown
**Last Updated:** YYYY-MM-DD
```

Add system to affected layer's "Provides" section:
```markdown
## Layer [N]: [NAME]

**Provides:**

- <name> - <what it provides> (`src/path/to/system`)
```

Update what this system enables:
```markdown
**Dependencies to build something new here:**
- <name> provides <capability>
- Enables building: [list of now-buildable systems]
```

---

### 2. Update System Backlog

**In `TASKS/BACKLOG_SYSTEMS.md`:**

Update header:
```markdown
**Last Updated:** YYYY-MM-DD
```

Remove completed system from backlog (history lives in git, not lists).

Update affected layer - note systems that are now buildable:
```markdown
## Layer [N]: [NAME]

**[System]** ← NOW BUILDABLE (<name> dependency complete)
- Description
- Requires: [list including <name>]
```

---

### 3. Update Current Plan

**In `TASKS/CURRENT_PLAN.md`:**

```markdown
# Current Plan

**Status:** ✅ System Complete
**Last Completed:** YYYY-MM-DD - <name>

---

## Ready for Next Task

**Choose next action:**
- `/SYSTEM/SELECT` - Pick next system to build
- `/REFINE/SELECT` - Pick violation to fix

---

**See:**
- `BACKLOG_SYSTEMS.md` - Systems to build
- `BACKLOG_REFINEMENTS.md` - Violations backlog
- `DEPENDENCY_STACK.md` - Current capabilities
```

---

### 4. Create Final Commit

```bash
git add TASKS/DEPENDENCY_STACK.md TASKS/BACKLOG_SYSTEMS.md TASKS/CURRENT_PLAN.md TASKS/PLANS/<name>_SYSTEM.md TASKS/PLANS/<name>_ITERATION_*.md
git commit -m "close: complete <name> system

[concise description]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

### 5. Merge to Main

```bash
# Ensure on main
git checkout main

# Merge with no-fast-forward
git merge --no-ff system/<name> -m "Merge system/<name>

[concise description]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"

# Delete system branch
git branch -d system/<name>

# Verify
git log --oneline -3
```

---

## Outputs

- [ ] DEPENDENCY_STACK.md updated (current state only)
- [ ] BACKLOG_SYSTEMS.md updated (completed + buildable)
- [ ] CURRENT_PLAN.md updated
- [ ] Final commit created
- [ ] Merged to main
- [ ] Branch deleted
- [ ] System state verified

---

## Exit Criteria

**Complete when:**
- Backlogs show current state (not history)
- Stack shows what's buildable now (not past)
- Detailed metrics archived in plan files
- Changes merged to main
- Ready to select next task

**→ Next: SYSTEM/SELECT or REFINE/SELECT**

---

## Core Rule

**Backlogs are decision tools, not archives.**

Show what's buildable now. Show what's next. Point to plan files for history.

Details live in `PLANS/<name>_SYSTEM.md` and iteration docs. Backlogs stay lean.

---

**Archive in plan files. Track current state in backlogs. Keep both truthful.**
