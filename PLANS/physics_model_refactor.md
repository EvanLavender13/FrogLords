# Phase 3.9: Physics Model Refactor
## Weightlifter-Primary Ground Collision

**Status:** REQUIRED before Phase 4

**Goal:** Invert bumper/weightlifter relationship to match correct physical model while preserving current feel.

---

## Problem Statement

### Current (Incorrect) Implementation
```
bumper sphere (character.position)
    ↓
collision resolution moves bumper
    ↓
ground_height recorded
    ↓
spring offset = 0.3m (cosmetic)
    ↓
render: position + spring_offset
    ↓
weightlifter = bumper + (0, -0.4, 0)  [UNUSED - never queried]
```

**Critical flaws:**
- Weightlifter sphere exists but does nothing (never used for collision)
- Spring is rendering trick, not physical constraint between bodies
- No "foot on ground, body suspended" relationship exists
- Blocks Phase 4 features (step-up, IK foot placement, leg compression)

### Correct Physical Model
```
weightlifter sphere ("foot")
    ↓
resolve_ground_collision(weightlifter)
    ↓
spring extends upward from weightlifter
    ↓
bumper = weightlifter + spring_length (compresses on impact)
    ↓
character.position = bumper (or weighted center)
```

**Why this matters:**
- Step-up detection needs weightlifter to probe ledges
- IK foot placement needs actual ground contact point
- Leg compression requires spring to deform pose, not translate whole body
- Clear physics model prevents future confusion

---

## Design Principles Applied

**From CLAUDE.md:**
1. **Clarity over cleverness** - Correct model is more understandable
2. **Simplicity over sophistication** - Two spheres with clear roles
3. **Stupidly simple cores, emergent complexity** - Spring connects two points
4. **Physics core never compromised** - Fix foundation before building up

**From Animation.md:**
- "First, do no harm to gameplay" - Preserve responsive feel
- "Build around simple primitives" - Weightlifter/bumper clarity
- "Physics core → reactive systems" - Fix Layer 1 before Layer 4

**From PhysicsSystemDesign.md:**
- "Physics simulation is single source of truth"
- "Predictable, deterministic integration"
- Work bottom-up through stack

---

## Implementation Approach

### Phase 1: Refactor Ground Collision (Core) ✅ COMPLETE

**Status:** Implemented and validated - feels great!

**Implementation:**

