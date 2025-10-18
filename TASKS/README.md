# Task Commands

**Principled development workflows as executable slash commands.**

---

## Quick Start

Link this directory to Claude Code's command directory:

```bash
# In your .claude directory
ln -s /path/to/TASKS /path/to/.claude/commands
```

Or copy individual task files to `.claude/commands/`.

---

## System-Building Cycle

**Build from certainty → Test → Validate → Strengthen**

```
/system-select      → Choose system from backlog
/system-graybox     → Build simplest version (cubes/primitives)
/system-iterate     → Test until patterns emerge
/system-validate    → Score against Six Pillars
/system-integrate   → Replace primitives with production
/system-strengthen  → Update foundation, document learning
```

**Full cycle:** SELECT → GRAYBOX → ITERATE → VALIDATE → INTEGRATE → STRENGTHEN

**Deferral points:** Any step can discover "we shouldn't build this" (that's success)

---

## Refinement Cycle

**Find violations → Remove complexity → Measure improvement**

```
/refine-audit    → Search codebase for principle violations
/refine-select   → Choose violation to fix
/refine-refine   → Delete, simplify, or document
/refine-measure  → Calculate before/after metrics
```

**Full cycle:** AUDIT → SELECT → REFINE → MEASURE

**Two paths:**
- **Path A (Trivial):** SELECT → REFINE → MEASURE (skip planning)
- **Path B (Standard):** SELECT → REFINE → MEASURE (with planning)

---

## When to Use

### System-Building
**Use when:**
- Foundation ≥90% certain for next layer
- Building new capabilities
- Adding primitives
- Enabling emergence

**Outputs:**
- New system in production
- Foundation strengthened
- Emergence documented
- New possibilities enabled

---

### Refinement
**Use when:**
- Foundation ≥70% stable
- Between system builds
- Violations accumulating
- Technical debt visible

**Outputs:**
- Violations removed
- Complexity reduced
- Principles restored
- Foundation strengthened

---

## The Philosophy

### System Building = Growth

Build upward from solid foundations:
- SELECT: Choose what foundation supports
- GRAYBOX: Prove it works with primitives
- ITERATE: Test until patterns emerge
- VALIDATE: Score against principles (avg ≥7.0)
- INTEGRATE: Polish when certain
- STRENGTHEN: Document learning, update foundation

**Goal:** Enable emergence through composable systems

---

### Refinement = Simplification

Remove accumulated complexity:
- AUDIT: Make violations visible
- SELECT: Choose highest-impact violation
- REFINE: Delete > Simplify > Document
- MEASURE: Quantify improvement

**Goal:** Restore principles through continuous reduction

---

## File Structure

```
TASKS/
├── README.md                # This file
│
├── SYSTEM/                  # System-building cycle
│   ├── SELECT.md           # Choose from backlog
│   ├── GRAYBOX.md          # Build with primitives
│   ├── ITERATE.md          # Test until stable
│   ├── VALIDATE.md         # Score principles
│   ├── INTEGRATE.md        # Production polish
│   └── STRENGTHEN.md       # Close cycle, update docs
│
└── REFINE/                  # Refinement cycle
    ├── AUDIT.md            # Find violations
    ├── SELECT.md           # Choose violation
    ├── REFINE.md           # Execute fix
    └── MEASURE.md          # Calculate metrics
```

---

## Example Usage

### Starting a New System

```bash
# 1. Check foundation readiness
# Review DEPENDENCY_STACK.md

# 2. Choose system to build
/system-select

# 3. Build graybox
/system-graybox

# 4. Iterate until stable
/system-iterate

# 5. Validate against principles
/system-validate

# 6. If approved, integrate
/system-integrate

# 7. Strengthen foundation
/system-strengthen
```

---

### Running a Refinement

```bash
# 1. Find violations
/refine-audit

# 2. Choose highest-impact
/refine-select

# 3. Fix it
/refine-refine

# 4. Measure improvement
/refine-measure
```

---

## Integration with Backlogs

### System Building Uses:
- **BACKLOG_SYSTEMS.md** - Systems to build
- **DEPENDENCY_STACK.md** - Foundation state
- **PLANS/<name>_SYSTEM.md** - System documentation
- **PLANS/<name>_VALIDATE.md** - Principle scores
- **EMERGENCE.md** - Documented surprises

### Refinement Uses:
- **BACKLOG_REFINEMENTS.md** - Violations to fix
- **DEPENDENCY_STACK.md** - Foundation state
- **PLANS/REFINE_<name>.md** - Refinement documentation

---

## Principles Integration

**Every task enforces the Six Pillars:**

1. **Radical Simplicity** - Can anything be removed?
2. **Composable Functions** - Does it compose cleanly?
3. **Mathematical Foundations** - Is it mathematically proven?
4. **Emergent Behavior** - Does it enable, not prescribe?
5. **Consistency** - Is it predictable?
6. **Principled Development** - Can decisions be traced?

**Validation gates ensure principle alignment.**

---

## Metrics Tracked

### System Building
- Foundation certainty (%)
- Principle scores (1-10 per pillar)
- Emergent behaviors (count)
- Iteration cycles (count)
- Time to completion (hours/days)

### Refinement
- LOC removed (count)
- Complexity reduction (score)
- Special cases eliminated (count)
- Magic numbers derived (count)
- Principle score improvement (+points)

---

## The Cycle

```
Build System
    ↓
Foundation Strengthened
    ↓
Run Audit
    ↓
Refine Violations
    ↓
Foundation Strengthened
    ↓
Build System
    ↓
...
```

**The codebase breathes: grow, simplify, grow, simplify.**

---

## Customization

### For Your Project

**Edit task files to:**
- Add project-specific validation
- Change complexity thresholds
- Adjust principle scoring
- Add automation hooks
- Integrate tools (linters, profilers)

**Keep the philosophy:**
- Build from certainty
- Iterate toward truth
- Validate continuously
- Remove complexity
- Measure objectively

---

## Meta-Commands

**Not implemented yet, but useful:**

```
/workflow-status    # Show current state (which system/refinement in progress)
/foundation-check   # Quick foundation certainty report
/principle-scores   # Aggregate principle scores across systems
/emergence-log      # View documented emergence
```

---

## The Commitment

**These workflows exist to:**
- Serve the Six Pillars
- Build from certainty
- Enable emergence
- Remove complexity
- Document truth

**Never use to:**
- Ship faster by skipping validation
- Build on uncertain foundations
- Prescribe outcomes over enabling
- Add complexity for convenience
- Ignore principle violations

---

**Build systems. Remove complexity. Serve principles.**

**This is the way.**
