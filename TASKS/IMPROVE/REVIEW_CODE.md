# Review Improvement Code (Standard Path Only)

**Prerequisites:** Completed EXECUTE (Standard Path); full validation checklist passed; execution notes appended to `PLANS/IMPROVE_<name>.md`

---

### 1. Review Development Principles

Read `AGENTS.md` for development principles and coding standards:
- snake_case naming conventions
- 4-space indent, braces on same line
- Dependency flow (Foundation → Character → Rendering → App)
- "Clarity over cleverness" and "Simplicity over sophistication"

### 2. Gather Context

Extract improvement name from branch (`improve/<name>`):

1. Read `PLANS/IMPROVE_<name>.md`:
   - Original issue and proposed fix
   - Plan stages and approach
   - Execution notes for each stage
   - Any deviations or complications
2. Identify all modified files from execution notes
3. Read changed code and surrounding context

### 3. Perform Code Review

Cross-reference implemented code against principles and plan goals.

**For Each Modified File:**

Check Correctness:
- Change solves stated problem
- Logic is sound
- No off-by-one errors or typos
- Edge cases handled appropriately

Verify Principles Adherence:
- **Naming:** snake_case for all user code; descriptive, clear names; consistent with surrounding code
- **Structure:** Appropriate placement (right file, right scope); minimal changes (no unnecessary refactoring); comments updated if behavior/intent changed
- **Architecture:** Dependency flow correct (Foundation → Character → Rendering → App); no reverse dependencies introduced; follows layered architecture

**Check for Side Effects:**

Unintended Changes:
- No scope creep (only what was planned changed)
- No "while we're here" additions
- No unrelated formatting changes (beyond clang-format)

Behavioral Impact:
- Behavior preserved (or intentionally changed as planned)
- No new bugs introduced
- Performance not degraded

Dependency Impact:
- All `#include` statements necessary and minimal
- No circular dependencies created
- Check callers/users of changed functions/types still work correctly

**Validate Against Checklist:**

Re-verify the validation checklist from PLAN was properly executed:
- Build output reviewed (no errors, no warnings)
- Behavior matches intent
- Architecture principles followed
- Code quality tools clean (clang-format, clang-tidy)
- Learnings captured in execution notes

### 4. Append Review to Planning Document

Add review analysis to `PLANS/IMPROVE_<name>.md` using template below.

**Decision Options:**
- **APPROVED:** All checks passed; principles followed; no side effects; quality standards met → Proceed to FINALIZE
- **FIX_ISSUES:** Specific problems found (missed edge case, naming violation, unintended behavior change, missing comment update) → Return to EXECUTE with fix list
- **ROLLBACK:** Fundamental issues with approach; unintended complexity revealed; risk too high for reward → Rollback changes, update backlog with learnings

### Tone & Constraints

- Concise and direct; no preamble or postamble
- Detail scales with risk and complexity
- Cite specific file:line references
- Focus on actionable fixes over opinions
- Be thorough but not pedantic
- Distinguish between "must fix" and "nice to have"
- Approve good work; don't block on trivial preferences

---

### Code Review Template

**Append to `PLANS/IMPROVE_<name>.md`:**

```markdown
---

## CODE REVIEW

**Date:** [YYYY-MM-DD]
**Status:** APPROVED | FIX_ISSUES | ROLLBACK

### Summary

[One paragraph: Is implementation approved? Major issues? Recommendation?]

### Violations (Must Fix)

- **[Category]:** [Issue]
  - **File:** `src/path/to/file.cpp:line`
  - **Principle:** [Which principle violated]
  - **Fix:** [How to address]

**If none:** No violations found.

### Concerns (Consider)

- **[Description]:** [Why concerning]
  - **File:** `src/path/to/file.cpp:line`
  - **Impact:** [Potential issues]

**If none:** Implementation looks solid.

### Code Quality Checks

- Naming: snake_case
- Formatting: 4-space indent, braces on same line
- Dependency flow respected
- No scope creep
- Comments accurate
- No side effects

### Files Reviewed

**`path/to/file.cpp`:**
- Changes: [Brief summary]
- Correctness: ✓ [or issues]
- Principles: ✓ [or issues]
- Side Effects: None [or describe]

[Repeat for each file]

### Actionable Items

- [ ] [Specific action needed]

**If none:** Ready to proceed with finalization.

### Recommendation

**Reasoning:** [Brief justification for status]

**Next Steps:**
- If APPROVED: Proceed to FINALIZE
- If FIX_ISSUES: Address items, re-review
- If ROLLBACK: Defer improvement, update backlog
```

---

## Common Issues to Watch For

**Architectural:**
- Higher layer depending on lower layer (okay)
- Lower layer depending on higher layer (NOT okay)
- Rendering including App headers (violation)

**Naming:**
- camelCase instead of snake_case
- Abbreviations or unclear names
- Inconsistent with surrounding code

**Scope Creep:**
- Extra refactoring not in plan
- Opportunistic improvements
- Fixed unrelated issues "while here"

**Quality:**
- Stale comments after code change
- Missing const where appropriate
- Unnecessary includes added
- Formatting not applied
