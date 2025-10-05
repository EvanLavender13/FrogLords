# Collision System Implementation Plan

**Date:** October 5, 2025  
**Purpose:** Implement sphere-vs-AABB collision detection for terrain/platform handling  
**Sources:** GDC 2013 (David Rosen), AnimationDoc1.md, Elegance.md, lifter_bumper_system_analysis.md

**Status:** ⚠️ **MAJOR REVISION** - Single-sphere experiment supersedes dual-sphere implementation

---

## Revision History

**October 5, 2025 - Critical Design Revelation**
- **DISCOVERED:** Misinterpreted Overgrowth sphere configuration from GDC screenshot
- **Reality:** Lifter rests ON ground (zero penetration), bumper is ELEVATED above
- **Our Implementation:** Had it backward - buried lifter, grounded bumper
- **Result:** All Phase A/B complexity (burial depth, authority arbitration) was self-inflicted
- **Action:** Pivoted to single-sphere experiment, which accidentally implements correct lifter behavior
- **Status:** Single-sphere working excellently, validates simplified approach
- **Reference:** See `REVIEWS/overgrowth_sphere_configuration_reanalysis.md`

**October 5, 2025 - Phase A Completion Update**
- Documented actual Phase A implementation (surface-classified approach)
- Added playtest justifications for deviation from original "lifter-first" plan
- Removed code examples from Phase A (now implemented in codebase)
- Added validation results and bug resolution notes
- Updated success criteria to reflect achieved state
- Cross-referenced supporting documents (reviews, bug reports, action summaries)

---

## Design Philosophy

### The Overgrowth Approach (Revised Understanding)

From David Rosen's GDC 2013 presentation:

> "I had this one green sphere as a bumper sphere it lets him fly it off objects that he runs into [and] a weightlifter's sphere which moves up over small obstacles."

**Core Principle:** Two spheres + simple environment geometry = emergent locomotion behaviors.

**No complex collision shapes mentioned:**
- No triangle meshes
- No heightmaps
- No capsules, cylinders, or convex hulls
- Just spheres against simple geometric primitives

### ⚠️ Critical Insight from Screenshot Analysis (October 5, 2025)

**What We Misunderstood:**
From the GDC talk screenshot showing the dual-sphere configuration:
- **Lifter sphere (white):** Rests cleanly ON ground surface (zero penetration)
- **Bumper sphere (green):** ELEVATED above ground, surrounds upper body
- **Spatial separation:** No overlap, no authority conflicts

**What We Implemented (Incorrectly):**
- Lifter: Buried 0.10m into ground for "stability"
- Bumper: At ground level, fighting lifter for authority
- Result: Complex arbitration, adaptive penetration, self-inflicted bugs

**Correct Implementation (Likely):**
- **Lifter:** Primary ground contact, drives vertical (Y) position
- **Bumper:** Elevated obstacle buffer, drives horizontal (XZ) avoidance
- **Clean separation:** Bumper too high to touch ground → no authority conflicts

**Implication:** The dual-resolver system on main branch may be solving problems we invented by misunderstanding the geometry.

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

## Phase B: Step-Up Behavior - DESIGN REVISED

**Status:** ✅ **COMPLETE** (through natural collision only)

**Goal:** Character automatically steps up onto obstacles within lifter radius height.

**Rationale:** From GDC talk - "weightlifter's sphere which moves up over small obstacles"

**Prerequisites:** Phase A complete ✅

**Design Decision (October 5, 2025):** After implementation and playtesting, determined that **no special step-up logic is needed**. The weightlifter sphere's geometry naturally determines traversable obstacle height through existing slope collision handling. This emergent behavior is more elegant than explicit step-up thresholds.

### B.1: Natural Collision Investigation

**Status:** ✅ **COMPLETE** (October 5, 2025)

**File:** `src/character/controller.cpp`

**Design Summary:**
Investigated step-up behavior and determined that natural collision handling is superior:

**Key Findings from Playtesting:**
1. **Small steps (≤0.15m):** Naturally traversable via slope collision handling
2. **Medium steps (0.20-0.30m):** Explicit step-up logic felt like "teleportation"
3. **Large steps (≥0.40m):** Should block the character (intentional obstacle)

**Design Principle Applied:**
> "Simple mechanics, complex interactions" - Let sphere geometry define traversability

**Implementation Decision:**
Removed all explicit step-up logic. The weightlifter sphere's radius (~0.45m) and the existing slope collision threshold (`SLOPE_NORMAL_THRESHOLD = 0.5f` / 60°) naturally determine what can be climbed:

