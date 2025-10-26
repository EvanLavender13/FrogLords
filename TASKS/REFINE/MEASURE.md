# MEASURE - Quantify the Improvement

**You know what you changed. Count it.**

---

## Purpose

Measure the specific violation you fixed. Document learning. Update foundation certainty.

---

## Prerequisites

- Read @TASKS/CURRENT_PLAN.md
- Refinement complete

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
[What changed quantitatively]
<!-- END: MEASURE/METRICS -->
```

**Negative is victory.**

---

### 3. What Violation Was Fixed?

**You know what you fixed. Count that specific thing.**

Add specific violation metrics to the MEASURE/METRICS section if quantifiable.

**Don't search for arbitrary patterns. Count what you actually fixed.**

---

### 4. Document Learning

**This is the critical output.**

**Update plan:**
```markdown
<!-- BEGIN: MEASURE/LEARNING -->
[Root cause, prevention strategy, and any patterns discovered]
<!-- END: MEASURE/LEARNING -->
```

**CRITICAL: Keep concise. Focus on actionable learning. Delete philosophical expansions.**

**Learning compounds. Document it concisely.**

---

## Outputs

- [ ] Changes quantified
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
