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
- [ ] Implementation plan
- [ ] Core mechanic
- [ ] Math implementation
- [ ] Debug visualization
- [ ] Build verification

**Excluded:**
- Art/models/audio
- Polish/juice
- Optional features
- Variations

**Complexity:** Small | Medium | Large
<!-- END: GRAYBOX/SCOPE -->
```

---

### 2. Plan Implementation

**Before writing code, map the structure.**

Update `PLANS/<name>_SYSTEM.md` — add:

```markdown
<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to modify:**
List each file path and what changes.

**Call structure:**
Describe function call chain.

**Debug data flow:**
Describe how computed values reach visualization.

**Integration points:**
Identify which existing functions modified and where new code added.
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->
```

---

### 2.5. Validate Plan

**Get a second opinion before implementing:**

Use Codex to validate the implementation plan against principles.

```bash
echo "@TASKS/PLANS/<name>_SYSTEM.md @PRINCIPLES.md @CONVENTIONS.md [your validation question]" | codex e 2>/dev/null
```

**Evaluate feedback:**
- Concerns raised? Revise plan
- Alternative suggested? Consider it
- Need clarification? Continue with `codex e resume --last 2>/dev/null`
- Validation confirms? Proceed

**Document the review** in `PLANS/<name>_SYSTEM.md`:

```markdown
<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** YYYY-MM-DD

**Question asked:**
[What you asked for validation]

**Feedback received:**
- [Key points from the review]
- [Concerns or recommendations]

**Impact on implementation:**
- [How feedback changed your approach]
- [What you kept vs modified]
<!-- END: GRAYBOX/REVIEW -->
```

---

### 3. Build Core

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

### 4. Self-Test

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

**Build and run:**
- Does it compile?
- Does it run without crashing?
- Does debug visualization show the system working?

---

### 5. Document Results

In `PLANS/<name>_SYSTEM.md` — add:

```markdown
<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Build successful
- [x] Debug viz working
- [ ] Ready for iteration

**Works:**
-

**Doesn't:**
-

**Surprises:**
-

**Next:**
- ITERATE phase
<!-- END: GRAYBOX/RESULTS -->
```

Update `CURRENT_PLAN.md` phase: `GRAYBOX → ITERATE`

---

### 6. Commit

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

- [ ] Implementation plan documented
- [ ] Core implemented (primitives)
- [ ] Debug viz functional
- [ ] Build successful
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
