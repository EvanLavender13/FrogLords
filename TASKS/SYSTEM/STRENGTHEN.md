# STRENGTHEN - Close the Cycle

**Document learning. Update foundation. Celebrate emergence. Enable the next layer.**

---

## Purpose

Close the system-building cycle by capturing all learning, updating foundation certainty, and enabling future systems that can now be built on this solid ground.

**Every system teaches. This step ensures we remember the lessons.**

---

## Prerequisites

- [ ] Integration complete
- [ ] All tests passing
- [ ] System in production
- [ ] `TASKS/PLANS/<name>_SYSTEM.md` fully documented
- [ ] `TASKS/PLANS/<name>_VALIDATE.md` shows approval

---

## Process

### 1. Reflect on the Journey

In `TASKS/PLANS/<name>_SYSTEM.md`, add final reflection:

```markdown
## System Complete - Reflection

**Started:** [Date]
**Completed:** [Date]
**Total Time:** [hours/days/weeks]

---

### What We Planned

**Original goal:**
[What we thought we were building]

**Expected behavior:**
[What we expected to happen]

**Anticipated challenges:**
[What we thought would be hard]

---

### What We Built

**Actual system:**
[What we actually built]

**Actual behavior:**
[What actually happens]

**Actual challenges:**
[What was actually hard]

---

### What We Learned

**Surprises (good):**
1.
2.
3.

**Surprises (bad):**
1.
2.
3.

**Serendipitous discoveries:**
-
-

**Mathematical insights:**
-
-

**Principle insights:**
-
-

---

### What Changed

**From plan to reality:**
- Planned: <original>
- Built: <actual>
- Because: <what we learned>

**Systems we thought we'd enable:**
-
-

**Systems we actually enable:**
-
-

---

### What Emerged

**Documented emergent behaviors:**
1. [Behavior + why it's valuable]
2. [Behavior + why it's valuable]
3. [Behavior + why it's valuable]

**Player strategies enabled:**
-
-

**Unexpected possibilities:**
-
-

---

### Metrics

**Complexity:**
- Lines of code: __
- Files modified: __
- Dependencies: __

**Quality:**
- Test coverage: __%
- Principle avg: __/10
- Individual scores: [list]

**Iteration:**
- Graybox cycles: __
- Playtests: __
- Deferrals: __
- Major pivots: __

**Performance:**
- FPS impact: __ fps
- Memory: __ MB
- Load time: __ ms

---

### Foundation Impact

**Certainty before:**
- This layer: __%
- Overall: __%

**Certainty after:**
- This layer: __%
- Overall: __%
- Improvement: +__%

**Systems now buildable:**
-
-

---

### Would We Do It Again?

**Yes/No:** __

**Why:**
[Honest assessment - was this worth building?]

**What we'd change:**
-
-

**What we'd keep:**
-
-
```

---

### 2. Update Dependency Stack

Open `TASKS/DEPENDENCY_STACK.md` and update:

**This layer:**
```markdown
## Layer [N]: [LAYER_NAME]

**Certainty:** [NEW_%]
**Last Updated:** [TODAY]

**Systems at this layer:**
- [Existing systems]
- **[NEW SYSTEM]** ← Just completed

**Foundation status:**
- [x] System [new] added
- [ ] Layer certainty recalculated
- [ ] Cascade to dependent layers calculated
```

**Calculate cascade:**
If this layer is now 90% certain:
- Next layer certainty = 0.9 × [next layer score]
- Layer after = 0.9² × [that layer score]
- Continue upward

**Update each dependent layer with cascaded certainty.**

---

### 3. Update System Backlog

Open `TASKS/BACKLOG_SYSTEMS.md`:

**Move completed system:**
```markdown
## Recently Completed

- **[SYSTEM NAME]**: [Description]
  - Completed: [Date]
  - Layer: [N]
  - Certainty contribution: +[%]
  - Emerged: [Key behaviors]
  - Enables: [Future systems]
```

**Update "Buildable Now" sections:**

For each layer that now has ≥90% foundation:
- Move systems from "Waiting on [dependencies]" to "Buildable Now"
- Note which dependency was just satisfied

**Add discovered opportunities:**
```markdown
## [Appropriate Layer]

**Buildable Now:**
- **[NEW OPPORTUNITY]**: [Discovered during <system>]
  - Depends on: [Including system we just built]
  - Enables: [Emergent possibilities]
  - Certainty needed: [%]
```

---

### 4. Update Refinement Backlog

Open `TASKS/BACKLOG_REFINEMENTS.md`:

**Add any violations discovered during building:**
```markdown
## Discovered During System Building

- **[LOCATION]**: [Violation found while building <system>]
  - Principle: [Which pillar]
  - Severity: [Level]
  - Fix: [Delete | Simplify | Document]
  - Found: [During which phase]
```

**Note patterns:**
```markdown
## Patterns

**Recurring violations:**
- [If this is the Nth time we've seen this violation type]
- Pattern: [Description]
- Root cause: [Why does this keep happening?]
- Prevention: [How to avoid in future systems?]
```

---

### 5. Celebrate Emergence

**Create a lightweight emergence log:**

