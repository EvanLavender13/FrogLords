# Improve Backlog Grooming

### 1. Review Document Accumulation

1. Open `PLANS/IMPROVE_BACKLOG.md`
2. Identify signs of document bloat:
   - Completed items (✓) with extensive learning notes still inline
   - Items in Deferred section that haven't moved or gained clarity for 2+ iterations
   - Multiple completed items with redundant learning notes (same lesson documented multiple times)
   - Completed section spanning >1 month of history

### 2. Prune Completed Items

1. **Archive Old Completions:**
   - Move completed items older than 1 month to `PLANS/ARCHIVE/improve_completed_<YYYYMM>.md`
   - Keep only recent completions (last 3-4 weeks) for learning capture
   - Exception: Keep completed items that serve as explicit reference templates for similar future work
   - Create archive file structure:
     ```markdown
     # Improve Backlog Completed - <Month Year>
     
     ## Critical
     [moved items]
     
     ## High
     [moved items]
     
     ## Medium
     [moved items]
     
     ## Low
     [moved items]
     ```

2. **Consolidate Learning Notes:**
   - For completed items remaining, trim "Learning" sections to 1-3 key bullets
   - Remove redundant learnings that duplicate information across multiple items
   - Merge similar learnings into single, comprehensive notes

3. **Remove Redundant Documentation Links:**
   - If learning note already captures essential information, consider removing `Document:` link
   - Keep links only if they provide architectural details needed for future similar work

### 3. Clean Deferred Section

1. **Remove Stale Deferred Items:**
   - Delete deferred items that haven't gained clarity after 2+ iterations
   - Remove items where prerequisites are unlikely to be met in next 2-3 iterations
   - Exception: Keep "rule of three" deferrals if 2nd occurrence exists and 3rd is plausible

2. **Update Occurrence Counts:**
   - For "rule of three" items, verify occurrence count is still accurate
   - Add note if new occurrence was found: "Occurrences: 2 → 3 (as of <date>)"
   - Move to appropriate priority section if 3rd occurrence now exists

3. **Refresh Defer Reasons:**
   - Update "Defer Reason" if situation has changed
   - Remove items where defer reason no longer applies but item still shouldn't be done

### 4. Consolidate Priority Sections

1. **Collapse Empty Sections:**
   - If Critical, High, Medium, or Low sections are empty, replace with single line:
     ```markdown
     ## Critical
     (None)
     ```
   - Exception: Keep empty sections if you expect them to be populated soon

2. **Revalidate Priorities:**
   - Check if any Medium items have become High (e.g., pattern now has 3+ uses)
   - Check if any High items have become Critical (e.g., architectural violation spreading)
   - Check if any items should be downgraded (e.g., complexity lower than expected)

3. **Group Related Items:**
   - If multiple items target same file/system, consider grouping under subsections
   - Example:
     ```markdown
     ## Medium
     
     ### Character / Controller
     **Item 1**
     ...
     
     **Item 2**
     ...
     ```

### 5. Verify Stability Gate Compliance

1. **Check Dependency Stack:**
   - Open `PLANS/DEPENDENCY_STACK.md`
   - For each backlog item, verify target system has ≥70% certainty
   - Add note to items targeting unstable systems: "⚠️ Blocked: <system> <70% certain (as of <date>)"
   - Move blocked items to Deferred section with updated reason

2. **Update Tags:**
   - Verify all items have appropriate tags from Tags Reference section
   - Add missing tags (especially `#architecture`, `#pattern-extraction`, `#deferred`)
   - Remove obsolete tags

### 6. Refresh Metadata

1. **Update "Last Review" Date:**
   - Change to current date at top of document

2. **Verify Rule of Three Note:**
   - Ensure guidance section still includes: "Pattern extraction requires 3+ occurrences unless establishing canonical data source (document exception)."
   - Check that all pattern extraction items either:
     - Have 3+ occurrences documented
     - Have explicit exception reason noted

### 7. Archive Historical Context

If archiving completed items:

1. Create `PLANS/ARCHIVE/` directory if it doesn't exist
2. Create monthly archive file: `improve_completed_YYYYMM.md`
3. Move completed items older than 1 month (keep structure: Critical → High → Medium → Low)
4. Add link in Completed section of main document:
   ```markdown
   ## Completed
   
   (Recent completions; see `PLANS/ARCHIVE/improve_completed_*.md` for history)
   ```

### 8. Verify Consistency

1. **Cross-check with WORKFLOW.md:**
   - Ensure complexity point ranges match (Trivial: 1-2, Standard: 3-8)
   - Verify severity/priority definitions align

2. **Cross-check with Recent Commits:**
   - Search git history for recent IMPROVE commits
   - Ensure all completed improvements are documented in backlog
   - Add missing completions if found

3. **Scan Active PLANS/ Directory:**
   - List all `IMPROVE_*.md` files
   - Verify each has corresponding Completed entry in backlog
   - Add missing entries if found

### Tone & Constraints

- Be aggressive about archiving; Completed section should focus on *recent* learnings (last month)
- Remove deferred items that haven't gained traction—if they're important, they'll resurface
- Collapse priority sections if empty; visual noise is worse than empty markers
- Keep guidance section minimal; detailed rules belong in IMPROVE/WORKFLOW.md
- Documents should feel "actionable" after grooming (not historical archives)

---

### Example: Before Grooming

```markdown
## Completed

### Medium / Character / Tuning
**Consolidate Tuning Constants** ✓
- *Completed:* October 13, 2025
- *Files:* `src/character/tuning.cpp`
- *Fix:* Moved duplicated `FRICTION_RATIO` constant to a file-level anonymous namespace.
- *Learning:* Simple cleanups like this are quick wins for improving code quality.
- *Document:* `PLANS/IMPROVE_consolidate_tuning_constants.md`

### Medium / Input
**Unused stdio Include** (done)
- *Completed:* October 13, 2025
- *Files:* `src/input/input.cpp`
- *Fix:* Removed unused `<cstdio>` include
- *Learning:* GUI plot overlays still rely on `<cstdio>`; keep include there until helper exists
- *Document:* `PLANS/IMPROVE_remove_unused_stdio_include_input.md`

### Medium / Character / Controller
**Unused stdio Include** (done)
- *Completed:* October 13, 2025
- *Files:* `src/character/controller.cpp`
- *Fix:* Removed unused `<cstdio>` include
- *Learning:* Rendering debug formatting still uses `<cstdio>`; remove only where unused
- *Document:* `PLANS/IMPROVE_remove_unused_stdio_include_controller.md`
```

### Example: After Grooming

```markdown
## Completed

(Recent completions; see `PLANS/ARCHIVE/improve_completed_*.md` for history)

### Medium / Character / Tuning
**Consolidate Tuning Constants** ✓
- *Completed:* October 13, 2025
- *Learning:* File-scope constants in anonymous namespaces work well for derived values

### Medium / Input & Controller
**Remove Unused stdio Includes** (done)
- *Completed:* October 13, 2025
- *Files:* `src/input/input.cpp`, `src/character/controller.cpp`
- *Learning:* Remove unused includes; keep `<cstdio>` only where GUI plot overlays need it
```

---

### When to Run This Task

Run this grooming task when:
- Completed section has 10+ items
- Completed items span >1 month of history
- Deferred section has items unchanged for 2+ iterations
- Starting a new review cycle and want clean slate
- Learning notes feel repetitive across multiple items

Recommended frequency: Monthly, or every 8-12 completed improve items.
