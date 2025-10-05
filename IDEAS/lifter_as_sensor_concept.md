# Idea: Lifter Sphere as Collision Sensor (Not Resolver)

**Date:** October 5, 2025  
**Context:** Single-sphere collision experiment, observation during playtesting  
**Status:** Conceptual - Not yet implemented

---

## Core Concept

**Current Paradigm (Dual-Sphere Resolver):**
- Lifter sphere *resolves* collisions → directly modifies character position
- Bumper sphere *resolves* collisions → directly modifies character position
- Requires authority arbitration between the two spheres

**Proposed Paradigm (Single Resolver + Sensor):**
- **Bumper sphere** is the ONLY resolver → modifies character position
- **Lifter sphere** is a SENSOR → detects terrain, influences bumper's movement behavior
- No authority arbitration needed (bumper always has authority)

---

## The Insight

> "What if the purpose of the lifter sphere wasn't to collide and be resolved; but to collide and inform the movement of the bumper?"

This reframes the lifter from a **physics object** to a **perception system**.

### Natural World Analogy

**Animals don't have multiple collision-resolving body parts:**
- Eyes detect obstacles → inform leg movement
- Whiskers detect proximity → guide body navigation
- Feet probe ground → adjust gait and posture

**The lifter sphere could function like:**
- A **foot probe** that detects upcoming terrain changes
- An **anticipatory sensor** that modifies walking behavior before the main body (bumper) collides
- A **terrain classifier** that adjusts movement parameters based on surface type

---

## How It Could Work

### 1. Sensor Detection Phase

```cpp
// Lifter detects terrain (does NOT resolve position)
sphere_collision lifter_contact = detect_sphere_aabb(weightlifter, box);

if (lifter_contact.hit) {
    // Classify what the lifter is sensing
    terrain_info info;
    info.type = classify_terrain(lifter_contact.normal);
    info.distance = lifter_contact.penetration;
    info.normal = lifter_contact.normal;
    
    // Store for bumper to use
    sensed_terrain = info;
}
```

### 2. Bumper Resolution with Sensor Data

```cpp
// Bumper resolves collision (ONLY bumper modifies position)
sphere_collision bumper_contact = resolve_sphere_aabb(bumper, box);

if (bumper_contact.hit) {
    // Standard collision resolution
    position += bumper_contact.normal * bumper_contact.penetration;
    
    // BUT: Use lifter sensor data to modify behavior
    if (sensed_terrain.type == UPCOMING_STEP) {
        // Apply upward "hop" impulse to help climb
        velocity.y += calculate_step_assist(sensed_terrain.distance);
    }
    else if (sensed_terrain.type == STEEP_SLOPE) {
        // Reduce horizontal speed for stability
        velocity *= 0.8f;
    }
    else if (sensed_terrain.type == EDGE_DETECTED) {
        // Slow down near edges
        velocity *= 0.5f;
    }
}
```

---

## Potential Benefits

### 1. **Eliminates Authority Arbitration**
- Only one sphere modifies position (bumper)
- No surface classification thresholds needed
- No adaptive penetration limits
- No "which sphere wins?" logic

### 2. **Emergent Behavior Through Sensor Feedback**
- Lifter detects step → bumper applies small upward impulse → natural climbing motion
- Lifter detects edge → bumper slows down → cautious navigation
- Lifter detects slope → bumper adjusts friction → stable hill traversal

### 3. **Separation of Concerns**
- **Bumper:** "Where can I physically be?" (collision resolution)
- **Lifter:** "What terrain am I about to encounter?" (perception/prediction)
- Clean conceptual division

### 4. **Predictive Movement**
- Lifter sphere is offset *below* bumper → it hits terrain *first*
- Provides "look-ahead" information before bumper collides
- Like a foot testing the ground before committing weight

### 5. **Tunable Assistance**
- Step-up "hop" force can be tuned without changing collision geometry
- Slope stability can be parameter-driven
- Edge detection sensitivity is adjustable

---

## Implementation Complexity Analysis

