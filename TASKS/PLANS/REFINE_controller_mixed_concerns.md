# Refinement: Controller Mixed Concerns

**Extract animation and orientation subsystems from controller for clean composition**

---

## Selected

**Date:** 2025-10-17
**Complexity:** Standard
**Path:** B (standard refinement workflow)

---

## Violation Details

**Location:**
- File: `src/character/controller.{h,cpp}`
- Lines: 114-117 (subsystem members), 141-150 (update_reactive_systems), 153-167 (get_world_transform)
- System: Character Controller (Layer 3)

**Principle Violated:**
- Primary: **Composable Functions**
- Score before: 9.0/10

**Severity:** HIGH

**Type:** Coupling - three distinct concerns in one struct

---

## Current State

**What exists now:**

```cpp
struct controller {
    // ... physics state (position, velocity, acceleration) ...

    // Reactive animation layer
    character::animation_state animation;

    // Orientation system
    orientation_system orientation;

    void apply_input(const camera_input_params& cam_params, float dt);
    void update(const collision_world* world, float dt);
    void update_reactive_systems(float dt);  // ← Updates both subsystems
    glm::mat4 get_world_transform() const;   // ← Composes from both subsystems
};
```

The controller is responsible for:
1. **Physics simulation** (apply_input, update) - core responsibility
2. **Animation state** (landing springs, vertical offset)
3. **Orientation** (yaw rotation from input)

**Why this violates principles:**

- **Composability broken:** Can't modify animation without touching controller
- **Coupling:** Three systems in one struct means three reasons to change
- **Testing difficulty:** Can't test animation or orientation in isolation
- **Reusability blocked:** Can't reuse animation system for other entities
- **Special case risk:** Future features (different character types) force controller to handle all variants

**Impact:**
- Blocks: Independent animation/orientation development
- Cascades to: Any system that wants to add character variants
- Foundation impact: Keeps Layer 3 at 94% (need 95%)

---

## Fix Approach

**Primary approach:** SIMPLIFY

**From:** Controller owns and manages subsystems

**To:** Controller provides physics state; subsystems read from it

**Strategy: Separation with unidirectional data flow**

### Option A: Extract subsystems entirely (preferred)

**Controller becomes:**
```cpp
struct controller {
    // Physics state only (source of truth)
    glm::vec3 position;
    glm::vec3 velocity;
    // ... other physics fields ...

    void apply_input(...);
    void update(...);
    // NO subsystems, NO update_reactive_systems
};
```

**Subsystems become independent:**
```cpp
struct character_reactive_systems {
    orientation_system orientation;
    character::animation_state animation;

    void update(const controller& ctrl, float dt);
    glm::mat4 get_visual_transform(const controller& ctrl) const;
};
```

**Data flow:**
```
Input → Controller (physics) → Reactive Systems (visual) → Rendering
```

**Pros:**
- Clean separation of concerns
- Controller testable in isolation
- Subsystems reusable
- Unidirectional data flow

**Cons:**
- Need to pass controller to subsystems
- One more type to manage
- Caller must coordinate update order

### Option B: Keep subsystems but enforce boundaries (fallback)

Keep structure but document that:
- Controller owns physics
- Subsystems are read-only consumers
- No bidirectional dependencies

**Pros:**
- Minimal code change
- Less refactoring risk

**Cons:**
- Still coupled at struct level
- Doesn't solve composability
- Only documents the problem, doesn't fix it

**Decision: Option A** - proper separation worth the effort

---

## Implementation Plan

1. **Create character_reactive_systems.{h,cpp}**
   - Move orientation_system member
   - Move animation_state member
   - Add update(const controller&, float dt)
   - Add get_visual_transform(const controller&)

2. **Update controller.{h,cpp}**
   - Remove animation member
   - Remove orientation member
   - Remove update_reactive_systems()
   - Remove get_world_transform() (moves to reactive_systems)

3. **Update game_world.cpp** (or wherever controller is used)
   - Add character_reactive_systems instance
   - Call reactive_systems.update(controller, dt) after controller.update()
   - Use reactive_systems.get_visual_transform(controller) for rendering

4. **Update any other usage sites**
   - Search for controller.orientation
   - Search for controller.animation
   - Redirect to reactive_systems

5. **Validation**
   - Build succeeds
   - Behavior identical (orientation and landing animation still work)
   - No new dependencies introduced

---

## Success Criteria

**This refinement succeeds when:**
- [x] Controller contains only physics state and logic
- [x] Animation and orientation extracted to separate type
- [x] Data flow is unidirectional (controller → reactive_systems)
- [x] All tests passing
- [x] Visual behavior identical (orientation, landing spring)
- [x] No bidirectional dependencies
- [x] Subsystems can be tested independently

---

## Estimated Metrics

**Before:**
- LOC controller.h: 126
- LOC controller.cpp: 168
- Principle score (Composable Functions): 9.0/10
- Responsibilities: 3 (physics, animation, orientation)
- Coupling: High (all in one struct)

**After (estimated):**
- LOC controller.h: ~90 (-36)
- LOC controller.cpp: ~140 (-28)
- LOC character_reactive_systems.h: ~30 (new)
- LOC character_reactive_systems.cpp: ~40 (new)
- Net LOC change: +6 (acceptable for decoupling)
- Principle score: 10.0/10 (+1.0)
- Responsibilities: 1 (physics only)
- Coupling: None (unidirectional)

**Layer 3 impact:**
- Controller: 90% → 100% (+10%)
- Weighted: 94% → ~95% (+1%)

---

## Risk Assessment

**What could go wrong:**
- Break update order dependencies (animation expects post-physics state)
- Lose just_landed flag management (lifecycle across update calls)
- Transform composition order changes (orientation × animation)
- Performance regression (extra indirection)

**Mitigation:**
- Document explicit update order in reactive_systems
- Keep just_landed management in controller, reactive_systems reads it
- Test transform composition explicitly
- Profile before/after (expect zero overhead)

**Rollback plan:**
- Revert commit
- Delete character_reactive_systems files
- Restore controller members
- Clean rebuild

---

## Notes

**Why this matters:**

This separation enables:
1. **Future character variants** - different entities can share reactive_systems
2. **Independent testing** - test physics without animation overhead
3. **Animation reuse** - landing spring usable on other entities
4. **Clear boundaries** - physics vs visual separation explicit

**Philosophical alignment:**

> "Build from orthogonal primitives. Each piece does exactly one thing, does it perfectly, and combines cleanly with others."

Controller should do physics. Reactive systems should do visual response. They compose, not merge.

---

**Delete first. Simplify second. Document last.**
