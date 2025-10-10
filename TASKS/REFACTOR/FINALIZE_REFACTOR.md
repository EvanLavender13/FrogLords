# Finalize Refactor

### 1. Review Development Principles

Read `AGENTS.md` to understand knowledge capture and learning principles:
- Document learnings while fresh
- Capture patterns that worked
- Note anti-patterns discovered

### 2. Gather Context

1. **Recall Refactor:** Name and category from SELECT_REFACTOR
2. **Review Execution:** Read execution summary, issues encountered
3. **Review Validation:** Read validation results from refactor plan

### 3. Update Refactor Backlog

Open `PLANS/REFACTOR_BACKLOG.md` and perform updates:

#### Move Item to Completed Section

1. **Find Item:** Locate the refactor item in its current section (Critical/High/Medium/Low)
2. **Move to Completed:** Cut the item and paste into "Completed" section
3. **Add Completion Details:**
   ```markdown
   - **[Refactor Title]**
     - *Completed:* [YYYY-MM-DD]
     - *Category:* [category]
     - *Files:* [list of modified files]
     - *Outcome:* [Brief description of what was achieved]
     - *Learnings:* [Key insights from this refactor - see section 4 below]
     - *Impact:* [Measurable improvements: e.g., "Reduced duplication from 5→1", "Clarified API usage at 12 call sites"]
   ```

#### Note Related Opportunities

If execution/validation revealed new refactor opportunities:

1. **Check if already in backlog:** Search for similar items
2. **Add new items:** Create entries in appropriate priority section
3. **Reference completed refactor:** Note what triggered this discovery
4. **Don't bundle:** Each new opportunity is a separate item

#### Update Deferred Section (If Applicable)

If similar deferred items now became viable due to this refactor:

1. **Review Deferred section:** Look for items waiting on prerequisites
2. **Update "Reconsider When":** Note that prerequisite completed
3. **Consider promoting:** If now viable, move to appropriate priority section

### 4. Document Learnings

Capture insights while fresh. Add to the completed item's "Learnings" field:

#### Pattern Learnings
- **What patterns emerged?** "Extracting X utility reduced Y pattern from N locations"
- **Abstraction quality?** "Function-level abstraction was right granularity; class would be over-engineering"
- **Reusability achieved?** "Abstraction worked for all 5 call sites without special cases"

#### Process Learnings
- **Estimate accuracy?** "Estimated 3 points, actual 4 points due to hidden GUI dependencies"
- **Risk assessment?** "Medium risk was accurate; staged approach prevented issues"
- **Plan effectiveness?** "Stage 2 migration was more complex than expected; future plans should buffer call site updates"

#### Technical Learnings
- **Hidden dependencies?** "Debug visualizations always need updating for state changes"
- **Testing insights?** "Manual testing caught edge case that code review missed"
- **Performance?** "Extracting to function had no performance impact (verified with profiling)"

#### Anti-Patterns Discovered
- **Avoid in future?** "Don't refactor systems undergoing active feature work (conflicting changes)"
- **Red flags?** "Call site count >10 requires more careful staging than initially planned"
- **Mistakes made?** "Should have searched for indirect dependencies more thoroughly"

#### Example Learning Entry
```markdown
*Learnings:*
- Staged approach essential for 15+ call sites; enabled catching migration bugs early
- Hidden dependency in character_panel required updating; add GUI check to future impact analysis
- "Rule of three" validated: pattern appeared exactly 5 times, abstraction eliminated all
- Estimated 3 points, actual 2 points; well-scoped refactors can be less complex than expected when well-planned
```

### 5. Update Documentation (If Applicable)

If refactor changes public APIs or system architecture:

#### Check Documentation Files
- [ ] `README.md` (if project-level architecture changed)
- [ ] `NOTES/` (if architectural patterns documented there)
- [ ] Code comments (if extracted function needs usage documentation)

#### Update as Needed
- Update API documentation for renamed/changed functions
- Update architecture diagrams if system boundaries changed
- Add usage examples for newly extracted utilities

**Most refactors don't need doc updates** (internal changes). Only update if:
- Public API changed
- System ownership moved
- New patterns/utilities introduced for project-wide use

### 6. Propose Next Steps

Inform user that finalization is complete:

```markdown
## Refactor Finalized: [Refactor Name]

**Completed:** [YYYY-MM-DD]
**Category:** [category]
**Impact:** [brief description]

### Changes
- Modified [N] files
- Updated [M] call sites
- [Key achievement, e.g., "Reduced duplication from 5→1 instances"]

### Learnings
- [Key insight 1]
- [Key insight 2]

### Next Steps

**Backlog Updated:**
- Moved item to Completed section in REFACTOR_BACKLOG.md
- Documented learnings and impact
- [If applicable: Added N new refactor opportunities discovered]

```

### 7. Clean Up Planning Documents (Optional)

Consider archiving planning documents:

```markdown
**Optional Cleanup:**
- Archive `PLANS/refactor_<name>.md` to `PLANS/ARCHIVE/`
- Archive `PLANS/refactor_review_<name>.md` to `PLANS/ARCHIVE/`
- Keep validation results with completed item in backlog
```

**Defer cleanup:** Can batch-archive old plans periodically rather than immediately.

### Tone & Constraints

- Concise and complete; close the loop
- Capture learnings while memory is fresh
- Measurable outcomes preferred (reduced N→M, clarified X call sites)
- Honest about estimates vs actuals (improves future planning)
- Document both successes and mistakes (learning orientation)
- Update backlog thoroughly (enables future prioritization)
- Prepare for next refactor (continuous improvement cycle)

## Success Criteria

Finalization complete when:
- [ ] Refactor item moved to Completed section in backlog
- [ ] Learnings documented comprehensively
- [ ] Impact measured and recorded
- [ ] Related opportunities captured
- [ ] Documentation updated (if applicable)
- [ ] User informed of completion and next steps
