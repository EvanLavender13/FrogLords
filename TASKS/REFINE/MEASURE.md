# MEASURE - Quantify the Improvement

**What gets measured gets improved. Document the reduction.**

---

## Purpose

Calculate objective before/after metrics to quantify the refinement's impact and update foundation certainty.

**Subjective "it's better" isn't enough. Measure the improvement.**

---

## Prerequisites

- [ ] Refinement complete
- [ ] All tests passing
- [ ] `TASKS/PLANS/REFINE_<name>.md` exists
- [ ] Changes committed

---

## Process

### 1. Calculate Code Metrics

**Lines of Code (LOC):**
```bash
# Measure specific files changed
# Before (use git to check old version):
git show HEAD~N:<file> | wc -l

# After:
wc -l <file>

# Or for whole system:
find src -name "*.ts" | xargs wc -l
```

**In** `TASKS/PLANS/REFINE_<name>.md`:
```markdown
## Metrics: Lines of Code

**Files affected:**
- `src/system/example.ts`: 450 → 280 (-170 lines)
- `src/utils/helper.ts`: 120 → 0 (-120 lines, deleted)

**Total LOC change:** -290 lines

**Percentage reduction:** -___%
```

---

### 2. Calculate Complexity Metrics

**Cyclomatic Complexity:**
```bash
# Use your language's complexity analyzer
# TypeScript: eslint with complexity rules
# Python: radon cc
# etc.
```

**In** `TASKS/PLANS/REFINE_<name>.md`:
```markdown
## Metrics: Cyclomatic Complexity

**Before:**
- `functionName()`: complexity 15
- `otherFunction()`: complexity 8

**After:**
- `functionName()`: complexity 6
- `otherFunction()`: deleted

**Average complexity change:** __ → __ (-__)
```

---

### 3. Count Special Cases

**Search for special-case patterns:**
```bash
# Entity-specific checks
rg "if.*type.*==|instanceof" <file>

# Player-specific logic
rg "entity.*===.*player|isPlayer" <file>

# Hard-coded values
rg "entity.*===.*['\"]" <file>
```

**In** `TASKS/PLANS/REFINE_<name>.md`:
```markdown
## Metrics: Special Cases

**Before:**
- Entity-type checks: 5
- Hard-coded entity references: 3
- Special player logic: 2

**After:**
- Entity-type checks: 0 (-5)
- Hard-coded entity references: 0 (-3)
- Special player logic: 0 (-2)

**Total special cases removed:** 10
```

---

### 4. Count Magic Numbers

**Before and after:**
```bash
# Find unexplained constants
rg "\b(0\.[0-9]+|[2-9][0-9]+)\b" <file> | grep -v "// "
```

**In** `TASKS/PLANS/REFINE_<name>.md`:
```markdown
## Metrics: Magic Numbers

**Before:**
- Unexplained constants: 8
- Documented constants: 2

**After:**
- Unexplained constants: 0 (-8)
- Documented constants: 10 (+8)

**All constants now derived from:** [first principles, desired behavior, etc.]
```

---

### 5. Re-Score Principles

**For the violated principle:**

**Score 1-10 using original criteria from VALIDATE.md**

**In** `TASKS/PLANS/REFINE_<name>.md`:
```markdown
## Metrics: Principle Scores

### [Violated Principle]

**Before:** __/10
**After:** __/10
**Improvement:** +__

**Evidence of improvement:**
- [What was fixed]
- [How principle restored]
- [Remaining issues, if any]

### Other Affected Principles

**[If refinement improved other principles too]**

**[Principle Name]:**
- Before: __/10
- After: __/10
- Improvement: +__

**Overall principle alignment:**
- Average before: __/10
- Average after: __/10
- Improvement: +__
```

---

### 6. Measure Foundation Impact

**How does this affect foundation certainty?**

**In** `TASKS/PLANS/REFINE_<name>.md`:
```markdown
## Metrics: Foundation Impact

**System refined:** [name]
**Layer:** [1-5]

**Layer certainty:**
- Before: __%
- After: __%
- Change: +__%

**Cascade effect:**
- Dependent layer A: _% → __%
- Dependent layer B: _% → __%

**Overall foundation:**
- Before: __%
- After: __%
- Change: +__%

**Systems now buildable:**
[Any systems that crossed 90% threshold?]
```

---

### 7. Document Learning

**In** `TASKS/PLANS/REFINE_<name>.md`:
```markdown
## Reflection

**What we refined:**
[One-line summary]

**Why it violated principles:**
[Root cause]

**How we fixed it:**
- Deleted: [what was removed]
- Simplified: [what was made simpler]
- Documented: [what was justified]

**What we learned:**
- [Insight 1]
- [Insight 2]
- [Insight 3]

**How to prevent this violation in future:**
- [Process change]
- [Tool/automation]
- [Code review focus]
- [Developer education]

**Pattern identified:**
[If this is part of a larger pattern]

**Remaining work:**
[Related violations to fix later]

**Would we do it again?**
[Was this refinement worth the effort?]
```

---

### 8. Update Backlog

