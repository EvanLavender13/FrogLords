# Review Codebase for Improvement Items

### 1. Read Guiding Principles

Read `AGENTS.md` to ensure all evaluations align with:
- Development principles (clarity, simplicity, elegance)
- Layered architecture (Foundation → Character → Rendering → App)
- Code standards (snake_case, naming conventions)
- Quality guidelines (rule of three, stability gates)

### 2. Perform Systematic Review

Scan `src/` directory focusing on `.cpp` and `.h` files across all subsystems.

**Categories to Examine:**

#### Architecture & Dependencies
- Reverse dependencies (lower layers including higher layers)
- Circular dependencies
- Violated layering (Rendering depending on App, Character depending on Rendering, etc.)
- Tight coupling where loose coupling preferred

#### Code Quality
- Duplicate logic (3+ identical/similar patterns)
- Dead code (commented blocks, unused functions, orphaned systems)
- Debugging leftovers (printf, debug logging, temporary hacks)
- Magic numbers needing named constants
- Complex logic needing decomposition

#### Conventions & Style
- Naming violations (camelCase/PascalCase instead of snake_case)
- Inconsistent constant scoping (file-level vs function-level)
- Missing const-correctness
- Unused includes or excessive dependencies
- Inconsistent formatting (should be caught by clang-format)

#### Principles
- Reactive systems controlling core logic (violates layered approach)
- Manual animation variants instead of procedural generation
- Content restrictions limiting future flexibility
- Missing abstractions (parameters vs assets)

#### Documentation
- Stale comments after code changes
- Missing comments on non-obvious public API
- Over-commented obvious code
- Misleading or contradictory comments

### 3. Categorize Findings

**Severity/Priority Guide:**

**Critical** (Fix immediately):
- Architectural violations (layer dependencies reversed)
- Correctness bugs
- Principle violations blocking future work

**High** (Fix soon):
- Significant tech debt
- Pattern extraction ready for use (3+ occurrences)
- Code smells impacting maintainability

**Medium** (Nice to have):
- Quality improvements
- Minor inconsistencies
- Single-use helper opportunities

**Low** (Polish):
- Nitpicks
- Cosmetic improvements
- Convention alignment

### 4. Verify Rule of Three

**For pattern extractions:**
1. Count occurrences of pattern
2. Verify pattern is identical (not just similar)
3. Only flag if 3+ occurrences OR canonical data consolidation

**Exception:** Critical architectural issues bypass rule of three.

### 5. Check Stability

Cross-reference `PLANS/DEPENDENCY_STACK.md`:
- Note systems at ≥70% certainty (ready for improvement)
- Flag systems <70% (defer improvements until stable)
- Mark systems under active feature development (avoid touching)

### 6. Document in Improve Backlog

Open `PLANS/IMPROVE_BACKLOG.md` and add findings:

#### Structured Entry Format

```markdown
### [Subsystem] / [Component]
**[Issue Title]**
- *File(s):* `src/path/to/file.cpp:123` [specific line/range]
- *Issue:* [Concise description of problem]
- *Fix:* [Specific proposed improvement]
- *Rationale:* [Why this matters - cite AGENTS.md principles]
- *Complexity:* [1-8 points]
- *Tags:* #architecture | #cleanup | #pattern-extraction | #simplification | #conventions | etc.
```

#### Group by Subsystem

- Character (controller, locomotion, orientation, tuning, animation)
- Rendering (debug_draw, scene, renderers)
- Foundation (collision, easing, springs, math helpers)
- App (game_world, runtime)
- Input
- GUI
- Camera

#### Tag Appropriately

- `#architecture` - Layer violations, dependency issues
- `#pattern-extraction` - Duplicate code ready for abstraction
- `#cleanup` - Dead code, unused includes, debugging leftovers
- `#simplification` - Reduce complexity without changing behavior
- `#constants` - Magic numbers, duplicate constants
- `#conventions` - Naming, formatting, style inconsistencies
- `#includes` - Include dependency management
- `#deferred` - Not ready yet (needs stability or prerequisites)
- `#batch-candidate` - Similar to other items, good for batching

### 7. Note Batch Opportunities

If 3+ similar items found:
- Group them in the backlog
- Add `#batch-candidate` tag
- Note: "Batch opportunity: fix all together"

Examples:
- Multiple unused includes across files
- Several magic numbers in same system
- Related naming violations

### 8. Summarize Findings

**Report to user:**

```
Codebase review complete. Found X items:

Critical: N
- [Brief item 1]
- [Brief item 2]

High: N
- [Brief item 1]

Medium: N
Low: N

Batch opportunities: N
- [Group description]

Stability notes:
- [Systems that are/aren't ready for improvement]

Backlog updated: PLANS/IMPROVE_BACKLOG.md
```

### 9. Propose Next Steps

**If Critical items found:**
- Recommend immediate selection of Critical items
- Flag architectural violations as blocking

**If only High/Medium/Low:**
- Suggest interleaving improvements with feature work
- Note best candidates based on stability and impact

**If batch opportunities:**
- Suggest batching related trivial fixes

---

## Tone & Constraints

- Concise and direct
- Concrete, actionable findings over opinions
- Cite specific file paths and line ranges
- Detail scales with severity (Critical gets more explanation)
- Avoid bikeshedding; flag real issues only
- Document findings, don't implement during review
- Respect stability gates (note but don't push improvements on unstable code)
- Be conservative with Critical severity (reserve for true violations)

---

## Quality Checks

**Before finalizing backlog update:**

- [ ] All items have concrete file references
- [ ] Severity/priority assignments justified
- [ ] Proposed fixes are specific and actionable
- [ ] Tags applied appropriately
- [ ] Complexity estimates reasonable
- [ ] Rule of three verified for pattern extractions
- [ ] Stability checked against DEPENDENCY_STACK.md
- [ ] Batch opportunities identified
- [ ] Principles cited where applicable

---

## Integration Notes

- Run REVIEW_CODEBASE between feature iterations
- After completing features, check for patterns to extract
- Periodically (e.g., monthly) to catch drift
- When onboarding to learn codebase structure
- Before major refactors to understand current state