```cpp
// No special step-up logic needed
// The weightlifter sphere's geometry naturally determines traversable
// obstacle height through the existing slope collision handling.
```

**Test Geometry Added:**
**File:** `src/app/runtime.cpp`

Added graduated step obstacles for testing:
```cpp
// Step-up test: Graduated steps (0.15m, 0.30m, 0.40m, 0.60m)
for (int i = 0; i < 4; ++i) {
    float height = 0.15f * (i + 1);
    aabb step;
    step.center = glm::vec3(-5.0f + i * 2.0f, height * 0.5f, -8.0f);
    step.half_extents = glm::vec3(0.8f, height * 0.5f, 0.8f);
    scn.add_collision_box(step);
}
```

**Traversability Characteristics:**
- **Maximum climbable height:** Emerges naturally from weightlifter sphere radius (~0.45m)
- **Smooth traversal:** Small obstacles handled by slope collision (no special cases)
- **Natural blocking:** Larger obstacles prevent passage without explicit thresholds

**Alignment with Project Principles:**
- ✅ **Simple mechanics, complex interactions** - Sphere geometry defines behavior
- ✅ **Stupidly simple cores, emergent complexity** - No step-up logic needed
- ✅ **Parameters > assets** - Sphere radius is the only relevant parameter
- ✅ **No content restrictions** - Level designers can use any obstacle height
- ✅ **Iteration over planning** - Discovered through playtesting that explicit logic was unnecessary

**Result:** Phase B complete through natural collision system. No additional implementation required.

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

### Essential Feature (Phase B) ✅ **COMPLETE** (October 5, 2025)
1. ✅ **B.1 Complete:** Investigated step-up behavior through playtesting
2. ✅ **B.2 Complete:** Determined natural collision is superior to explicit logic
3. ✅ **B.3 Complete:** Removed step-up constants and logic
4. ✅ **B.4 Complete:** Validated with graduated step test geometry (kept for testing)
5. ✅ **B.5 Complete:** Confirmed natural collision feel is smooth and responsive
6. ✅ **B.6 Complete:** No threshold tuning needed (emergent behavior)

**Actual Outcome:** Character traverses obstacles naturally based on weightlifter sphere geometry. No explicit step-up logic needed.

**Design Note:** Original plan assumed explicit step-up logic was necessary. Playtesting revealed that natural collision handling produces superior feel and aligns better with project principles of emergent complexity from simple mechanics.

### Polish (Phase C) 🔲 NOT STARTED
1. 🔲 Multi-iteration resolution (2-3 passes per frame)
2. 🔲 Velocity damping on impact
3. 🔲 Stability tuning for complex geometry
4. 🔲 (Optional) Spring-damper step-up transition if instant lift feels jarring

**Planned Outcome:** Smooth, stable collision response in all scenarios. Time-based transitions if needed.

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
- **REVIEWS/collision_system_phase_a_review.md** - Phase A implementation analysis
- **REVIEWS/collision_system_phase_b_review.md** - Phase B plan review (identified step_height calculation error)

---

## Success Criteria

✅ **Phase A Complete:** (achieved October 5, 2025)
- ✅ Character stands on all platform boxes in scene
- ✅ Character slides along walls without passing through
- ✅ Ground plane still works as fallback
- ✅ No settling/sinking behavior on landing
- ✅ Stable spawn pose and ground contact
- ✅ Test environment includes dedicated wall geometry

✅ **Phase B Complete:** (achieved October 5, 2025)
- ✅ Character walks up small steps (≤0.15m) smoothly via natural collision
- ✅ Character is naturally blocked by medium-large steps (≥0.30m)
- ✅ No special step-up logic needed
- ✅ Traversability emerges from weightlifter sphere geometry
- ✅ Feel is smooth and responsive (no teleportation effect)
- ✅ No manual jumping required for naturally traversable obstacles

🔲 **All Phases Complete When:**
- Character navigates entire test level naturally
- Collision feels smooth and predictable in all scenarios
- "Intuitive and responsive" (GDC design goal achieved)
- Debug visualization aids rapid iteration

---

## Progress Documentation - Phase B Design Revision

**Date:** October 5, 2025  
**Status:** ✅ **COMPLETE**

### Design Journey
Phase B underwent significant revision through iteration and playtesting:

**Initial Approach:**
- Implemented explicit step-up logic with thresholds (MIN/MAX heights, speed checks)
- Logic detected horizontal collisions and applied instant vertical lift
- Added tuning parameters for step height, speed, and normal angle ranges

**Playtesting Discovery:**
- Small steps (0.15m) felt smooth and natural
- Larger steps (0.30m+) felt like "teleportation" with instant lift
- Investigation revealed 0.15m steps already worked via existing slope collision

