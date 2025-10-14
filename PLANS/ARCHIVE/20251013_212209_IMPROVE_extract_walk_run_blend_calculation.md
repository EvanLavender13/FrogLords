# Extract Walk/Run Blend Calculation

**Severity/Priority:** High

**Current Issue:** The same blend factor calculation for walking and running is duplicated in `src/character/locomotion.cpp` in both the `update()` and `get_current_pose()` methods.

**Files Affected:**
- `src/character/locomotion.cpp`
- `src/character/locomotion.h`

**Proposed Fix:** Extract the duplicated logic into a new private helper function `compute_walk_run_blend()` within the `locomotion_system` class.

**Rationale:** This change will reduce code duplication and create a single source of truth for the walk/run blend calculation. This improves maintainability and follows the "Clarity over cleverness; simplicity over sophistication" principle from AGENTS.md.

**Workflow Path:** Path B (Standard)

**Estimated Complexity:** 3 points

**Risk Level:** Low - This is a simple refactoring of existing logic within a single class.

**Tags:** #pattern-extraction #locomotion

---

## PLAN

**Created:** 2025-10-15 00:00 UTC

**Branch:** `improve/extract_walk_run_blend_calculation`

### Impact Analysis

**Affected Systems:**
- `character/locomotion`

**Dependencies:**
- None

**Risk Level:** Low

**Risk Factors:**
- None

**Mitigation:**
- The change is small and easy to verify.

### Proposed Changes

**Stage 1: Introduce `compute_run_blend()`**
- Add the private function declaration to `locomotion.h`.
- Implement the function in `locomotion.cpp`.
- Validation: The code builds successfully.

**Stage 2: Refactor `get_run_blend()` and `update()`**
- Modify `get_run_blend()` to call `compute_run_blend()`.
- In `update()`, replace the duplicated logic with a call to `compute_run_blend()`.
- Validation: The code builds and runs correctly.

### Validation Checklist

#### Build & Compile
- [ ] Clean build succeeds (no errors, no warnings)
- [ ] All affected files compile

#### Correctness
- [ ] The walk/run blend behavior is unchanged.
- [ ] No new compiler warnings.

#### Architecture
- [ ] No changes to the public API of `locomotion_system`.

#### Code Quality
- [ ] Duplicated logic is removed.
- [ ] `clang-format` and `clang-tidy` pass.

### Rollback Strategy

**Abort Conditions:**
- If any stage fails to build or introduces incorrect behavior that cannot be easily fixed.

**Rollback Steps:**
1. Use `git restore` to discard changes to the affected files.
2. Return to the `main` branch.
3. Re-evaluate the plan.

---

## PLAN REVIEW

**Date:** 2025-10-15
**Status:** REJECTED

### Summary

The plan was rejected because the change is unnecessary. The review found that the logic for calculating the walk/run blend is not duplicated. The `get_run_blend()` function is called from two different locations, but the logic itself is not duplicated.

### Learning

This highlights a failure in the planning process. The duplication was not verified before the plan was created. This should have been caught during the analysis phase of planning.

---

## PLAN REVIEW

**Date:** 2025-10-14
**Status:** REVISE

### Summary

The plan correctly identifies genuine duplication (the blend calculation appears in both `update()` line 66-71 and `get_run_blend()` line 65-72—they're identical). However, close inspection reveals there is **no actual duplication**. The `get_run_blend()` method already encapsulates the calculation, and `update()` at line 27 calls `get_run_blend()` to obtain the blend value. The perceived duplication stems from misreading the code structure. The current implementation already follows the DRY principle with a single source of truth.

### Violations (Must Fix)

- **False Premise:** Plan misidentifies non-existent duplication
  - **Principle:** Clarity over cleverness (don't add complexity without justification)
  - **Fix:** Re-examine source code. The blend calculation exists once in `get_run_blend()` and is called by `update()` at line 27. No extraction needed.

### Risks & Concerns

- **Unnecessary Refactoring:** Introducing `compute_run_blend()` when `get_run_blend()` already serves this purpose adds indirection without benefit
  - **Mitigation:** Cancel this improvement; verify no actual duplication exists

### Risk Assessment

**Risk Level:** Low (safe change if executed)
**Reward Level:** Zero (solves non-existent problem)
**Decision:** Defer—no work needed

### Stage Quality

**Stage Breakdown:** N/A (plan unnecessary)
**Validation Checklist:** N/A

### Actionable Items

- [ ] Verify code examination: `get_run_blend()` is already the single source of truth
- [ ] Close this improvement branch without changes
- [ ] Remove from `IMPROVE_BACKLOG.md` or mark as "investigated—no action needed"

### Recommendation

**Reasoning:** The code already implements the desired pattern. `get_run_blend()` encapsulates the blend calculation logic, and `update()` calls it to obtain the blend weight (line 27 in `update()` uses `get_run_blend()`). No duplication exists. This improvement addresses a non-existent problem.

**Next Steps:**
- Close branch without changes
- Return to main
- Mark backlog item as investigated and closed