### Simpler Than Dual-Resolver
- ✅ No authority arbitration between spheres
- ✅ No overlapping responsibilities
- ✅ No surface classification for authority
- ✅ Single source of truth for position (bumper only)

### More Complex Than Pure Single-Sphere
- ❌ Requires terrain classification logic
- ❌ Requires sensor → behavior mapping rules
- ❌ Requires tuning sensor response parameters
- ❌ Still need to test both spheres (though only bumper resolves)

### Compared to Current System
- **Dual-Resolver (Current):** ~150 lines, 3 loops, 2 authority thresholds
- **Single-Sphere (Experiment):** ~45 lines, 1 loop, 0 thresholds
- **Sensor-Based (Hypothetical):** ~80-100 lines, 1 loop, 0 authority thresholds, N behavior rules

**Verdict:** More complex than pure single-sphere, but potentially more capable without authority arbitration.

---

## Potential Behaviors Enabled

### 1. **Intelligent Step-Up**
- Lifter detects obstacle height
- If height < 0.30m and velocity.y ≈ 0: Apply upward hop (assists climbing)
- If height ≥ 0.30m: Do nothing (let player jump manually)
- **Result:** Automatic traversal of small steps without teleportation feeling

### 2. **Slope Stability**
- Lifter detects steep slope (normal.y < 0.7)
- Bumper reduces horizontal acceleration (harder to climb)
- Bumper increases friction (less sliding down)
- **Result:** Natural difficulty scaling with terrain steepness

### 3. **Edge Detection**
- Lifter enters air while bumper is grounded
- Character is approaching a ledge
- Reduce max speed or apply backward bias force
- **Result:** Safer navigation near drop-offs (optional player-assist)

### 4. **Terrain-Adaptive Movement**
- Lifter classifies surface material (box color? metadata?)
- Bumper adjusts friction/bounce based on material
- **Result:** Ice = slippery, rubber = bouncy, etc. (future feature)

### 5. **Look-Ahead Collision Avoidance**
- Lifter detects wall while moving forward
- Bumper pre-emptively reduces speed
- **Result:** Smoother deceleration when approaching obstacles

---

## Alignment with Project Principles

### ✅ **Simplicity over Sophistication**
- Single resolver (bumper) is simpler than dual-resolver
- Sensor data is optional enhancement, not core logic

### ✅ **Elegant Systems: Simple Components, Complex Interactions**
- Bumper: Simple collision resolution
- Lifter: Simple sensor detection
- **Interaction:** Sensor data modifies resolver behavior → emergent movement

### ✅ **Procedural Foundation: Reactive Systems Interpret Core State**
- **Core logic:** Bumper collision resolution (never compromised)
- **Reactive system:** Lifter sensor interpretation (enhances core logic)
- Proper layering maintained

### ✅ **Multi-Use Tools**
- Lifter serves multiple purposes:
  - Step-up assistance
  - Edge detection
  - Slope classification
  - Terrain prediction
- One component, many emergent uses

### ⚠️ **Clarity over Cleverness**
- **Potential concern:** Sensor → behavior mapping could become a complex ruleset
- **Mitigation:** Keep sensor responses simple and parameter-driven
- **Test:** If rules exceed ~5 cases, it's too clever

---

## Comparison to Current Systems

### Dual-Resolver (Main Branch)
- **Pros:** Step-up works naturally (when it works)
- **Cons:** Authority arbitration, bug-prone, complex

### Pure Single-Sphere (Current Experiment)
- **Pros:** Maximally simple, stable, predictable
- **Cons:** No step-up capability, no terrain adaptation

### Sensor-Based (This Idea)
- **Pros:** Single resolver (simple), sensor enables features (capable), no authority arbitration
- **Cons:** More complex than pure single-sphere, requires tuning sensor responses
- **Sweet spot?** Potentially balances simplicity and capability

---

## Questions to Answer Through Experimentation

### 1. **Does Sensor-Assisted Movement Feel Natural?**
- Is the lifter → hop transition smooth or jarring?
- Does it feel like "assisted climbing" or "automatic teleportation"?
- **Test:** Implement 0.15m step with small upward impulse when lifter detects it

