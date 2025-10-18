# CLOSE - Archive the Refinement

**Measurement complete. Now update the system state and close the loop.**

---

## Purpose

Update backlog and stack to reflect current state. Archive detailed metrics in plan file. Prepare for next cycle.

**Keep backlogs lean. Detailed history lives in PLANS/REFINE_*.md.**

---

## Prerequisites

- [ ] Refinement complete
- [ ] MEASURE complete (metrics in plan file)
- [ ] `TASKS/PLANS/REFINE_<name>.md` fully documented
- [ ] All changes committed to refinement branch

---

## Process

### 1. Update Backlog (Keep Lean!)

**In `TASKS/BACKLOG_REFINEMENTS.md`:**

**Update header:**
```markdown
**Foundation:** NEW_% → Target: 95%
**Last Updated:** YYYY-MM-DD
```

**Remove fixed violation from "Active Violations":**
- If violation is completely resolved, delete the entry
- If partially fixed, update description and severity

**Update pattern library if new pattern discovered:**
```markdown
### Pattern: <Pattern Name>
- **Detection:** How to spot it
- **Root cause:** Why it happens
- **Fix:** How to resolve
- **Prevention:** How to avoid
- **Related fixes:** <System> (YYYY-MM-DD)
```

**Update priority order:**
```markdown
## Priority Order

**Path to 95% foundation:**

1. **Next violation** (X pts) ← NEXT
2. **Another violation** (Y pts)

**Estimated:** ~X refinements remaining
```

**Add brief pointer in "Recent Activity":**
```markdown
## Recent Activity

**See individual plan files for detailed metrics:**
- `PLANS/REFINE_<name>.md` (YYYY-MM-DD) - <Principle> +X.X
- [Keep only last 3-5 entries]
```

**That's it!** No massive history sections. Details live in plan files.

---

### 2. Update Dependency Stack (Keep Current!)

**In `TASKS/DEPENDENCY_STACK.md`:**

**Update header:**
```markdown
**Overall Foundation:** NEW_%
**Last Updated:** YYYY-MM-DD
```

**Update layer diagram:**
```markdown
Layer 3: SYSTEMS         [NEW_%] ← updated certainty
```

**Update cascade:**
```markdown
**Cascade:** 0.XX × 0.XX × 0.XX = **NEW_% survival**
```

**Update affected layer table:**
```markdown
## Layer 3: SYSTEMS (NEW_%)

| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| <System> | NEW_% | ✅ Refined | `path/to/file` |
```

**Update blocking issues:**
```markdown
**Blocking:**
1. ~~Fixed issue~~ ✅ COMPLETE
2. Remaining issue (priority)
```

**Update "Next Actions":**
```markdown
## Next Actions

**To reach 95% Layer 3:**
1. Next refinement (X points)

**Estimated:** ~X refinements remaining
```

**Add brief pointer in "Recent Changes":**
```markdown
## Recent Changes

- YYYY-MM-DD: <System> → NEW_% (<brief description>)
- [Keep only last 3-5 entries]

**See:** `PLANS/REFINE_*.md` for detailed metrics
```

**That's it!** Stack shows current state, not history.

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
- `BACKLOG_REFINEMENTS.md` - Current violations to fix
- `BACKLOG_SYSTEMS.md` - Systems to build
- `DEPENDENCY_STACK.md` - Foundation status and build rules
```

---

### 4. Create Final Commit

**Commit the bookkeeping updates:**

```bash
git add TASKS/BACKLOG_REFINEMENTS.md TASKS/DEPENDENCY_STACK.md TASKS/CURRENT_PLAN.md

git commit -m "close: <name> refinement - backlog and stack updated

Foundation: OLD_% → NEW_% (+__%)
Layer 3: OLD_% → NEW_% (+__%)

Next: <Next refinement or milestone>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

**Keep commit message lean - details are in plan file.**

---

### 5. Merge to Main

**Merge refinement branch:**

