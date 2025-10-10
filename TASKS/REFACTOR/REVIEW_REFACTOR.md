# Review Refactor Implementation

### 1. Review Development Principles

Read `AGENTS.md` for coding standards and quality guidelines:
- Naming conventions (snake_case)
- File organization and dependency flow
- Code formatting (4-space indent, braces on same line)
- Documentation guidelines
- "Clarity over cleverness"
- "Simplicity over sophistication"

### 2. Gather Context

1. **Recall Refactor Plan:** Read `PLANS/refactor_<refactor_name>.md`
2. **Review Execution Summary:** Check list of modified files from `EXECUTE_REFACTOR.md`
3. **Review Before/After:** Understand the intended transformation
4. **Read Modified Code:** Read all changed source files (`.h` and `.cpp`)

### 3. Verify Implementation Correctness

Cross-reference implementation against the refactor plan:

#### Goal Achievement
- **Transformation Match:** Does the implementation match the "after" example from plan?
- **All Call Sites Updated:** Were all call sites from migration checklist updated?
- **Hidden Dependencies:** Were debug/GUI/test updates completed?
- **Cleanup Complete:** Was old code removed (if cleanup stage planned)?

#### Code Quality Standards
- **Naming:** snake_case for all new symbols (functions, variables, classes)
- **Formatting:** 4-space indent, braces on same line, consistent style
- **Organization:** Correct file structure, header guards, include order
- **Documentation:** Public APIs have clear comments explaining purpose

#### Dependency Flow
- **Bottom-Up:** New includes respect layering (Foundation → Character → Rendering → App)
- **No Circular Dependencies:** New code doesn't introduce cycles
- **Single Source of Truth:** No duplicate system instances created

### 4. Principle Alignment Check

Verify the refactor aligns with core principles:

#### Clarity Over Cleverness
- **Readability:** Is refactored code easier to understand than original?
- **Intent:** Do function/variable names clearly express purpose?
- **No Clever Tricks:** Are we using straightforward approaches, not clever shortcuts?
- **Comments:** Are complex sections explained (but simple code self-documenting)?

#### Simplicity Over Sophistication
- **Complexity Reduction:** Is refactored code genuinely simpler?
- **No Over-Engineering:** Does abstraction level match problem complexity?
- **YAGNI Respected:** Are we solving current problems, not imagined futures?
- **Fewer Moving Parts:** Does refactor reduce cognitive load?

#### Pattern Extraction Quality (If Applicable)
- **Right Abstraction Level:** Is extracted function/class at appropriate granularity?
- **Reusability:** Can abstraction be used in all identified locations?
- **Parameters:** Are parameters clear and minimal?
- **No Premature Generalization:** Does it solve the specific 3+ uses, not general case?

### 5. Check for Unintended Side Effects

Look for problems introduced by the refactor:

#### Correctness Issues
- **Logic Errors:** Did refactoring introduce bugs (off-by-one, wrong operators)?
- **Edge Cases:** Are edge cases still handled correctly?
- **Initialization:** Are variables initialized properly in new code?
- **Memory Management:** If applicable, are allocations/deallocations balanced?

#### Behavioral Changes
- **Preserved Behavior:** Does refactored code behave identically to original?
- **New Behavior:** Any unintended behavior changes?
- **Performance:** Any obvious performance regressions (e.g., unnecessary copies)?
- **Side Effects:** Are side effects (mutations, I/O) preserved correctly?

#### Integration Issues
- **Call Site Correctness:** Are all call sites using new API correctly?
- **Type Safety:** Are types used correctly (no dangerous casts)?
- **Const Correctness:** Are const qualifiers appropriate?
- **Include Dependencies:** Are all necessary headers included?

#### Compilation & Warnings
- **Clean Build:** Code compiles without errors
- **No Warnings:** No new compiler warnings introduced
- **No Dead Code:** Old code removed (unless migration strategy keeps it temporarily)
- **Complete Migration:** No half-migrated state (all call sites updated or none)

### 6. Review Scope Discipline

Verify refactor stayed within bounds:

- **Plan Adherence:** Changes match what refactor plan specified
- **No Scope Creep:** No unrelated changes or "while we're here" modifications
- **Deviations Justified:** Any deviations from plan are documented and necessary
- **New Opportunities:** New refactor opportunities tracked separately (not bundled in)

### 7. Assess Risk Factors

