# Finalize Improvement

**Prerequisites:**
- **Trivial Path:** Completed SELECT, EXECUTE; execution notes in `PLANS/IMPROVE_<name>.md`
- **Standard Path:** Completed SELECT, PLAN, REVIEW_PLAN, EXECUTE, REVIEW_CODE (approved); execution and review notes in `PLANS/IMPROVE_<name>.md`

---

### 1. Final Validation

**Quick sanity checks:**

```powershell
# Clean build
cmake --build build/ninja --clean-first

# Format check
.\scripts\run_clang_format.ps1

# Lint check
.\scripts\run_clang_tidy.ps1
```

Verify all pass with no errors or warnings.

### 2. Review Complete Change

**Read git diff:**
```powershell
git diff
# or
git diff main
```

Verify:
- [ ] Only intended files changed
- [ ] Changes match plan/description
- [ ] No debug code, commented blocks, or temp changes
- [ ] No unintended whitespace or formatting changes (beyond clang-format)

### 3. Update Improve Backlog

Open `PLANS/IMPROVE_BACKLOG.md`:

#### 3.1. Move Item to Completed

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

#### 3.2. Check for Similar Items

Scan backlog for related items that:
- Affect same files or systems
- Use similar patterns or techniques
- Could be batched in future

**If 2+ similar items found:**
- Add note to backlog: "Batch opportunity: [Item A, Item B, Item C] (similar fix pattern)"
- Tag items with `#batch-candidate`

#### 3.3. Note New Discoveries

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

### 4. Document Learnings

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

### Final Validation

- [x] Clean build passes
- [x] clang-format clean
- [x] clang-tidy clean
- [x] No new warnings
- [x] Git diff reviewed
- [x] Backlog updated
- [x] Learnings captured
````

### 5. Prepare Commit Message

Create commit message following project conventions (see `TASKS/COMMIT.md` for full guidance):

**Format:**
```
[Improve] <Brief description of change>

<Optional: More detailed explanation if needed>

<Optional: Rationale or context>

Files:
- path/to/file1.cpp
- path/to/file2.h

Fixes: Issue described in IMPROVE_BACKLOG.md
Doc: PLANS/IMPROVE_<name>.md
```

**Examples:**

```
[Improve] Remove unused stdio includes from input and controller

Cleanup unused <cstdio> includes left over from debugging.

Files:
- src/input/input.cpp
- src/character/controller.cpp

Doc: PLANS/IMPROVE_unused_includes.md
```

```
[Improve] Decouple debug_draw from app/game_world

Extract velocity_trail_state to rendering/velocity_trail.h to fix
architectural violation (Rendering must not depend on App).

Files:
- src/rendering/velocity_trail.h (new)
- src/rendering/debug_draw.cpp
- src/app/game_world.h

Fixes: Critical architectural violation in IMPROVE_BACKLOG.md
Doc: PLANS/IMPROVE_decouple_debug_draw_from_app.md
```

**Append commit message to document:**

````markdown
### Commit Message

```
[Paste prepared commit message]
```
````

### 6. Clean Up Branch (Standard Path)

If on `improve/<name>` branch:

```powershell
# Switch back to main
git checkout main

# Delete improvement branch (after commit)
# User will do this manually after committing
```

**Note for document:** "Branch `improve/<name>` can be deleted after commit."

### 7. State Completion

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
Commit message prepared (see document)

Ready to commit. Review changes and commit when ready.
```

---

## Tone & Constraints

- Concise summary of accomplishment
- Capture learnings while fresh
- Be honest about time estimates (builds calibration)
- Note batch opportunities (reduces future overhead)
- Prepare commit message but don't commit (user control)
- Celebrate completion (improvement work is valuable!)
