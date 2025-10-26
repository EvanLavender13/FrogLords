# ITERATE - Refine What Works

**Graybox proved it can work. Iteration proves it should exist.**

---

## Purpose

Refine crude but correct implementation into quality code. Simplify architecture. Extract patterns. Harden through playtesting. Prepare for external validation.

---

## Prerequisites

- [ ] Graybox functional
- [ ] Debug visualization working
- [ ] Build successful
- [ ] `PLANS/<name>_SYSTEM.md` exists with GRAYBOX/RESULTS

---

## Anti-Patterns

Review @TASKS/PATTERNS.md for known anti-patterns.

Prevention is cheaper than detection.

**Documentation-only changes:** Do not create iterations for doc sync. Edit system doc directly and commit.

---

## Context Selection

Review @TASKS/CONTEXT/INDEX.md to identify relevant architectural patterns.

Use tags to find applicable context cards before beginning refinement.

---

## Process

### 1. Create or Resume Iteration Document

**Check `PLANS/<name>_SYSTEM.md` for iteration list:**

**If no iteration list exists (first iteration):**

Create `PLANS/<name>_ITERATION_1.md`:

```markdown
# Iteration 1: <name>

**Started:** YYYY-MM-DD
**Status:** In Progress

---

<!-- BEGIN: ITERATE/SCOPE -->
## Refinement Scope

What must improve from GRAYBOX:

- [ ] Code quality (simplification, dead code removal, clarity)
- [ ] Architecture (pattern extraction, principle alignment)
- [ ] Stability (playtest hardening, edge case handling)
- [ ] Performance (obvious waste elimination)
<!-- END: ITERATE/SCOPE -->

---
```

Add iteration list to `PLANS/<name>_SYSTEM.md` (after GRAYBOX/RESULTS):

```markdown
---

## Iterations

- [ITERATION_1.md](ITERATION_1.md) - In Progress
```

**If iteration list exists (resuming from VALIDATE/REVISE):**

Read most recent `PLANS/<name>_ITERATION_<N>.md` to find VALIDATE/DECISION.

Create `PLANS/<name>_ITERATION_<N+1>.md`:

```markdown
# Iteration <N+1>: <name>

**Started:** YYYY-MM-DD
**Previous:** [ITERATION_<N>.md](ITERATION_<N>.md)
**Status:** In Progress

---

<!-- BEGIN: ITERATE/CONTEXT -->
## Context from Previous Iteration

**Decision:** REVISE (from ITERATION_<N>)

**Required changes:**
- [copy from ITERATION_<N> VALIDATE/DECISION]
<!-- END: ITERATE/CONTEXT -->

---
```

Update iteration list in `PLANS/<name>_SYSTEM.md`:

```markdown
## Iterations

- [ITERATION_1.md](ITERATION_1.md) - REVISE
- [ITERATION_<N+1>.md](ITERATION_<N+1>.md) - In Progress
```

---

### 2. External Review (ITERATION_1 only)

**Get a second opinion on the crude implementation before refining.**

**Skip this step if resuming from VALIDATE/REVISE** - use findings from VALIDATE/DECISION instead.

**For ITERATION_1 only:**

Use Codex to review actual code:

```bash
bash scripts/bash/codex.sh "TASKS/PLANS/<name>_SYSTEM.md PRINCIPLES.md CONVENTIONS.md TASKS/PATTERNS.md TASKS/CONTEXT/INDEX.md [relevant files] [your review question]"
```

**IMPORTANT:** Use `run_in_background: true` in Bash tool call.

**WAIT for user confirmation that review is complete.** This takes several minutes. Do NOT monitor output. User will confirm when finished.

**Continue conversation if needed:**
```bash
bash scripts/bash/codex.sh --resume "[follow-up question]"
```

Document in iteration doc (`PLANS/<name>_ITERATION_1.md`):

```markdown
<!-- BEGIN: ITERATE/EXTERNAL_REVIEW -->
## External Review (Codex)

**Date:** YYYY-MM-DD

**Question asked:**
[What you asked for review]

**Findings:**
- [Key findings from Codex]

**Priority issues:**
- [What must be addressed in refinement]
<!-- END: ITERATE/EXTERNAL_REVIEW -->
```

