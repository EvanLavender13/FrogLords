# Plan: Debug Visualization System

**Status:** READY FOR IMPLEMENTATION
**Created:** 2025-10-17
**Complexity:** 3 points
**Certainty:** 95% (pending Stage 0 reconnaissance)
**Estimated Time:** 11 hours
**Estimated Lines:** ~340 lines

---

## The System

**Primitive:** Visual representation of mathematical state
**Mathematics:** Linear transformations, vector operations, spherical geometry
**Composes With:** All systems (physics, animation, collision, input, state machines)

**Core Truth:** 1:1 mapping between internal state and visual representation. Visual correctness = mathematical correctness.

---

## The Validation

### Isolation Tests

**Mathematical Correctness:**
- Right-hand rule: X × Y = Z
- Vector normalization preserved after operations
- Arrow length proportional to velocity magnitude
- Sphere radius matches physics collision radius
- Transform extraction preserves orthonormality

**Assertion-Based Validation:**
- Preconditions on all inputs (finite, positive, normalized)
- Postconditions on all outputs (distances correct, properties preserved)
- Invariants on mathematical properties (orthonormality, right-handedness)
- Startup checks for foundation assumptions (GLM coordinate system)

### Debug Visualization

**The system IS the debug visualization.** Visual correctness = mathematical correctness.

**Visual Tests:**
- RGB axes form orthogonal triad
- Velocity arrows point in movement direction
- Wireframes align with collision boundaries
- Transforms follow entity motion

### Success Criteria

**Mathematical:**
- All assertions pass in debug builds
- Startup validation checks pass
- No NaN/Inf propagation

**Visual:**
- Axes match camera controls (W = forward = +Z blue)
- Arrows scale with velocity
- Wireframes touch ground when grounded
- Toggle on/off with F3, zero gameplay impact

---

## The Foundation

### Dependencies (All Present)

✓ Rendering core (100% - sokol-gfx)
✓ Scene graph (100% - existing)
✓ Coordinate system (95% - will validate to 100%)
✓ GLM math library (100%)
✓ Debug line rendering (EXISTS - `src/rendering/debug_draw.cpp`)

**Discovery:** Debug draw already exists! Must investigate current implementation.

---

## The Stages

### Stage 0: Reconnaissance [FIRST]

**Principle:** Build on what exists (Addition by Subtraction)
**Purpose:** Understand existing debug_draw before planning changes

**Actions:**
1. Read `src/rendering/debug_draw.h` and `debug_draw.cpp`
2. Read `src/rendering/debug_primitives.h` (if exists)
3. Check what debug rendering capabilities already exist
4. Identify what's missing vs what we need

**Validation:**
- [ ] Know what exists
- [ ] Know what's missing
- [ ] Know if we're adding or replacing

**Outcome:** Refined plan based on reality, not assumptions

**Time:** 2 hours
**Risk:** None (reconnaissance only)

---

### Stage 1: Assertion Framework Foundation

**Principle:** Solid Mathematical Foundations
**Purpose:** Establish validation infrastructure before building system

**Files to Create:**
- `src/foundation/debug_assert.h` - Assertion macros and helpers

**Implementation:**
```cpp
// Core macros
FL_ASSERT(expr, msg)
FL_ASSERT_NORMALIZED(v, msg)
FL_ASSERT_FINITE(v, msg)
FL_ASSERT_POSITIVE(x, msg)
FL_PRECONDITION(expr, msg)
FL_POSTCONDITION(expr, msg)
FL_INVARIANT(expr, msg)

// Helper functions
verify_coordinate_frame(x, y, z, context)
```

**Key Design:**
- Compiled out in release builds (`#ifdef _DEBUG`)
- Math-aware (epsilon comparisons for floats)
- Self-documenting (clear messages)
- Zero dependencies (just cassert + GLM)

**Validation:**
- [ ] Compiles in debug and release modes
- [ ] Assertions trigger on violations
- [ ] No overhead in release builds
- [ ] Messages are clear and actionable

**Estimated Lines:** ~100 lines
**Time:** 1 hour
**Risk:** Low (pure validation code, no side effects)

---

### Stage 2: Reconnaissance Results & API Design

