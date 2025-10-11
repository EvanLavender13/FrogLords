# Review Refactor Plan

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core principles:
- "Clarity over cleverness"
- "Simplicity over sophistication"
- "Wait for third use" (rule of three)
- Bottom-up dependency flow
- Single source of truth

### 2. Gather Context

Extract refactor name from branch (`refactor/<refactor_name>`):

1. Read `PLANS/refactor_<refactor_name>.md` (description & plan sections)
2. Read `PLANS/REFACTOR_BACKLOG.md` (original backlog entry)
3. Read `PLANS/DEPENDENCY_STACK.md` (check tree visualization for system certainty)

### 3. Perform Analysis

Cross-reference plan against principles and best practices:

**Core Principle Checks:**
- **Clarity Over Cleverness:** Does "after" genuinely improve readability? Are we introducing clever abstractions that obscure intent?
- **Simplicity Over Sophistication:** Is refactor truly simpler, or just different complexity? Are we over-engineering?
- **Rule of Three:** Pattern appears 3+ times (unless Critical architectural fix)? Instances truly identical?

**Stability & Risk Checks:**
- **System Stability:** All affected systems ≥70% certainty? Any under active feature development?
- **Risk Assessment:** Is risk level accurate? Does migration strategy address it? Are abort conditions clear?
- **Scope Discipline:** Does plan stick to goal? Any sneaky "while we're here" additions?

**Migration Strategy Checks:**
- **Stage Appropriateness:** Medium/High risk uses staged approach? Stages logically ordered (prepare → migrate → cleanup)?
- **Call Site Coverage:** All call sites from Impact Analysis included? Indirect dependencies? Debug/GUI/test code?
- **Hidden Dependencies:** Initialization order? Performance? Documentation updates?

**Validation & Safety Checks:**
- **Testing Protocol:** Behavior preservation testable? Test scenarios specific? Regression detection reliable?
- **Rollback Viability:** Rollback plan concrete (not just "revert")? Trigger conditions clear?

**Complexity Checks:**
- **Estimate:** Total ≤8 points? Stage estimates realistic?
- **Red Flags:** Unexpected complexity? Too many edge cases? Should defer?

**Before/After Quality:**
- **Code Examples:** Do examples clearly show improvement? Is "after" genuinely better?
- **Rationale:** Does rationale cite specific principles? Benefits concrete?

### 4. Check for Anti-Patterns

Flag these refactoring anti-patterns:
- **Premature Abstraction:** Extracting patterns <3 times
- **Over-Generalization:** Building for imagined futures
- **Complexity Shifting:** Moving complexity without reducing it
- **Scope Creep:** Bundling unrelated refactors
- **Unstable Foundation:** Refactoring systems <70% certainty
- **Clever Code:** Refactoring toward compact/clever vs clear/simple
- **Missing Rollback:** No realistic undo plan

### 5. Categorize Findings

**Violations (Must Fix):**
- Direct principle contradictions or critical flaws
- Examples: Rule of three violation, certainty <70%, no rollback plan, scope includes unrelated changes

**Risks (Should Address):**
- Concerns that increase danger or complexity
- Examples: Underestimated risk, missing migration steps, insufficient validation

**Suggestions (Consider):**
- Improvements that would strengthen plan
- Examples: Could split smaller, alternative approach simpler, additional validation

### 6. Append Review to Refactor Document

Append to `PLANS/refactor_<refactor_name>.md`:

```markdown
---

## Plan Review

**Date:** [YYYY-MM-DD]
**Reviewer:** Claude (AI Assistant)

### Summary

[One paragraph: Is plan sound? Major concerns? Recommendation to proceed/revise/defer?]

### Violations & Critical Issues

[Direct contradictions that MUST be fixed before proceeding]

- **Violation:** [Category]
  - **Source:** [Section in plan]
  - **Principle:** [Which principle violated]
  - **Details:** [Specific issue]
  - **Suggestion:** [How to fix]

**If none:** No critical issues found.

### Risks & Concerns

[Items that increase risk; should be addressed but may not block]

- **Risk:** [Description]
  - **Source:** [Section in plan]
  - **Concern:** [Why risky]
  - **Mitigation:** [How to reduce]

**If none:** Risk assessment appears thorough.

### Suggestions for Improvement

[Optional improvements; not critical]

- **Suggestion:** [Description]
  - **Rationale:** [Why this helps]
  - **Impact:** [How much better]

**If none:** Plan appears comprehensive.

### Principle Alignment Check

**Clarity Over Cleverness:**
- [ ] "After" code clearer than "before"
- [ ] No clever abstractions obscuring intent
- [ ] New developer could understand

**Simplicity Over Sophistication:**
- [ ] Genuinely reduces complexity
- [ ] Not over-engineering
- [ ] Abstraction matches problem domain

**Rule of Three:**
- [ ] Pattern appears 3+ times (or Critical fix)
- [ ] Not abstracting prematurely
- [ ] Instances truly identical

**Stability Requirement:**
- [ ] All systems ≥70% certainty
- [ ] No active development
- [ ] Safe to refactor now

### Migration Strategy Assessment

**Stage Design:**
- [ ] Staged approach for Medium/High risk
- [ ] Stages logically ordered
- [ ] Each stage independently verifiable

**Coverage:**
- [ ] All call sites included
- [ ] Indirect dependencies accounted for
- [ ] Debug/GUI/test code included

**Hidden Dependencies:**
- [ ] Initialization order considered
- [ ] Performance implications assessed
- [ ] Documentation updates planned

### Safety Assessment

**Testing Protocol:**
- [ ] Behavior preservation testable
- [ ] Test scenarios specific/measurable
- [ ] Regression detection reliable
- [ ] Per-stage and final validation planned

**Rollback Plan:**
- [ ] Procedure concrete
- [ ] Trigger conditions clear
- [ ] Backup strategy defined
- [ ] Quick recovery possible

### Complexity Assessment

**Estimated Complexity:** [X points]
- [ ] Total ≤8 points
- [ ] Stage estimates realistic
- [ ] No unexpected complexity

**Recommendation:**
- [ ] Proceed as planned
- [ ] Revise plan (address issues)
- [ ] Split into smaller refactors
- [ ] Defer (too risky/complex)

### Actionable Items

[Checklist of specific actions needed]

- [ ] [Action 1]
- [ ] [Action 2]

**If none:** Plan approved. Ready for IMPLEMENT_REFACTOR.

### Final Recommendation

**Status:** APPROVED | REVISE | DEFER

**Reasoning:** [Brief justification]

**Next Steps:**
- If APPROVED: Proceed to IMPLEMENT_REFACTOR
- If REVISE: Address actionable items, re-review
- If DEFER: Update REFACTOR_BACKLOG with findings
```

### 7. Propose Review

Present review to user. Summarize:
- Overall assessment (approved/revise/defer)
- Critical issues that must be fixed
- Risk level and confidence
- Recommended next steps

### Tone & Constraints

- Concise and direct; detail scales with severity
- Cite specific principles from AGENTS.md
- Distinguish violations from suggestions
- Be honest about risk and complexity
- Focus on actionable recommendations
- Celebrate good plans (don't manufacture concerns)
- Flag red flags proactively (better to defer now than fail during execution)
