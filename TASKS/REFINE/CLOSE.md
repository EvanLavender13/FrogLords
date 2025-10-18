# CLOSE - Archive the Refinement

**Measurement complete. Now update the system state and close the loop.**

---

## Purpose

Update backlog, dependency stack, and system records to reflect the completed refinement. Archive the work and prepare for the next cycle.

**Metrics without recording are forgotten. Close the loop.**

---

## Prerequisites

- [ ] Refinement complete
- [ ] MEASURE complete (metrics calculated)
- [ ] `TASKS/PLANS/REFINE_<name>.md` fully documented
- [ ] All changes committed to refinement branch

---

## Process

### 1. Update Backlog

**Move violation from active to completed:**

In `TASKS/BACKLOG_REFINEMENTS.md`:

**Remove from active section:**
- Delete or comment out the violation entry from its principle section
- Remove from "By Severity" section
- Update "In Progress" section

**Add to "Recently Completed":**
```markdown
## Recently Completed

**YYYY-MM-DD: <Refinement Name>**
- Location: `<file/system>`
- Completed: YYYY-MM-DD
- Principle: <Principle Restored>
- Score: __/10 → __/10 (+__)
- Pattern: <Pattern name if identified>
- Learning: <Key insight>

**Metrics:**
- LOC removed: __ (or +__ if additive)
- Special cases: __ → __ (-__)
- Magic numbers: __ → __ (-__)
- [Other relevant metrics]

**Foundation impact:**
- Layer X: __% → __% (+__%)
- Overall: __% → __% (+__%)

**Test suites:** [If applicable]
[Description of tests added]

**All tests: X/X passing ✓**

**Reusable artifacts:** [If applicable]
[Templates, patterns, tools created]

**ROI:** [Exceptional/High/Medium/Low]
- Effort: [hours]
- Impact: [description]
- Risk: [Zero/Low/Medium]

---
```

**Update patterns section if new pattern discovered:**
```markdown
## Patterns

**<Pattern Name>** (discovered: YYYY-MM-DD)
- Violation type: [Type]
- Root cause: [Why this happens]
- Solution: [How to fix]
- Prevention: [How to avoid]
- Related refinements: [List]
```

**Update priority order:**
- Remove completed item
- Renumber remaining items
- Update progress metrics

---

### 2. Update Dependency Stack

**In `TASKS/DEPENDENCY_STACK.md`:**

**Update header:**
```markdown
**Last Updated:** YYYY-MM-DD (<refinement name> complete)
```

**Update Current State diagram:**
```markdown
Layer X: [NAME]         [NEW_%] ← updated certainty
```

**Update Overall Foundation Certainty:**
```markdown
**Overall Foundation Certainty:** NEW_%
```

**Update cascade calculation:**
```markdown
**Current cascade:**
- Layer 1: __%
- Layer 2: __%
- Layer 3: __%
- **Overall:** X.XX × X.XX × X.XX = **__% survival chance**
```

**Update affected layer:**
```markdown
## Layer X: NAME

**Certainty:** NEW_%
**Status:** [Updated status]
**Last Changed:** YYYY-MM-DD (<refinement description>)

### Systems

**<System Name> [NEW_%]:**
- [Updated description if system changed]
- Location: `<path>`
- [Add test info if tests added]
- Status: ✓ [Updated status]
```

**Update completion checklist:**
```markdown
### Completion Checklist
- [x] ✅ <Item now complete>
- [Updated checkmarks]

**Blocking issues:** [Updated list or "None" if complete]
```

**Update certainty scores:**
```markdown
**Layer X (<Name>): NEW_%**
- <System>: __/10 = __% (updated score)
- **Average: __ = __%**
```

**Add to History > Refinements Completed:**
```markdown
**YYYY-MM-DD: <Refinement Name>**
- Layer: X (<Name>)
- System: <System Name>
- Before: __% → After: __% (+__%)
- Layer impact: __% → __% (+__%)
- [Cascade impact if applicable]
- Overall foundation: __% → __% (+__%)
- Violation: <Description>
- Fix: <What was done>
- [Test info if applicable]
- Learning: <Key insight>
- Pattern: <Pattern name if identified>
- [Impact statement if major milestone]
```

**Update cascade history:**
```markdown
**After <refinement>:** X% × Y% × Z% = **__% ✓** (<milestone if reached>)
```

**Update repair priority if applicable:**
```markdown
1. ✅ ~~**<Refinement name>**~~ (COMPLETED YYYY-MM-DD)
[Renumber remaining items]
```

**Update patterns/notes if applicable:**
```markdown
**Patterns observed:**
- **<New pattern>** (<new learning>)
```

**Update Current Status Summary:**
```markdown
**Overall Foundation:** NEW_%

**Layer Status:**
- Layer X: NEW_% - [Updated description]

**Next Action:** <Next refinement or milestone>

**Progress:** OLD_% → NEW_% (+__%), [remaining work]
```