**Principle:** Radical Simplicity
**Purpose:** Design minimal API based on existing code + needs

**After Stage 0, define:**

**API Requirements:**
```cpp
namespace debug_draw {
    // Core primitives
    void axes(const mat4& transform, float length);
    void velocity_arrow(vec3 position, vec3 velocity, float scale);
    void sphere_wireframe(vec3 center, float radius);

    // Supporting functionality
    void arrow(vec3 start, vec3 direction, float length, vec3 color);
    void line(vec3 start, vec3 end, vec3 color);

    // Lifecycle
    void init();
    void begin_frame();
    void end_frame();

    // Control
    void set_enabled(bool enabled);
    bool is_enabled();

    // Startup validation
    namespace validation {
        void run_startup_checks();
    }
}
```

**Design Decisions:**
- Immediate mode (no retained geometry)
- World-space coordinates (no manual projection)
- Color constants: RGB_RED, RGB_GREEN, RGB_BLUE, RGB_MAGENTA
- Toggle key: F3 (standard debug convention)

**Validation:**
- [ ] API is minimal (nothing extra)
- [ ] API is composable (primitives combine)
- [ ] API is stateless (pure functions where possible)
- [ ] API fits existing rendering patterns

**Estimated Lines:** 0 (just documentation)
**Time:** 30 minutes (after reconnaissance)
**Risk:** Low (design phase)

---

### Stage 3: Core Primitive - RGB Axes

**Principle:** Bottom-Up Construction
**Purpose:** First visible primitive - validates coordinate system

**Files to Modify:**
- `src/rendering/debug_draw.h` - Add axes() declaration
- `src/rendering/debug_draw.cpp` - Implement axes()

**Implementation:**
```cpp
void axes(const glm::mat4& transform, float length) {
    FL_PRECONDITION(length > 0.0f, "axis length must be positive");

    // Extract axes from transform
    vec3 x_axis = vec3(transform[0]);
    vec3 y_axis = vec3(transform[1]);
    vec3 z_axis = vec3(transform[2]);
    vec3 position = vec3(transform[3]);

    // Validate transform
    verify_coordinate_frame(x_axis, y_axis, z_axis, "axes()");
    FL_ASSERT_FINITE(position, "axes position");

    // Draw axes
    line(position, position + x_axis * length, RGB_RED);
    line(position, position + y_axis * length, RGB_GREEN);
    line(position, position + z_axis * length, RGB_BLUE);
}
```

**Integration Point:**
- Character rendering: `debug_draw::axes(character.transform, 1.0f);`
- Camera target: `debug_draw::axes(camera.look_at_transform, 0.5f);`

**Validation:**
- [ ] Axes are visible in world
- [ ] X (red) points right when character faces forward
- [ ] Y (green) points up
- [ ] Z (blue) points forward
- [ ] Axes rotate with character
- [ ] Axes remain perpendicular (visual check)
- [ ] All assertions pass

**Visual Test:**
1. Stand still → Axes align with world
2. Rotate character → Axes rotate, stay perpendicular
3. Move → Axes stay attached to character

**Estimated Lines:** ~30 lines (function + assertions)
**Time:** 1 hour (including integration)
**Risk:** Low (simple rendering, validates coordinate system)

---

### Stage 4: Core Primitive - Velocity Arrow

**Principle:** Pure Functions Over Accumulated State
**Purpose:** Visualize motion vector - validates physics

**Files to Modify:**
- `src/rendering/debug_draw.h` - Add velocity_arrow() + arrow()
- `src/rendering/debug_draw.cpp` - Implement both

