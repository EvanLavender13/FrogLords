# Align Task Consistency

**Purpose:** Ensure all workflow tasks maintain consistent structure, tone, format, and alignment with principles. Prevents drift when individual tasks are updated.

---

## When to Run This Task

Run ALIGN_TASKS when:
- A specific workflow task is modified (format, structure, tone)
- AGENTS.md principles are updated
- New workflows are added
- Inconsistencies are noticed across tasks
- After major refactors to task structure

---

## Consistency Dimensions

### 1. Structural Consistency

**Common Patterns Across All Tasks:**
- Numbered step sections (### N. Step Name)
- "Review Development Principles" as step 1 (when applicable)
- "Gather Context" as step 2 (when applicable)
- "Append [Output] to Document" for plan updates
- "Tone & Constraints" as final section
- Markdown code blocks for templates
- Checklist format for validation steps

**Workflow-Specific Patterns:**
- **SELECT tasks:** Backlog → Selection Criteria → Verify → Write Description
- **PLAN tasks:** Gather Context → Analyze → Design Strategy → Create Checklists → Estimate → Append Plan
- **IMPLEMENT tasks:** Review Standards → Gather Context → Execute → Verify → Append Summary
- **REVIEW tasks:** Gather Context → Perform Analysis → Categorize Findings → Append Review → Report
- **FINALIZE tasks:** Validate → Update Backlog → Document Learnings → Next Steps

### 2. Tone & Voice Consistency

**From AGENTS.md:**
- "Concise and direct. No preamble/postamble. Detail scales with risk/complexity."

**Applied to Tasks:**
- Imperative voice for instructions ("Read X", "Create Y", "Verify Z")
- Bullet points over paragraphs for checklists
- Examples when helpful, not exhaustive
- Specific file paths (e.g., `PLANS/feature_<name>.md`)
- Clear success criteria and abort conditions

**Red Flags (inconsistent tone):**
- Overly verbose explanations
- Apologetic or tentative language
- Redundant "make sure to" phrasing
- Academic/formal tone
- Missing concrete examples

### 3. Template Consistency

**Document Templates Should:**
- Use consistent markdown heading levels
- Follow same status format (`**Status:** APPROVED | REVISE | DEFER`)
- Use consistent checkbox format (`[ ]` not `- [ ]` in code blocks)
- Share common sections across similar workflows:
  - `### Summary` (one paragraph assessment)
  - `### [Category] (Must Fix | Should Address | Consider)` (severity-based)
  - `### Actionable Items` (checklist)
  - `### Recommendation` or `### Next Steps`
- Match verbosity to task complexity:
  - IMPROVE Trivial: Minimal (fix is straightforward)
  - IMPROVE Standard: Moderate (staged, risky, architectural)
  - FEATURE: Comprehensive (uncertain, exploratory)

### 4. Principle Alignment

**All Tasks Should:**
- Reference AGENTS.md explicitly in "Review Development Principles"
- Cite specific principles when checking work:
  - "Clarity over cleverness"
  - "Simplicity over sophistication"
  - "Wait for third use" (rule of three)
  - "Graybox before polish"
  - Bottom-up dependency flow
- Check against DEPENDENCY_STACK.md for stability (≥70% certainty)
- Verify coding standards:
  - snake_case naming
  - 4-space indent
  - Braces on same line
  - Dependency flow (Foundation → Character → Rendering → App)

**Anti-Pattern Checks:**
- Premature abstraction (<3 uses)
- Over-engineering for imagined futures
- Polish before proven structure
- Reactive layers controlling core logic
- Building on uncertain foundations (<70%)

### 5. Cross-Workflow Consistency

**Review Tasks (REVIEW_PLAN, REVIEW_CODE, REVIEW_IMPLEMENTATION):**
- All should output similar status format
- All should categorize findings by severity
- All should provide actionable next steps
- Verbosity scales with complexity (IMPROVE Trivial < IMPROVE Standard < FEATURE)

**Finalize Tasks (FINALIZE, FINALIZE_FEATURE):**
- All should run final validation
- All should update backlog (move to Completed)
- All should document learnings
- All should propose next steps

**Plan Tasks (PLAN, PLAN_IMPLEMENTATION):**
- All should analyze impact/risk
- All should create migration/implementation strategy
- All should provide validation checklists
- All should estimate complexity

---

## Execution Protocol

### Step 1: Identify Trigger

Determine what prompted this alignment check:
- Specific task was modified (which one?)
- Principle update in AGENTS.md (what changed?)
- Noticed inconsistency (where?)
- General audit (periodic check)

### Step 2: Map Affected Tasks

Based on trigger, identify tasks that need alignment:

**If modifying a REVIEW task:**
- Compare all review tasks: REVIEW_PLAN, REVIEW_CODE, REVIEW_IMPLEMENTATION
- Check template format consistency
- Verify severity categorization matches

**If modifying a FINALIZE task:**
- Compare all finalize tasks: FINALIZE (IMPROVE), FINALIZE_FEATURE
- Check backlog update patterns
- Verify learning documentation format

**If updating AGENTS.md principles:**
- Check all tasks that reference AGENTS.md (most of them)
- Verify principle citations are current
- Update anti-pattern checks

**If changing template structure:**
- Identify all tasks using similar templates
- Update to match new structure

### Step 3: Analyze Inconsistencies

For each affected task, check:

**Structural:**
- [ ] Step numbering follows pattern
- [ ] Common sections present (Gather Context, Tone & Constraints)
- [ ] Template format matches peer tasks
- [ ] Checklists use consistent format

**Tone:**
- [ ] Concise and direct (no preamble/postamble)
- [ ] Imperative voice
- [ ] Detail appropriate to complexity
- [ ] Specific file paths and examples

**Principle Alignment:**
- [ ] References correct AGENTS.md principles
- [ ] Includes relevant anti-pattern checks
- [ ] Checks DEPENDENCY_STACK.md for stability
- [ ] Verifies coding standards

**Template Output:**
- [ ] Status format matches peer tasks
- [ ] Severity categories consistent
- [ ] Actionable items clear
- [ ] Verbosity appropriate

### Step 4: Update Tasks

For each identified inconsistency:

1. **Document the pattern** being enforced (from trigger task or AGENTS.md)
2. **Update task files** to match pattern
3. **Verify output templates** align across peer tasks
4. **Check tone** matches "Concise and direct" guideline

### Step 5: Verify Cross-References

Check that tasks correctly reference each other:
- SELECT → PLAN → REVIEW → IMPLEMENT → REVIEW → FINALIZE
- Backlog files referenced consistently
- Planning document paths follow pattern
- Branch naming conventions match

### Step 6: Document Changes

Create summary of alignment changes:

```markdown
## Task Alignment Summary

**Date:** [YYYY-MM-DD]
**Trigger:** [What prompted this alignment]

### Changes Made

**[Task Name]:**
- [Change 1]
- [Change 2]

**[Task Name]:**
- [Change 1]

### Patterns Enforced

- [Pattern description]
- [Pattern description]

### Consistency Verification

- [ ] Structural patterns match
- [ ] Tone follows "concise and direct"
- [ ] Templates consistent across peer tasks
- [ ] Principle references current
- [ ] Cross-references correct
```

---

## Common Alignment Patterns

### Pattern: Review Task Template

**Enforced across:** REVIEW_PLAN, REVIEW_CODE, REVIEW_IMPLEMENTATION

```markdown
**Date:** [YYYY-MM-DD]
**Status:** APPROVED | REVISE | DEFER

### Summary
[One paragraph assessment]

### Violations (Must Fix)
- **[Category]:** [Issue]
  - **Fix:** [How to address]

### Risks & Concerns
- **[Description]:** [Why risky]
  - **Mitigation:** [How to reduce]

### Actionable Items
- [ ] [Action]

### Recommendation
**Reasoning:** [Brief justification]
**Next Steps:** [What to do next]
```

### Pattern: Principle Reference

**Enforced in:** Step 1 of most PLAN/REVIEW/IMPLEMENT tasks

```markdown
### 1. Review Development Principles

Read `AGENTS.md` to ensure alignment with:
- "Clarity over cleverness"
- "Simplicity over sophistication"
- [Workflow-specific principles]
```

### Pattern: Context Gathering

**Enforced in:** Step 2 of most tasks

```markdown
### 2. Gather Context

Extract [feature|improvement] name from branch (`[workflow]/<name>`):

1. Read `PLANS/[workflow]_<name>.md`
2. Read affected source files
3. [Workflow-specific context]
```

### Pattern: Backlog Update

**Enforced in:** All FINALIZE tasks

```markdown
### [N]. Update [Workflow] Backlog

Open `PLANS/[WORKFLOW]_BACKLOG.md`:

**Move Item to Completed:**
1. Find item in current section
2. Cut and paste to "Completed" section
3. Add completion details:
   - *Completed:* [Date]
   - *Impact:* [What changed]
   - *Learnings:* [Key insights]
```

---

## Success Criteria

Alignment complete when:
- [ ] All affected tasks follow same structural pattern
- [ ] Tone matches "concise and direct" across tasks
- [ ] Templates consistent across peer tasks (review/finalize/plan)
- [ ] Principle references current with AGENTS.md
- [ ] Output format matches across similar tasks
- [ ] Cross-references correct (file paths, branch names)
- [ ] Verbosity appropriate to task complexity
- [ ] No redundant sections across workflow phases

---

## Tone & Constraints

- Systematic and thorough; consistency is the goal
- Enforce patterns, don't invent new ones
- Preserve workflow-specific differences (IMPROVE Trivial vs Standard vs FEATURE verbosity)
- Document rationale for patterns being enforced
- Use diffs/comparisons to show before/after
- Verify changes don't break workflow logic
