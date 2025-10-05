# Collision System Implementation Plan

**Date:** October 5, 2025  
**Purpose:** Implement sphere-vs-AABB collision detection for terrain/platform handling  
**Sources:** GDC 2013 (David Rosen), AnimationDoc1.md, Elegance.md, lifter_bumper_system_analysis.md

**Status:** Phase A complete, Phase B ready to begin

---

## Revision History

**October 5, 2025 - Phase A Completion Update**
- Documented actual Phase A implementation (surface-classified approach)
- Added playtest justifications for deviation from original "lifter-first" plan
- Removed code examples from Phase A (now implemented in codebase)
- Added validation results and bug resolution notes
- Updated success criteria to reflect achieved state
- Cross-referenced supporting documents (reviews, bug reports, action summaries)

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

**Status:** ✅ **COMPLETE** (with playtest revisions)

**Goal:** Character collides with boxes. Can stand on platforms, bump into walls.

**Scope:** Surface-classified collision response. No step-up yet.

### Implementation Summary

Phase A implementation diverged from the original "lifter-first" plan based on playtesting feedback. The final implementation uses a three-phase approach that emerged from empirical testing:

1. **Phase 1a:** Bumper handles flat ground (`normal.y > 0.9f`)
2. **Phase 1b:** Weightlifter handles slopes (`0.5f < normal.y ≤ 0.9f`)
3. **Phase 2:** Bumper handles walls (`normal.y ≤ 0.5f`)

### Design Decisions

**Original Plan:** Weightlifter handles all grounding, bumper handles all walls.

**Playtest Issue:** Character exhibited visible "settling" behavior on flat platforms. The weightlifter's intentional 0.10m burial created visual penetration artifacts and unstable ground contact on horizontal surfaces.

**Revised Approach:** Split ground authority by surface angle:
- **Bumper on flat surfaces** → Clean, immediate contact with zero penetration
- **Lifter on slopes/edges** → Smooth transitions with stability burial logic
- **Bumper on walls** → Sliding and collision response

**Rationale:** This division leverages each sphere's geometric position naturally. The bumper (outer sphere, primary contact volume) provides responsive solid feel on clean surfaces. The weightlifter (recessed sphere) handles transitional terrain where grip and stability matter.

**Principle Alignment:**
- ✅ "Iteration over planning" - Adapted based on feel testing
- ✅ "Graybox until mechanics proven" - Empirical validation drove design
- ✅ "Honor serendipity" - Discovered behavior over pre-planned spec

### Key Implementation Details

**Surface Classification Constants:**
```cpp
FLAT_GROUND_NORMAL_THRESHOLD = 0.9f;  // cos(~25°) - bumper authority
SLOPE_NORMAL_THRESHOLD = 0.5f;        // cos(60°) - lifter/wall boundary
INTENDED_BURIAL_DEPTH = 0.10f;        // Lifter penetration target on slopes
```

**Collision Resolution Method:**
- `controller::resolve_box_collisions(const scene&)` - Three-phase collision loop
- Integrated into `controller::update()` after physics integration
- Ground plane remains as fallback for areas without collision boxes

**Files Modified:**
- `src/character/controller.cpp` - Added `resolve_box_collisions()`, updated constants
- `src/character/controller.h` - Added method declaration
- `src/app/runtime.cpp` - Enhanced test environment with wall geometry

### Validation Results

**Functional Testing:**
- ✅ Character stands on flat platforms with instant, solid contact
- ✅ Character handles sloped surfaces (0.5-0.9 normal.y) smoothly
- ✅ Character slides along walls without passing through
- ✅ Ground plane fallback works correctly
- ✅ No settling/sinking behavior on landing
- ✅ Spawn pose stable (bumper at 0.50m, lifter at 0.35m)

**Test Environment:**
- 5 platform boxes at varying heights (1.0m to 8.5m)
- 5 wall boxes for collision validation:
  - Long wall (8m) for sustained sliding tests
  - Corner walls (L-shape) for multi-surface collision
  - Gap walls (1.8m apart) for tight space navigation

**Bug Resolution:**
Fixed "character slow sink" issue documented in `ISSUES/character_slow_sink.md`. Root cause was lifter claiming authority over flat ground, causing gradual settling toward 0.10m burial target. Solution: bumper handles flat surfaces with zero-penetration contact.

