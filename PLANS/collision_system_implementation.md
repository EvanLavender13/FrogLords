# Collision System Implementation Plan

**Date:** October 5, 2025  
**Purpose:** Implement sphere-vs-AABB collision detection for terrain/platform handling  
**Sources:** GDC 2013 (David Rosen), AnimationDoc1.md, Elegance.md, lifter_bumper_system_analysis.md

---

## Design Philosophy

### The Overgrowth Approach

From David Rosen's GDC 2013 presentation:

> "I had this one green sphere as a bumper sphere it lets him fly it off objects that he runs into [and] a weightlifter's sphere which moves up over small obstacles."

**Core Principle:** Two spheres + simple environment geometry = emergent locomotion behaviors.

**No complex collision shapes mentioned:**
- No triangle meshes
- No heightmaps
- No capsules, cylinders, or convex hulls
- Just spheres against simple geometric primitives

### Elegance Requirements (from Elegance.md)

✅ **Simple mechanics** - "stupidly simple cores, emergent complexity"  
✅ **High reuse frequency** - "million-repetition thinking"  
✅ **Multi-use tools** - Lifter does grounding AND step-up  
✅ **No content restrictions** - Box-based levels can be freely authored  
✅ **Matched scales** - 0.5m spheres vs 0.5-2.0m boxes = natural interactions

**Decision:** World geometry = **Axis-Aligned Bounding Boxes (AABBs)** only.

---

## Current State

### What We Have

**Collision Primitives (foundation/collision_primitives.h):**
```cpp
struct sphere {
    glm::vec3 center;
    float radius;
};

struct aabb {
    glm::vec3 center;
    glm::vec3 half_extents;
};
```

**Collision Resolution (foundation/collision.{h,cpp}):**
```cpp
struct sphere_collision {
    bool hit;
    glm::vec3 normal;
    float penetration;
};

sphere_collision resolve_sphere_aabb(sphere& s, const aabb& box);
```
✅ **Already implemented and working**

**Character Controller:**
- Two spheres: `bumper` (0.50m), `weightlifter` (0.45m)
- Ground plane collision (infinite plane at y=0)
- Physics integration (position, velocity, acceleration)

**Scene System:**
- `scene::add_collision_box(aabb)` - adds boxes to world
- `scene::collision_boxes()` - returns all collision boxes
- Currently ~10 test platforms in `runtime.cpp`

### What's Missing

❌ Character controller doesn't test against `scene::collision_boxes()`  
❌ Only ground plane collision is resolved  
❌ Platforms/obstacles in scene are currently decoration only

---

## Implementation Phases

---

## Phase A: Basic Box Collision (Minimum Viable)

**Goal:** Character collides with boxes. Can stand on platforms, bump into walls.

**Scope:** Lifter grounding + bumper sliding. No step-up yet.

### A.1: Add Box Collision Loop to Character Controller

**File:** `src/character/controller.cpp`

**Location:** In `controller::update()`, after physics integration, before ground plane collision.

**Changes:**
1. Accept `scene` parameter (change signature from `(const scene*, float)` to actually use scene)
2. Add collision testing loop against all boxes
3. Test **weightlifter first** (grounding), then **bumper** (sliding)

**Implementation:**
```cpp
void controller::update(const scene* scn, float dt) {
    // [Existing physics integration code...]
    position += velocity * dt;
    
    // Reset grounded state
    is_grounded = false;
    
    // Update collision sphere positions
    bumper.center = position;
    weightlifter.center = position + WEIGHTLIFTER_OFFSET;
    
    // ===== NEW: Box collision resolution =====
    if (scn) {
        resolve_box_collisions(*scn);
    }
    
    // Ground plane (fallback if no box collision)
    resolve_ground_collision();
    
    // [Existing cleanup code...]
}
```

### A.2: Implement `resolve_box_collisions()` Method

**File:** `src/character/controller.cpp` (implementation)  
**File:** `src/character/controller.h` (declaration)

**Add to header:**
```cpp
private:
    void resolve_ground_collision();
    void resolve_box_collisions(const scene& scn);  // NEW
```

