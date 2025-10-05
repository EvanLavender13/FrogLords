# Overgrowth Sphere Configuration Reanalysis

**Date:** October 5, 2025  
**Source:** GDC 2013 talk screenshot, fresh viewing  
**Context:** Single-sphere collision experiment, questioning dual-sphere design

---

## The Critical Observation

### What We've Been Implementing (WRONG):
```
     [BUMPER] ← Position = character center
        |
        | WEIGHTLIFTER_DROP = 0.15m
        v
    [LIFTER] ← Offset below, partially buried in ground
```

**Our assumption:** Lifter is buried 0.10m into ground for "stability"

### What the GDC Screenshot Actually Shows (CORRECT):
```
    [BUMPER] ← Raised ABOVE character center
        |
        | Some offset (unknown)
        v
     Position ← Character reference point
        |
        | Zero or minimal offset
        v
    [LIFTER] ← Rests ON ground surface (not buried)
```

**Reality:** Lifter touches ground cleanly, bumper is elevated

---

## Implications of This Misunderstanding

### 1. **We've Been Fighting Geometry, Not Implementing Design**

**Our implementation:**
- Lifter deliberately buried 0.10m for "stability"
- Required adaptive penetration limits (0.02m vs 0.10m)
- Surface classification to decide burial depth
- Complex authority arbitration

**Likely correct implementation:**
- Lifter rests on ground with zero penetration (like bumper does now)
- Bumper floats above ground, only hits walls and obstacles
- **No burial depth needed**
- **No adaptive penetration logic needed**

### 2. **The "Character Slow Sink" Bug Was Self-Inflicted**

We spent significant effort debugging why the character gradually sank into flat ground. The root cause: **we told it to** (0.10m intended burial).

The Overgrowth design likely never had this problem because the lifter wasn't meant to be buried.

### 3. **We Misunderstood the Role of Each Sphere**

**Our interpretation:**
- Bumper: Primary collision, handle flat ground
- Lifter: Handle slopes, intentionally bury for stability

**Likely correct interpretation:**
- **Lifter:** Primary ground contact (walks on ground surface)
- **Bumper:** Obstacle avoidance (slides against walls, elevated above ground)

This is **completely inverted** from what we implemented.

---

## What David Rosen Actually Said (Revisited)

> "I had this one green sphere as a bumper sphere it lets him fly it off objects that he runs into [and] a weightlifter's sphere which moves up over small obstacles."

### Our Past Interpretation:
- Bumper = primary collision
- Lifter = step-up assistant

### Possible Correct Interpretation:
- **Lifter** = primary ground locomotion ("moves up over small obstacles")
- **Bumper** = collision buffer ("lets him fly off objects that he runs into")

The lifter **moves** (active, primary), the bumper **reacts** (passive, secondary).

---

## The Configuration We Should Have Been Testing

### Geometric Setup:
```cpp
constexpr float LIFTER_RADIUS = 0.50f;        // Larger, primary contact
constexpr float BUMPER_RADIUS = 0.45f;        // Smaller, elevated
constexpr float BUMPER_ELEVATION = 0.40f;     // Bumper center above lifter center
const glm::vec3 BUMPER_OFFSET(0.0f, BUMPER_ELEVATION, 0.0f);
```

### Collision Logic:
```cpp
// Phase 1: Lifter handles ground
// - Zero-penetration contact with ground surfaces
// - Character position tracks lifter center
// - Natural step-up via sphere geometry (no burial)

// Phase 2: Bumper handles obstacles
// - Elevated sphere only hits walls and obstacles
// - Never touches flat ground (too high)
// - Provides "padding" around upper body
```

### Key Difference:
- **Lifter** drives vertical position (Y-axis)
- **Bumper** drives horizontal collision avoidance (X/Z-axis)
- Clean separation of responsibilities

---

## Why This Configuration Makes Sense

### 1. **Natural Step-Up Without Special Logic**
- Lifter sphere (0.50m radius) touches ground
- Small obstacle (0.15m high) contacts lower hemisphere of lifter
- Lifter naturally "rolls" up and over via sphere geometry
- **No intended burial needed**
- **No adaptive penetration needed**

