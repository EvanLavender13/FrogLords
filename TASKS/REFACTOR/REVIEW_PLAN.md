# Review Refactor Plan

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles for refactor evaluation:
- "Clarity over cleverness"
- "Simplicity over sophistication"
- "Abstract repeated patterns into systems; prefer parameters over assets"
- "Wait for third use" (rule of three before abstracting)
- Bottom-up dependency flow
- Single source of truth

### 2. Gather Context

1. **Read Planning Documents:**
   - `PLANS/refactor_<refactor_name>.md` (the refactor plan)
   - `PLANS/REFACTOR_BACKLOG.md` (original backlog entry)
   - `PLANS/DEPENDENCY_STACK.md` (system certainty scores)

2. **Read Impact Analysis:** Review findings from `ANALYZE_IMPACT.md` to understand scope

### 3. Perform Analysis

Cross-reference the refactor plan against principles and best practices. The goal is to identify risks, premature abstractions, or misalignments before execution.

#### Core Principle Checks

- **Clarity Over Cleverness:**
  - Does the "after" code genuinely improve readability?
  - Are we introducing clever abstractions that obscure intent?
  - Can a new developer understand the refactored code easily?

- **Simplicity Over Sophistication:**
  - Is the refactor truly simpler, or just different complexity?
  - Are we over-engineering a solution for the problem?
  - Does the abstraction match the problem domain?

- **Rule of Three Verification:**
  - Does the pattern appear 3+ times (unless Critical priority architectural fix)?
  - Are we abstracting prematurely based on speculation?
  - Are the pattern instances truly identical, not just similar?

#### Stability & Risk Checks

- **System Stability:**
  - Are all affected systems ≥70% certainty in DEPENDENCY_STACK.md?
  - Is any system currently under active feature development?
  - Should we wait for more stability before refactoring?

- **Risk Assessment:**
  - Is the risk level (Low/Medium/High) accurately assessed?
  - Does the migration strategy appropriately address the risk?
  - Is the rollback plan realistic and complete?
  - Are abort conditions clearly defined?

- **Scope Discipline:**
  - Does the plan stick to the stated refactor goal?
  - Are we sneaking in additional changes ("while we're here")?
  - Should any scope additions be tracked separately?

#### Migration Strategy Checks

- **Stage Appropriateness:**
  - For Medium/High risk: Is staged approach used?
  - For Low risk: Is linear approach acceptable?
  - Are stages logically ordered (prepare → migrate → cleanup)?
  - Can each stage be verified independently?

- **Call Site Coverage:**
  - Are all call sites from ANALYZE_IMPACT included in migration plan?
  - Are indirect dependencies accounted for?
  - Are debug/GUI/test code updates included?

- **Hidden Dependencies:**
  - Are all hidden dependencies from ANALYZE_IMPACT addressed?
  - Initialization order changes considered?
  - Performance implications assessed?
  - Documentation updates planned?

#### Validation & Safety Checks

- **Testing Protocol:**
  - Is behavior preservation testable?
  - Are test scenarios specific and measurable?
  - Can we detect regressions reliably?
  - Is validation planned per-stage and at completion?

- **Rollback Viability:**
  - Is rollback plan concrete (not just "revert")?
  - Are rollback trigger conditions clear?
  - Is backup branch strategy mentioned if needed?
  - Can we recover quickly if things go wrong?

#### Effort & Complexity Checks

- **Duration Estimate:**
  - Is total effort ≤16 hours (2 work days)?
  - If >16 hours: Should this be split into smaller refactors?
  - Are stage estimates realistic?

- **Complexity Red Flags:**
  - Does the plan reveal unexpected complexity?
  - Are there too many edge cases to handle?
  - Should this be deferred until more clarity?

#### Before/After Quality

- **Code Examples:**
  - Do before/after examples clearly show the improvement?
  - Is the "after" code genuinely better?
  - Are the improvements measurable (less duplication, clearer names, fewer lines)?

- **Rationale Strength:**
  - Does the rationale cite specific principles?
  - Are the benefits concrete, not theoretical?
  - Does the refactor solve a real problem, not an imagined one?

### 4. Check for Anti-Patterns

Flag these common refactoring anti-patterns:

- **Premature Abstraction:** Extracting patterns seen <3 times
- **Over-Generalization:** Building for imagined futures, not current needs
- **Complexity Shifting:** Moving complexity around without reducing it
- **Scope Creep:** Bundling multiple unrelated refactors together
- **Unstable Foundation:** Refactoring systems <70% certainty
- **Clever Code:** Refactoring toward compact/clever instead of clear/simple
- **Missing Rollback:** No realistic plan to undo if problems arise

### 5. Categorize Findings

Organize findings by severity:

#### Violations (Must Fix)
Direct contradictions of principles or critical flaws:
- Pattern appears only 2 times (rule of three violation)
- System certainty <70% (stability violation)
- No rollback plan (safety violation)
- Scope includes unrelated changes (scope discipline violation)

#### Risks (Should Address)
Concerns that increase danger or complexity:
- Risk level seems underestimated
- Migration strategy missing key steps
- Hidden dependencies not fully addressed
- Validation protocol insufficient for risk level

