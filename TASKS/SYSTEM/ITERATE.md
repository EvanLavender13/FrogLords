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

## Process

### 1. Define Contract

In `PLANS/<name>_SYSTEM.md` — add:

```markdown
<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

**Mathematical properties:**
- [ ] [property that must always hold]

**Edge cases:**
- [ ] [case that must be handled]

**Consistency:**
- [ ] Same input → same output
- [ ] Frame-rate independent
- [ ] No special cases
<!-- END: ITERATE/CONTRACT -->
```

---

### 2. Iteration Cycle

**Repeat until contract passes:**

**Pick one:**
- Mathematical property to validate
- Edge case to handle
- Debug assertion to add (foundation code only)

**Implement:**
- Small change (fast to revert)
- Maintain debug viz
- Stay in graybox

**Test:**
- Build and run
- Try to break it
- Check contract

**Commit or revert:**

Pass:
```bash
git commit -m "iterate: <name> - <what>

Validated: <property>
Status: <X/Y>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

Fail:
```bash
git reset --hard HEAD
```

**Contract complete?** → Playtest

---

### 3. Playtest

**When assertions stop teaching.**

**Foundation-first validation:**
- Most contracts proven via debug assertions (foundation code)
- Manual testing validates gameplay feel, not math
- If you're manually checking math, add an assertion instead

**Solo playtest workflow:**

1. **Build and run** (DO NOT commit yet)
2. **Claude presents checklist** from contract
3. **Test each item** - report violations or "all pass"
4. **Violations found?**
   - Fix → rebuild → retest
   - Document in playtest section
5. **All pass?** → Commit

**Repeat until contract complete.**

**Document** in `PLANS/<name>_SYSTEM.md`:

```markdown
<!-- BEGIN: ITERATE/PLAYTEST_[N] -->
### Playtest [N]

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
<!-- END: ITERATE/PLAYTEST_[N] -->
```

**Stop when no new violations found.**

---

### 4. Document

In `PLANS/<name>_SYSTEM.md`:

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
- [ ] Ready for VALIDATE
<!-- END: ITERATE/COMPLETE -->
```

---

### 5. Commit

```bash
git commit -m "iterate: <name> complete

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

**→ VALIDATE** (principle scoring)

**Defer if:**
- Contract unprovable
- Math fails
- Edges unsolvable

---

**Test fast. Validate thoroughly.**
