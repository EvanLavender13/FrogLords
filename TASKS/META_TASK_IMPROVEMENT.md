# Task System Improvement Proposal

**Problem:** Tasks are too verbose, causing bloated outputs and unnecessary checklists.

**Root cause:** Philosophy gets repeated in tasks AND outputs instead of living in PRINCIPLES.md.

---

## Current Issues

1. **Repetitive philosophy** - Every task re-explains the six pillars
2. **Unused checklists** - Many validation lists never get checked
3. **Verbose outputs** - Documents inherit task verbosity
4. **Hard to scan** - Takes too long to find actual instructions
5. **Off-track drift** - Too much context causes overthinking

---

## Proposed Structure

### PRINCIPLES.md (Beef Up)
**Role:** Comprehensive philosophical foundation (read once, internalize)

Add sections:
- **How to Apply Each Pillar** (concrete examples)
- **Common Anti-Patterns** (what violates principles)
- **Decision Framework** (flowchart: when stuck, ask these questions)
- **Validation Checklist** (single master checklist for ALL features)

**Length:** As long as needed (one-time read, reference document)

### TASKS/*.md (Streamline)
**Role:** Concise, actionable instructions

New format:
```markdown
# Task Name

**Purpose:** One sentence.

## Principles Check
- Pillar X: How it applies to THIS task specifically (1-2 sentences)
- Pillar Y: How it applies (1-2 sentences)

## Prerequisites
- Thing 1
- Thing 2

## Steps
1. Do X (what, not why)
2. Do Y
3. Validate: Check Z

## Output
- Deliverable 1
- Deliverable 2

## Red Flags
- If you see X, stop and reconsider
- If Y happens, you violated Pillar Z
```

**Length:** 1-2 pages max

---

## Example Refactor

### BEFORE (Current EXECUTE task - ~420 lines)
```markdown
# Execute Feature

**Build truth, validate continuously, advance deliberately.**

---

## The Purpose
Implement validated stages from the approved plan...
**The best execution reveals the plan was wrong...**

## Prerequisites
- Approved plan from REVIEW_PLAN
- Foundation certainty ≥90%
...

## Phase 1: Pre-Execution Validation
### Gather Truth
1. Read PRINCIPLES.md - internalize the Six Pillars
...

## Phase 2: Execute Planned Stages
### For Each Stage:
#### 1. Pre-Stage Validation
...
#### 2. Implement the Mathematics
...
[continues for 400+ more lines]
```

### AFTER (Proposed)
```markdown
# Execute Feature

**Purpose:** Implement approved plan stages with validation at each step.

## Principles Check
- **Simplicity:** Build only what the stage requires, nothing extra
- **Mathematics:** Validate formulas before integrating
- **Consistency:** Each stage must leave system in working state

## Prerequisites
- [ ] Plan approved (REVIEW_PLAN complete)
- [ ] Foundation certainty ≥90%
- [ ] Debug visualization available

## Steps

### Pre-Execution
1. Read plan: `PLANS/<feature>_PLAN.md`
2. Read review: `PLANS/<feature>_PLAN_REVIEW.md`
3. Verify prerequisites above

### For Each Stage
1. **Implement** minimal code for stage
2. **Validate** mathematics (numeric tests, edge cases)
3. **Visualize** if needed (debug tools)
4. **Mark complete** in plan doc with:
   - Timestamp
   - What worked
   - What surprised you

### Post-Execution
1. Update plan with `## EXECUTION COMPLETE` section
2. Document deviations (if any)
3. Note emergent discoveries

## Output
- Code changes (per plan)
- Updated plan doc (execution notes)
- Working, tested system

## Red Flags
- Stage skipping → violates validation principle
- Scope creep ("while I'm here...") → violates simplicity
- No visualization → violates "can it be seen?"
- Math not verified → violates foundations
```

**Length:** ~50 lines vs 420 lines (90% reduction)

---

## Benefits

1. **Faster execution** - Find instructions quickly
2. **Less bloat** - Outputs only document actual work
3. **Clear focus** - Philosophy in PRINCIPLES, instructions in TASKS
4. **Reusable checklists** - Master list in PRINCIPLES, reference when needed
5. **Easier maintenance** - Update philosophy once, not in every task

---

## Migration Plan

### Phase 1: Update PRINCIPLES.md
- Add "Application Guide" section
- Add "Master Validation Checklist"
- Add "Anti-Pattern Gallery"
- Add "Decision Flowcharts"

### Phase 2: Refactor TASKS
Priority order:
1. `TASKS:FEATURE:EXECUTE` (most verbose, most used)
2. `TASKS:FEATURE:PLAN` (second most verbose)
3. `TASKS:FEATURE:REVIEW_PLAN`
4. Others as needed

### Phase 3: Test
Run one feature end-to-end with new system, verify:
- Faster to read and execute
- Outputs are concise
- Philosophy still maintained
- Nothing important lost

---

## Key Insight

**The workflow itself must follow the principles!**

### Current Violations
1. **Simplicity** ✗ - Tasks are bloated, unjustified complexity
2. **Composability** ✗ - Philosophy mixed into tasks (not orthogonal)
3. **Consistency** ✗ - Multiple sources of truth (principles repeated everywhere)

### Proposed Alignment
1. **Simplicity** ✓ - Each piece minimal and justified (PRINCIPLES = philosophy, TASKS = process)
2. **Composability** ✓ - Orthogonal concerns (read principles once, apply via tasks)
3. **Consistency** ✓ - Single source of truth (PRINCIPLES.md is canonical)
4. **Emergence** ✓ - Framework enables good work without prescribing every detail
5. **Mathematics** ✓ - Process is verifiable (checklists in PRINCIPLES, apply when needed)
6. **Development** ✓ - Workflow decisions traceable to principles (this document!)

**Separation of concerns:**
- PRINCIPLES = WHY (philosophy, deep understanding)
- TASKS = HOW (process, concrete steps)
- CODE = WHAT (implementation)

Currently we're mixing WHY into HOW, violating composability and simplicity.

---

## Recommendation

**Start with PRINCIPLES.md enhancement**, then refactor EXECUTE task as proof of concept.

If it works well, apply to other tasks incrementally.

**Goal:** Maintain philosophical rigor, eliminate philosophical repetition.

---

**This document:** 150 lines (demonstrating the target conciseness)