**Implementation:**
```cpp
void velocity_arrow(vec3 position, vec3 velocity, float scale) {
    FL_PRECONDITION(scale > 0.0f, "scale must be positive");
    FL_ASSERT_FINITE(position, "velocity_arrow position");
    FL_ASSERT_FINITE(velocity, "velocity vector");

    float speed = glm::length(velocity);

    // Don't render negligible velocities
    const float min_speed = 0.01f;
    if (speed < min_speed) return;

    vec3 direction = velocity / speed;
    float length = speed * scale;

    FL_POSTCONDITION(
        glm::epsilonEqual(glm::length(direction), 1.0f, 0.001f),
        "direction must be normalized"
    );

    arrow(position, direction, length, RGB_GREEN);
}

void arrow(vec3 start, vec3 direction, float length, vec3 color) {
    FL_PRECONDITION(length >= 0.0f, "arrow length must be non-negative");
    FL_ASSERT_NORMALIZED(direction, "arrow direction");
    FL_ASSERT_FINITE(start, "arrow start");

    vec3 end = start + direction * length;

    // Draw shaft
    line(start, end, color);

    // Draw simple arrowhead (two lines forming V)
    vec3 perp1 = get_perpendicular(direction);
    vec3 perp2 = cross(direction, perp1);
    float head_size = std::min(length * 0.2f, 0.1f);

    vec3 head_base = end - direction * head_size;
    line(end, head_base + perp1 * head_size * 0.5f, color);
    line(end, head_base - perp1 * head_size * 0.5f, color);

    FL_POSTCONDITION(
        glm::epsilonEqual(glm::distance(start, end), length, 0.001f),
        "arrow length must match requested length"
    );
}
```

**Integration Point:**
- Character rendering: `debug_draw::velocity_arrow(character.position, character.velocity, 0.5f);`

**Validation:**
- [ ] No arrow when standing still
- [ ] Arrow appears when moving
- [ ] Arrow points in movement direction
- [ ] Arrow length increases with speed
- [ ] Arrow color is green
- [ ] Arrowhead is visible
- [ ] All assertions pass

**Visual Test:**
1. Stand still → No arrow
2. Walk forward → Green arrow points forward
3. Run faster → Arrow gets longer
4. Change direction → Arrow rotates immediately
5. Jump → Arrow points up then down

**Estimated Lines:** ~60 lines (two functions + helpers + assertions)
**Time:** 2 hours (arrowhead geometry requires care)
**Risk:** Low (simple vector math)

---

### Stage 5: Core Primitive - Sphere Wireframe

**Principle:** Mathematical Truth
**Purpose:** Visualize collision boundary - validates physics

**Files to Modify:**
- `src/rendering/debug_draw.h` - Add sphere_wireframe()
- `src/rendering/debug_draw.cpp` - Implement sphere_wireframe()

**Implementation:**
```cpp
void sphere_wireframe(vec3 center, float radius, vec3 color) {
    FL_PRECONDITION(radius > 0.0f, "sphere radius must be positive");
    FL_ASSERT_FINITE(center, "sphere center");

    const int segments = 16;
    const float angle_step = 2.0f * M_PI / segments;

    // Draw 3 circles (XY, XZ, YZ planes)
    // XY circle (around Z axis)
    for (int i = 0; i < segments; i++) {
        float angle1 = i * angle_step;
        float angle2 = (i + 1) * angle_step;
        vec3 p1 = center + vec3(cos(angle1), sin(angle1), 0) * radius;
        vec3 p2 = center + vec3(cos(angle2), sin(angle2), 0) * radius;
        line(p1, p2, color);
    }

    // XZ circle (around Y axis)
    for (int i = 0; i < segments; i++) {
        float angle1 = i * angle_step;
        float angle2 = (i + 1) * angle_step;
        vec3 p1 = center + vec3(cos(angle1), 0, sin(angle1)) * radius;
        vec3 p2 = center + vec3(cos(angle2), 0, sin(angle2)) * radius;
        line(p1, p2, color);
    }

    // YZ circle (around X axis)
    for (int i = 0; i < segments; i++) {
        float angle1 = i * angle_step;
        float angle2 = (i + 1) * angle_step;
        vec3 p1 = center + vec3(0, cos(angle1), sin(angle1)) * radius;
        vec3 p2 = center + vec3(0, cos(angle2), sin(angle2)) * radius;
        line(p1, p2, color);
    }
}
```

**Integration Point:**
- Character rendering: `debug_draw::sphere_wireframe(character.position, character.collision_radius, RGB_MAGENTA);`

**Validation:**
- [ ] Sphere is visible
- [ ] Sphere radius matches collision sphere
- [ ] Sphere center matches character position
- [ ] Sphere touches ground when grounded
- [ ] Sphere doesn't interpenetrate visible geometry
- [ ] All assertions pass

