# VALIDATE - Principle Evaluation

**Principles judge all work equally. No exceptions.**

---

## Purpose

Evaluate system against principles. External validation. Approve, revise, or reject.

---

## Prerequisites

- Read @TASKS/CURRENT_PLAN.md
- `TASKS/PLANS/<name>_ITERATION_<N>.md` has ITERATE/COMPLETE

---

## Process

### 0. Understand the Implementation

**Read the artifacts:**

```
- PLANS/<name>_ITERATION_<N>.md
- PLANS/<name>_SYSTEM.md
```

**Identify:**
- What was actually built
- Which files were modified
- Whether prerequisites are met

**Do not proceed until you understand what exists.**

---

### 1. External Review

**Dual independent principle evaluation (Codex + Gemini).**

**Critical:** Plans are tools for thinking, not contracts. Code is truth. Do not flag doc-code divergence—implementation evolves, docs capture decisions at a moment.

Launch both reviews in parallel:

```bash
bash scripts/bash/codex.sh "TASKS/PLANS/<name>_ITERATION_<N>.md TASKS/PLANS/<name>_SYSTEM.md PRINCIPLES.md CONVENTIONS.md TASKS/PATTERNS.md TASKS/CONTEXT/INDEX.md [relevant files] [strong validation prompt]"

bash scripts/bash/gemini.sh "TASKS/PLANS/<name>_ITERATION_<N>.md TASKS/PLANS/<name>_SYSTEM.md PRINCIPLES.md CONVENTIONS.md TASKS/PATTERNS.md TASKS/CONTEXT/INDEX.md [relevant files] [strong validation prompt]"
```

**IMPORTANT:** Use `run_in_background: true` in both Bash tool calls.

**WAIT for user confirmation that both reviews are complete.** This takes several minutes. Do NOT monitor output. User will confirm when finished.

**Continue conversation if needed:**
```bash
bash scripts/bash/codex.sh --resume "[follow-up prompt]"
```

**After user confirms completion,** retrieve results from both tools and document review in iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tools:** Codex + Gemini (dual review)
**Date:** YYYY-MM-DD

**Convergent Findings:**
- [Where both reviewers agreed]

**Divergent Findings:**
- Codex: [unique findings]
- Gemini: [unique findings]

**Principle Violations:**
- [Principle]: [violation description with evidence]

**Strengths:**
- [What aligns well with principles]

**Assessment:** [Overall quality evaluation, note convergence/divergence]
<!-- END: VALIDATE/REVIEW -->
```

---

### 2. Make Decision

**Guidelines:**
- **APPROVED** = Principles upheld, violations minor or non-existent
- **REVISE** = Fixable violations exist, core is sound
- **REJECT** = Fundamental principle violations, unfixable without restart

**Doc-code divergence:** Plans describe intent, graybox creates reality. Code is truth. Doc-code drift during iteration is acceptable—implementation evolution visible in GRAYBOX/REVIEW section.

**Update** iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** APPROVED | REVISE | REJECT

**Reasoning:** [why this decision based on principle alignment]

**Required changes (if REVISE):**
- [specific fixes to align with principles]

**Learning (if REJECT):**
- [what violated principles]
- [why unfixable]
- [what we learned]
<!-- END: VALIDATE/DECISION -->
```

---

### 3. Branch on Decision

**Decision made?** → Follow the appropriate path:

**If APPROVED:**
- → Continue to Step 4 (Write Unit Tests)

**If REVISE:**
- → Skip to Step 6 (Commit)
- → Return to ITERATE with required changes

**If REJECT:**
- → Skip to Step 6 (Commit)
- → Document learning and discuss next steps

---

### 4. Write Unit Tests (APPROVED only)

**Foundation/primitive code only.** High-level systems use debug assertions.

**What to test:**
- Mathematical correctness
- Behavioral properties
- Edge cases
- Parameter ranges

**Reference:** `tests/foundation/test_spring_damper.cpp`

**Create:** `tests/foundation/test_<name>.cpp`

**All tests must pass before continuing.**

**Document** in iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: VALIDATE/TESTS -->
## Foundation Tests

**File:** tests/foundation/test_<name>.cpp
**Functions:** [list]
**Count:** [N]
**Status:** ✓ Passing
<!-- END: VALIDATE/TESTS -->
```

---

### 5. Document Emergence (APPROVED only)

In iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: VALIDATE/EMERGENCE -->
## Emergence

**Surprising behaviors:**
- [what emerged that we didn't plan]

**Enables (future):**
- [what this makes possible]

**Learned:**
- [insights gained]
<!-- END: VALIDATE/EMERGENCE -->
```

---

### 6. Update Status and Commit

Update iteration doc status:
```markdown
**Status:** <APPROVED|REVISE|REJECT>
```

Update `PLANS/<name>_SYSTEM.md` iteration list:
```markdown
- [ITERATION_<N>.md](ITERATION_<N>.md) - <APPROVED|REVISE|REJECT>
```

Commit:
```bash
git add PLANS/<name>_ITERATION_<N>.md PLANS/<name>_SYSTEM.md
git commit -m "validate: <name> iteration <N> - <APPROVED|REVISE|REJECT>

<Concise decision>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

---

## Outputs

- [ ] External review completed
- [ ] Decision made
- [ ] Unit tests written (APPROVED only, foundation code only)
- [ ] Emergence documented (APPROVED only)
- [ ] Committed

---

## Next

**If APPROVED:**
→ CLOSE

**If REVISE:**
→ ITERATE (minor fixes)
→ GRAYBOX (major rework)

**If REJECT:**
→ Document learning
→ Delete system code
→ Return to SELECT

---

**Evaluate honestly. Validate externally. Principles over shipping.**
