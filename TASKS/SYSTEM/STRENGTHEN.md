# STRENGTHEN - Close the System Cycle

**Archive the system. Update foundation. Enable next layer.**

---

## Purpose

Update backlogs to current state. Archive detailed metrics in plan file. Merge to main.

**Learning captured in VALIDATE/EMERGENCE. This updates tracking.**

---

## Prerequisites

- [ ] System APPROVED in VALIDATE
- [ ] All tests passing
- [ ] `TASKS/PLANS/<name>_ITERATION_<N>.md` fully documented
- [ ] All changes committed to system branch

---

## Process

### 1. Update Dependency Stack

**In `TASKS/DEPENDENCY_STACK.md`:**

Update header:
```markdown
**Overall Foundation:** NEW_%
**Last Updated:** YYYY-MM-DD
```

Update layer diagram and cascade:
```markdown
Layer [N]: [NAME]         [NEW_%]

**Cascade:** 0.XX × 0.XX × 0.XX = **NEW_% survival**
```

Update affected layer table:
```markdown
| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| <name> | NEW_% | ✅ Complete | `src/path/to/system` |
```

**Calculation:**
```markdown
## Layer [N]: [NAME]

**Certainty:** NEW_%
**Last Updated:** YYYY-MM-DD

**Systems at this layer:**
- [existing system 1]
- [existing system 2]
- **<name>** (principle avg: X.X/10 = XX%)

**Layer calculation:**
Average all systems: ([sys1]% + [sys2]% + [new]%) / count = NEW_%
```

Update next actions:
```markdown
## Next Actions

**To reach 95% Layer [N+1]:**
1. [Next system needed] (dependency ready: yes/no)

**Systems now buildable:** [list]
```

---

### 2. Update System Backlog

**In `TASKS/BACKLOG_SYSTEMS.md`:**

Update header:
```markdown
**Foundation:** NEW_%
**Last Updated:** YYYY-MM-DD
```

Move completed system:
```markdown
## Recently Completed

- **<name>**: [description]
  - Completed: YYYY-MM-DD
  - Layer: [N]
  - Score: X.X/10
  - Enables: [systems now buildable]
```

Update "Buildable Now" sections:

For each layer where foundation now ≥90%, move systems from "Waiting" to "Buildable Now":
```markdown
## Layer [N]: [NAME]

**Buildable Now:**
- **[System]**: [description] ← NOW BUILDABLE (dependency <name> complete)
  - Depends on: [list including <name>]
  - Certainty needed: XX% (current: YY%)
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

## Quick Reference

**Foundation:** NEW_%
**Target:** 95% (Layer 3)

**Layer Status:**
- Layer 1 (Core): __% - Status
- Layer 2 (Primitives): 100% ✅ - Complete
- Layer 3 (Systems): NEW_% - [status]
- Layer 4 (Variation): __% - [status]

**Next Priority:** [Next system or refinement]

---

**See:**
- `BACKLOG_SYSTEMS.md` - Systems to build
- `BACKLOG_REFINEMENTS.md` - Violations backlog
- `DEPENDENCY_STACK.md` - Foundation status
```

---

### 4. Create Final Commit

```bash
git add TASKS/DEPENDENCY_STACK.md TASKS/BACKLOG_SYSTEMS.md TASKS/CURRENT_PLAN.md TASKS/PLANS/<name>_SYSTEM.md TASKS/PLANS/<name>_ITERATION_*.md

git commit -m "strengthen: complete <name> system

Foundation: OLD_% → NEW_% (+X.X%)
Layer [N]: OLD_% → NEW_% (+X.X%)
Score: X.X/10

Enables: [systems now buildable]

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

Score: X.X/10
Foundation: OLD_% → NEW_% (+X.X%)
Enables: [systems]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"

# Delete system branch
git branch -d system/<name>

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

# Verify system moved to completed
grep -A5 "Recently Completed" TASKS/BACKLOG_SYSTEMS.md

# Verify current plan cleared
head -10 TASKS/CURRENT_PLAN.md
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

Details live in iteration docs. Backlogs stay lean.

---

**Archive in plan files. Track current state in backlogs. Keep both truthful.**
