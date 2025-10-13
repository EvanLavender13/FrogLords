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

1. Read `PLANS/REFACTOR_<refactor_name>.md` (description & plan sections)
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

Append to `PLANS/REFACTOR_<refactor_name>.md`:

```markdown
---

## Plan Review

**Date:** [YYYY-MM-DD]
**Status:** APPROVED | REVISE | DEFER

### Summary

[One paragraph: Is plan sound? Major concerns? Recommendation?]

### Violations (Must Fix)

- **[Category]:** [Issue] in [section]
  - **Principle:** [Which principle violated]
  - **Fix:** [How to address]

**If none:** No violations found.

### Risks & Concerns

- **[Description]:** [Why risky] in [section]
  - **Mitigation:** [How to reduce]

**If none:** Risk assessment appears sound.

### Suggestions (Consider)

- **[Description]:** [Why helpful]

**If none:** Plan appears comprehensive.

### Principle Checks

- [ ] "After" code clearer than "before"
- [ ] Genuinely reduces complexity
- [ ] Pattern appears 3+ times (or Critical fix)
- [ ] All systems ≥70% certainty
- [ ] Staged approach for Medium/High risk
- [ ] All call sites included
- [ ] Hidden dependencies considered
- [ ] Validation protocol concrete
- [ ] Total ≤8 points

### Actionable Items

- [ ] [Action needed]

**If none:** Ready for IMPLEMENT_REFACTOR.

### Recommendation

**Reasoning:** [Brief justification for status]

**Next Steps:**
- If APPROVED: Proceed to IMPLEMENT_REFACTOR
- If REVISE: Address items, re-review
- If DEFER: Update REFACTOR_BACKLOG
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