#### Suggestions (Consider)
Improvements that would strengthen the plan:
- Could split into smaller refactors
- Alternative approach might be simpler
- Additional validation step would increase confidence

### 6. Generate Review Document

Save review to `PLANS/refactor_review_<refactor_name>.md` using template below:

```markdown
# Refactor Plan Review: [Refactor Name]

**Date:** [YYYY-MM-DD]
**Reviewer:** Claude (AI Assistant)

---

## 1. Summary

[One paragraph: Is the plan sound? Major concerns? Recommendation to proceed/revise/defer?]

---

## 2. Violations & Critical Issues

[Direct contradictions of principles or critical flaws that MUST be fixed before proceeding]

- **Violation:** [Category - e.g., Rule of Three, Stability Gate]
  - **Source:** `PLANS/refactor_<name>.md` [section]
  - **Principle:** [Which principle from AGENTS.md is violated]
  - **Details:** [Specific issue description]
  - **Suggestion:** [How to fix this violation]

[Repeat for each violation]

**If no violations:** No critical issues found.

---

## 3. Risks & Concerns

[Items that increase risk or could cause problems; should be addressed but may not block execution]

- **Risk:** [Risk description]
  - **Source:** `PLANS/refactor_<name>.md` [section]
  - **Concern:** [Why this is risky]
  - **Mitigation:** [How to reduce this risk]

[Repeat for each risk]

**If no risks:** Risk assessment appears thorough and appropriate.

---

## 4. Suggestions for Improvement

[Optional improvements that would strengthen the plan but aren't critical]

- **Suggestion:** [Improvement description]
  - **Rationale:** [Why this would help]
  - **Impact:** [How much better would the plan be]

[Repeat for each suggestion]

**If no suggestions:** Plan appears comprehensive.

---

## 5. Principle Alignment Check

### Clarity Over Cleverness
- [ ] "After" code is clearer than "before"
- [ ] No clever abstractions that obscure intent
- [ ] New developer could understand refactored code

### Simplicity Over Sophistication
- [ ] Refactor genuinely reduces complexity
- [ ] Not over-engineering the solution
- [ ] Abstraction matches problem domain

### Rule of Three
- [ ] Pattern appears 3+ times (or Critical architectural fix)
- [ ] Not abstracting prematurely
- [ ] Pattern instances are truly identical

### Stability Requirement
- [ ] All affected systems ≥70% certainty
- [ ] No systems under active development
- [ ] Safe to refactor now

---

## 6. Migration Strategy Assessment

### Stage Design
- [ ] Staged approach for Medium/High risk
- [ ] Stages logically ordered
- [ ] Each stage independently verifiable

### Coverage
- [ ] All call sites from ANALYZE_IMPACT included
- [ ] Indirect dependencies accounted for
- [ ] Debug/GUI/test code included

### Hidden Dependencies
- [ ] Initialization order considered
- [ ] Performance implications assessed
- [ ] Documentation updates planned

---

## 7. Safety Assessment

### Testing Protocol
- [ ] Behavior preservation testable
- [ ] Test scenarios specific and measurable
- [ ] Regression detection reliable
- [ ] Per-stage and final validation planned

### Rollback Plan
- [ ] Rollback procedure concrete
- [ ] Trigger conditions clear
- [ ] Backup strategy defined
- [ ] Quick recovery possible

---

## 8. Effort & Complexity Assessment

**Estimated Duration:** [X hours/days]
- [ ] Total ≤16 hours
- [ ] Stage estimates realistic
- [ ] No unexpected complexity revealed

**Recommendation:**
- [ ] Proceed as planned
- [ ] Revise plan (address issues above)
- [ ] Split into smaller refactors
- [ ] Defer (too risky/complex)

---

## 9. Actionable Items

[Checklist of specific actions needed based on findings]

- [ ] [Action 1 - e.g., "Verify third instance of pattern in src/foo.cpp"]
- [ ] [Action 2 - e.g., "Add rollback procedure for Stage 2"]
- [ ] [Action 3 - e.g., "Update validation checklist with performance check"]

**If no actions needed:** Plan approved. Ready for EXECUTE_REFACTOR.

---

## 10. Final Recommendation

**Status:** APPROVED | REVISE | DEFER

**Reasoning:** [Brief justification for recommendation]

**Next Steps:**
- If APPROVED: Proceed to EXECUTE_REFACTOR
- If REVISE: Address actionable items, then re-review
- If DEFER: Update REFACTOR_BACKLOG with findings, return to SELECT_REFACTOR
```

### 7. Propose Review

Present the review document to the user. Summarize:
- Overall assessment (approved/revise/defer)
- Critical issues that must be fixed
- Risk level and confidence in plan
- Recommended next steps

### Tone & Constraints

- Concise and direct; detail scales with severity
- Cite specific principles from AGENTS.md
- Distinguish critical violations from suggestions
- Be honest about risk and complexity
- Focus on actionable recommendations
- Celebrate good plans (don't manufacture concerns)
- Flag red flags proactively (better to defer now than fail during execution)
