# ITERATE - Harden the Foundation

**Graybox proved it can work. Iteration proves it always works.**

---

## Purpose

Validate mathematical foundations. Handle edge cases. Stabilize through testing.

---

## Prerequisites

- [ ] Graybox functional
- [ ] Debug visualization working
- [ ] Build successful
- [ ] `PLANS/<name>_SYSTEM.md` exists

---

## Anti-Patterns

Before implementing, review @TASKS/PATTERNS.md for known code and process anti-patterns.

Prevention is cheaper than detection.

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

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Define what must be proven about this system. Choose categories that serve validation.

- [ ] [contract item to validate]
<!-- END: ITERATE/CONTRACT -->

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

**Baseline contract:** See ITERATION_<N>/CONTRACT
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

### 2. Iteration Cycle

**One contract item at a time. Validate, then commit.**

For each unchecked contract item:
1. **Implement** the fix (small changes only, maintain debug visualization)
2. **Build** successfully
3. **Manual verification** (if changes affect runtime behavior):
   - List what user must verify
   - Ask user to run app and verify
   - Wait for confirmation
4. **Document** validation in contract
5. **Commit** on success, revert on failure

If resuming from VALIDATE/REVISE, turn violations into contract items and prioritize them.

When all contract items are checked, proceed to playtest.

**Commit format:**
```bash
git commit -m "iterate: <name> - <what>

Validated: <property>
Status: <X/Y>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

### 3. Playtest

**When assertions stop teaching.**

**Foundation-first validation:**
- Most contracts proven via debug assertions (foundation code)
- Manual testing validates gameplay feel, not math
- If you're manually checking math, add an assertion instead

**Solo playtest workflow:**

1. **Build**
2. **Claude presents checklist** from contract
3. **Test each item** - report violations or "all pass"
4. **Violations found?**
   - Fix → rebuild → retest
   - Document in playtest section
5. **All pass?** → Commit

**Repeat until contract complete.**

**Document in iteration doc** (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: ITERATE/PLAYTEST -->
### Playtest <N>

**Date:** [YYYY-MM-DD]
**Tester:** [name]

**Violations:**
- [x] [contract item that failed]
  - Root cause: [why it failed]
  - Fix: [what was changed] ✓ FIXED

**Emergent:**
- [unexpected behaviors observed]

**Fix:**
- [files changed]
- Commit: [hash]
- ✅ VERIFIED: [confirmation]
<!-- END: ITERATE/PLAYTEST -->
```

**Stop when no new violations found.**

---

### 4. Document

Add to iteration doc (`PLANS/<name>_ITERATION_<N>.md`):

```markdown
<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties:** [how validated]
**Edges:** [what handled]
**Assertions:** [count added]
**Playtests:** [count]

**Status:**
- [x] Contract proven
- [x] Stable
- [x] Ready for VALIDATE
<!-- END: ITERATE/COMPLETE -->
```

---

### 5. Update Status and Commit

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

Contract: proven
Playtests: <count>
Assertions: <count>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

---

## Outputs

- [ ] Contract defined
- [ ] All properties validated
- [ ] All edges handled
- [ ] Assertions added (foundation code)
- [ ] Stable through playtests
- [ ] Committed

---

## Next

→ VALIDATE (principle scoring in iteration doc)

**Defer if:**
- Contract unprovable
- Math fails
- Edges unsolvable

---

**Test fast. Validate thoroughly.**