Ground collision now uses weightlifter sphere ([character_controller.cpp:121-155](../src/character/character_controller.cpp#L121-155)):

```cpp
void character_controller::resolve_ground_collision() {
    // Update weightlifter position before collision check
    weightlifter.center = position + vec3(0.0f, -0.4f, 0.0f);

    // Simple ground plane at y=0
    float distance_to_ground = weightlifter.center.y - ground_y;
    float penetration = weightlifter.radius - distance_to_ground;

    if (penetration > 0.0f) {
        // Push weightlifter out of ground
        weightlifter.center.y += penetration;

        // Update position to maintain offset
        position = weightlifter.center + vec3(0.0f, 0.4f, 0.0f);

        // Remove velocity into ground
        if (velocity.y < 0.0f) {
            velocity.y = 0.0f;
        }

        is_grounded = true;
    }
}
```

Box collision checks weightlifter first ([character_controller.cpp:157-205](../src/character/character_controller.cpp#L157-205)):

```cpp
void character_controller::resolve_box_collisions(const scene* scn) {
    for (const auto& box : scn->collision_boxes()) {
        // Check weightlifter first (ground contact)
        sphere weightlifter_copy = weightlifter;
        weightlifter_copy.center = position + vec3(0.0f, -0.4f, 0.0f);

        if (box.resolve_sphere_collision(weightlifter_copy, collision_normal)) {
            // Update position based on weightlifter displacement
            vec3 displacement = weightlifter_copy.center - weightlifter.center;
            position += displacement;
            weightlifter.center = weightlifter_copy.center;

            if (collision_normal.y > 0.7f) {
                is_grounded = true;
                ground_height = box.center.y + box.half_extents.y;
            }
        }

        // Then check bumper (obstacle/ceiling collisions)
        sphere bumper_copy = bumper;
        bumper_copy.center = position;

        if (box.resolve_sphere_collision(bumper_copy, collision_normal)) {
            position = bumper_copy.center;
            // Remove velocity into surface...
        }
    }
}
```

**Key fixes:**
- Initial position adjusted to 0.4m (accounts for weightlifter radius 0.3m + spring offset 0.3m)
- Weightlifter visualization uses actual physics position `character->weightlifter.center` ([main.cpp:310](../src/main.cpp#L310))
- Spring offset only affects bumper rendering, not weightlifter

**Validation:**
- ✅ Weightlifter touches ground when standing idle
- ✅ Movement and jumping feel preserved
- ✅ Platform collision works correctly
- ✅ No visual or gameplay regressions

**Debug visualization improvements:**
- Debug feet now follow weightlifter ground contact height ([main.cpp:370](../src/main.cpp#L370))
- Feet correctly appear on platforms, not stuck at y=0

### Phase 2: Spring Connects Weightlifter → Bumper

**Spring becomes structural element:**

```cpp
// locomotion_system.h
class locomotion_system {
    spring_damper vertical_spring;  // Connects weightlifter to bumper

    // Spring equilibrium = distance between spheres at rest
    static constexpr float rest_length = 0.4f;  // Bumper/weightlifter offset

    void update(const character_state& state, float dt);
    skeletal_pose get_current_pose() const;
};
```

**Spring physically separates spheres:**

```cpp
void locomotion_system::update(const character_state& state, float dt) {
    // Spring seeks rest length (0.4m separation)
    vertical_spring.update(rest_length, dt);

    // Step impulses compress spring
    if (is_left_step(prev_phase, current_phase) || is_right_step(prev_phase, current_phase)) {
        float impulse_magnitude = state.speed * step_impulse_scale;
        vertical_spring.apply_impulse(-impulse_magnitude);  // Compress
    }

    // Landing impacts compress spring
    if (state.just_landed) {
        float impact = state.landing_velocity * landing_scale;
        vertical_spring.apply_impulse(impact);  // Compress (velocity is negative)
    }
}

skeletal_pose locomotion_system::get_current_pose() const {
    // Spring length determines bumper offset from weightlifter
    float spring_length = vertical_spring.get_position();

    pose.root_offset.y = spring_length;  // Bumper height above weightlifter

    return pose;
}
```

### Phase 3: Character Position Calculation

**Position as weighted center or direct assignment:**

**Option A: Weighted center (more physically accurate)**
```cpp
void character_controller::update_position() {
    // Weightlifter at ground, bumper suspended by spring
    bumper.center = weightlifter.center + vec3(0, spring_length, 0);

    // Position = weighted average (or just use bumper)
    position = (weightlifter.center + bumper.center) * 0.5f;
}
```

**Option B: Direct bumper tracking (simpler)**
```cpp
void character_controller::update_position() {
    // Weightlifter at ground, bumper suspended by spring
    bumper.center = weightlifter.center + vec3(0, spring_length, 0);

    // Position = bumper (upper body is "character center")
    position = bumper.center;
}
```

**Recommendation:** Start with Option B (simpler, matches current behavior).

### Phase 4: Preserve Current Feel

**Tuning to match Phase 3.5 behavior:**

Current spring seeks `0.3m` above ground. New spring connects two spheres `0.4m` apart at rest.

**Parameter mapping:**
```cpp
// OLD (Phase 3.5): Spring offset from ground
target_height = ground_height + 0.3f;

// NEW (Phase 3.9): Spring rest length between spheres
rest_length = 0.4f;  // Weightlifter radius (0.3) + bumper radius (0.3) - overlap (0.2)

// Visual result should match: bumper at ~0.3m above ground
// Weightlifter at ground (0.0) + spring (0.4) - bumper_radius (0.3) = 0.1m error
```

**Correct rest length calculation:**
```cpp
// Weightlifter radius: 0.3m
// Bumper radius: 0.3m
// Desired bumper center height: 0.3m above ground
// Therefore: weightlifter.center.y = 0.3 (grounded)
//            bumper.center.y = 0.3 + spring_length
// For bumper center at 0.6m (0.3 above ground): spring_length = 0.3m

rest_length = 0.3f;  // Matches current visual result
```

**Validation:**
- Standing idle: bumper at same height as Phase 3.5
- Landing compression: same depth, same recovery
- Step impulses: same bounce rhythm

---

## Implementation Steps

### Task 1: Update Ground Collision (1 hour)
1. Move ground collision from bumper to weightlifter
2. Update `resolve_ground_collision()` to use weightlifter sphere
3. Update box collision to check weightlifter first
4. Test on flat ground and platforms

### Task 2: Restructure Spring Connection (1 hour)
1. Change spring target from `ground_height + 0.3` to `rest_length` constant
2. Calculate bumper position from weightlifter + spring_length
3. Update position calculation (choose Option A or B)
4. Verify spring compression/extension direction correct

### Task 3: Preserve Current Feel (1-2 hours)
1. Test standing idle height (should match Phase 3.5)
2. Test landing compression depth (should match Phase 3.5)
3. Adjust `rest_length` parameter if needed
4. Test step impulse bounce (should match Phase 3.5)
5. Iterate until feel identical to Phase 3.5

### Task 4: Enable Phase 4 Features (30 min)
1. Add step-up probe using weightlifter position
2. Document weightlifter as ground truth for IK foot targets
3. Document spring as leg compression driver (Phase 4 TODO)
4. Update comments to reflect correct physical model

### Task 5: Validation Testing (30 min)
1. Flat ground locomotion (idle, walk, run)
2. Platform jumping and landing
3. Edge cases (rapid transitions, high falls)
4. Compare feel to Phase 3.5 recordings/notes
5. Verify deterministic behavior (same input → same output)

**Total estimate: 4-5 hours**

---

## Success Criteria

### Functional Requirements
- ✅ Weightlifter handles all ground collision (flat plane + platforms)
- ✅ Spring physically connects weightlifter → bumper
- ✅ Bumper position calculated from weightlifter + spring_length
- ✅ Character position derived from sphere positions (not independent)

### Feel Requirements
- ✅ Identical to Phase 3.5 behavior (standing, walking, running, landing)
- ✅ Same spring compression depth on impacts
- ✅ Same bounce rhythm during locomotion
- ✅ No regression in responsiveness

### Technical Requirements
- ✅ Weightlifter usable for step-up detection (Phase 4)
- ✅ Weightlifter provides IK foot target (Phase 4)
- ✅ Spring length can drive leg compression (Phase 4)
- ✅ Clear separation of concerns (ground vs. obstacle collision)

---

## Validation Protocol

**No automated tests** (project principle). Validate through runtime observation.

### Test Sequence (30 min)

**1. Ground collision validation:**
- Stand idle on flat ground → same height as Phase 3.5
- Walk/run on flat ground → same bounce as Phase 3.5
- Jump and land → same compression as Phase 3.5

**2. Platform collision validation:**
- Jump onto low platform → lands correctly
- Jump onto high platform → heavy impact, deep compression
- Walk off platform → falls and lands smoothly

**3. Spring behavior validation:**
- Monitor spring graph → same shape as Phase 3.5
- Check compression depth → same magnitude as Phase 3.5
- Verify recovery time → same timing as Phase 3.5

**4. Edge case validation:**
- Rapid direction changes → stable
- High-speed platform approach → no explosions
- Multiple quick jumps → deterministic

**5. Phase 4 readiness:**
- Weightlifter position logged at ground contact
- Spring length usable for leg deformation (verify value range)
- Step-up logic can query weightlifter (code review, no runtime test yet)

---

## File Changes

**Modified files:**
- `src/character/character_controller.h` - Weightlifter-primary collision
- `src/character/character_controller.cpp` - Update collision resolution
- `src/character/locomotion.h` - Spring as structural connection
- `src/character/locomotion.cpp` - Spring rest length, position calc

**No new files needed.**

**No changes expected:**
- `src/foundation/spring_damper.h/cpp` - Math unchanged
- `src/rendering/scene.h/cpp` - Collision primitives unchanged
- `src/main.cpp` - Usage unchanged (internal refactor)

---

## Risk Mitigation

**Risk: Feel changes during refactor**
- Mitigation: Record Phase 3.5 spring compression values before starting
- Mitigation: Tune `rest_length` parameter to match exact height
- Mitigation: Compare spring graphs side-by-side

**Risk: Position calculation breaks collision**
- Mitigation: Start with Option B (bumper = position, simpler)
- Mitigation: Validate collision on each step (Task 1 → 2 → 3)
- Mitigation: Keep Phase 3.5 code available for reference

**Risk: Spring direction inverts**
- Mitigation: Clear diagram of weightlifter → spring → bumper flow
- Mitigation: Verify impulse signs (compression = negative displacement)
- Mitigation: Test landing and step separately

---

## Phase 4 Unlocks

**Once refactor complete:**
- ✅ Step-up detection: Probe ledges with weightlifter position
- ✅ IK foot placement: Use weightlifter as ground truth
- ✅ Leg compression: Spring length drives knee bend angle
- ✅ Skeleton hierarchy: Spring deforms pose instead of translating whole body

**Physical model clarity:**
- Weightlifter = foot contact point
- Spring = leg/suspension compliance
- Bumper = torso collision volume
- Position = body center of mass

---

## Deferred Items

**Not in Phase 3.9 scope:**

### Slope/Angled Surface Handling
- Current: Only flat ground and platform tops (normal.y > 0.7)
- Deferred: Ramp/slope locomotion (no test geometry exists)
- When: Phase 3.6 or when slopes added to game

### Step-Up Implementation
- Current: Step-up detection enabled by weightlifter position
- Deferred: Actual step-up logic (requires ledge raycasting)
- When: Phase 4+ or when needed for level design

### Spring Drives Leg IK
- Current: Spring translates whole body (root offset)
- Deferred: Spring drives knee bend via IK targets
- When: Phase 4 after skeleton system implemented

---

## Anti-Patterns to Avoid

**Don't:**
- Change parameters without recording Phase 3.5 baseline
- Implement step-up during refactor (separate concerns)
- Add slopes or complex terrain (validate on simple first)
- Optimize prematurely (correctness before performance)

**Do:**
- Preserve exact feel from Phase 3.5
- Test incrementally (Task 1 → 2 → 3 → 4 → 5)
- Document physical model clearly
- Keep refactor minimal (only collision + spring, no features)

---

## Lessons Applied

**From Phase 3.5:**
- Current implementation works and feels good (ship it, then refactor)
- Graybox reveals conceptual issues early (fix before detail)
- Refactor deserves dedicated session (fresh context)

**From project principles:**
- "Work bottom-up through stack" - Fix Layer 1 before Layer 4
- "Detail only current implementation" - No Phase 4 features yet
- "Expect upper stack changes" - But don't let bad foundation propagate

---

> "The best time to fix technical debt is after you've validated the feel. The second-best time is before you build on top of it."

**Commitment:** This refactor is non-negotiable before Phase 4. Core physics model must be correct.

Preserve feel. Fix implementation. Unlock Phase 4.
