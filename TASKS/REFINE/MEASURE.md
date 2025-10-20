# MEASURE - Quantify the Improvement

**You know what you changed. Count it.**

---

## Purpose

Measure the specific violation you fixed. Document learning. Update foundation certainty.

---

## Prerequisites

- [ ] Refinement complete
- [ ] Tests passing
- [ ] Changes committed

---

## Process

### 1. What Changed?

```bash
# Files modified
git diff --stat HEAD~1

# Lines added/removed
git diff --shortstat HEAD~1

# Full diff
git diff HEAD~1
```

**You know which files you touched. Count those.**

---

### 2. How Much Was Removed?

**For each file changed:**

```bash
# Before
git show HEAD~1:<file> | wc -l

# After
wc -l <file>
```

**Update** `TASKS/PLANS/REFINE_<name>.md`:
```markdown
<!-- BEGIN: MEASURE/METRICS -->
## Metrics

**Files:**
- `<file>`: ___ → ___ (-___ lines)

**Total:** -___ lines
<!-- END: MEASURE/METRICS -->
```

**Negative is victory.**

---

### 3. What Violation Was Fixed?

**You know what you fixed. Count that specific thing.**

**Examples:**

If you **removed magic numbers**, count them:
```bash
# Grep for the pattern YOU removed
git show HEAD~1:<file> | grep -c "<pattern>"  # Before
grep -c "<pattern>" <file>                     # After
```

If you **deleted special cases**, count them:
```bash
# Count the specific checks YOU removed
git diff HEAD~1 <file> | grep "^-" | grep -c "<check>"
```

If you **extracted duplicated logic**, count instances:
```bash
# How many times did this pattern appear?
git show HEAD~1:<file> | grep -c "<duplicated_code>"  # Before
grep -c "<duplicated_code>" <file>                     # After
```

**Update plan** (add to MEASURE/METRICS section):
```markdown
**Violations removed:**
- <specific_violation>: ___ → ___ (-___)
```

**Don't search for arbitrary patterns. Count what you actually fixed.**

---

### 4. Document Learning

**This is the critical output.**

**Update plan:**
```markdown
<!-- BEGIN: MEASURE/LEARNING -->
## Learning

**Root cause:** [Why this violation existed - 1 line]

**Prevention:** [How to avoid creating this again - 1-2 lines]

**Pattern:** [Is this part of a larger problem? - 1 line, optional]

**Remaining work:** [Related violations to fix later - 1 line, optional]
<!-- END: MEASURE/LEARNING -->
```

**CRITICAL: Keep under 10 lines total. Root cause + prevention only. Delete philosophical expansions.**

**Learning compounds. Document it concisely.**

---

## Outputs

- [ ] Lines removed counted
- [ ] Specific violation counted
- [ ] Learning documented
- [ ] `TASKS/PLANS/REFINE_<name>.md` complete

---

## Exit Criteria

**Complete when:**
- All metrics in plan
- Learning documented

**Incomplete measurement = wasted refinement.**

---

## Next Step

**→ REFINE/CLOSE** (update CURRENT_PLAN, merge to main)

---

**Measure what you changed. Document what you learned.**
