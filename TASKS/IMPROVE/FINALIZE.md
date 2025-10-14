# Finalize Improvement

**Prerequisites:**
- **Trivial Path:** Completed SELECT, EXECUTE; execution notes in `PLANS/IMPROVE_<name>.md`
- **Standard Path:** Completed SELECT, PLAN, REVIEW_PLAN, EXECUTE, REVIEW_CODE (approved); execution and review notes in `PLANS/IMPROVE_<name>.md`

---

### 1. Capture New Discoveries

If during execution you discovered:
- Additional issues not in backlog
- Related improvements
- Architectural concerns

**Add them as new backlog items:**

```markdown
## [Appropriate Section]

### [Subsystem]
**[Issue Name]**
- *File:* `path/to/file.cpp`
- *Issue:* [What's wrong]
- *Fix:* [Suggested improvement]
- *Discovered During:* IMPROVE_<original_name>
- *Complexity:* [X points]
- *Tags:* [appropriate tags]
```

**Note:** BACKLOG_GROOMING task handles moving completed items, finding batch opportunities, and broader maintenance.

### 2. Document Learnings

Add final section to `PLANS/IMPROVE_<name>.md`:

````markdown
---

## FINALIZATION

**Finalized:** [Date/Time]

### Changes Summary

**Files Modified:**
- `path/to/file1.cpp` - [What changed]
- `path/to/file2.h` - [What changed]

**Impact:**
[What was achieved - solved problem, removed duplication, fixed architecture violation, etc.]

### New Items Discovered

**Added to Backlog:**
- [Item name] (added to backlog)
- [Another item] (added to backlog)

### Learnings

**What Worked Well:**
- [Technique, approach, or tool that helped]
- [Another success point]

**What Was Harder Than Expected:**
- [Challenge encountered]
- [How it was overcome]

**Insights for Future Work:**
- [Pattern or principle learned]
- [Advice for similar improvements]

**Time Estimate Accuracy:**
- Estimated: [X points / Y hours]
- Actual: [A points / B hours]
- Variance: [Under/Over/Accurate]
````

### 3. State Completion

**Output to user:**

```
Improvement finalized: [Item Name]

Summary:
- [What was changed]
- [Impact/benefit]

Files modified: X
Complexity: Y points
Path: A (Trivial) | B (Standard)

New items discovered: [N]

Document: PLANS/IMPROVE_<name>.md

Ready for user review on branch improve/<name>.
```

---

## Tone & Constraints

- Concise summary of accomplishment
- Capture learnings while fresh
- Be honest about time estimates (builds calibration)
- No git operations (user handles all commits/merges)
- Celebrate completion (improvement work is valuable!)
