# Pick Maintenance Item

### 1. Review Development Principles

Read `AGENTS.md` to synthesize coding standards and quality guidelines for:
- Identifying highest-impact maintenance work
- Understanding severity classifications
- Prioritizing based on code health and stability

### 2. Review Maintenance Backlog

1. Open `PLANS/MAINTENANCE_BACKLOG.md`
2. Scan sections: Critical → High → Medium → Low
3. Look for items that:
   - Have clear, actionable descriptions
   - Affect stable, well-understood code
   - Can be completed independently

### 3. Select Item

Choose **one item** based on:
- **Severity:** Higher first (Critical > High > Medium > Low)
- **Impact:** Prefer fixes improving multiple call sites or preventing bugs
- **Clarity:** Select items with concrete "Suggested Fix" sections
- **Isolation:** Prefer single-file over multi-file when possible

### 4. Verify Item Details

Confirm:
- Files and line numbers specified
- Expected behavior change clear
- No dependencies on incomplete items

### 5. Classify Complexity

**Path A: Trivial** (<5 lines, single file, mechanical)
- Examples: Remove redundant include, fix typo, adjust formatting
- Next: IMPLEMENT_FIX → FINALIZE_FIX (skip review)

**Path B: Standard** (Multi-line, behavior change, or multi-file)
- Examples: Extract constants, refactor logic, fix case sensitivity
- Next: IMPLEMENT_FIX → REVIEW_FIX → FINALIZE_FIX

### 6. Create Maintenance Document

Create description and save to `PLANS/maintenance_<item_name>.md`:

```markdown
# [Item Name]

**Severity:** [Critical | High | Medium | Low]

**Current Issue:** [What problem exists today? 1-2 sentences]

**Files Affected:** [Which files will change?]

**Suggested Fix:** [What improvement will be made? 1-2 sentences]

**Workflow Path:** Path A (Trivial) | Path B (Standard)

**Estimated Complexity:** [1-2 points]
```

### 7. Optional: Create Branch (Path B only)

For Standard fixes with higher risk:
```powershell
./scripts/create-branch.ps1 maintenance <item_name>
```

Trivial fixes work directly on current branch.

### 8. Document Selection

State clearly:
- Item name/description
- Severity level
- Files affected
- Workflow path (A or B)
- Document created: `PLANS/maintenance_<item_name>.md`

### Tone & Constraints

- Concise and direct
- Prefer high-severity over low-severity
- Avoid uncertain or actively-changing systems
- Select one at a time (batch similar later if pattern emerges)
- Skip items requiring architectural decisions