**Implementation (basic version - no step-up):**
```cpp
void controller::resolve_box_collisions(const scene& scn) {
    // Phase 1: Weightlifter collision (grounding detection)
    for (const auto& box : scn.collision_boxes()) {
        sphere lifter_copy = weightlifter;
        sphere_collision col = resolve_sphere_aabb(lifter_copy, box);
        
        if (col.hit) {
            // Check if this is a ground surface (normal points mostly upward)
            if (col.normal.y > 0.5f) {
                // Apply intentional burial logic (same as ground plane)
                constexpr float INTENDED_BURIAL_DEPTH = 0.10f;
                float excess = col.penetration - INTENDED_BURIAL_DEPTH;
                
                if (excess > 0.0f) {
                    // Push up by excess only
                    position += col.normal * excess;
                    weightlifter.center = position + WEIGHTLIFTER_OFFSET;
                }
                
                // Set grounded state
                is_grounded = true;
                ground_normal = col.normal;
                ground_height = box.center.y + box.half_extents.y;
                
                // Kill downward velocity
                if (glm::dot(velocity, col.normal) < 0.0f) {
                    velocity -= col.normal * glm::dot(velocity, col.normal);
                }
            }
        }
    }
    
    // Phase 2: Bumper collision (sliding/bouncing)
    for (const auto& box : scn.collision_boxes()) {
        sphere bumper_copy = bumper;
        sphere_collision col = resolve_sphere_aabb(bumper_copy, box);
        
        if (col.hit) {
            // Push out of collision
            position += col.normal * col.penetration;
            
            // Update sphere positions
            bumper.center = position;
            weightlifter.center = position + WEIGHTLIFTER_OFFSET;
            
            // Remove velocity into surface
            float vel_into_surface = glm::dot(velocity, col.normal);
            if (vel_into_surface < 0.0f) {
                velocity -= col.normal * vel_into_surface;
            }
        }
    }
}
```

### A.3: Include Required Headers

**File:** `src/character/controller.cpp`

Add:
```cpp
#include "foundation/collision.h"  // For resolve_sphere_aabb
```

### A.4: Update Call Site

**File:** `src/app/runtime.cpp`

No changes needed - already passes `&scn` to `character.update()`.

### Testing Phase A

**Expected Behavior:**
- ✅ Character stands on platform boxes
- ✅ Character slides along walls
- ✅ Character cannot pass through boxes
- ✅ Ground plane still works as fallback
- ⚠️ Character **cannot** step up onto boxes yet (handled in Phase B)

**Test Cases:**
1. Walk onto existing platforms - should stand on top
2. Jump and land on platforms - should stop on surface
3. Run into walls - should slide along them
4. Walk off platform edge - should fall to ground plane

---

## Phase B: Step-Up Behavior

**Goal:** Character automatically steps up onto obstacles within lifter radius height.

**Rationale:** From GDC talk - "weightlifter's sphere which moves up over small obstacles"

### B.1: Enhance Lifter Collision with Step-Up Logic

**File:** `src/character/controller.cpp`

**Modify `resolve_box_collisions()` weightlifter phase:**

```cpp
// Phase 1: Weightlifter collision (grounding + step-up)
for (const auto& box : scn.collision_boxes()) {
    sphere lifter_copy = weightlifter;
    sphere_collision col = resolve_sphere_aabb(lifter_copy, box);
    
    if (col.hit) {
        // Check if this is a ground surface (normal points mostly upward)
        if (col.normal.y > 0.5f) {
            // [Existing grounding logic - unchanged]
        }
        // NEW: Check if this is a step-up situation (hitting from side)
        else if (col.normal.y < 0.3f && col.normal.y > -0.3f) {
            // Horizontal collision - potential step-up
            
            // Calculate step height: how much vertical lift needed to clear obstacle
            float step_height = col.penetration;
            
            // Only step up if:
            // 1. Step is within lifter's capability (< lifter radius ~0.45m)
            // 2. Character is moving horizontally toward the obstacle
            float horizontal_speed = glm::length(glm::vec3(velocity.x, 0, velocity.z));
            bool moving_into_obstacle = glm::dot(velocity, col.normal) < -0.1f;
            
            if (step_height < weightlifter.radius && 
                moving_into_obstacle && 
                horizontal_speed > 0.5f) {
                
                // Apply step-up: lift character vertically
                position.y += step_height + 0.01f; // +epsilon for clearance
                
                // Update sphere positions
                bumper.center = position;
                weightlifter.center = position + WEIGHTLIFTER_OFFSET;
                
                // Mark as grounded (stepping onto surface)
                is_grounded = true;
                ground_normal = glm::vec3(0, 1, 0);
            }
            else {
                // Step too high - treat as wall (bumper will handle)
            }
        }
    }
}
```

