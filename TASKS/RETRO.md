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

Read completed plans in `TASKS/PLANS/`. Find what repeats.

**≥3 occurrences = pattern. <3 = noise (defer).**

### 2. Extract Important Ideas

Find insights that generalize beyond specific plans. These become backlog items.

### 3. Categorize Patterns

Group patterns by type.

### 4. Generate Actions

For each pattern, decide the fix:

**Context files** - Technical details and exceptions
**Workflow updates** - Fix friction, simplify steps
**Backlog updates** - New work discovered
**PRINCIPLES.md** - Rare. Only if principle itself is wrong
**Defer** - Acceptable when unclear or low impact

**Delete > Simplify > Document**

---

### 5. Document Findings

Create `TASKS/PLANS/RETRO_<date>.md`. Keep it concise - patterns found, actions taken.

**Keep document under 150 lines. Delete bloat.**

---

### 6. Archive and Commit

```bash
# Archive completed plans
mkdir -p TASKS/RETROS/RETRO_$(date +%Y-%m-%d)
mv TASKS/PLANS/*.md TASKS/RETROS/RETRO_$(date +%Y-%m-%d)/
```

Clear retrospective notification from `CURRENT_PLAN.md`.

```bash
git add TASKS/RETROS/ TASKS/CURRENT_PLAN.md TASKS/PLANS/
git commit -m "retro: <brief summary>

[concise description]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Outputs

- [ ] Patterns extracted (≥3 occurrences)
- [ ] Important ideas captured
- [ ] Actions listed
- [ ] `RETRO_<date>.md` created (<150 lines)
- [ ] Plans archived
- [ ] Retro committed

---

## Next

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
