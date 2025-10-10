# Finalize Refactor

### 1. Review Development Principles

Read `AGENTS.md` to understand knowledge capture principles:
- Document learnings while fresh
- Capture patterns that worked
- Note anti-patterns discovered

### 2. Gather Context

Extract refactor name from branch (`refactor/<refactor_name>`):

1. Read `PLANS/refactor_<refactor_name>.md`
2. Review execution summary and issues encountered
3. Review validation results from plan

### 3. Run Final Validation

Execute the validation checklist from the refactor plan:
- [ ] All test scenarios pass
- [ ] No regressions in affected systems
- [ ] Debug visualizations work
- [ ] Performance characteristics preserved
- [ ] [Any other checks from plan]

### 4. Update Refactor Backlog

Open `PLANS/REFACTOR_BACKLOG.md`:

**Move Item to Completed:**
1. Find item in current section (Critical/High/Medium/Low)
2. Cut and paste to "Completed" section
3. Add completion details:

```markdown
- **[Refactor Title]**
  - *Completed:* [YYYY-MM-DD]
  - *Category:* [category]
  - *Files:* [modified files]
  - *Outcome:* [What was achieved]
  - *Learnings:* [Key insights - see section 5]
  - *Impact:* [Measurable improvements: "Reduced duplication 5→1", "Clarified API at 12 call sites"]
```

**Note Related Opportunities:**
If execution revealed new refactor opportunities:
1. Check if already in backlog
2. Add new items to appropriate priority section
3. Reference completed refactor that triggered discovery
4. Don't bundle (each is separate item)

**Update Deferred Section:**
If similar deferred items became viable due to this refactor:
1. Review Deferred section for items waiting on prerequisites
2. Update "Reconsider When" notes
3. Consider promoting to priority section if now viable

### 5. Document Learnings

Capture insights while fresh (add to completed item's "Learnings"):

**Pattern Learnings:**
- What patterns emerged?
- Was abstraction at right granularity?
- Did reusability work for all call sites?

**Process Learnings:**
- Was estimate accurate? Why/why not?
- Was risk assessment accurate?
- Did plan work as expected?

**Technical Learnings:**
- Hidden dependencies discovered?
- Testing insights?
- Performance impacts?

**Anti-Patterns Discovered:**
- What to avoid in future?
- Red flags to watch for?
- Mistakes made?

**Example:**
```markdown
*Learnings:*
- Staged approach essential for 15+ call sites; caught migration bugs early
- Hidden dependency in character_panel; add GUI check to future impact analysis
- "Rule of three" validated: 5 instances, abstraction eliminated all
- Estimated 3 points, actual 2 points; well-planned refactors can be simpler
```

### 6. Update Documentation (If Applicable)

**Only if refactor changes public APIs or system architecture:**

Check and update:
- [ ] `README.md` (project-level architecture changes)
- [ ] `NOTES/` (architectural patterns)
- [ ] Code comments (extracted functions needing usage docs)

**Most refactors don't need doc updates.** Only update if:
- Public API changed
- System ownership moved
- New patterns/utilities for project-wide use

### 7. Append Finalization Status

Append to `PLANS/refactor_<refactor_name>.md`:

```markdown
---

## Finalization

**Date:** [YYYY-MM-DD]
**Status:** COMPLETED

**Final Validation Results:**
- [Validation checklist results]

**Documentation Updates:**
- [Docs updated]
[Or "None required"]

**Next Steps:**
- Moved to Completed in REFACTOR_BACKLOG.md
- Learnings documented in backlog entry
```

### 8. Propose Next Steps

Inform user of completion:

```markdown
## Refactor Finalized: [Refactor Name]

**Completed:** [YYYY-MM-DD]
**Impact:** [Brief description]

### Changes
- Modified [N] files
- Updated [M] call sites
- [Key achievement]

### Learnings
- [Key insight 1]
- [Key insight 2]

### Next Steps

**Backlog Updated:**
- Moved to Completed in REFACTOR_BACKLOG.md
- Documented learnings and impact
- [If applicable: Added N new opportunities]

**Planning Document:**
- `PLANS/refactor_<name>.md` contains complete history
```

### Tone & Constraints

- Concise and complete; close the loop
- Capture learnings while memory fresh
- Measurable outcomes preferred (reduced N→M, clarified X call sites)
- Honest about estimates vs actuals (improves future planning)
- Document both successes and mistakes
- Update backlog thoroughly
- Prepare for next refactor (continuous improvement)

### Success Criteria

Finalization complete when:
- [ ] Refactor moved to Completed in backlog
- [ ] Learnings documented comprehensively
- [ ] Impact measured and recorded
- [ ] Related opportunities captured
- [ ] Documentation updated (if applicable)
- [ ] User informed of completion
