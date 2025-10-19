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

### 1. Score Principles

In current iteration doc (`PLANS/<name>_ITERATION_<N>.md`) — add:

```markdown
<!-- BEGIN: VALIDATE/SCORES -->
## Principle Scores

**[Principle Name]:** __/10
- [Key question]
- Evidence: [what supports this score]

[Repeat for each principle in PRINCIPLES.md]

**Average:** __/10
<!-- END: VALIDATE/SCORES -->
```

**Scoring:**
- 10: Exemplary
- 7-9: Good
- 4-6: Issues
- 1-3: Violations

---

### 2. External Review

**Independent principle evaluation.**

```bash
echo "@TASKS/PLANS/<name>_ITERATION_<N>.md @TASKS/PLANS/<name>_SYSTEM.md @PRINCIPLES.md @CONVENTIONS.md @src/foundation/<relevant>.cpp [your validation question]" | codex e 2>/dev/null
```

**Continue conversation if needed:**
```bash
echo "[follow-up question]" | codex e resume --last 2>/dev/null
```

**Synthesize:**
- Compare external evaluation with self-scores
- Adjust scores based on evidence
- Identify missed violations
- Make final decision

**Document review** in iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex | Gemini
**Date:** YYYY-MM-DD

**Feedback:**
- [Key points from review]
- [Score adjustments made]
- [Violations identified]

**Final scores:** [adjusted scores]
<!-- END: VALIDATE/REVIEW -->
```

---

### 3. Make Decision

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

### 4. Branch on Decision

**Decision made?** → Follow the appropriate path:

**If APPROVED:**
- → Continue to Step 5 (Write Unit Tests)

**If REVISE:**
- → Skip to Step 7 (Commit)
- → Return to ITERATE with required changes

**If REJECT:**
- → Skip to Step 7 (Commit)
- → Document learning and discuss next steps

---

### 5. Write Unit Tests (APPROVED only)

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

### 6. Document Emergence (APPROVED only)

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

### 7. Update Status and Commit

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

- [ ] Principles scored
- [ ] External review completed
- [ ] Decision made
- [ ] Unit tests written (APPROVED only, foundation code only)
- [ ] Emergence documented (APPROVED only)
- [ ] Committed

---

## Next

**If APPROVED:**
→ INTEGRATE

**If REVISE:**
→ ITERATE (minor fixes)
→ GRAYBOX (major rework)

**If REJECT:**
→ Document learning
→ Delete system code
→ Return to SELECT

---

**Score honestly. Validate externally. Principles over shipping.**
