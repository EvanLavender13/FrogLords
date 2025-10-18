# GRAYBOX - Build Simplest Possible Version

**Validate with cubes. Prove it works before polish.**

---

## Purpose

Build the core mechanic in the simplest possible form. No art, no polish, no extras. Just the mathematical truth of the system, made visible.

**If it doesn't work in graybox, it won't work polished. Save months by discovering this in hours.**

---

## Prerequisites

- [ ] `TASKS/PLANS/<name>_SYSTEM.md` exists with core identified
- [ ] Feature branch `system/<name>` active
- [ ] Dependencies at ≥90% certainty
- [ ] Mathematical approach clear

---

## Process

### 1. Define Graybox Scope

Update `TASKS/PLANS/<name>_SYSTEM.md` with:

```markdown
## Graybox Implementation

**Visual representation:**
- Core mechanic: [cubes, spheres, lines, etc.]
- Debug info: [overlays, colors, labels]
- Interaction: [primitives only]

**What's included:**
- [ ] Core mechanic only
- [ ] Mathematical implementation
- [ ] Debug visualization
- [ ] Validation harness
- [ ] Self-test capability

**What's excluded:**
- Final art/models
- Audio
- Polish/juice
- Optional features
- Variations

**Estimated graybox time:** [hours]
```

---

### 2. Build Core Mechanic

**Implementation priority:**
1. Mathematical correctness (proven, not guessed)
2. Debug visualization (make behavior visible)
3. Minimal interaction (keyboard/mouse primitives)
4. Self-test harness (automated validation)

**Rules:**
- Use primitives (cubes, spheres, lines, debug draw)
- No assets (textures, models, sounds)
- No polish (tweening, particles, juice)
- No extras (features beyond core)

**If you're tempted to add something, ask:**
- Is this the core mechanic? (No → don't add)
- Will this help validate? (No → don't add)
- Can this wait for polish? (Yes → don't add)

---

### 3. Create Debug Visualization

**Make the invisible visible:**
- Show mathematical state (vectors, values, ranges)
- Show decision points (where system chooses behavior)
- Show relationships (what affects what)
- Color-code states (normal, edge cases, errors)

**Example:**
```
- Physics forces: Draw arrows showing direction/magnitude
- State machine: Color code current state
- Ranges: Visualize min/max/current
- Collisions: Highlight contact points
- Paths: Draw predicted trajectories
```

**If you can't see it, you can't validate it.**

---

### 4. Implement Validation Harness

Create automated tests for:
- **Mathematical correctness**: Edge cases, boundary conditions
- **Consistency**: Same input = same output
- **No special cases**: General rules only
- **Predictability**: Behavior matches model

**Example test structure:**
```typescript
describe('System: <name>', () => {
  it('mathematical correctness: <property>', () => {
    // Prove mathematical property holds
  });

  it('consistency: same input produces same output', () => {
    // Verify deterministic behavior
  });

  it('no special cases: general rules apply', () => {
    // Ensure no entity-specific logic
  });
});
```

---

### 5. Self-Test

**Play it yourself first:**
- Does the core mechanic feel right?
- Can you see what's happening (debug viz)?
- Does math behave as proven?
- Any surprises (good or bad)?

**Document immediately:**
- What works?
- What doesn't?
- What surprised you?
- What emerged unexpectedly?

**Iterate rapidly:**
- Short cycles (minutes to hours)
- Small changes
- Frequent tests
- Follow serendipity (but verify mathematically)

---

### 6. Update System Document

In `TASKS/PLANS/<name>_SYSTEM.md`, add:

```markdown
## Graybox Results

**Implementation Status:**
- [x] Core mechanic functional
- [x] Mathematical validation passing
- [x] Debug visualization working
- [ ] Edge cases handled
- [ ] Emergent behaviors observed

**What Works:**
-
-

**What Doesn't:**
-
-

**Surprises (Serendipity):**
-
-

**Mathematical Validation:**
- Proven: <properties>
- Tested: <edge cases>
- Holds: <invariants>

**Next Iteration Needed:**
-
-
```

---

### 7. Commit Graybox

```bash
git add .
git commit -m "graybox: implement <name> core mechanic

Core: <what works>
Validated: <math properties>
Emergent: <surprises>
Status: <ready for iteration | needs work | defer>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

---

## Outputs

- [ ] Core mechanic implemented (primitives only)
- [ ] Debug visualization functional
- [ ] Validation harness created
- [ ] Self-tests passing
- [ ] Initial playtest completed
- [ ] Surprises documented
- [ ] Graybox committed

---

## Exit Criteria

**Proceed to ITERATE when:**
- Core mechanic works in graybox
- Mathematical validation passes
- Debug visualization shows behavior clearly
- Ready for playtesting with others

**Defer if:**
- Core mechanic fundamentally flawed
- Math doesn't work as expected
- Too complex (can't simplify to core)
- Foundation actually insufficient
- Better to try different approach

**Deferral is success here. Most systems should fail in graybox. That's why we graybox.**

---

## Next Step

**→ ITERATE** (if core works)
**→ Document and defer** (if core doesn't work)

---

## The Graybox Philosophy

### Why Primitives?

**Cheap to build** → Fast iteration
**Cheap to change** → Follow serendipity
**Cheap to throw away** → No sunk cost fallacy

### Why Now?

**Before planning** → Plans assume graybox works
**Before art** → Don't polish broken mechanics
**Before complexity** → Prove core before building on it

### What Success Looks Like

**It's ugly** → That's the point
**It's minimal** → That's the goal
**It works** → That's the proof
**It surprises** → That's emergence

---

**Validate with cubes. Polish when certain.**