### B.2: Tuning Parameters

Add constants to controller.cpp:
```cpp
namespace {
// Step-up behavior
constexpr float MAX_STEP_HEIGHT = 0.40f;  // Slightly less than lifter radius
constexpr float MIN_STEP_SPEED = 0.5f;    // Must be moving to step up
}
```

### Testing Phase B

**Expected Behavior:**
- ✅ Walking into 0.15m box → steps up smoothly
- ✅ Walking into 0.30m box → steps up smoothly
- ✅ Walking into 0.45m box → steps up (at limit)
- ✅ Walking into 0.60m box → blocked (too high)
- ✅ Standing still against box → no step-up (not moving)

**Test Geometry (add to runtime.cpp scene setup):**
```cpp
// Step-up test: Graduated steps (0.15m, 0.30m, 0.45m, 0.60m)
for (int i = 0; i < 4; ++i) {
    float height = 0.15f * (i + 1);
    aabb step;
    step.center = glm::vec3(-5.0f + i * 2.0f, height * 0.5f, -8.0f);
    step.half_extents = glm::vec3(0.8f, height * 0.5f, 0.8f);
    scn.add_collision_box(step);
}
```

---

## Phase C: Collision Response Tuning

**Goal:** Refine collision behavior for feel and stability.

### C.1: Multi-Iteration Collision Resolution

**Problem:** Single-pass collision can leave character embedded if pushed into another box.

**Solution:** Iterate collision resolution 2-3 times per frame.

```cpp
void controller::resolve_box_collisions(const scene& scn) {
    constexpr int MAX_ITERATIONS = 3;
    
    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        bool any_collision = false;
        
        // [Existing lifter + bumper collision loops]
        // Set any_collision = true if col.hit
        
        if (!any_collision) {
            break; // Converged - no more collisions
        }
    }
}
```

### C.2: Velocity Damping on Collision

**Problem:** Character can gain energy from repeated collisions.

**Solution:** Slightly damp horizontal velocity on collision.

```cpp
// After resolving bumper collision:
if (col.hit) {
    // [Existing position correction]
    
    // Damp horizontal velocity on impact
    glm::vec3 horizontal_vel(velocity.x, 0, velocity.z);
    float impact_strength = glm::dot(horizontal_vel, col.normal);
    if (impact_strength < -1.0f) { // Significant impact
        velocity *= 0.95f; // 5% energy loss
    }
}
```

### C.3: Collision Priority (Lifter Always First)

**Rationale:** Grounding detection must happen before bumper sliding to prevent "jumping" on platforms.

Already implemented in Phase A (lifter loop before bumper loop).

### Testing Phase C

**Test for stability:**
- Run into corner between two walls - should not jitter
- Jump onto platform edge - should settle smoothly
- Run along wall - should slide without bouncing

---

## Phase D: Debug Visualization

**Goal:** Visualize collision state for debugging.

### D.1: Add Collision Visualization

**File:** `src/rendering/debug_draw.cpp`

Add function:
```cpp
void draw_collision_state(draw_context& ctx, const controller& character,
                          const scene& scn) {
    // Draw all collision boxes
    for (const auto& box : scn.collision_boxes()) {
        box_dimensions dims{
            box.half_extents.x * 2.0f,
            box.half_extents.y * 2.0f,
            box.half_extents.z * 2.0f
        };
        wireframe_mesh box_mesh = generate_box(dims);
        box_mesh.position = box.center;
        
        // Color based on collision state
        glm::vec4 color = box.center.y < 0.5f ? 
            glm::vec4(0.5f, 0.5f, 0.5f, 0.3f) :  // Ground-level: gray
            glm::vec4(0.7f, 0.3f, 0.3f, 0.5f);   // Platforms: red
        
        ctx.renderer.draw(box_mesh, ctx.cam, ctx.aspect, color);
    }
    
    // Draw ground contact point (if grounded)
    if (character.is_grounded) {
        wireframe_mesh contact = ctx.unit_sphere_4;
        contact.position = character.weightlifter.center - 
            glm::vec3(0, character.weightlifter.radius, 0);
        contact.scale = glm::vec3(0.05f);
        ctx.renderer.draw(contact, ctx.cam, ctx.aspect, glm::vec4(0, 1, 0, 1));
    }
}
```

