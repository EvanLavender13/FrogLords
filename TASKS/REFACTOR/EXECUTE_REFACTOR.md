# Execute Refactor

### 1. Review Development Principles

Read `AGENTS.md` to ensure changes adhere to coding standards:
- Naming conventions (snake_case)
- File organization and dependency flow
- Code formatting (4-space indent, braces on same line)
- Documentation guidelines

### 2. Gather Context

1. **Read Refactor Plan:** Open `PLANS/refactor_<refactor_name>.md`
2. **Understand Migration Strategy:** Review staged/linear approach and per-stage goals
3. **Review Call Sites:** Familiarize yourself with all locations that need updating
4. **Check Validation Protocol:** Know what to verify after each stage

### 3. Create Backup (If High Risk)

For high-risk refactors, create safety branch:

```bash
git checkout -b backup/refactor_<refactor_name>
git push -u origin backup/refactor_<refactor_name>
git checkout main  # or current working branch
```

### 4. Execute Migration Strategy

Follow the approach defined in the refactor plan:

#### Staged Approach (Medium/High Risk)

Execute each stage independently with verification between stages.

---

**Stage 1: Prepare**

1. **Recall Stage Goal:** [Read from refactor plan]
2. **Implement Changes:**
   - Create new abstractions/functions/classes
   - Add new code WITHOUT removing old code
   - Ensure new code compiles independently
3. **Verify Stage 1:**
   - [ ] Code compiles without errors/warnings
   - [ ] [Stage-specific check from plan]
4. **Commit Stage 1:**
   ```bash
   git add [files]
   git commit -m "refactor(<system>): prepare <refactor_name> - <stage goal>"
   ```

**Checkpoint:** If verification fails, debug before proceeding. If unfixable quickly, revert Stage 1 commit.

---

**Stage 2: Migrate**

1. **Recall Stage Goal:** [Read from refactor plan]
2. **Update Call Sites:**
   - Work through call site checklist from refactor plan
   - Update one call site at a time
   - Verify compilation after every 3-5 call sites
3. **Update Hidden Dependencies:**
   - Debug visualizations
   - GUI panels
   - Test code
4. **Verify Stage 2:**
   - [ ] All call sites updated (check against plan)
   - [ ] Code compiles without errors/warnings
   - [ ] [Stage-specific behavior check from plan]
5. **Commit Stage 2:**
   ```bash
   git add [files]
   git commit -m "refactor(<system>): migrate to <refactor_name> - <stage goal>"
   ```

**Checkpoint:** If call sites fail to compile or behavior breaks, fix immediately. If complexity exceeds estimate, consider aborting.

---

**Stage 3: Cleanup**

1. **Recall Stage Goal:** Remove old code
2. **Remove Old Implementation:**
   - Delete old functions/classes/constants
   - Remove deprecated APIs
   - Clean up temporary migration scaffolding
3. **Verify No References Remain:**
   - Search codebase for old symbol names
   - Verify no compilation errors (unused code removed)
4. **Verify Stage 3:**
   - [ ] No references to old code
   - [ ] Code compiles without errors/warnings
   - [ ] [Final stage check from plan]
5. **Commit Stage 3:**
   ```bash
   git add [files]
   git commit -m "refactor(<system>): cleanup <refactor_name> - remove old implementation"
   ```

**Checkpoint:** If old code is still referenced, migration is incomplete. Fix before proceeding.

---

#### Linear Approach (Low Risk)

For simple refactors with <5 call sites:

1. **Implement All Changes:**
   - Add new code
   - Update all call sites
   - Remove old code
   - Update hidden dependencies

2. **Verify Correctness:**
   - [ ] Code compiles without errors/warnings
   - [ ] [All checks from validation protocol]

3. **Commit:**
   ```bash
   git add [files]
   git commit -m "refactor(<system>): <brief description of refactor>"
   ```

### 5. Track Changes

After execution completes, document:

```markdown
## Execution Summary

**Approach Used:** Staged | Linear

**Files Modified:**
- `src/file1.cpp` - [what changed]
- `src/file2.h` - [what changed]
- `src/file3.cpp` - [what changed]
[complete list]

**Commits Created:**
- [commit-hash] - Stage 1: [description]
- [commit-hash] - Stage 2: [description]
- [commit-hash] - Stage 3: [description]

**Issues Encountered:**
- [Issue 1 and how resolved]
- [Issue 2 and how resolved]
[or "None" if smooth execution]

**Deviations from Plan:**
- [Any changes to original plan]
[or "None" if plan followed exactly]
```

### 6. Mechanical Correctness Check

Before proceeding to REVIEW_REFACTOR, verify:

- **Compilation:** Code compiles without errors or warnings
- **Naming:** All new symbols follow snake_case convention
- **Formatting:** 4-space indentation, consistent brace style
- **Includes:** New includes respect dependency flow (Foundation → Character → Rendering → App)
- **No Dead Code:** Old code removed (unless deliberately keeping for migration)

### 7. Handle Abort Conditions

If any abort condition from refactor plan is triggered:

1. **Stop Execution:** Do not proceed further
2. **Document State:** What was completed, what failed, why
3. **Rollback Decision:**
   - If salvageable: Fix and continue
   - If too complex: Rollback per plan's rollback procedure
4. **Update Backlog:**
   - Mark refactor as deferred
   - Document why (complexity exceeded estimate, unforeseen issues, etc.)
   - Note what conditions needed for viability
5. **Return to SELECT_REFACTOR:** Choose different refactor

### 8. Determine Next Step

If execution succeeded:
- Proceed to `REVIEW_REFACTOR.md` for comprehensive code review

If execution was aborted:
- Rollback changes
- Update `PLANS/REFACTOR_BACKLOG.md` with findings
- Return to `SELECT_REFACTOR.md`

### Tone & Constraints

- Methodical and careful; follow plan exactly
- Verify after each stage (staged approach) or immediately (linear approach)
- Commit frequently (per-stage for staged, once for linear)
- Document deviations from plan
- Abort proactively if complexity exceeds estimate
- Preserve coding standards (formatting, naming, organization)
- No scope creep: implement only what's in the plan

## Abort Condition Examples

Trigger rollback if:
- Build breaks and fix isn't obvious within 15 minutes
- Call site count was underestimated by >50%
- Unforeseen dependencies discovered
- Complexity significantly exceeds estimate
- Unclear how to proceed while maintaining correctness