```bash
# Ensure on main
git checkout main

# Merge with no-fast-forward (preserve history)
git merge --no-ff refine/<name> -m "Merge refine/<name> - <brief summary>

<Principle> +X.X points
Foundation +__% (OLD% → NEW_%)

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

### 6. Verify System State

**Quick verification:**

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

**All should reflect current state (not detailed history).**

---

### 7. Check Retrospective Threshold

**Check if retro is recommended:**

```bash
# Count completed plans
PLAN_COUNT=$(ls TASKS/PLANS/REFINE_*.md TASKS/PLANS/*_SYSTEM.md 2>/dev/null | wc -l)

# Check if retro threshold reached (every 5 plans)
if [ $((PLAN_COUNT % 5)) -eq 0 ] && [ $PLAN_COUNT -gt 0 ]; then
  echo "📊 Retrospective threshold reached ($PLAN_COUNT plans)"
  echo "Consider running RETRO before next cycle"
fi
```

**If threshold reached, update CURRENT_PLAN.md to recommend retro:**

Add after the "Ready for Next Task" section:

```markdown
## Recommended Next Action

**📊 RETRO recommended** - <N> plans completed since start
**Run:** Review TASKS/RETRO.md and extract patterns

**Or proceed with:**
- `/REFINE/SELECT` - Pick next violation to fix
- `/SYSTEM/SELECT` - Pick next system to build
```

**This is a recommendation, not a requirement. Developer decides when to run.**

---

## Outputs

- [ ] BACKLOG_REFINEMENTS.md updated
  - [ ] Header foundation % updated
  - [ ] Active violations updated (removed or modified)
  - [ ] Pattern library updated (if new pattern)
  - [ ] Priority order updated
  - [ ] Recent activity pointer added (brief!)
- [ ] DEPENDENCY_STACK.md updated
  - [ ] Header foundation % updated
  - [ ] Layer diagram updated
  - [ ] Cascade calculation updated
  - [ ] Layer table updated
  - [ ] Blocking issues updated
  - [ ] Recent changes pointer added (brief!)
- [ ] CURRENT_PLAN.md updated
  - [ ] Status set to "Refinement Complete"
  - [ ] Last completed recorded
  - [ ] Quick reference updated
- [ ] Final commit created (lean message)
- [ ] Merged to main
- [ ] Branch deleted
- [ ] System state verified
- [ ] Retrospective threshold checked (if ≥5 plans, recommend in CURRENT_PLAN.md)

---

## Exit Criteria

**Refinement cycle complete when:**
- Backlogs show **current state** (not history)
- Stack shows **what's possible now** (not past)
- Detailed metrics archived in `PLANS/REFINE_<name>.md`
- Changes merged to main
- Ready to select next task

**→ Next: REFINE/SELECT or SYSTEM/SELECT**

---

## Closure Philosophy

### Detailed History Lives in Plan Files

**The plan file (`PLANS/REFINE_<name>.md`) contains:**
- Complete metrics (LOC, principle scores, etc.)
- Full reflection and learning
- Detailed "would we do it again?" analysis
- Pattern identification
- ROI calculation

**The backlogs/stack contain:**
- Current violations (active, not completed)
- Current certainties (now, not history)
- Current blocking issues (what prevents progress)
- Next actions (what to do)
- Brief pointers to plan files (for history)

### Keep Backlogs Action-Oriented

**Backlogs are decision tools, not archives.**

When someone opens BACKLOG_REFINEMENTS.md, they should instantly see:
- What's broken right now?
- What should I fix next?
- What patterns should I watch for?

NOT:
- What was fixed 3 months ago?
- How many LOC were removed?
- What was the ROI on past work?

### Keep Stack Current-Focused

**Stack shows what's possible NOW.**

When someone opens DEPENDENCY_STACK.md, they should instantly see:
- What's the foundation certainty?
- What can I build?
- What's blocking progress?
- What's next?

NOT:
- Detailed cascade history
- Past refinement metrics
- Long "how we got here" narratives

### Celebrate in the Right Place

**Detailed metrics = plan files**
**Current state = backlogs/stack**
**Principles/patterns = PRINCIPLES.md and backlog pattern library**

Each document has a purpose. Respect it.

### Trust the Plan Files

**Every refinement has a PLANS/REFINE_<name>.md file.**

That's where the story lives. That's where the learning is archived. That's where future you goes to understand what happened and why.

The backlogs just point to it.

---

**Archive in plan files. Track current state in backlogs. Keep both truthful.**
