# Review Refactor Implementation

### 1. Review Development Principles

Read `AGENTS.md` for coding standards and quality guidelines:
- Naming conventions (snake_case)
- File organization and dependency flow
- Code formatting (4-space indent, braces on same line)
- "Clarity over cleverness"
- "Simplicity over sophistication"

### 2. Gather Context

Extract refactor name from branch (`refactor/<refactor_name>`):

1. Read `PLANS/refactor_<refactor_name>.md` (description, plan, execution sections)
2. Review before/after examples from plan
3. Review execution summary for modified files
4. Read all changed source files (`.h` and `.cpp`)

### 3. Verify Implementation

**Goal Achievement:**
- Transformation matches "after" example?
- All call sites from migration checklist updated?
- Hidden dependencies (debug/GUI/test) updated?
- Cleanup complete (old code removed)?

**Code Quality:**
- Naming: snake_case for all new symbols
- Formatting: 4-space indent, braces on same line
- Organization: Correct file structure, header guards, include order
- Documentation: Public APIs have clear comments

**Dependency Flow:**
- Bottom-up: Includes respect layering (Foundation → Character → Rendering → App)
- No circular dependencies introduced
- Single source of truth maintained

### 4. Principle Alignment

**Clarity Over Cleverness:**
- Is refactored code easier to understand?
- Do names clearly express purpose?
- Are we using straightforward approaches?

**Simplicity Over Sophistication:**
- Is code genuinely simpler?
- Does abstraction level match problem complexity?
- Are we solving current problems, not imagined futures?

**Pattern Extraction (if applicable):**
- Is abstraction at appropriate granularity?
- Can it be used in all identified locations?
- Are parameters clear and minimal?

### 5. Check for Side Effects

**Correctness:**
- Logic errors introduced (off-by-one, wrong operators)?
- Edge cases still handled correctly?
- Variables initialized properly?

**Behavior:**
- Does code behave identically to original?
- Any unintended behavior changes?
- Obvious performance regressions (unnecessary copies)?

**Integration:**
- All call sites using new API correctly?
- Types used correctly (no dangerous casts)?
- All necessary headers included?

**Compilation:**
- Compiles without errors or warnings?
- No dead code remaining?
- No half-migrated state?

### 6. Review Scope Discipline

- Changes match refactor plan?
- No unrelated "while we're here" modifications?
- Deviations documented and necessary?
- New opportunities tracked separately (not bundled)?

### 7. Assess Risk Factors

- Did complexity match estimate?
- Was call site count accurate?
- Were all hidden dependencies found?
- Any surprises informing future risk assessment?

### 8. Append Code Review to Refactor Document

Append to `PLANS/refactor_<refactor_name>.md`:

```markdown
---

## Code Review

**Date:** [YYYY-MM-DD]
**Reviewer:** Claude (AI Assistant)

### Goal Achievement

**Transformation Match:** ✓ Matches | ⚠ Partial | ✗ Mismatch
**All Call Sites Updated:** ✓ Complete | ⚠ Some Missed | ✗ Many Missed
**Hidden Dependencies:** ✓ All Updated | ⚠ Some Missed | ✗ Many Missed
**Cleanup Complete:** ✓ Done | ⚠ Partial | ✗ Not Done

**Details:**
- [Specific observations]

### Code Quality Standards

**Naming:** ✓ Correct | ⚠ Minor Issues | ✗ Major Issues
**Formatting:** ✓ Consistent | ⚠ Minor Issues | ✗ Major Issues
**Organization:** ✓ Correct | ⚠ Minor Issues | ✗ Major Issues
**Documentation:** ✓ Adequate | ⚠ Needs Improvement | ✗ Missing

**Issues:**
- [Issue: description, location]
[Or "None"]

### Principle Alignment

**Clarity Over Cleverness:** ✓ Improved | ≈ Neutral | ✗ Degraded
- [Observation about clarity]

**Simplicity Over Sophistication:** ✓ Simplified | ≈ Neutral | ✗ More Complex
- [Observation about simplicity]

**Pattern Extraction (if applicable):** ✓ Appropriate | ✗ Premature/Over-general
- [Observation about abstraction]

### Unintended Side Effects

**Correctness:** ✓ Verified | ⚠ Potential Issues | ✗ Bugs Found
**Behavior Preservation:** ✓ Preserved | ⚠ Changes Unclear | ✗ Changed
**Performance:** ✓ No Regression | ⚠ Needs Profiling | ✗ Obvious Regression

**Issues:**
- [Issue: description, location, severity]
[Or "None detected"]

### Scope Discipline

**Plan Adherence:** ✓ Exact | ⚠ Minor Deviations | ✗ Major Deviations
**Scope Creep:** ✓ None | ⚠ Minor | ✗ Significant

**Deviations:**
- [What changed from plan, why]
[Or "None"]

### Risk Assessment Review

**Complexity:** As Estimated | Underestimated | Overestimated
**Call Site Count:** Accurate | Underestimated | Overestimated
**Hidden Dependencies:** All Found | Some Missed | Many Missed

**Surprises:**
- [Unforeseen issues]
[Or "None"]

### Critical Issues (Must Fix Before Finalization)

- [ ] [Critical issue: description, file, fix needed]
[Or "None"]

### Recommendations

**Status:** APPROVED | FIX ISSUES | ROLLBACK

**Reasoning:** [Brief justification]

**Next Steps:**
- If APPROVED: Proceed to FINALIZE_REFACTOR and run validation checklist
- If FIX ISSUES: Address critical items, re-review
- If ROLLBACK: Execute rollback, update backlog, return to SELECT_REFACTOR

**Confidence Level:** High | Medium | Low
```

### 9. Propose Review

Present review report. Summarize:
- Overall quality assessment
- Critical issues that must be fixed
- Principle alignment (clarity, simplicity)
- Recommended next action

### 10. Await Guidance

Do not proceed to finalization. Await user decision to:
- Fix identified issues
- Accept minor deviations as acceptable
- Proceed to FINALIZE_REFACTOR
- Rollback the refactor

### Tone & Constraints

- Thorough and rigorous; this is the quality gate
- Detail scales with severity
- Cite specific file paths and line numbers
- Distinguish critical issues from polish opportunities
- Be honest about quality
- Focus on objective measures: correctness, clarity, simplicity
- Consider maintenance burden: will future developers understand this?
