# RETRO - Retrospective Analysis

**Extract patterns. Simplify process. Delete bloat.**

---

## Purpose

Find patterns (≥3 occurrences). Create context files for technical details. Update workflow to prevent friction. **Almost never update PRINCIPLES.md.**

---

## Prerequisites

- ≥5 completed plans in `TASKS/PLANS/`
- No active work in progress

---

## Process

### 1. Scan Plans - Extract Patterns

Read "Reflection" section from each completed plan. Count occurrences of:
- Same principle violated
- Same mistake repeated
- Same workflow friction
- Same confusion

**Also audit document quality** - measure each plan file:
- LOC changed in code vs. LOC in documentation
- Plans >500 lines for <50 LOC changes = bloat
- Repetitive sections (saying same thing multiple ways)
- Redundancy across metrics/reflection/summary

**≥3 occurrences = pattern. <3 = noise (defer).**

### 2. Categorize Patterns

**A. Principle Violations** - Which principles broken repeatedly?
**B. Emergent Behaviors** - What unexpected interactions worked?
**C. Workflow Friction** - Which steps painful/unclear?
**D. Documentation Gaps** - What confusion repeated?
**E. Documentation Bloat** - Which plans violate Radical Simplicity in their own writing?

### 3. Generate Actions

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

### 4. Document Findings

**Create `TASKS/PLANS/RETRO_<date>.md` - Keep it SHORT:**

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
- Actions: <count> context files, <count> workflow updates, <count> backlog
- Execute: Context files first, then workflow updates
- Is workflow working? <brief reflection>

---
```

**Keep document under 100 lines. Delete bloat.**

---

## Exit Criteria

**Retro complete when:**
- [ ] Patterns extracted (≥3 occurrences only)
- [ ] Actions listed (context files, workflow updates, backlog)
- [ ] Document <100 lines
- [ ] PRINCIPLES.md updates rare or none

**→ Execute actions OR continue with REFINE/SELECT or SYSTEM/SELECT**

---

## Philosophy

**≥3 occurrences = pattern. <3 = noise, defer.**

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

## Document Quality

**Check each plan:**
- LOC ratio: 800 lines documenting 4 LOC = bloat
- Redundancy: Same metrics repeated 3+ times = delete
- Verbosity: Can you say it in half the words?

**Fix:** Delete sections. Merge duplicates. State facts once.

**Radical Simplicity applies to documentation.**

---

**Extract patterns. Create context files. Simplify workflow. Rarely touch PRINCIPLES.**