**Visual Test:**
1. Stand on ground → Sphere touches floor
2. Jump → Sphere lifts off ground
3. Move → Sphere follows character
4. Collide with wall → Sphere shows boundary

**Estimated Lines:** ~50 lines (three circles + assertions)
**Time:** 1.5 hours (circle geometry)
**Risk:** Low (standard wireframe sphere)

---

### Stage 6: Toggle Control & Integration

**Principle:** Do No Harm to Gameplay
**Purpose:** Enable/disable without side effects

**Files to Modify:**
- `src/rendering/debug_draw.h` - Add control functions
- `src/rendering/debug_draw.cpp` - Implement toggle state
- `src/input/input.h` - Add F3 key check
- `src/app/runtime.cpp` - Wire up toggle

**Implementation:**
```cpp
// debug_draw.cpp
namespace debug_draw {
    static bool s_enabled = true; // Start enabled in debug builds

    void set_enabled(bool enabled) {
        s_enabled = enabled;
    }

    bool is_enabled() {
        return s_enabled;
    }

    // All draw functions check:
    void axes(const mat4& transform, float length) {
        if (!s_enabled) return;
        // ... rest of function
    }
}

// runtime.cpp
void Runtime::handle_input() {
    if (input::key_pressed(SAPP_KEYCODE_F3)) {
        bool enabled = debug_draw::is_enabled();
        debug_draw::set_enabled(!enabled);
    }
}
```

**Integration Points:**
- Main loop: Check F3 key
- All debug draw calls: Early return if disabled

**Validation:**
- [ ] F3 toggles visualization on/off
- [ ] No visual artifacts when toggling
- [ ] No performance impact when disabled
- [ ] No gameplay changes when toggling
- [ ] State persists across frames

**Visual Test:**
1. Press F3 → Visualization disappears
2. Press F3 again → Visualization reappears
3. Play with viz off → Gameplay identical
4. Toggle during motion → No glitches

**Estimated Lines:** ~30 lines (state + toggle logic)
**Time:** 1 hour
**Risk:** Low (simple boolean state)

---

### Stage 7: Startup Validation

**Principle:** Validate Before Integration
**Purpose:** Verify mathematical assumptions at startup

**Files to Modify:**
- `src/rendering/debug_draw.cpp` - Add validation::run_startup_checks()
- `src/app/runtime.cpp` - Call validation on startup

**Implementation:**
```cpp
namespace debug_draw::validation {

void run_startup_checks() {
#if FL_DEBUG_VALIDATION
    // Verify GLM coordinate system
    vec3 x = {1, 0, 0};
    vec3 y = {0, 1, 0};
    vec3 z = {0, 0, 1};
    verify_coordinate_frame(x, y, z, "GLM basis");

    // Verify rotation preserves orthonormality
    mat4 rot = glm::rotate(mat4(1.0f), glm::radians(45.0f), vec3(0, 1, 0));
    vec3 rx = vec3(rot[0]);
    vec3 ry = vec3(rot[1]);
    vec3 rz = vec3(rot[2]);
    verify_coordinate_frame(rx, ry, rz, "Rotated frame");

    // Verify cross product
    vec3 cross = glm::cross(x, y);
    FL_ASSERT(glm::all(glm::epsilonEqual(cross, z, 0.0001f)),
              "X × Y must equal Z");

    printf("[DEBUG_DRAW] Startup validation passed\n");
#endif
}

} // namespace validation
```

**Integration Point:**
```cpp
// runtime.cpp
void Runtime::init() {
    // ... existing init
    debug_draw::validation::run_startup_checks();
}
```

**Validation:**
- [ ] Checks run at startup in debug builds
- [ ] Success message printed
- [ ] Asserts trigger if GLM is misconfigured
- [ ] No checks in release builds

**Estimated Lines:** ~40 lines (validation functions)
**Time:** 1 hour
**Risk:** Low (pure validation)

---

### Stage 8: Documentation & Visual Checklist

**Principle:** Principled Development
**Purpose:** Document usage and expected behavior

**Files to Create/Update:**
- `PLANS/DEBUG_VISUALIZATION_FEATURE.md` - Update with implementation notes
- `src/rendering/debug_draw.h` - Add usage documentation