**File:** `src/rendering/debug_draw.h`

Add declaration:
```cpp
void draw_collision_state(draw_context& ctx, const controller& character,
                          const scene& scn);
```

### D.2: Call Visualization in Runtime

**File:** `src/app/runtime.cpp`

In `render_world()`:
```cpp
// After existing debug draws:
debug::draw_collision_state(debug_ctx, character, scn);
```

---

## Phase E: Optimization (Future)

**Note:** Only implement if profiling shows collision is a bottleneck (unlikely with ~10-100 boxes).

### Potential Optimizations

1. **Spatial Partitioning:**
   - Broad phase: Only test boxes near character (within 5m radius)
   - Simple grid or octree

2. **Early-Out Tests:**
   - Skip boxes far above/below character
   - Skip boxes character is moving away from

3. **Collision Caching:**
   - Remember last colliding box
   - Test it first next frame

**Defer until box count > 100.**

---

## Implementation Order Summary

### Minimum Viable (Phase A)
1. ✅ Add `resolve_box_collisions()` method
2. ✅ Implement weightlifter grounding detection
3. ✅ Implement bumper sliding
4. ✅ Call from `update()` loop

**Outcome:** Character collides with world geometry.

### Essential Feature (Phase B)
1. ✅ Add step-up logic to weightlifter collision
2. ✅ Tune step height limits
3. ✅ Test with graduated step geometry

**Outcome:** Character walks up stairs/obstacles naturally.

### Polish (Phase C)
1. ✅ Multi-iteration resolution
2. ✅ Velocity damping
3. ✅ Stability tuning

**Outcome:** Smooth, stable collision response.

### Debugging (Phase D)
1. ✅ Collision box visualization
2. ✅ Ground contact indicators

**Outcome:** Easy to diagnose collision issues.

### Future (Phase E)
- Spatial optimization (if needed)

---

## Testing Strategy

### Test Suite from `lifter_collision_test_suite.md`

Once implemented, validate against:

1. **Step-Up Tests** - boxes at 0.15m, 0.30m, 0.45m, 0.60m heights
2. **Bumper/Lifter Separation** - verify lifter steps up, bumper slides on same obstacles
3. **Uneven Terrain** - staggered platforms, slopes (via multiple boxes)
4. **Edge Cases** - corners, narrow gaps, ceiling collision

### Integration with Existing Systems

**Locomotion:** Surveyor wheel should handle varying ground height naturally  
**Orientation:** Yaw rotation unchanged (still velocity-driven)  
**Animation:** Pose offset already removed from collision spheres (previous work)

---

## References

- **GDC 2013 Talk** (Notes/GDC.md) - Source material for two-sphere design
- **AnimationDoc1.md** - Physics-first animation philosophy
- **Elegance.md** - Simple mechanics, emergent complexity
- **lifter_bumper_system_analysis.md** - Current sphere configuration
- **lifter_collision_test_suite.md** - Test geometry designs

---

## Success Criteria

✅ **Phase A Complete When:**
- Character stands on all platform boxes in scene
- Character slides along walls without passing through
- Ground plane still works as fallback

✅ **Phase B Complete When:**
- Character walks up 0.15m steps without jumping
- Character walks up 0.30m steps without jumping
- Character is blocked by 0.60m obstacles
- No manual jumping required to navigate terrain

✅ **All Phases Complete When:**
- Character navigates entire test level naturally
- Collision feels smooth and predictable
- "Intuitive and responsive" (GDC design goal achieved)

---

**Status:** Phase A.1 complete. Ready for testing.  
**Next Action:** Implement Phase A.2 - Test basic box collision.
