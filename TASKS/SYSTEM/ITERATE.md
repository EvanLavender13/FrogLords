# ITERATE - Refine What Works

**Graybox proved it can work. Iteration proves it should exist.**

---

## Purpose

Refine crude but correct implementation into quality code. Simplify architecture. Extract patterns. Harden through playtesting. Prepare for external validation.

---

## Prerequisites

- [ ] Graybox functional
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

**Check for existing iterations:**

Find all iteration documents for this system:
```bash
ls PLANS/<name>_ITERATION_*.md
```

**If no iterations exist:**

Create `PLANS/<name>_ITERATION_1.md`.

**If iterations exist:**

Find highest iteration number N, read `PLANS/<name>_ITERATION_<N>.md` for VALIDATE/DECISION.

Create `PLANS/<name>_ITERATION_<N+1>.md`.

Add to iteration doc:

```markdown
<!-- BEGIN: ITERATE/CONTEXT -->
[Decision from previous iteration, required changes]
<!-- END: ITERATE/CONTEXT -->
```

---

### 2. External Review (ITERATION_1 only)

Skip if resuming from VALIDATE/REVISE.

Use Codex to review graybox code:

```bash
bash scripts/bash/codex.sh "TASKS/PLANS/<name>_SYSTEM.md PRINCIPLES.md CONVENTIONS.md TASKS/PATTERNS.md TASKS/CONTEXT/INDEX.md [relevant files] [your review prompt]"
```

**IMPORTANT:** Use `run_in_background: true` in Bash tool call.

Continue to internal review while external review runs.

---

### 3. Internal Review (ITERATION_1 only)

Skip if resuming from VALIDATE/REVISE.

Review graybox code while external review runs.

Add to iteration doc:

```markdown
<!-- BEGIN: ITERATE/INTERNAL_REVIEW -->
[Internal review findings]
<!-- END: ITERATE/INTERNAL_REVIEW -->
```

**After completing internal review, WAIT for user confirmation that external review is complete.** This takes several minutes. Do NOT monitor output. User will confirm when finished.

Continue conversation with external review if needed:
```bash
bash scripts/bash/codex.sh --resume "[follow-up question]"
```

Add external review to iteration doc:

```markdown
<!-- BEGIN: ITERATE/EXTERNAL_REVIEW -->
[External review findings]
<!-- END: ITERATE/EXTERNAL_REVIEW -->
```

---

### 4. Define Refinement Scope

Based on internal and external review findings, define actionable refinement goals.

Add to iteration doc:

```markdown
<!-- BEGIN: ITERATE/SCOPE -->
[Actionable refinement goals based on review findings]
<!-- END: ITERATE/SCOPE -->
```

---

### 5. Refine Code

Address findings from reviews and scope. Work incrementally. Build after each change. Commit when complete.

Add to iteration doc:

```markdown
<!-- BEGIN: ITERATE/REFINEMENT -->
[What changed in the code]
<!-- END: ITERATE/REFINEMENT -->
```

---

### 6. Playtest

Harden through use.

**Playtest workflow:**

1. Build
2. Claude presents test scenarios
3. User tests and reports
4. Violations found? Fix → rebuild → retest
5. All stable? Commit

Repeat until stable.

Add to iteration doc (multiple playtest sections allowed):

```markdown
<!-- BEGIN: ITERATE/PLAYTEST -->
[Playtest findings and fixes]
<!-- END: ITERATE/PLAYTEST -->
```

Stop when stable through multiple sessions.

---

### 7. Commit

Commit iteration document:

```bash
git add PLANS/<name>_ITERATION_<N>.md
git commit -m "iterate: <name> iteration <N> complete

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

---

## Outputs

- [ ] Internal review completed
- [ ] External review completed (ITERATION_1 only)
- [ ] Scope defined
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
