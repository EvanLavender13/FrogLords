# Validate Refactor Behavior

### 1. Review Development Principles

Read `AGENTS.md` to understand testing philosophy and quality expectations:
- Iteration over planning (tight feedback loops)
- Graybox before polish (functional testing first)
- Gameplay-first principles

### 2. Gather Context

1. **Read Refactor Plan:** Open `PLANS/refactor_<refactor_name>.md`
2. **Review Validation Protocol:** Section 5 of refactor plan (testing checklist)
3. **Review Before/After:** Understand what behavior should be preserved
4. **Check Review Report:** Any concerns from `REVIEW_REFACTOR.md` to watch for

### 3. Build and Verify Compilation

Before testing behavior, ensure code compiles cleanly:

```bash
# Build the project
[build command for project]
```

Verify:
- [ ] Build succeeds without errors
- [ ] No new compiler warnings
- [ ] All changed files compiled

**If build fails:** Return to EXECUTE_REFACTOR to fix compilation issues.

### 4. Execute Validation Protocol

Work through the testing checklist from the refactor plan systematically.

#### For Each Test Scenario:

1. **Setup Test Conditions:** [As described in refactor plan]
2. **Execute Test:** [Specific actions to perform]
3. **Observe Behavior:** [What to look for]
4. **Compare to Expected:** [Does it match pre-refactor behavior?]
5. **Document Result:** Pass / Fail / Unclear

---

### Example Validation Checklist Format

```markdown
## Validation Results: [Refactor Name]

**Date:** [YYYY-MM-DD]
**Build Status:** ✓ Success | ✗ Failed

---

### Test Scenario 1: [Description from plan]

**Test Steps:**
1. [Step 1]
2. [Step 2]
3. [Step 3]

**Expected Behavior:** [What should happen]

**Actual Behavior:** [What actually happened]

**Result:** ✓ PASS | ✗ FAIL | ⚠ UNCLEAR

**Notes:** [Any observations, edge cases, or concerns]

---

### Test Scenario 2: [Description from plan]

[Repeat format for each test scenario]

---

### System-Specific Checks

#### [System 1]: [e.g., Locomotion]
- [ ] [Specific check 1]
- [ ] [Specific check 2]
- **Result:** ✓ PASS | ✗ FAIL
- **Notes:** [Observations]

#### [System 2]: [e.g., Rendering]
- [ ] [Specific check 1]
- [ ] [Specific check 2]
- **Result:** ✓ PASS | ✗ FAIL
- **Notes:** [Observations]

---

### Regression Detection

**Regression Checklist:**
- [ ] No crashes in previously stable code
- [ ] No visual artifacts introduced
- [ ] No physics instabilities
- [ ] No input response degradation
- [ ] Debug visualizations work correctly
- [ ] GUI panels display correctly
- [ ] [Any system-specific regression checks]

**Regressions Found:**
- [Regression 1: description, severity]
- [Regression 2: description, severity]

[Or "None detected"]

---

### Performance Characteristics

**Performance Spot Checks:**
- [ ] Frame rate similar to pre-refactor
- [ ] No obvious hitches or stuttering
- [ ] [Any specific performance-critical paths verified]

**Performance Issues:**
- [Issue 1: description, suspected cause]

[Or "No obvious degradation"]

---

### Hidden Dependencies Verification

**Debug/GUI Updates:**
- [ ] Debug visualizations work
- [ ] GUI panels display correct data
- [ ] [Any other hidden dependencies from plan]

**Issues Found:**
- [Issue 1: what broke, where]

[Or "All working correctly"]

---

### Overall Assessment

**Tests Passed:** [X / Y]
**Regressions Found:** [count]
**Performance:** No Issues | Minor Concerns | Significant Degradation

**Status:** VALIDATED | REGRESSIONS FOUND | UNCLEAR RESULTS

**Recommendation:**
- If VALIDATED: Proceed to FINALIZE_REFACTOR
- If REGRESSIONS FOUND: Fix issues, re-execute affected parts, re-validate
- If UNCLEAR RESULTS: Additional testing needed or rollback if results too ambiguous
```

---

### 5. Investigate Failures

For any test that fails:

1. **Identify Root Cause:**
   - Is it a correctness bug in refactored code?
   - Is it an edge case that wasn't preserved?
   - Is it a call site that was missed?
   - Is it a hidden dependency that wasn't updated?

2. **Assess Severity:**
   - **Critical:** Crashes, data corruption, core functionality broken
   - **High:** Major feature broken, obvious visual bugs
   - **Medium:** Minor feature issue, edge case broken
   - **Low:** Cosmetic issue, rare edge case

3. **Document Findings:**
   - What broke
   - Where (file/function if known)
   - Why (root cause analysis)
   - How to fix (proposed solution)

### 6. Handle Validation Results

Based on validation outcome:

#### All Tests Pass → VALIDATED
- Document success
- Note any interesting observations
- Proceed to FINALIZE_REFACTOR

#### Minor Issues Found → FIX AND RE-VALIDATE
- Document issues
- Return to EXECUTE_REFACTOR to fix
- Re-run affected tests after fix
- If fixes work, proceed to FINALIZE_REFACTOR

#### Major Regressions Found → ASSESS ROLLBACK
- Document regressions with severity
- Assess fix complexity:
  - **Quick fix (< 1 hour):** Fix and re-validate
  - **Complex fix (> 1 hour or unclear):** Consider rollback
- Consult rollback trigger conditions from refactor plan
- If rolling back: Execute rollback procedure, update backlog, return to SELECT_REFACTOR

#### Unclear Results → INVESTIGATE OR ROLLBACK
- If behavior is ambiguous (can't tell if correct):
  - Review original code behavior
  - Add more specific test cases
  - Consult with user if needed
- If investigation reveals problem: Fix or rollback
- If investigation confirms correctness: Document reasoning and proceed

### 7. Update Validation Report

Complete the validation results document and save any findings for finalization.

### 8. Decide Next Action

**If validation passed:**
- State: "Validation complete. All tests passed. Ready for FINALIZE_REFACTOR."
- Proceed to `FINALIZE_REFACTOR.md`

**If issues found:**
- State issues clearly with severity
- Recommend: Fix (return to EXECUTE_REFACTOR) or Rollback
- Await user decision

**If unclear:**
- State what's unclear and why
- Recommend: Additional investigation or rollback if ambiguity is high
- Await user guidance

### Tone & Constraints

- Systematic and thorough; follow validation protocol exactly
- Be honest about test results (don't rationalize failures)
- Document observations clearly (reproducible steps)
- Distinguish between test failures and unclear results
- Flag regressions proactively (better to catch now than in production)
- Consider severity when recommending fix vs rollback
- Testing focus: functional behavior, not code quality (that was REVIEW_REFACTOR)

### Red Flags (Consider Rollback)

- Multiple test scenarios fail
- Critical functionality broken
- Unfixable regression discovered
- Fix complexity exceeds original refactor estimate
- Behavior is fundamentally different from original
- Unable to verify correctness (tests inconclusive)
