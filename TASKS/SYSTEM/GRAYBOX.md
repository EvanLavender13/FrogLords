# GRAYBOX - Build Simplest Possible Version

**Validate with cubes. Prove it works before polish.**

---

## Purpose

Build the core mechanic in the simplest possible form. No art, no polish, no extras. Just the mathematical truth of the system, made visible.

**If it doesn't work in graybox, it won't work polished. Save months by discovering this in hours.**

---

## Prerequisites

- `PLANS/<name>_SYSTEM.md` exists with core identified
- Branch `system/<name>` active
- Dependencies ≥90%
- Mathematical approach clear

---

## Process

### 1. Define Scope

Update `PLANS/<name>_SYSTEM.md` — add:

```markdown
<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox

**Visual:**
- Core: [cubes, spheres, lines]
- Debug: [overlays, colors, labels]
- Input: [primitives only]

**Included:**
- [ ] Core mechanic
- [ ] Math implementation
- [ ] Debug visualization
- [ ] Validation harness
- [ ] Self-test

**Excluded:**
- Art/models/audio
- Polish/juice
- Optional features
- Variations

**Complexity:** Small | Medium | Large
<!-- END: GRAYBOX/SCOPE -->
```

---

### 2. Build Core

**Priority:**
1. Mathematical correctness - proven, not guessed
2. Debug visualization - make behavior visible
3. Minimal interaction - keyboard/mouse primitives
4. Self-test harness - automated validation

**Rules:**
- Primitives only (cubes, spheres, lines)
- No assets (textures, models, sounds)
- No polish (tweening, particles, juice)
- No extras (features beyond core)

**Before adding anything:**
- Is this the core? (No → don't add)
- Does this validate? (No → don't add)
- Can this wait? (Yes → don't add)

---

### 3. Debug Visualization

**Make invisible visible:**
- Mathematical state (vectors, values, ranges)
- Decision points (where system chooses)
- Relationships (what affects what)
- Color-code states (normal, edge, error)

**Examples:**
- Physics forces → arrows (direction/magnitude)
- State machine → color current state
- Ranges → show min/max/current
- Collisions → highlight contacts
- Paths → draw trajectories

**Can't see it? Can't validate it.**

---

### 4. Validation Harness

**Test for:**
- **Math correctness** - edge cases, boundaries
- **Consistency** - same input = same output
- **No special cases** - general rules only
- **Predictability** - behavior matches model

**Structure:**
```typescript
describe('System: <name>', () => {
  it('math: <property>', () => { /* prove property */ });
  it('consistency: deterministic', () => { /* verify */ });
  it('no special cases', () => { /* no entity-specific logic */ });
});
```

---

### 5. Self-Test

**Play it:**
- Core feel right?
- Debug viz clear?
- Math behaves as proven?
- Any surprises?

**Document now:**
- Works?
- Doesn't?
- Surprised?
- Emerged?

**Iterate fast:**
- Short cycles (minutes to hours)
- Small changes
- Frequent tests
- Follow serendipity (verify mathematically)

---

### 6. Document Results

In `PLANS/<name>_SYSTEM.md` — add:

```markdown
<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Math validation passing
- [x] Debug viz working
- [ ] Edge cases handled
- [ ] Emergence observed

**Works:**
-

**Doesn't:**
-

**Surprises:**
-

**Validation:**
- Proven:
- Tested:
- Holds:

**Next:**
-
<!-- END: GRAYBOX/RESULTS -->
```

Update `CURRENT_PLAN.md` phase: `GRAYBOX → ITERATE`

---

### 7. Commit

```bash
git add .
git commit -m "graybox: <name> core mechanic

Works: <what>
Validated: <properties>
Emergent: <surprises>
Status: <ready | needs work | defer>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Outputs

- [ ] Core implemented (primitives)
- [ ] Debug viz functional
- [ ] Validation harness created
- [ ] Tests passing
- [ ] Playtest completed
- [ ] Results documented
- [ ] Committed

---

## Next

**→ ITERATE** (if core works)

**Defer if:**
- Core fundamentally flawed
- Math doesn't work as expected
- Too complex to simplify
- Foundation insufficient
- Different approach needed

**Deferral is success. Most systems should fail in graybox. That's why we graybox.**

---

**Cheap to build. Cheap to change. Cheap to throw away. Validate with cubes.**