Evaluate whether risk assessment was accurate:

- **Complexity Match:** Did complexity match estimate, or was it underestimated?
- **Call Site Count:** Was call site count accurate?
- **Hidden Dependencies:** Were all hidden dependencies found during execution?
- **Unforeseen Issues:** Any surprises that should inform future risk assessment?

### 8. Generate Review Report

Provide concise analysis:

```markdown
## Refactor Review: [Refactor Name]

**Date:** [YYYY-MM-DD]

---

### 1. Implementation Summary

**Files Modified:** [count] files
**Approach Used:** Staged | Linear
**Plan Adherence:** Exact | Minor Deviations | Major Deviations

---

### 2. Goal Achievement

- [x] Transformation matches "after" example
- [x] All call sites from plan updated
- [x] Hidden dependencies addressed
- [x] Cleanup completed (old code removed)

[Or list incomplete items]

---

### 3. Code Quality Verification

**Naming:** ✓ Pass | ✗ Issues found
**Formatting:** ✓ Pass | ✗ Issues found
**Organization:** ✓ Pass | ✗ Issues found
**Dependency Flow:** ✓ Pass | ✗ Issues found

**Issues Found:**
- [Issue 1: file, line, description]
- [Issue 2: file, line, description]

[Or "None" if all pass]

---

### 4. Principle Alignment

**Clarity Over Cleverness:** ✓ Improved | ≈ Neutral | ✗ Degraded
- [Specific observation about clarity]

**Simplicity Over Sophistication:** ✓ Simplified | ≈ Neutral | ✗ More Complex
- [Specific observation about simplicity]

**Pattern Extraction (if applicable):** ✓ Appropriate | ✗ Premature/Over-general
- [Observation about abstraction quality]

---

### 5. Unintended Side Effects

**Correctness:** ✓ Verified | ⚠ Potential Issues | ✗ Bugs Found
**Behavior Preservation:** ✓ Preserved | ⚠ Changes Unclear | ✗ Changed
**Performance:** ✓ No Regression | ⚠ Needs Profiling | ✗ Obvious Regression

**Issues Found:**
- [Issue 1: description, location, severity]
- [Issue 2: description, location, severity]

[Or "None detected" if clean]

---

### 6. Scope Discipline

**Plan Adherence:** ✓ Exact | ⚠ Minor Deviations | ✗ Major Deviations
**Scope Creep:** ✓ None | ⚠ Minor | ✗ Significant

**Deviations:**
- [Deviation 1: what changed from plan, why]

[Or "None" if plan followed exactly]

---

### 7. Risk Assessment Review

**Complexity:** As Estimated | Underestimated | Overestimated
**Call Site Count:** Accurate | Underestimated | Overestimated
**Hidden Dependencies:** All Found | Some Missed | Many Missed

**Surprises:**
- [Unforeseen issue 1]
- [Unforeseen issue 2]

[Or "None" if everything as expected]

---

### 8. Critical Issues (Must Fix Before Finalization)

- [ ] [Critical Issue 1: description, file, fix needed]
- [ ] [Critical Issue 2: description, file, fix needed]

[Or "None" if ready for finalization]

---

### 9. Recommendations

**Status:** APPROVED | FIX ISSUES | ROLLBACK

**Reasoning:** [Brief justification]

**Next Steps:**
- If APPROVED: Proceed to FINALIZE_REFACTOR and run the validation checklist captured in the plan
- If FIX ISSUES: Address critical items, then re-review
- If ROLLBACK: Execute rollback procedure from plan, update backlog, return to SELECT_REFACTOR

**Confidence Level:** High | Medium | Low
```

### 9. Propose Review

Present the review report. Summarize:
- Overall quality assessment
- Critical issues that must be fixed
- Principle alignment (clarity, simplicity)
- Recommended next action

### 10. Await Guidance

Do not proceed to finalization. Await user decision on whether to:
- Fix identified issues
- Accept minor deviations as acceptable
- Proceed to `FINALIZE_REFACTOR.md`
- Rollback the refactor

### Tone & Constraints

- Thorough and rigorous; this is the quality gate
- Detail scales with severity (critical issues get deep analysis)
- Cite specific file paths and line numbers
- Distinguish critical issues from polish opportunities
- Be honest about quality (celebrate good work, flag real problems)
- Focus on objective measures: correctness, clarity, simplicity
- Consider maintenance burden: will future developers understand this code?
