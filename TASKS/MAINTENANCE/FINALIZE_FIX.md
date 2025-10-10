# Finalize Maintenance Fix

### 1. Review Development Principles

Read `AGENTS.md` to ensure backlog updates follow standards and capture learnings.

### 2. Verify Completion

Confirm fix is ready:
- **Path A (Trivial):** Implementation complete
- **Path B (Standard):** Review passed with approval

If unresolved issues remain, return to IMPLEMENT_FIX.

### 3. Update Maintenance Backlog

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
- **Completed:** [date]
```

### 4. Document Learnings (Optional)

If fix revealed insights:
- Add note to completed item
- If pattern appears elsewhere, add new item to backlog
- Note process improvements

### 5. Check for Related Items

Scan for batchable items:
- If 2+ similar items exist, note for batch processing
- If fix revealed additional issues, add to backlog

### 6. Prepare Commit

Prepare commit message:
- **Domain:** Code domain (`character`, `rendering`, `gui`) or `chore` for cross-cutting
- **Description:** Brief imperative (e.g., "remove redundant includes")
- **Why:** Reference severity and impact
- **Changes:** Keep minimal (single-line items)
- **Outcome:** State what's cleaner/fixed

**Example:**
```
chore: remove redundant includes from character_panel

Eliminates redundant #include "character/tuning.h" already provided
via character_panel.h, reducing compilation dependencies.

- Remove duplicate include from character_panel.cpp

Include chain now correctly flows through header.
```

### 7. Recommend Next Steps

After commit, suggest SELECT_ITEM for next fix or return to feature development.

### Tone & Constraints

- Concise and systematic
- Ensure backlog reflects completion
- Capture learnings while fresh
- Recommend batching if pattern emerges
- Prune completed section periodically
- User handles git operations