**Documentation:**
```cpp
// debug_draw.h header comments:
/**
 * Debug Visualization System
 *
 * Provides immediate-mode debug rendering primitives for visualizing
 * game state. All functions are no-ops when disabled or in release builds.
 *
 * Usage:
 *   debug_draw::axes(transform, 1.0f);           // RGB axes
 *   debug_draw::velocity_arrow(pos, vel, 0.5f);  // Green arrow
 *   debug_draw::sphere_wireframe(pos, radius);   // Magenta sphere
 *
 * Toggle: Press F3 to enable/disable
 *
 * Coordinate System:
 *   X = Red (right)
 *   Y = Green (up)
 *   Z = Blue (forward)
 */
```

**Visual Checklist:**
See "Visual Validation Checklist" section below.

**Estimated Lines:** ~100 lines (documentation)
**Time:** 1 hour
**Risk:** None (documentation only)

---

## The Risks

### Mathematical Risks

**Risk:** GLM coordinate system assumptions wrong
- **Likelihood:** Very Low (GLM is standard, proven)
- **Mitigation:** Startup validation checks verify assumptions immediately

**Risk:** Floating point precision issues with normalization
- **Likelihood:** Low
- **Mitigation:** Epsilon comparisons in assertions (0.001f tolerance)

**Risk:** Transform extraction doesn't preserve orthonormality
- **Likelihood:** Low (GLM handles this)
- **Mitigation:** verify_coordinate_frame() catches violations

### Integration Risks

**Risk:** Existing debug_draw has conflicting API
- **Likelihood:** Medium (file exists, unknown contents)
- **Mitigation:** Stage 0 reconnaissance before planning modifications

**Risk:** Rendering system doesn't support line drawing
- **Likelihood:** Low (debug_draw.cpp exists, likely has lines)
- **Mitigation:** Inspect existing implementation first

**Risk:** Performance impact from debug rendering
- **Likelihood:** Low (~100 lines per frame is negligible)
- **Mitigation:** Toggle off for performance testing, compile out in release

### Visual Risks

**Risk:** Visualization obscures gameplay
- **Likelihood:** Low (wireframes and lines are semi-transparent typically)
- **Mitigation:** Toggle with F3, use non-intrusive colors

