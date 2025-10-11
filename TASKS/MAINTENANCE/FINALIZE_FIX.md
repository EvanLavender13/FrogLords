# Finalize Maintenance Fix

### 1. Review Development Principles

Read `AGENTS.md` to understand knowledge capture principles:
- Document learnings while fresh
- Capture patterns that worked
- Note anti-patterns discovered

### 2. Verify Completion

Confirm fix is ready:
- **Path A (Trivial):** Implementation complete
- **Path B (Standard):** Review passed with approval

If unresolved issues remain, return to IMPLEMENT_FIX.

### 3. Gather Context

1. Read `PLANS/maintenance_<item_name>.md`
2. Review implementation and review sections
3. Extract item name and affected files

### 4. Update Maintenance Backlog

1. Open `PLANS/MAINTENANCE_BACKLOG.md`
2. Locate item in severity section (Critical/High/Medium/Low)
3. Cut entire item block
4. Paste into `## Completed` section
5. Add completion metadata:

```markdown
### [Item Name]
- **Files:** [files]
- **Severity:** [severity]
- **Description:** [description]
- **Resolution:** [1-sentence summary]
- **Completed:** [YYYY-MM-DD]
- **Learnings:** [Key insights - see section 5]
- **Document:** PLANS/maintenance_<name>.md
```

### 5. Document Learnings

Capture insights while fresh (add to completed item's "Learnings"):

**Pattern Learnings:**
- What patterns emerged?
- Similar issues elsewhere?
- Opportunities for batching?

**Process Learnings:**
- Was complexity classification accurate?
- Did the fix uncover deeper issues?
- Should item have been a refactor instead?

**Technical Learnings:**
- Hidden dependencies discovered?
- Side effects observed?
- Standards violations found?

**Example:**
```markdown
*Learnings:*
- Investigation revealed architectural concern; logged to REFACTOR_BACKLOG
- Classification as "trivial" was accurate; 2-minute fix
- Pattern appears in 3 other files; created batch item in backlog
```

### 6. Check for Related Items

Scan for batchable items:
- If 2+ similar items exist, note for batch processing
- If fix revealed additional issues, add to backlog
- If deeper architectural issues found, consider REFACTOR_BACKLOG

### 7. Append Finalization Status

Append to `PLANS/maintenance_<item_name>.md`:

```markdown
---

## Finalization

**Date:** [YYYY-MM-DD]
**Status:** COMPLETED

**Backlog Update:**
- Moved to Completed in MAINTENANCE_BACKLOG.md
- Learnings documented in backlog entry

**Related Items:**
- [New items added to backlog]
[Or "None identified"]

**Next Steps:**
- Ready for commit
```

### 8. Recommend Next Steps

Inform user of completion and suggest next action:

```markdown
## Maintenance Fix Finalized: [Item Name]

**Completed:** [YYYY-MM-DD]
**Severity:** [severity]

### Changes
- Modified [N] file(s)
- [Key achievement]

### Learnings
- [Key insight 1]
- [Key insight 2]

### Next Steps
1. Review commit message
2. Execute git commit (manual)
3. If on branch, merge to main
4. Ready for next maintenance item or return to feature work

**Suggested Action:** [SELECT_ITEM for next fix | Return to feature workflow]
```

### Tone & Constraints

- Concise and systematic
- Ensure backlog reflects completion
- Capture learnings while fresh
- Recommend batching if pattern emerges
- Document path taken (Trivial vs Standard)
- Complete history in `PLANS/maintenance_<name>.md`
- User handles git operations
