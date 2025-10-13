# Review Improvement Plan (Standard Path Only)

**Prerequisites:** Completed PLAN; plan appended to `PLANS/IMPROVE_<name>.md`

---

### 1. Review Development Principles

Read `AGENTS.md` to ensure alignment with:
- "Clarity over cleverness"
- "Simplicity over sophistication"
- Bottom-up dependency flow
- "Wait for third use" (rule of three)
- Graybox before polish

### 2. Gather Context

Extract improvement name from branch (`improve/<name>`):

1. Read `PLANS/IMPROVE_<name>.md`:
   - Initial description (severity, issue, proposed fix)
   - Full plan (impact analysis, stages, validation checklist)
   - Verify plan addresses original issue completely
2. Read `PLANS/DEPENDENCY_STACK.md` to check system stability
3. Read affected source files to understand current state

### 3. Perform Analysis

Cross-reference the plan against development principles. Identify violations, misalignments, and risks.

**Verify Against Principles:**
- **Clarity & Simplicity:** Solution is simplest approach that works; no unnecessary abstraction or over-engineering; names follow conventions (snake_case, descriptive)
- **Architecture:** Respects layering (Foundation → Character → Rendering → App); no reverse dependencies introduced; dependencies flow one direction only
- **Rule of Three:** Pattern verified to appear 3+ times OR canonical data justification documented; not extracting prematurely
- **Stability Gate:** Affected systems ≥70% certainty (check `DEPENDENCY_STACK.md`); no active feature work on these systems; OR Critical severity bypasses this check

**Check for Scope Creep:**
- Plan does more than backlog item described
- "While we're here" additions
- Opportunistic improvements bundled in
- Mixing multiple backlog items
- **If found:** Note new improvements in `IMPROVE_BACKLOG.md`; keep current plan focused on single item

**Assess Risk vs Reward:**

Risk Indicators:
- Files affected: 1-2 (Low) | 3-5 (Medium) | 6+ (High)
- Systems touched: 1 (Low) | 2-3 (Medium) | 4+ (High)  
- Certainty: 90%+ (Low) | 70-90% (Medium) | <70% (High)
- Behavioral change: None (Low) | Minor (Medium) | Significant (High)

Reward Indicators:
- Fixes architectural violation (High)
- Enables future work (Medium-High)
- Improves code clarity (Medium)
- Reduces duplication (Low-Medium)
- Cosmetic cleanup (Low)

Decision Matrix:
- High Risk + Low Reward → Defer (not worth it)
- High Risk + High Reward → Proceed with caution
- Low Risk + Any Reward → Proceed
- Medium Risk + Medium+ Reward → Proceed

**Verify Stage Breakdown:**
- Each stage should be independently buildable
- Clear validation criteria per stage
- Takes <30 minutes to implement
- Leaves codebase in working state
- Progresses logically toward goal
- **If stages too large:** Request decomposition

**Check Validation Checklist:**
Must include:
- Build & compile verification
- Behavioral correctness checks
- Architecture principle compliance
- Code quality standards (formatting, linting)
- Edge case handling (if applicable)
- **If incomplete:** Request specific additional validation criteria

**Review Rollback Strategy:**
Must have:
- Clear abort conditions defined
- Rollback steps documented
- Branch strategy allows clean revert
- Intermediate commit points identified (for multi-stage work)

### 4. Append Review to Planning Document

Add review analysis to `PLANS/IMPROVE_<name>.md` using template below.

**Decision Options:**
- **APPROVED:** Plan is sound; all principles satisfied; risk/reward acceptable → Proceed to EXECUTE
- **REVISE:** Minor issues found; small clarifications required; stage decomposition needed; validation checklist incomplete → Return to PLAN with specific feedback
- **DEFER:** Major issues; risk too high for reward; scope creep significant; stability gate not met (and not Critical); architectural concerns require design discussion → Update backlog with deferral reason

### Tone & Constraints

### 5. Append to Planning Document

If decision is made, append review summary to `PLANS/IMPROVE_<name>.md`:

```markdown
---

## PLAN REVIEW

**Date:** [YYYY-MM-DD]
**Status:** APPROVED | REVISE | DEFER

### Summary

[One paragraph: Is plan aligned with principles? Major concerns? Recommendation?]

### Violations (Must Fix)

- **[Category]:** [Issue] in [source]
  - **Principle:** [Violated principle from AGENTS.md]
  - **Fix:** [How to address]

**If none:** No violations found.

### Risks & Concerns

- **[Description]:** [Why risky/concerning] in [source]
  - **Mitigation:** [How to reduce risk]

**If none:** Plan appears well-aligned.

### Risk Assessment

**Risk Level:** Low | Medium | High
**Reward Level:** Low | Medium | High
**Decision:** Proceed | Proceed with Caution | Defer

### Stage Quality

**Stage Breakdown:** ✓ | Needs Decomposition
[Notes if stages too large]

**Validation Checklist:** ✓ | Incomplete
[Missing items if incomplete]

### Actionable Items

- [ ] [Specific action needed]

**If none:** Ready to proceed with implementation.

### Recommendation

**Reasoning:** [Brief justification for status]

**Next Steps:**
- If APPROVED: Proceed to EXECUTE
- If REVISE: Address items, re-review
- If DEFER: Update IMPROVE_BACKLOG with findings
```

---

### Tone & Constraints

- Concise and direct; no preamble or postamble
- Detail scales with risk and complexity
- Cite specific principles from `AGENTS.md`
- Focus on actionable recommendations
- Be objective and principle-driven
- Balance risk aversion with pragmatism (perfect is enemy of good)
- Defer only when truly necessary