### 2. **How Many Sensor Rules Are Needed?**
- Can we get good behavior with 2-3 rules?
- Or does it balloon into 10+ special cases?
- **Principle:** If > 5 rules, it's too complex

### 3. **Is It Better Than Pure Single-Sphere?**
- Single-sphere is working well in current experiment
- Does sensor-based add enough value to justify the complexity?
- **Test:** Compare feel side-by-side

### 4. **Can Sensor Data Be Ignored by Default?**
- Start with sensor detection but no behavior changes
- Add behaviors incrementally
- **Goal:** Prove sensor doesn't complicate the core

---

## Implementation Roadmap (If Pursued)

### Phase 1: Sensor Detection Only (No Behavior Changes)
1. Add `terrain_sensor` struct to controller
2. Lifter detects collisions, populates sensor data
3. Bumper ignores sensor data, resolves normally
4. **Validate:** No change in movement feel (baseline)

### Phase 2: Single Sensor Behavior (Step-Up)
5. If lifter detects obstacle < 0.25m and bumper hits it
6. Apply small upward velocity impulse (e.g., +1.5 m/s)
7. **Test:** Does this feel like natural climbing or jarring hop?

### Phase 3: Tune and Validate
8. Adjust impulse magnitude, obstacle height threshold
9. Compare feel to pure single-sphere
10. **Decision:** Keep sensor-based or revert to pure single-sphere?

### Phase 4: Optional Extensions (Only if Phase 3 succeeds)
11. Add slope stability (lifter detects steep slope → reduce acceleration)
12. Add edge detection (lifter in air → slow down near ledges)
13. Add terrain material response (future feature)

---

## Recommended Action

### Immediate: **Complete Single-Sphere Experiment First**
- Don't pursue sensor-based approach until single-sphere is fully validated
- The single-sphere is working well; finish playtesting it
- Document results in principle check review

### Future: **Time-Boxed Sensor Experiment (If Single-Sphere Feels Limiting)**
- Only pursue if playtesting reveals: "I wish I could climb 0.15m steps automatically"
- Time-box to 2-3 hours (same as single-sphere experiment)
- Use sensor-based as fallback if single-sphere is too limiting

### Long-Term: **Likely Outcome**
- **Prediction:** Pure single-sphere will feel good enough
- Step-up for 0.15m obstacles is probably unnecessary (players can jump)
- Sensor-based is interesting but likely over-engineered for current needs
- **Archive this idea** for future reference if terrain complexity increases

---

## Final Thought

> "Simple is the law of the natural world."

The sensor-based approach is **conceptually elegant** (separation of perception and action), but **pragmatically complex** (requires ruleset for sensor → behavior mapping).

The pure single-sphere approach is **maximally simple** and aligns perfectly with the project's principles.

**Occam's Razor:** The simplest solution that works is usually correct. The single-sphere is working in the current experiment. The sensor-based approach is a **solution in search of a problem**.

**However:** If future playtesting reveals that players genuinely miss automatic step-up, the sensor paradigm is a **better foundation** than the dual-resolver paradigm. It avoids authority arbitration while enabling terrain-adaptive behavior.

**Status:** Interesting idea. Archive for later. Continue single-sphere experiment.

---

## Appendix: Why Animals Don't Use Dual-Resolvers

Biological locomotion systems don't have "two collision-resolving body parts that arbitrate authority." Instead:

- **Sensors detect** (eyes, whiskers, antennae, feet)
- **Brain interprets** (terrain classification, obstacle analysis)
- **Muscles execute** (legs, wings, fins)

The proposed sensor-based system mimics this:
- **Lifter detects** (collision sensor)
- **Controller interprets** (terrain classification, behavior rules)
- **Bumper executes** (position resolution, velocity changes)

This is more **biomimetic** than dual-resolver, which has no natural analog.

But nature also demonstrates that **simpler is better**: Single-celled organisms navigate complex environments with minimal sensing. The pure single-sphere may be the "single-celled organism" solution—no brain, no sensors, just direct reaction to immediate collisions. And it works.
