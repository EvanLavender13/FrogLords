# RETRO - Retrospective Analysis

**Extract patterns. Simplify process. Delete bloat.**

---

## Purpose

Find patterns (≥3 occurrences). Create context files for technical details. Update workflow to prevent friction. **Almost never update PRINCIPLES.md.**

---

## Prerequisites

- ≥5 completed plans in `TASKS/PLANS/` (refinements or systems)
- No active work in progress

---

## Process

### 1. Scan Plans - Extract Patterns

**For refinement plans (`REFINE_*.md`):**
- Read "Reflection" and "Learning" sections

**For system plans (`<name>_ITERATION_*.md`):**
- Read "VALIDATE/EMERGENCE" sections (learning, surprising behaviors)
- Read "VALIDATE/SCORES" sections (principle violations, low scores)
- Read "ITERATE/PLAYTEST_*" sections (violations found during playtests)

**Count occurrences across all plans:**
- Same principle violated
- Same mistake repeated
- Same workflow friction
- Same confusion

**Also audit document quality** - measure each plan file:
- Repetition within document (same info stated multiple times)
- Redundancy across sections (metrics/reflection/summary duplicate each other)
- Verbosity (can sections be said in half the words?)
- Trivial changes with excessive process documentation (2-line fixes with 200-line plans)

**Note:** Multi-file refactors and architectural changes naturally require more documentation than their net LOC suggests. Judge by repetition and redundancy, not by ratio alone.

**≥3 occurrences = pattern. <3 = noise (defer).**

### 2. Extract Important Ideas

**For refinement plans:**
- Look in "Learning" sections

**For system plans:**
- Look in "VALIDATE/EMERGENCE" sections (what was learned, what emerged)
- Look in final iteration's "Learned" subsection

**Extract:**
- Patterns discovered that generalize
- Validation/testing approaches proposed
- Infrastructure decisions deferred
- Future work identified (what the system enables)
- System audits suggested

**These become backlog items** - refinements, systems, or infrastructure tasks.

### 3. Categorize Patterns

**A. Principle Violations** - Which principles broken repeatedly?
**B. Emergent Behaviors** - What unexpected interactions worked?
**C. Workflow Friction** - Which steps painful/unclear?
**D. Documentation Gaps** - What confusion repeated?
**E. Documentation Bloat** - Which plans violate Radical Simplicity in their own writing?
**F. Important Ideas** - New patterns, systems, or infrastructure identified?

### 4. Generate Actions

**For each pattern (≥3 occurrences):**

**PRINCIPLES.md Update (RARE):**
- Only if principle itself is wrong or unclear
- NOT for technical exceptions or details
- Example: "Principle needs complete rewrite" or "Principle contradicts itself"

**Context File (COMMON):**
- Technical details go here, not PRINCIPLES
- Examples: `ACCUMULATED_STATE.md`, `PRIMITIVES_GUIDE.md`
- Inject into CLAUDE.md if high-value

**Workflow Update (COMMON):**
- Fix friction: unclear steps, missing checklists, wrong templates
- Direct changes: "Add step X", "Delete section Y", "Clarify Z"

**Backlog Update (COMMON):**
- New refinements/systems discovered
- Priority updates based on pattern frequency

**Defer (ACCEPTABLE):**
- <3 occurrences = noise
- Unclear action = needs more data
- Low impact = not worth effort

**Delete > Simplify > Document**

---

### 5. Document Findings

**Create `TASKS/PLANS/RETRO_<date>.md` - Keep it SHORT:**

**Note:** If `RETRO_<date>.md` already exists, use `RETRO_<date>_2.md`, `RETRO_<date>_3.md`, etc.

```markdown
# Retrospective: <Date>

**<N> plans analyzed. <X> patterns found.**

---

## Patterns (≥3 occurrences)

**Principle Violations:**
1. <Pattern name> (×<count>) - <systems>
2. ...

**Emergent Behaviors:**
1. <Pattern name> (×<count>) - <what worked>
2. ...

**Workflow Friction:**
1. <Pattern name> (×<count>) - <task affected>
2. ...

**Documentation Gaps:**
1. <Pattern name> (×<count>) - <what's missing>
2. ...

**Deferred (<3 occurrences):**
1. <Pattern name> (×2) - revisit when <condition>

---

## Important Ideas from Plans

**<Idea Name>** (<Source Plan>):
- <Key insight 1-liner>
- <Application/generalization>
- <Next steps or backlog item created>

---

## Actions

**Context Files (COMMON):**
- Create `<FILE>.md` for <technical details>
- Inject into CLAUDE.md: Yes/No

**Workflow Updates (COMMON):**
- `<TASK>.md`: <direct change>
- `<TASK>.md`: <direct change>

**Backlog Updates (COMMON):**
- Add refinement: <name>
- Update priority: <name>

**PRINCIPLES.md Updates (RARE - only if principle itself is wrong):**
- None (technical details go in context files)

---

## Summary

- Patterns: <count>
- Important Ideas: <count>
- Actions: <count> context files, <count> workflow updates, <count> backlog
- Execute: Context files first, then workflow updates
- Work status: <X completed, Y deferred/rejected> - Distinguish completion from process success
- Workflow status: <brief reflection on whether process caught issues appropriately>

---
```

**Keep document under 150 lines. Delete bloat.**

---

### 6. Archive Plans and Clear Notification

**Archive completed plans:**
```bash
# Create retro archive directory (reuse if exists)
mkdir -p TASKS/RETROS/RETRO_$(date +%Y-%m-%d)

# Move all plans (refinements, systems, iterations, and retro doc itself)
mv TASKS/PLANS/*.md TASKS/RETROS/RETRO_$(date +%Y-%m-%d)/
```

**Clear retrospective notification from CURRENT_PLAN.md:**

Remove the "Recommended Next Action" section if present:
```markdown
## Recommended Next Action

**📊 RETRO recommended** - <N> plans completed since start
**Run:** Review TASKS/RETRO.md and extract patterns

**Or proceed with:**
- `/REFINE/SELECT` - Pick next violation to fix
- `/SYSTEM/SELECT` - Pick next system to build
```

**Commit:**
```bash
git add TASKS/RETROS/ TASKS/CURRENT_PLAN.md
git commit -m "retro: <brief summary of patterns found>

<N> plans analyzed, <X> patterns extracted
Actions: <brief action summary>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Exit Criteria

**Retro complete when:**
- [ ] Patterns extracted (≥3 occurrences only)
- [ ] Important ideas captured (new systems, patterns, infrastructure)
- [ ] Actions listed (context files, workflow updates, backlog)
- [ ] Document <150 lines
- [ ] PRINCIPLES.md updates rare or none
- [ ] Plans archived to `RETROS/RETRO_<date>/`
- [ ] Retrospective notification removed from CURRENT_PLAN.md
- [ ] Retro committed

**→ Execute actions OR continue with REFINE/SELECT or SYSTEM/SELECT**

---

## Philosophy

Repetition reveals truth. Individual events teach nothing.

**Workflow success (5/5 plans completing smoothly) is not a pattern to fix.**

**PRINCIPLES.md is philosophy, not technical manual.**

Technical details belong in context files. Exceptions belong in context files. Examples belong in context files.

Only update PRINCIPLES.md if the principle itself is wrong.

**The best retrospective deletes more than it adds.**

Delete workflow steps. Simplify templates. Remove friction.

**Defer freely.**

No changes needed is valid. It means the process works.

---

**Extract patterns. Create context files. Simplify workflow. Rarely touch PRINCIPLES.**