**Risk:** Coordinate system doesn't match intuition
- **Likelihood:** Medium (this is what we're validating!)
- **Mitigation:** This is THE POINT - visualization will reveal truth

---

## The Emergence

### Intended Behaviors
- See entity orientation instantly (RGB axes)
- See motion vectors clearly (velocity arrows)
- See collision boundaries precisely (wireframes)
- Validate coordinate system assumptions (no more 3-day crises)
- Toggle visualization on/off as needed

### Possible Emergent Behaviors
- Discover subtle physics bugs (velocity discontinuities)
- Notice animation/physics mismatches (visual != physics)
- Identify frame-rate dependencies (jitter in arrows)
- Reveal accumulated drift (axes not orthogonal over time)
- Spot unexpected force interactions (arrow directions)

### Surprising Emergent Behaviors
- May reveal bugs in systems we thought were correct
- Could show player control is less responsive than believed
- Might expose precision issues in transforms
- Will likely prevent multiple future debugging crises
- Could inspire new gameplay mechanics (seeing forces/motion)

---

## Visual Validation Checklist

After implementation, verify all behaviors:

### RGB Axes
- [ ] X (red) points right relative to entity
- [ ] Y (green) points up
- [ ] Z (blue) points forward
- [ ] Axes rotate with entity
- [ ] Axes remain perpendicular during rotation
- [ ] Axes length is 1.0 world unit

### Velocity Arrow
- [ ] No arrow when standing still (speed < 0.01)
- [ ] Green arrow appears when moving
- [ ] Arrow points in movement direction
- [ ] Arrow length scales with speed
- [ ] Arrow updates immediately on direction change
- [ ] Arrow points up during jump ascent
- [ ] Arrow points down during jump descent

### Sphere Wireframe
- [ ] Magenta wireframe visible
- [ ] Radius matches collision sphere
- [ ] Center matches entity position
- [ ] Sphere touches ground when grounded
- [ ] Sphere lifts during jump
- [ ] No interpenetration with visible geometry

### System Integration
- [ ] F3 toggles visualization on/off
- [ ] No visual artifacts during toggle
- [ ] No gameplay impact when enabled
- [ ] No performance impact when disabled
- [ ] Startup validation prints success message
- [ ] All assertions pass during normal gameplay

### Edge Cases
- [ ] Zero velocity handled correctly
- [ ] Rapid rotation doesn't break axes
- [ ] NaN/Inf caught by assertions (test by injecting)
- [ ] Large velocities don't cause visual glitches

---

## Complexity & Time Breakdown

**Total Complexity:** 3 points

| Stage | Points | Lines | Time |
|-------|--------|-------|------|
| 0: Reconnaissance | 0 | 0 | 2h |
| 1: Assertion Framework | 0.5 | ~100 | 1h |
| 2: API Design | 0 | 0 | 0.5h |
| 3: RGB Axes | 0.5 | ~30 | 1h |
| 4: Velocity Arrow | 1.0 | ~60 | 2h |
| 5: Sphere Wireframe | 0.5 | ~50 | 1.5h |
| 6: Toggle Control | 0.25 | ~30 | 1h |
| 7: Startup Validation | 0.25 | ~40 | 1h |
| 8: Documentation | 0 | ~30 | 1h |
| **TOTAL** | **3** | **~340** | **11h** |

---

## Dependencies & Prerequisites

### Must Exist (Verified)
✓ Rendering system (sokol-gfx)
✓ Scene graph
✓ GLM math library
✓ Input system
✓ Debug builds configuration (_DEBUG flag)

### Likely Exists (To Verify in Stage 0)
? `debug_draw.cpp` implementation
? Line rendering capability
? Color constants or utilities
? Frame lifecycle hooks

### Will Create
- `src/foundation/debug_assert.h` (Stage 1)
- Debug draw functions (Stages 3-5)
- Toggle control (Stage 6)
- Startup validation (Stage 7)

---

## Success Criteria

### We know it's complete when:
1. All assertions pass during normal gameplay
2. Startup validation prints success message
3. RGB axes match camera controls (W = forward = +Z blue)
4. Velocity arrows scale with speed and point in movement direction
5. Wireframes touch ground when grounded
6. F3 toggles visualization without gameplay impact
7. All items on visual checklist pass
8. Can identify coordinate system bugs in 30 seconds vs 3 days

### We know it's correct when:
1. Mathematical properties verified by assertions
2. Visual behavior matches expected behavior
3. No NaN/Inf propagation detected
4. Zero gameplay impact when enabled/disabled
5. Zero performance impact when disabled

---

## The Commitment

This plan will:
- ✓ Build in validated stages (8 reversible steps)
- ✓ Validate math at every level (assertions throughout)
- ✓ Maintain principles (read-only, composable, simple)
- ✓ Enable emergence (reveals unexpected behaviors)
- ✓ Provide visibility (the system IS visibility)

This plan will never:
- ✗ Affect gameplay (read-only, toggleable)
- ✗ Create coupling (orthogonal to all systems)
- ✗ Hide complexity (assertions document contracts)
- ✗ Prescribe outcomes (visualizes, doesn't control)
- ✗ Accumulate state (pure functions, immediate mode)

---

## Plan Summary

```
Plan Created: Debug Visualization System
Stages: 8 (0=reconnaissance, 1-7=implementation, 8=documentation)
Mathematical Validation: Assertion-based preconditions/postconditions/invariants
Debug Visualization: System validates itself (visual correctness = mathematical correctness)
Risk Level: Low
Certainty: 95% (pending Stage 0 reconnaissance)
Estimated Time: 11 hours
Estimated Lines: ~340 lines
```

---

## Next Step

**Begin Stage 0: Reconnaissance**

Investigate existing `src/rendering/debug_draw.cpp` to understand:
1. What API already exists
2. What capabilities are present
3. What needs to be added vs modified
4. Whether plan needs adjustment

After reconnaissance, either:
- Proceed to Stage 1 (if plan is valid)
- Revise plan (if discoveries require changes)

---

**This is the way.**
