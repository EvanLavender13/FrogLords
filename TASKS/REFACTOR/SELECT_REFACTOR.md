# Select Refactoring Opportunity

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles:
- "Clarity over cleverness"
- "Simplicity over sophistication"
- "Wait for third use" (rule of three)

### 2. Review Refactor Backlog

1. Open `PLANS/REFACTOR_BACKLOG.md`
2. Scan sections in priority order: Critical → High → Medium → Low
3. Choose **one item** that meets all must-have criteria

### 3. Selection Criteria

#### Must-Have
- **Stability Gate:** System at ≥70% certainty in `DEPENDENCY_STACK.md`
- **Rule of Three:** Pattern appears 3+ times (or Critical architectural flaw)
- **Clear Definition:** Current/proposed state concrete
- **No Blockers:** No missing prerequisites or active feature work on affected systems

#### Tie-Breakers
- Higher priority (Critical > High > Medium > Low)
- Higher impact (affects multiple systems, unblocks future work)
- Lower risk (Low > Medium > High)
- Better isolation (independent, doesn't touch unstable systems)

### 4. Verify Stability & Rule of Three

**Stability Check:**
1. Read `PLANS/DEPENDENCY_STACK.md` and locate affected systems in the dependency tree
2. Verify all affected systems show ≥70% certainty in the tree visualization
3. Check that systems are in "Core Gameplay Layer" (90-95%) or "Completed Features" (100% ✅)
4. Confirm no active feature development on these systems (check tree connections and Layer Details)

**Rule of Three Check (for pattern extractions):**
1. Read files mentioned in backlog item
2. Confirm pattern appears 3+ times
3. Verify pattern is identical (not just similar)

**If checks fail:** Skip to next candidate or defer until viable.

### 5. Write Refactor Description

Create description and save to `PLANS/REFACTOR_<refactor_name>.md`:

```markdown
# [Refactor Name]

**Current State:** [What pattern/problem exists today? 1-2 sentences]

**Proposed Change:** [What improvement will be made? 1-2 sentences]

**Affected Systems:** [Which systems and files will change?]

**Rationale:** [Why this improves the codebase - cite principles]

**Estimated Complexity:** [X points] (1-2: Simple, 3-5: Medium, 6-8: Complex)

**Risk Level:** Low | Medium | High [with brief justification]
```

### Tone & Constraints

- Favor high-priority items meeting stability requirements
- Avoid uncertain or actively-changing systems
- Respect "wait for third use" discipline
- Select only one item at a time

### When Nothing Qualifies

If no items meet criteria:
1. Document why (stability gates, rule violations, etc.)
2. Suggest conditions needed for viability
3. Consider running IDENTIFY_REFACTORS
4. Return to other workflows (FEATURE or MAINTENANCE)