**In** `TASKS/BACKLOG_REFINEMENTS.md`:

**Move violation to "Recently Completed":**
```markdown
## Recently Completed

- **<Location>**: <Description>
  - Completed: [Date]
  - LOC removed: __
  - Principle: <name>
  - Score: __/10 → __/10 (+__)
  - Pattern: [If part of larger pattern]
  - Learning: [Key insight]
```

**If pattern identified, add to Patterns section:**
```markdown
## Patterns

**<Pattern Name>** (refined: [Date])
- Violation type: [Type]
- Occurrences: [Count remaining]
- Root cause: [Why this keeps happening]
- Prevention: [How to stop it]
- Related refinements: [List]
```

---

### 9. Update Dependency Stack

**In** `TASKS/DEPENDENCY_STACK.md`:

**Update layer certainty:**
```markdown
## Layer [N]: [NAME]

**Certainty:** [NEW_%] (was [OLD_%])
**Last Updated:** [Today]

**Recent refinements:**
- [Name]: +[%] improvement

**Principle scores:**
- [Principle]: __/10 (improved from __/10)

**Status:**
- [x] Refinement completed
- [ ] Certainty recalculated
- [ ] Cascade updated
```

**Recalculate cascade to dependent layers.**

---

### 10. Final Commit and Merge

**Commit metrics:**
```bash
git add TASKS/PLANS/REFINE_<name>.md TASKS/BACKLOG_REFINEMENTS.md TASKS/DEPENDENCY_STACK.md
git commit -m "measure: <name> refinement complete

Metrics:
- LOC: -<number> lines
- Complexity: -<number>
- Special cases: -<number>
- Magic numbers: -<number>

Principle scores:
- <principle>: <before>/10 → <after>/10 (+<delta>)

Foundation: <before>% → <after>% (+<delta>%)

Learning: <key insight>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

**Merge to main:**
```bash
git checkout main
git merge --no-ff refine/<name>
git push origin main
```

---

## Outputs

- [ ] Code metrics calculated (LOC, complexity)
- [ ] Special cases counted
- [ ] Magic numbers counted
- [ ] Principle scores updated
- [ ] Foundation impact measured
- [ ] Learning documented
- [ ] Backlog updated
- [ ] Dependency stack updated
- [ ] Changes merged to main

---

## Metrics Summary Template

**Complete this for every refinement:**

```markdown
## Final Metrics Summary

**Refinement:** <name>
**Date:** [Date]
**Time Spent:** [hours]

### Code Reduction
- LOC: __ → __ (-__)
- Files: __ → __ (-__)
- Cyclomatic complexity: __ → __ (-__)

### Violation Removal
- Special cases: __ → __ (-__)
- Magic numbers: __ → __ (-__)
- Duplications: __ → __ (-__)

### Principle Alignment
- <Principle>: __/10 → __/10 (+__)
- Average: __/10 → __/10 (+__)

### Foundation Impact
- Layer certainty: __% → __% (+__%)
- Overall: __% → __% (+__%)
- Systems enabled: [count]

### Learning
- Root cause: [Why violation existed]
- Prevention: [How to avoid]
- Pattern: [Part of larger pattern?]

### Worth It?
- Effort: [hours]
- Impact: [High/Medium/Low]
- Would repeat: [Yes/No]
```

---

## Exit Criteria

**Refinement cycle complete when:**
- All metrics calculated
- Learning documented
- Backlog updated
- Foundation strengthened
- Merged to main

---

## Next Step

**→ REFINE/SELECT** (choose next violation)
**→ SYSTEM/SELECT** (if foundation now ready for new systems)

**The cycle continues: Refine → Measure → Refine → Build → Refine...**

---

## Measurement Philosophy

### Objective Over Subjective

**"It feels better" isn't enough.**

Measure:
- Lines removed (objective)
- Complexity reduced (objective)
- Special cases eliminated (objective)
- Principle scores (scored criteria)

**Numbers don't lie. Feelings do.**

### Trends Matter More Than Absolutes

**One refinement:** Small impact
**Ten refinements:** Visible trend
**Hundred refinements:** Culture change

**Track over time:**
- Is LOC trending down?
- Are principle scores improving?
- Is foundation strengthening?
- Are violations decreasing?

### Celebrate Negative Numbers

**In most engineering:**
- Positive change = good
- Negative change = bad

**In refinement:**
- **Negative LOC = victory**
- **Negative complexity = success**
- **Negative special cases = triumph**

**-1000 lines is cause for celebration.**

### Every Refinement Teaches

**Even small refinements reveal:**
- Why violations appear
- What patterns recur
- How to prevent future issues
- What to emphasize in reviews

**Document learning. Compound wisdom.**

### Foundation Strengthens Incrementally

**No single refinement transforms foundation.**
**But 10, 20, 50 refinements compound.**

Each refinement:
- Removes one violation
- Improves one score
- Teaches one lesson
- Strengthens foundation slightly

**Compounded over time = solid foundation.**

---

**Measure objectively. Document learning. Celebrate reduction. Compound improvement.**