### 2. **Bumper as "Personal Space" Bubble**
- Elevated above ground (doesn't touch floor)
- Only activates when character approaches walls/obstacles
- Prevents "body" from clipping into vertical surfaces
- Natural slide-along behavior on walls

### 3. **Biomimetic Behavior**
- **Lifter = Feet** (walk on ground, climb steps)
- **Bumper = Torso** (avoid bumping into things at chest height)
- Matches human locomotion: feet handle terrain, body avoids obstacles

### 4. **No Authority Arbitration**
- Lifter has authority over Y-axis (vertical position)
- Bumper has authority over XZ-plane (horizontal position)
- **Orthogonal responsibilities** → no conflicts

---

## Why Our Implementation Failed

We tried to make **both spheres** resolve **all collisions**:
- Lifter handles ground AND slopes AND walls (Phase 1b)
- Bumper handles ground AND walls (Phase 1a + Phase 2)
- Overlapping responsibilities → authority conflicts → bugs

**The correct design likely has:**
- Lifter handles ONLY ground surfaces (normal.y > threshold)
- Bumper handles ONLY vertical surfaces (normal.y ≤ threshold)
- **No overlap** → no authority conflicts

But even simpler: **Lifter drives Y, Bumper drives XZ**.

---

## How This Relates to Current Experiment

### The Single-Sphere System Is Working Because:
- We (accidentally) got the "single ground contact" part right
- Bumper-only collision has zero-penetration ground contact
- No burial depth, no adaptive logic, no authority arbitration
- **It's actually closer to the correct Overgrowth design than the dual-resolver was**

### The Irony:
We've been trying to implement a dual-sphere system, got it wrong, spent weeks debugging it, and the experiment to "simplify" it accidentally discovered what the original design might have been: **one sphere for ground, elevated sphere for obstacles**.

The single-sphere experiment isn't a simplification—**it's the ground contact sphere working correctly**.

---

## What We Should Actually Test Next

### Option A: Pure Single-Sphere (Current Experiment)
- Continue current test
- Validate that single-sphere feels good
- **Pros:** Maximally simple, working now
- **Cons:** No "body" collision buffer (bumper)

### Option B: Correct Dual-Sphere Configuration
- Lifter: Ground contact, zero penetration, drives Y position
- Bumper: Elevated, wall collision only, drives XZ position
- **Pros:** May be the actual Overgrowth design
- **Cons:** More complex than single-sphere

### Option C: Single-Sphere + Elevated Bumper (Hybrid)
- Keep current single-sphere logic for ground
- Add elevated bumper for wall collision only
- Bumper **never** touches ground (too high)
- **Pros:** Combines simplicity with "personal space" buffer
- **Cons:** Slightly more complex than pure single-sphere

---

## My Analysis

### The Missing Detail Problem

You're right: "lifter moves around on ground, bumper slides against walls" isn't much to go on.

**What we don't know from the GDC talk:**
- Exact sphere radii
- Exact vertical offset between spheres
- Which sphere has "authority" in what situations
- How penetration is resolved
- Whether lifter is ever buried (probably not)

**What we assumed (incorrectly):**
- Lifter should be buried for stability
- Both spheres resolve all collision types
- Complex authority arbitration is needed

**What's probably true:**
- Lifter rests on ground (zero penetration)
- Bumper is elevated, only hits obstacles
- Simple separation: lifter = Y-axis, bumper = XZ-plane

### The Harsh Truth

We may have been implementing **our interpretation of the design**, not **the actual design**. The screenshot suggests a fundamentally different configuration than what we built.

The dual-resolver system (main branch) might be solving problems that don't exist in the original design.

### The Good News

**The single-sphere experiment is working because we accidentally stumbled onto the correct principle:** clean ground contact with zero penetration.

Adding an elevated bumper sphere (Option C) might be the "correct" Overgrowth design, but the single-sphere proves the ground contact can work simply.

---

## Recommended Action

### 1. **Complete Single-Sphere Experiment** ✅
- Already working, keep testing
- Validates that ground contact alone is sufficient

### 2. **Document This Realization** ✅
- This file serves that purpose
- Update principle check with new understanding

### 3. **Time-Boxed Test: Elevated Bumper (2 hours)**
```cpp
// Keep current single-sphere logic for ground
// Add elevated bumper ONLY for walls
constexpr float BUMPER_ELEVATION = 0.40f;
sphere bumper;
bumper.center = position + glm::vec3(0, BUMPER_ELEVATION, 0);
bumper.radius = 0.45f;

// Bumper only tests horizontal collisions (normal.y ≤ 0.5)
for (auto& box : scn.collision_boxes()) {
    sphere_collision col = resolve_sphere_aabb(bumper, box);
    if (col.hit && col.normal.y <= 0.5f) {  // Walls only
        position += col.normal * col.penetration;  // Push XZ only
        // DON'T modify position.y (lifter handles that)
    }
}
```

### 4. **Compare Feel**
- Single-sphere (current)
- Single-sphere + elevated bumper (Option C)
- Which feels better?

---

## Final Thought

> "The video makes it seem so smooth and intuitive; but maybe there is just too much missing detail."

**Two possibilities:**

1. **Overgrowth's design is simpler than we thought** (likely)
   - Lifter on ground, bumper elevated, clean separation
   - We over-engineered it with burial depth and authority logic

2. **Overgrowth's design has hidden complexity we can't see** (possible)
   - There might be subtle tricks not visible in the talk
   - But Occam's Razor suggests it's actually simpler

**My bet:** The design is simpler. The screenshot shows clean sphere placement. Our implementation was complex because we made incorrect assumptions about burial depth and authority.

**The single-sphere experiment succeeded because it removed those incorrect assumptions.**

The elevated bumper (Option C) might be worth testing, but the single-sphere already proves the core principle works.

---

## Appendix: What We Got Wrong

| Aspect | Our Implementation | Likely Correct |
|--------|-------------------|----------------|
| Lifter position | Buried 0.10m in ground | Rests on ground surface |
| Bumper position | At character center | Elevated above center |
| Lifter penetration | Intentional burial | Zero penetration |
| Ground authority | Complex classification | Lifter handles all ground |
| Wall authority | Both spheres | Only bumper |
| Vertical position | Both spheres arbitrate | Lifter drives Y |
| Horizontal position | Both spheres arbitrate | Bumper drives XZ |
| Step-up mechanism | Intended burial allows climbing | Sphere geometry allows climbing |
| Complexity source | Authority arbitration | Clean separation of concerns |

**The core mistake:** We thought overlap and arbitration were necessary. The correct design likely uses **spatial separation** (elevation) to avoid overlap entirely.
