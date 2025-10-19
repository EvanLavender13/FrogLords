# VALIDATE - Principle Scoring

**Principles judge all work equally. No exceptions.**

---

## Purpose

Score system against principles. External validation. Approve, revise, or reject.

---

## Prerequisites

- [ ] Iteration complete
- [ ] Foundation contract proven
- [ ] Playtests stable
- [ ] `PLANS/<name>_ITERATION_<N>.md` has ITERATE/COMPLETE

---

## Process

### 1. External Review

**Independent principle evaluation.**

```bash
bash scripts/bash/codex.sh "@TASKS/PLANS/<name>_ITERATION_<N>.md @TASKS/PLANS/<name>_SYSTEM.md @PRINCIPLES.md @CONVENTIONS.md @src/foundation/<relevant>.cpp [your validation question]"
```

**IMPORTANT:** Use `run_in_background: true` in Bash tool call.

**WAIT for user confirmation that review is complete.** This takes several minutes. Do NOT monitor output. User will confirm when finished.

**Continue conversation if needed:**
```bash
bash scripts/bash/codex.sh --resume "[follow-up question]"
```

**After user confirms completion,** retrieve results and document review in iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex | Gemini
**Date:** YYYY-MM-DD

**Scores:**
[Codex/Gemini scores for each principle]

**Critical Violations Identified:**
- [List of violations with evidence]

**Final Average:** [score]/10
<!-- END: VALIDATE/REVIEW -->
```

---

### 2. Make Decision

**Thresholds:**
- Average ≥7.0 + all ≥6 = **APPROVED**
- Average 5.0-6.9 or any <6 = **REVISE**
- Average <5.0 = **REJECT**

**Update** iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** APPROVED | REVISE | REJECT

**Reasoning:** [why this decision]

**Required changes (if REVISE):**
- [specific fixes needed]

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

Average: <score>/10
Scores: [1,2,3,4,5,6]
Decision: <reasoning>

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
→ STRENGTHEN

**If REVISE:**
→ ITERATE (minor fixes)
→ GRAYBOX (major rework)

**If REJECT:**
→ Document learning
→ Delete system code
→ Return to SELECT

---

**Score honestly. Validate externally. Principles over shipping.**
