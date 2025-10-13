# Finalize Improvement

**Prerequisites:**
- **Trivial Path:** Completed SELECT, EXECUTE; execution notes in `PLANS/IMPROVE_<name>.md`
- **Standard Path:** Completed SELECT, PLAN, REVIEW_PLAN, EXECUTE, REVIEW_CODE (approved); execution and review notes in `PLANS/IMPROVE_<name>.md`

---

### 1. Update Improve Backlog

Open `PLANS/IMPROVE_BACKLOG.md`:

#### 1.1. Move Item to Completed

Find the item under its current section (Critical/High/Medium/Low) and:
1. Cut the entire item entry
2. Paste into the **Completed** section at the bottom
3. Add completion date and brief note

```markdown
## Completed

### [Original Section] / [Subsystem]
**[Item Name]** ✓
- *Completed:* October 13, 2025
- *Files:* `path/to/file.cpp`
- *Fix:* [Brief summary of what was done]
- *Learning:* [Key insight, if any]
- *Document:* `PLANS/IMPROVE_<name>.md`
```

#### 1.2. Check for Similar Items

Scan backlog for related items that:
- Affect same files or systems
- Use similar patterns or techniques
- Could be batched in future

**If 2+ similar items found:**
- Add note to backlog: "Batch opportunity: [Item A, Item B, Item C] (similar fix pattern)"
- Tag items with `#batch-candidate`

#### 1.3. Note New Discoveries

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

### Backlog Updates

**Item Status:** Moved to Completed in IMPROVE_BACKLOG.md

**New Items Discovered:**
- [Item name] (added to backlog)
- [Another item] (added to backlog)

**Batch Opportunities Noted:**
- [Related items that could be batched]

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

Backlog updated:
- Item moved to Completed
- [N new items added]
- [Batch opportunities noted]

Document: PLANS/IMPROVE_<name>.md

Ready for user to review and commit on branch improve/<name>.
```

---

## Tone & Constraints

- Concise summary of accomplishment
- Capture learnings while fresh
- Be honest about time estimates (builds calibration)
- Note batch opportunities (reduces future overhead)
- No git operations (user handles all git workflow)
- Celebrate completion (improvement work is valuable!)
