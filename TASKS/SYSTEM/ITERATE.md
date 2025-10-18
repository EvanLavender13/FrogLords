# ITERATE - Test, Learn, Refine

**Plan to the horizon of certainty. Test. Learn. Repeat.**

---

## Purpose

Improve the graybox through rapid cycles of testing and refinement until patterns emerge and the system stabilizes.

**Iteration is how we discover what the system actually is, not what we thought it would be.**

---

## Prerequisites

- [ ] Graybox functional (core mechanic works)
- [ ] Debug visualization working
- [ ] Validation harness passing
- [ ] `PLANS/<name>_SYSTEM.md` updated with graybox results

---

## Process

### 1. Determine Planning Horizon

**How uncertain is this system?**

**High uncertainty (novel, unproven concept):**
- Planning horizon: **Hours**
- Test every: **30-60 minutes**
- Changes per cycle: **Small, experimental**

**Medium uncertainty (proven pattern, new application):**
- Planning horizon: **Half day**
- Test every: **2-3 hours**
- Changes per cycle: **Moderate, targeted**

**Low uncertainty (well-understood, derivative):**
- Planning horizon: **Day**
- Test every: **Daily**
- Changes per cycle: **Larger, confident**

**The more uncertain, the shorter the planning horizon.**

---

## Iteration Loop

Repeat until patterns emerge:

### Step 1: Plan (Short Horizon)

**What to try next:**
- One idea from self-tests
- One edge case to fix
- One serendipitous discovery to explore
- One mathematical property to verify

**Planning questions:**
1. Does this serve player control?
2. Can this emerge from simpler rules?
3. Is this mathematically correct?
4. Will this behave consistently?
5. Can anything be removed?

**If answer to any is "no," don't implement it.**

---

### Step 2: Implement

Make the change:
- Keep it small (reversible in minutes)
- Maintain debug visualization
- Update validation tests
- Commit if it works, revert if it doesn't

**Speed over perfection.** Ugly code is fine in iteration. We'll clean it later.

---

### Step 3: Self-Test

Play the system:
- Does the change work as expected?
- Any new surprises?
- Any regressions?
- Better or worse?

**Document immediately:**
```markdown
### Iteration [N] - [Date/Time]

**Changed:**
-

**Result:**
- Works:
- Doesn't work:
- Surprised:

**Next:**
-
```

---

### Step 4: Validate

Run validation harness:
- [ ] All tests passing?
- [ ] Mathematical properties hold?
- [ ] No new special cases?
- [ ] Behavior still consistent?

**If validation fails, fix or revert.**

---

### Step 5: Commit or Revert

**If change improved system:**
```bash
git add .
git commit -m "iterate: <name> - <what changed>

Result: <what improved>
Validated: <properties checked>
Emergent: <surprises>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

**If change made things worse:**
```bash
git reset --hard HEAD  # Revert to last good state
```

**No attachment to failed experiments. Revert freely.**

---

### Step 6: Continue or Escalate

**Continue loop if:**
- More ideas to try
- Edge cases to handle
- Serendipity to explore
- System not yet stable

**Escalate to playtesting when:**
- Self-testing no longer teaches anything
- System feels stable
- Ready for fresh eyes
- Need to see how others use it

---

## Over-Shoulder Playtesting

**When self-tests stop teaching, watch others play.**

### Setup

**Find playtesters who:**
- Haven't seen this system before
- Match target skill level
- Can play without guidance

**Your role:**
- **Silent observer** (do not help)
- Stand behind them (out of sight)
- Take notes frantically
- Resist the urge to explain

---

### Watch For

**Where they fail:**
- Unclear what to do?
- Edge cases break system?
- Math behaves unexpectedly?

**Where they succeed:**
- Strategies you didn't anticipate?
- Uses you didn't design for?
- Emergent behaviors?

**What they say:**
- Confusion? ("What do I do?")
- Frustration? ("This doesn't work!")
- Delight? ("Oh, that's cool!")

---

### After Each Playtest

**Document in** `PLANS/<name>_SYSTEM.md`:

```markdown
### Playtest [N] - [Date]

**Tester:** [Experience level]

**Failures:**
- Confused by:
- Blocked by:
- Frustrated by:

**Successes:**
- Discovered:
- Mastered:
- Enjoyed:

**Emergent Behaviors:**
-
-

**Changes Needed:**
-
-
```

---

### Fix Issues

After each playtest:
1. Identify highest-priority issue
2. Implement fix (keep it small)
3. Validate (tests still pass?)
4. Commit
5. Find new playtester (someone who hasn't played)

**Repeat until patterns emerge.**

---

## When to Stop Iterating

**Stop when:**
- Same experiences repeat across multiple playtesters
- No new edge cases emerging
- System behaves predictably
- Emergent behaviors documented
- Mathematical validation complete
- Principle questions all answer "yes"

**Typical iteration counts:**
- Self-tests: 5-20 cycles
- Playtests: 6-12 people
- Total time: Days to weeks

**Don't stop early. Insufficient iteration is technical debt.**

---

## Update System Document

In `PLANS/<name>_SYSTEM.md`:

```markdown
## Iteration Complete

**Total Cycles:** [self-tests + playtests]
**Time Spent:** [hours/days]

**What Stabilized:**
- Core mechanic: <description>
- Mathematical model: <validated properties>
- Edge cases: <handled>

**Emergent Behaviors Documented:**
1. <unexpected behavior 1>
2. <unexpected behavior 2>
3. <unexpected behavior 3>

**Serendipitous Discoveries:**
- <what we found by accident>

**Final State:**
- [x] Core mechanic stable
- [x] Math validated
- [x] Edge cases handled
- [x] Playtester patterns repeating
- [x] Emergent behaviors documented
- [x] Debug visualization complete
- [ ] Ready for VALIDATE gate

**Changes from Original Plan:**
- Started with: <original idea>
- Ended with: <actual system>
- Why: <what we learned>
```

---

## Outputs

- [ ] System stable through iteration
- [ ] Self-tests completed (5-20 cycles)
- [ ] Playtests completed (6-12 people)
- [ ] Emergent behaviors documented
- [ ] Edge cases handled
- [ ] Mathematical validation complete
- [ ] All changes committed
- [ ] Ready for principle validation

---

## Exit Criteria

**Proceed to VALIDATE when:**
- Patterns repeating across playtesters
- No new edge cases emerging
- System feels stable
- Emergent behaviors documented
- Mathematical properties proven
- Confident in core mechanic

**Defer if:**
- Core mechanic still fundamentally flawed
- Can't resolve edge cases
- Math doesn't work in practice
- Too complex (can't simplify further)
- Better to try completely different approach

**Iterate more if:**
- Still discovering new behaviors
- Edge cases emerging
- System feels unstable
- Playtesters showing varied experiences

---

## Next Step

**→ VALIDATE** (principle check and scoring)

---

## The Iteration Philosophy

### Short Planning Horizons

**Uncertainty is high** → Plan minutes/hours ahead
**Uncertainty is low** → Plan days ahead
**Never plan beyond certainty horizon**

### Test Until Patterns Emerge

**One test reveals nothing**
**Ten tests reveal variance**
**Hundred tests reveal patterns**

### Follow Serendipity

**Halo discovered first-person was better than top-down**
**BioShock discovered Rapture after years of iteration**
**The Sims discovered characters were more fun than houses**

**Your system will surprise you. Let it.**

### The Paradox of Quality

**Working slowly, perfectly** → Few iteration cycles → Poor quality
**Working fast, imperfectly** → Many iteration cycles → High quality

**Iteration count matters more than iteration quality.**

---

**Plan short. Test often. Follow emergence.**
