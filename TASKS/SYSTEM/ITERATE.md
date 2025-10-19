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

**When self-tests stop teaching.**

**Setup:**
- Find 6-12 people
- Haven't seen system
- Match target skill

**Observe (silent):**
- Contract breaks
- Edge cases
- Emergent behaviors

**Document** in `PLANS/<name>_SYSTEM.md`:

```markdown
### Playtest [N]

**Violations:** [contract items that failed]
**Emergent:** [unexpected behaviors]
**Fix:** [what to address]
```

**Fix → test → commit → next tester**

**Stop when patterns repeat.**

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
