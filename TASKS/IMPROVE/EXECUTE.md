# Execute Improvement

**Prerequisites:**
- **Trivial Path:** Completed SELECT; created `PLANS/IMPROVE_<name>.md`; on `improve/<name>` branch
- **Standard Path:** Completed SELECT, PLAN, REVIEW_PLAN (approved); on `improve/<name>` branch

---

## Common Steps (Both Paths)

### Format and Lint

After making changes, always run the formatter and linter tasks.

Fix any formatting or linting issues introduced.

### Build Verification

Run the build task.

Verify build succeeds with no new errors or warnings.

---

## Path A: Trivial Execution (1-2 points)

### 1. Read Item Description

Open `PLANS/IMPROVE_<name>.md` and confirm:
- Exact files and line numbers
- Specific change to make
- No ambiguity in scope

### 2. Make the Change

Execute the fix directly:
- Edit file(s) as described
- Keep changes minimal and focused
- No opportunistic additions

### 3. Verify Correctness

**Quick checks:**
- [ ] Change matches description exactly
- [ ] No unintended side effects
- [ ] File still readable and sensible

### 4. Format, Lint, and Build

Run common steps above (Format and Lint, Build Verification).

### 5. Append Execution Notes

Add to `PLANS/IMPROVE_<name>.md`:

````markdown
---

## EXECUTION (Trivial Path)

**Executed:** [Date/Time]

### Changes Made

**File:** `path/to/file.cpp`
- Line X: [Specific change]

### Verification

- [x] Build succeeds
- [x] clang-format passed
- [x] clang-tidy clean
- [x] No new warnings

### Notes

[Any observations, complications, or learning points]
````

Proceed to FINALIZE.

---

## Path B: Standard Execution (3-8 points)

### 1. Read the Plan

Open `PLANS/IMPROVE_<name>.md` and review:
- All stages and their sequence
- Validation criteria for each stage
- Rollback strategy

### 2. Execute Stage-by-Stage

**For each stage:**

#### 2.1. Make Changes
- Follow plan exactly for this stage
- Keep changes minimal and focused
- No deviation from planned scope

#### 2.2. Validate Incrementally
- Build the project (see Build Verification in common steps)
- Check validation criteria for this stage
- Verify no regressions

#### 2.3. Format and Lint
Run common steps above (Format and Lint) after each stage.

#### 2.4. Document Stage Completion
Append notes to `PLANS/IMPROVE_<name>.md` after each stage:

````markdown
**Stage N Completed:** [Date/Time]
- Changes: [What was modified]
- Validation: ✓ Build clean, ✓ Criteria met
- Duration: [Actual time taken]
- Issues: [Any complications or deviations from plan]
````

### 3. Run Full Validation Checklist

After all stages complete, execute full validation from PLAN:

````markdown
## Validation Checklist

### Build & Compile
- [ ] Clean build succeeds (see Build Verification in common steps)
- [ ] No compilation errors
- [ ] No new warnings

### Correctness
- [ ] Behavior unchanged (or changed as intended)
- [ ] Edge cases handled
- [ ] No regressions observed

### Architecture
- [ ] Dependency flow correct (verify with file includes)
- [ ] No reverse dependencies
- [ ] Follows conventions (`CONVENTIONS.md`)

### Code Quality
- [ ] clang-format clean (see Format and Lint in common steps)
- [ ] clang-tidy clean (see Format and Lint in common steps)
- [ ] No duplicated logic remains
- [ ] Comments accurate and helpful
````

### 4. Handle Discovery Issues

**If abort condition met during execution:**

1. **Stop immediately**
2. Document what was attempted and why it failed
3. Rollback changes (discard or stash working changes)
4. Append to `PLANS/IMPROVE_<name>.md`:

````markdown
---

## EXECUTION ABORTED

**Aborted:** [Date/Time]

**Reason:** [Why we stopped - abort condition triggered]

**Stages Completed:** [Which stages were done before abort]

**Learning:** [What was discovered that caused abort]

**Recommendation:** [Defer, redesign, or escalate?]
````

5. Update backlog (move item to Deferred with reason)
6. Return to SELECT for next item

### 5. Append Complete Execution Notes

Add to `PLANS/IMPROVE_<name>.md`:

````markdown
---

## EXECUTION (Standard Path)

**Started:** [Date/Time]
**Completed:** [Date/Time]
**Total Duration:** [Actual time]

### Stage-by-Stage Summary

**Stage 1:** [Description]
- Completed: [Time]
- Issues: None | [Describe]

**Stage 2:** [Description]
- Completed: [Time]
- Issues: None | [Describe]

[... for all stages]

### Full Validation Results

[Paste completed validation checklist]

### Deviations from Plan

[Any differences from original plan and why]

### Learning Points

[What worked well, what was harder than expected, insights gained]
````

### 6. Proceed to Next Step

- **Trivial Path:** Proceed to FINALIZE
- **Standard Path:** Proceed to REVIEW_CODE

---

## Tone & Constraints

- Follow plan strictly; no improvisation
- Validate continuously (every stage builds and works)
- Stop at first abort condition (don't push through problems)
- Document everything (deviations, issues, timings)
- Keep changes focused (resist "while we're here" temptation)
- Preserve intermediate states (don't lose work if late stage fails)
- Build after every file edit (catch errors early)