### Known Limitations

- **No step-up capability** - Character cannot walk up obstacles (Phase B feature)
- **Single-pass resolution** - Complex multi-box collisions may show minor penetration (Phase C feature)
- **No velocity damping** - Repeated collisions don't dissipate energy (Phase C feature)
- **Three-loop structure** - Iterates collision boxes 3x per frame (acceptable for <50 boxes, optimization deferred to Phase E)

### References

- **Bug Report:** `ISSUES/character_slow_sink.md` - Documents settling behavior and fix
- **Review:** `REVIEWS/collision_system_phase_a_review.md` - Analysis of implementation vs. plan
- **Actions:** `REVIEWS/collision_phase_a_implementation_actions.md` - Code quality improvements applied

---

## Phase B: Step-Up Behavior

**Status:** 🔲 **PLANNED** (not yet implemented)

**Goal:** Character automatically steps up onto obstacles within lifter radius height.

**Rationale:** From GDC talk - "weightlifter's sphere which moves up over small obstacles"

**Prerequisites:** Phase A complete ✅

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

**Status:** 🔲 **PLANNED** (not yet implemented)

**Goal:** Refine collision behavior for feel and stability.

**Prerequisites:** Phase B complete

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

**Status:** 🔲 **PLANNED** (not yet implemented)

**Goal:** Visualize collision state for debugging.

**Prerequisites:** Phase C complete

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

**Status:** 🔲 **DEFERRED**

**Note:** Only implement if profiling shows collision is a bottleneck (unlikely with ~10-100 boxes).

**Prerequisites:** Phases A-D complete, performance profiling conducted

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

### Minimum Viable (Phase A) ✅ COMPLETE
1. ✅ Added `resolve_box_collisions()` method with three-phase approach
2. ✅ Implemented bumper flat-ground detection (`normal.y > 0.9f`)
3. ✅ Implemented weightlifter slope grounding (`0.5f < normal.y ≤ 0.9f`)
4. ✅ Implemented bumper wall sliding (`normal.y ≤ 0.5f`)
5. ✅ Added surface classification constants
6. ✅ Enhanced test environment with wall geometry

**Outcome:** Character collides with world geometry. Stable, responsive ground contact on all surface types.

**Playtest Notes:** Revised from plan-specified "lifter-first" to "surface-classified" approach based on stability testing. Bumper-on-flat eliminates settling artifacts, lifter-on-slopes enables smooth transitions.

### Essential Feature (Phase B) 🔲 NOT STARTED
1. 🔲 Add step-up logic to weightlifter collision
2. 🔲 Tune step height limits
3. 🔲 Add graduated step test geometry

**Planned Outcome:** Character walks up stairs/obstacles naturally.

### Polish (Phase C) 🔲 NOT STARTED
1. 🔲 Multi-iteration resolution (2-3 passes per frame)
2. 🔲 Velocity damping on impact
3. 🔲 Stability tuning for complex geometry

**Planned Outcome:** Smooth, stable collision response in all scenarios.

### Debugging (Phase D) 🔲 NOT STARTED
1. 🔲 Collision box visualization in scene
2. 🔲 Ground contact indicators
3. 🔲 Surface normal visualization

**Planned Outcome:** Easy to diagnose collision issues visually.

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

✅ **Phase A Complete:** (achieved October 5, 2025)
- ✅ Character stands on all platform boxes in scene
- ✅ Character slides along walls without passing through
- ✅ Ground plane still works as fallback
- ✅ No settling/sinking behavior on landing
- ✅ Stable spawn pose and ground contact
- ✅ Test environment includes dedicated wall geometry

🔲 **Phase B Complete When:**
- Character walks up 0.15m steps without jumping
- Character walks up 0.30m steps without jumping
- Character is blocked by 0.60m obstacles
- No manual jumping required to navigate stepped terrain

🔲 **All Phases Complete When:**
- Character navigates entire test level naturally
- Collision feels smooth and predictable in all scenarios
- "Intuitive and responsive" (GDC design goal achieved)
- Debug visualization aids rapid iteration

---

**Overall Status:** Phase A complete. Phase B ready to begin.  
**Next Action:** Implement Phase B - Step-up behavior for weightlifter sphere.