---

### 3. Internal Review

**Evaluate against context patterns.**

Review @TASKS/CONTEXT/INDEX.md for relevant cards based on:
- Codex feedback (ITERATION_1)
- VALIDATE/DECISION findings (ITERATION_N from REVISE)

Document in iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: ITERATE/INTERNAL_REVIEW -->
## Internal Review

**Context cards reviewed:**
- [list relevant cards from CONTEXT/INDEX.md]

**Patterns identified:**
- [good patterns to preserve]

**Anti-patterns detected:**
- [violations found, specific to context cards]

**Simplification opportunities:**
- [what can be removed or simplified]
<!-- END: ITERATE/INTERNAL_REVIEW -->
```

---

### 4. Refine Code

**Address findings from internal review.**

Work incrementally:
1. **Simplify** - Remove unnecessary complexity
2. **Extract** - Identify and extract reusable patterns
3. **Align** - Fix principle violations
4. **Optimize** - Remove obvious waste

**Build after each change.**

**Commit when complete:**
```bash
git commit -m "iterate: <name> - <what changed>

Simplified: <what>
Extracted: <patterns>
Aligned: <principles>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

Document changes in iteration doc:

```markdown
<!-- BEGIN: ITERATE/REFINEMENT -->
## Code Refinement

**Simplifications:**
- [what was removed or simplified]

**Extractions:**
- [patterns extracted]
- [new primitives/utilities created]

**Alignments:**
- [principle violations fixed]

**Performance:**
- [waste eliminated]

**Commit:** [hash]
<!-- END: ITERATE/REFINEMENT -->
```

---

### 5. Playtest

**Harden through use.**

**Playtest workflow:**

1. **Build**
2. **Claude presents test scenarios** covering:
   - Core functionality
   - Edge cases
   - Stress conditions
   - Feel/tuning
3. **User tests and reports**:
   - Violations
   - Unexpected behaviors
   - Feel issues
4. **Violations found?**
   - Fix → rebuild → retest
   - Document in playtest section
5. **All stable?** → Commit

**Repeat until stable.**

Document in iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: ITERATE/PLAYTEST -->
### Playtest <N>

**Date:** YYYY-MM-DD
**Tester:** [name]

**Violations:**
- [x] [issue description]
  - Root cause: [why]
  - Fix: [what changed] ✓ FIXED

**Emergent:**
- [unexpected behaviors]

**Feel:**
- [tuning adjustments made]

**Commit:** [hash]
<!-- END: ITERATE/PLAYTEST -->
```

**Stop when stable through multiple sessions.**

---

### 6. Document Completion

Add to iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Refinements:** [summary]
**Playtests:** [count]
**Stability:** ✓ PROVEN

**Ready for VALIDATE:**
- [x] Code quality improved
- [x] Architecture reviewed
- [x] Stable through playtesting
- [x] Performance acceptable
<!-- END: ITERATE/COMPLETE -->
```

---

### 7. Update Status and Commit

Update iteration doc status:
```markdown
**Status:** Ready for VALIDATE
```

Update `PLANS/<name>_SYSTEM.md` iteration list:
```markdown
- [ITERATION_<N>.md](ITERATION_<N>.md) - Ready for VALIDATE
```

Commit:
```bash
git add PLANS/<name>_ITERATION_<N>.md PLANS/<name>_SYSTEM.md
git commit -m "iterate: <name> iteration <N> complete

Refinements: <summary>
Playtests: <count>
Status: ready for validation

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

---

## Outputs

- [ ] Internal review completed
- [ ] Code refined
- [ ] Stable through playtests
- [ ] Committed
- [ ] Ready for VALIDATE

---

## Next

→ VALIDATE (external principle review)

**Defer if:**
- Refinement reveals fundamental flaws
- Architecture cannot be salvaged
- System should not exist

---

**Crude code validates the idea. Refined code validates the implementation.**