In `EMERGENCE.md` (create if doesn't exist):
```markdown
# Emergence Log

**Systems that surprised their creators**

---

## [System Name] - [Date]

**We built:** [Simple description]

**We expected:** [What we thought would happen]

**What emerged:**
1. **[Behavior 1]**: [Description + why it's cool]
2. **[Behavior 2]**: [Description + why it's cool]
3. **[Behavior 3]**: [Description + why it's cool]

**Player strategies we didn't design:**
-
-

**Future possibilities this enables:**
-
-

**Quote from playtest:**
> [Something a playtester said that captured the magic]

**Principle alignment:**
- This validates: [Which principle]
- Score: __/10 on Emergent Behavior

---
```

**This is your win condition. Document it.**

---

### 6. Final Commit on Branch

Commit all documentation updates:

```bash
git add TASKS/PLANS/ TASKS/BACKLOG_SYSTEMS.md TASKS/BACKLOG_REFINEMENTS.md TASKS/DEPENDENCY_STACK.md EMERGENCE.md
git commit -m "strengthen: complete <name> system

Reflection:
- Time: <duration>
- Iterations: <count>
- Emergent behaviors: <count>

Foundation:
- Before: <%> → After: <%>
- Improvement: +<%>
- Enables: <list systems>

Learning:
- Surprised by: <key insight>
- Changed from plan: <what and why>
- Would do again: <yes/no>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

---

### 7. Merge to Main

**Prepare for merge:**
```bash
# Ensure branch is clean
git status

# Rebase on main (if needed)
git fetch origin
git rebase origin/main

# Run all tests one more time
[run test suite]
```

**Merge to main:**
```bash
git checkout main
git merge --no-ff system/<name>
```

**Tag the milestone:**
```bash
git tag -a v<version>-<name> -m "System: <name>

Adds: <capability>
Enables: <future systems>
Emergent: <behaviors>
Foundation: <%> certainty"

git push origin main --tags
```

---

### 8. Archive Branch (Optional)

**Keep branches for historical reference:**
```bash
# Push branch to remote
git push origin system/<name>

# Don't delete - it's documentation of the journey
```

---

## Outputs

- [ ] Final reflection written
- [ ] `TASKS/DEPENDENCY_STACK.md` updated
- [ ] `TASKS/BACKLOG_SYSTEMS.md` updated (completed + newly buildable)
- [ ] `TASKS/BACKLOG_REFINEMENTS.md` updated (violations found)
- [ ] `EMERGENCE.md` updated (celebrate!)
- [ ] All documentation committed
- [ ] Merged to main
- [ ] Tagged milestone
- [ ] Foundation certainty increased

---

## The Foundation Recalculation

### Before This System

```
Layer [N]: [BEFORE_%]
```

### After This System

**Recalculate this layer:**
1. Score system 1-10 on each principle (from VALIDATE.md)
2. Average all systems at this layer
3. Convert to percentage (× 10)

**Example:**
- System A scores: 8.5 avg → 85%
- System B scores: 9.0 avg → 90%
- System C scores: 7.5 avg → 75%
- **Layer certainty: (85 + 90 + 75) / 3 = 83%**

### Cascade Upward

**Each dependent layer:**
Certainty = This layer certainty × Layer's own score

**Example cascade:**
- Layer 2: 90% certain
- Layer 3 own score: 80%
- Layer 3 final: 90% × 80% = 72%

**This is why we build bottom-up.**

---

## What This Enables

### Systems Now Buildable

**List all systems that:**
- Required this system as dependency
- Now have ≥90% foundation certainty
- Can be selected in next cycle

### Systems Now Closer

**List systems that:**
- Required this system
- Don't yet have ≥90% certainty
- Are closer to buildable

### Unexpected Opportunities

**List systems we didn't plan:**
- Discovered during emergence
- Enabled by surprising behaviors
- Added to backlog for future

---

## Metrics to Track

**Over time, watch trends:**

**Certainty trajectory:**
- Is foundation strengthening over time?
- Are we building bottom-up?
- Is cascade effect understood?

**Emergence rate:**
- How many emergent behaviors per system?
- Are we enabling or prescribing?
- Surprises increasing or decreasing?

**Iteration efficiency:**
- Cycles per system trending?
- Deferral rate healthy (20-40%)?
- Playtests revealing patterns faster?

**Principle alignment:**
- Average scores trending up?
- Specific principles consistently weak?
- Systems getting simpler over time?

---

## Next Step

**→ SELECT** (choose next system from newly buildable options)

**OR**

**→ REFINE** (if violations accumulated, strengthen foundation)

**The cycle continues. Build → Strengthen → Build → Refine → Build...**

---

## The Strengthening Philosophy

### Every System Is a Teacher

**We don't just build systems. We learn from them.**

- What surprised us?
- What was harder than expected?
- What emerged unexpectedly?
- What would we change?

**Document everything. Wisdom compounds.**

### Foundation Is Everything

**A 95% certain foundation can support anything.**
**A 60% certain foundation will collapse under its own weight.**

Every system either:
- Strengthens foundation (good)
- Weakens foundation (tech debt)
- Maintains foundation (neutral)

**Build to strengthen. Always.**

### Celebrate Emergence

**Emergence is the win condition.**

If a system doesn't surprise you, it's probably:
- Too simple (not a system)
- Too prescriptive (not enabling)
- Too early (foundation insufficient)

**When players discover strategies you didn't design, you've succeeded.**

### The Cycle Never Ends

**There is no "done." There is only:**
- Systems that strengthen foundation
- Systems that reveal opportunities
- Systems that enable emergence

**Build → Strengthen → Build → Refine → Build...**

**This is the way.**

---

**Document learning. Update foundation. Celebrate emergence. Enable the next layer.**