---

### 3. Groom Completed Items

**In `TASKS/PLANS/REFINE_<name>.md`:**

Verify document is complete:
- [ ] All metrics sections filled
- [ ] Reflection section complete
- [ ] Learning documented
- [ ] Pattern identified (if applicable)
- [ ] Success criteria all checked
- [ ] "Would we do it again?" answered

**No changes needed here** - MEASURE already completed this.

---

### 4. Create Final Commit

**Commit the bookkeeping updates:**

```bash
git add TASKS/BACKLOG_REFINEMENTS.md TASKS/DEPENDENCY_STACK.md TASKS/CURRENT_PLAN.md

git commit -m "measure: <name> refinement complete

Metrics:
- LOC: [change]
- [Key metrics]

Principle scores:
- <principle>: __/10 → __/10 (+__)

Foundation: __% → __% (+__%)

Learning: <key insight>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

### 5. Merge to Main

**Merge refinement branch:**

```bash
# Ensure on main
git checkout main

# Merge with no-fast-forward (preserve history)
git merge --no-ff refine/<name> -m "Merge branch 'refine/<name>'

<Brief summary of refinement>

Metrics:
- [Key metric 1]
- [Key metric 2]

Foundation: __% → __% (+__%)

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

**Delete refinement branch:**
```bash
git branch -d refine/<name>
```

**Verify merge:**
```bash
git log --oneline -3
```

---

### 6. Clear Current Plan

**Update `TASKS/CURRENT_PLAN.md`:**

**Refinement complete - clear the plan:**

```markdown
# Current Plan

**Status:** No active work
**Last Completed:** YYYY-MM-DD (<refinement name>)

---

**Ready to select next refinement or system.**

See:
- `TASKS/BACKLOG_REFINEMENTS.md` for violations to fix
- `TASKS/BACKLOG_SYSTEMS.md` for systems to build
- `TASKS/DEPENDENCY_STACK.md` for foundation status

---

## Quick Reference

**Foundation:** __% (updated YYYY-MM-DD)

**Layer Status:**
- Layer 1: __%
- Layer 2: __%
- Layer 3: __%

**Next Action:** Run REFINE/SELECT or SYSTEM/SELECT

**Path to 95%:** ~X more refinements
```

**Commit the cleared plan:**
```bash
git add TASKS/CURRENT_PLAN.md
# Include in the final bookkeeping commit
```

---

### 7. Verify System State

**Quick verification:**

```bash
# Verify main is clean
git status

# Verify foundation certainty updated
grep "Overall Foundation" TASKS/DEPENDENCY_STACK.md

# Verify refinement recorded
grep -A5 "Recently Completed" TASKS/BACKLOG_REFINEMENTS.md | head -20

# Verify priority order updated
grep -A10 "Priority Order" TASKS/BACKLOG_REFINEMENTS.md

# Verify current plan cleared
head -5 TASKS/CURRENT_PLAN.md
```

**All should reflect the completed refinement.**

---

## Outputs

- [ ] BACKLOG_REFINEMENTS.md updated
  - [ ] Violation moved to "Recently Completed"
  - [ ] Pattern documented (if applicable)
  - [ ] Priority order updated
- [ ] DEPENDENCY_STACK.md updated
  - [ ] Layer certainties recalculated
  - [ ] Cascade updated
  - [ ] History recorded
- [ ] CURRENT_PLAN.md cleared
  - [ ] Status set to "No active work"
  - [ ] Last completed recorded
  - [ ] Foundation status updated
- [ ] Final commit created
- [ ] Merged to main
- [ ] Branch deleted
- [ ] System state verified

---

## Exit Criteria

**Refinement cycle complete when:**
- System state reflects completion
- Backlog and stack updated
- Changes merged to main
- Learning archived
- Ready to select next refinement

**→ Next: REFINE/SELECT (choose next violation)**

---

## Closure Philosophy

### Archive for the Future

**Today's learning is tomorrow's wisdom.**

Record not just what was fixed, but:
- Why it was a violation
- How it was fixed
- What was learned
- How to prevent it

**Future you will thank present you for thorough documentation.**

### Celebrate Reduction

**Every refinement is a victory:**
- Negative LOC = progress
- Increased certainty = strength
- Patterns identified = knowledge
- Tests added = confidence

**Each small win compounds.**

### Maintain the Chain

**Broken documentation = broken process.**

The backlog and stack are not bureaucracy—they are:
- Foundation health monitors
- Progress trackers
- Learning repositories
- Decision tools

**Keep them current. Keep them truthful.**

### Close the Loop

**The refinement isn't complete until it's recorded.**

Without closure:
- Progress is invisible
- Learning is lost
- Patterns go unnoticed
- Foundation drift goes untracked

**Measure, record, close. Then repeat.**

---

**Record truthfully. Archive thoroughly. Close completely.**