**Elegant Solution:**
- Removed all explicit step-up logic entirely
- Let weightlifter sphere geometry naturally determine traversability
- Maximum climbable height emerges from sphere radius (~0.45m) and slope threshold (60°)

### Key Design Decision
**No special step-up logic needed.** The existing three-phase collision system (bumper flat ground, lifter slopes, bumper walls) already provides natural obstacle traversal for appropriately-sized steps.

### Files Modified
- `src/character/controller.cpp`: Removed step-up constants and logic
- `src/app/runtime.cpp`: Kept graduated step test geometry for validation
- `PLANS/collision_system_implementation.md`: Updated to reflect natural collision design

### Build & Quality Assurance
- ✅ **Compilation:** Successful build with simplified code
- ✅ **Code Standards:** Follows snake_case conventions, proper namespacing
- ✅ **Reduced Complexity:** Fewer lines of code, no special cases

### Validation Results
- ✅ **Small obstacles (≤0.15m):** Smooth natural traversal
- ✅ **Medium obstacles (0.20-0.30m):** Blocked as intended
- ✅ **Large obstacles (≥0.40m):** Blocked as intended
- ✅ **No teleportation effect:** All movement feels organic

### Alignment with Project Principles
- ✅ **Simple mechanics, complex interactions** - Sphere geometry defines all behavior
- ✅ **Stupidly simple cores, emergent complexity** - Zero special-case logic
- ✅ **Clarity over cleverness** - No thresholds, no conditionals, just physics
- ✅ **Iteration over planning** - Discovered through testing that explicit logic was unnecessary
- ✅ **Honor serendipity** - The "right" solution emerged through experimentation

**Result**: Phase B complete. Natural collision system provides elegant obstacle traversal without additional complexity.

---

---

## Phase B Bug Fix - October 5, 2025

**Issue Discovered:** Weightlifter sphere penetration on vertical step walls during certain approach speeds.

**Root Cause Analysis:**
The three-phase collision system had a gap in coverage:
1. Phase 1a: Bumper handles flat ground (`normal.y > 0.9`)
2. Phase 1b: Weightlifter handles slopes (`0.5 < normal.y ≤ 0.9`) **but not walls**
3. Phase 2: Bumper handles walls (`normal.y ≤ 0.5`)

**The Bug:**
When the bumper sphere passed a step edge but the weightlifter (offset below) collided with the vertical wall:
- Weightlifter collision detected (`col.hit = true`)
- Collision normal was horizontal (`normal.y ≈ 0.0`)
- Phase 1b's slope condition failed (`0.0 is not > 0.5`)
- **No collision resolution occurred** → weightlifter penetrated obstacle
- Phase 2 only checks bumper, so weightlifter wall collision was never resolved

**Solution Implemented:**
Added wall collision handling to Phase 1b (weightlifter):
```cpp
else if (col.normal.y <= SLOPE_NORMAL_THRESHOLD) {
    // Push character away from wall
    position += col.normal * col.penetration;
    // Remove velocity into wall
    // Update debug info
}
```

**Coverage Now Complete:**
- Phase 1a: Bumper → flat ground
- Phase 1b: Weightlifter → slopes **AND walls**
- Phase 2: Bumper → walls (redundant coverage, but ensures solid blocking)

**Files Modified:**
- `src/character/controller.cpp`: Added weightlifter wall collision branch

**Status:** Bug fixed, both spheres now handle all collision cases.

---

## Phase B Additional Bug Fix - Edge Sinking Issue

**Issue Discovered:** Weightlifter sphere gradually sinking into step surfaces when standing near edges, followed by sudden correction "nudge".

**Root Cause Analysis:**
1. **Coverage Gap:** Weightlifter Phase 1b only handled slopes (`0.5 < normal.y ≤ 0.9`), not flat surfaces (`normal.y > 0.9`)
2. **Burial Logic Flaw:** On slopes, intentional 0.10m burial was allowed, but this permitted excessive penetration on flat surfaces
3. **Sudden Correction:** When penetration exceeded threshold, correction applied all at once (the "nudge")

**The Problem:**
When character stood near step edge:
- Bumper handled flat ground in Phase 1a
- Weightlifter collided with step's flat top surface
- Phase 1b condition (`normal.y > 0.5 && normal.y ≤ 0.9`) **failed** for flat surfaces
- Weightlifter penetration went unresolved → visible sinking
- Eventually exceeded burial threshold → sudden forceful correction

**Solution Implemented:**
Extended Phase 1b to handle **all upward-facing surfaces** (both flat and slopes), with adaptive penetration limits:

```cpp
// Phase 1b now handles all ground surfaces (normal.y > 0.5)
if (col.normal.y > SLOPE_NORMAL_THRESHOLD) {
    bool is_flat = col.normal.y > FLAT_GROUND_NORMAL_THRESHOLD;
    float max_allowed_penetration = is_flat ? 0.02f : INTENDED_BURIAL_DEPTH;
    
    // Flat: 0.02m tolerance (minimal, just collision epsilon)
    // Slopes: 0.10m burial (stability on transitions)
}
```

**Penetration Limits:**
- **Flat surfaces (>0.9 normal.y):** 0.02m maximum (safety check, bumper already handled it)
- **Sloped surfaces (0.5-0.9 normal.y):** 0.10m intentional burial (stability for edges)
- **Walls (≤0.5 normal.y):** Full pushout (no penetration allowed)

**Coverage Now Complete:**
| Surface Type | Bumper | Weightlifter |
|--------------|--------|--------------|
| Flat Ground (>0.9) | Phase 1a ✅ | Phase 1b ✅ (safety) |
| Slopes (0.5-0.9) | — | Phase 1b ✅ |
| Walls (≤0.5) | Phase 2 ✅ | Phase 1b ✅ |

**Files Modified:**
- `src/character/controller.cpp`: Extended Phase 1b condition and added adaptive penetration logic

**Status:** Edge sinking and sudden corrections eliminated. Both spheres have complete, overlapping coverage.

---

---

## Single-Sphere Experiment (October 5, 2025)

**Branch:** `feature/single-sphere-collision-experiment`  
**Status:** ✅ **SUCCESS** - Working excellently

### Motivation

After discovering the sphere configuration misunderstanding, we time-boxed an experiment:
- Remove all dual-sphere complexity (authority arbitration, burial depth, adaptive logic)
- Use ONLY the bumper sphere for collision resolution
- Test if simpler system feels better

### Implementation

**Removed:**
- Three-phase collision loop
- Surface classification thresholds (FLAT_GROUND_NORMAL_THRESHOLD)
- Adaptive penetration limits (0.02m vs 0.10m)
- Weightlifter collision resolution
- Authority arbitration logic

**Kept:**
- Single collision loop testing bumper sphere only
- Zero-penetration ground contact
- Simple velocity projection on collision
- Grounded state detection (normal.y > SLOPE_NORMAL_THRESHOLD)

**Code Reduction:**
- **Before:** ~150 lines, 3 loops, 2 authority thresholds, 2 penetration limits
- **After:** ~45 lines, 1 loop, 1 slope threshold, 0 penetration limits
- **Reduction:** 70% less code

### Results

**Feel Testing:**
- ✅ Ground contact: Instant, solid, no settling
- ✅ Wall sliding: Smooth, predictable
- ✅ Platform navigation: Clean, responsive
- ✅ Step obstacles: Can't auto-climb (requires jump), but feels intentional
- ✅ Corners/edges: Stable, no jitter

**Bug Count:**
- **Dual-sphere (main):** 3 bugs fixed (slow sink, wall penetration, edge sinking)
- **Single-sphere (experiment):** 0 bugs encountered

**Complexity:**
- **Dual-sphere:** Complex arbitration, hard to reason about
- **Single-sphere:** Trivially simple, obviously correct

### Why It Works

**The single-sphere accidentally implements the correct lifter behavior:**
- Zero-penetration ground contact (not buried)
- Simple collision resolution
- No authority conflicts
- Clean, predictable physics

**The "loss" of step-up is negligible:**
- Only affected 0.15m obstacles (tiny)
- Players can jump over them
- Manual control feels more intentional than auto-climb

### Recommendation

**STRONGLY CONSIDER:** Make single-sphere the foundation for future work.

**Rationale:**
- Dramatically simpler (~70% code reduction)
- Zero bugs vs. 3 bugs in dual-sphere
- Aligns with "simplicity over sophistication" principle
- Easier to extend (add elevated bumper later if needed)

**Next Steps (If Adopted):**
1. Merge single-sphere experiment to main
2. Archive dual-sphere implementation as historical reference
3. If "body buffer" needed, add elevated bumper (walls-only, never ground)
4. Continue with Phase C/D tuning on simplified foundation

---

**Overall Status:**
- ❌ **Dual-Sphere (Main Branch):** Functional but complex, based on incorrect assumptions
- ✅ **Single-Sphere (Experiment):** Simple, stable, likely correct interpretation
- 🤔 **Elevated Bumper (Future):** Possible enhancement, but single-sphere sufficient for now

**Recommended Action:** Adopt single-sphere as primary implementation.
