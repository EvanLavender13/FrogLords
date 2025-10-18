# Systems Backlog

**Ideas awaiting foundation certainty. Organized by dependency level.**

**Last Updated:** 2025-10-17
**Overall Foundation:** 89% certain (up from 79% after magic number documentation)

---

## The Dependency Stack

### Layer 1: CORE GAMEPLAY [Current Certainty: 90%]
**The irreducible minimum. Physics sphere movement + camera.**

**Complete and Working:**
- **Runtime Loop**: Frame timing, event handling [100%]
  - Depends on: Sokol library
  - Enables: All game systems
  - Status: ✅ Solid foundation

- **Input System**: WASD keyboard input [95%]
  - Location: `src/input/`
  - Depends on: Runtime loop
  - Enables: All player control
  - Status: ✅ Clean, simple

- **Camera System**: Orbit/follow camera [95%]
  - Location: `src/camera/`
  - Depends on: Input, runtime
  - Enables: All visualization
  - Status: ✅ Working well

- **Physics Sphere**: Single sphere collision + force-based movement [90%]
  - Location: `src/character/controller.h` (position, velocity, acceleration)
  - Depends on: Foundation math
  - Enables: Character systems
  - Status: ⚠️ Accumulation pattern (see repairs)

**Core gameplay works:** Move sphere with WASD, camera follows, physics feels good.

---

### Layer 2: PRIMITIVES [Current Certainty: 98%]
**Fundamental composable functions. The mathematical truth.**

**Complete and Proven:**
- **Spring-Damper Math**: Critically-damped springs [98%]
  - Location: `src/foundation/spring_damper.{h,cpp}`
  - Formula: ζ = c/(2√(km)), critical when ζ=1
  - Documented: ✅ Derivations complete
  - Status: ⚠️ Needs validation test

- **Collision Primitives**: Sphere, AABB, collision detection [100%]
  - Location: `src/foundation/collision_primitives.h`, `collision.{h,cpp}`
  - Status: ✅ Mathematically proven

- **Math Utils**: Vector/quaternion operations [100%]
  - Location: `src/foundation/math_utils.h`
  - Depends on: GLM library
  - Status: ✅ Validated at startup

- **Easing Functions**: Smooth interpolation [100%]
  - Location: `src/foundation/easing.{h,cpp}`
  - Status: ✅ Standard formulas

- **Procedural Mesh**: Runtime mesh generation [100%]
  - Location: `src/foundation/procedural_mesh.{h,cpp}`
  - Status: ✅ Working

- **Debug Visualization**: Visual debugging primitives [98%]
  - Location: `src/rendering/debug_*`
  - Added: 2025-10-17
  - Enables: All future debugging
  - Status: ✅ Complete, F3 toggle

- **Debug Assertions**: Mathematical validation framework [100%]
  - Location: `src/foundation/debug_assert.h`
  - Status: ✅ Validates at startup

**Foundation is solid. Ready to build systems.**

---

### Layer 3: SYSTEMS [Current Certainty: 90%]
**Composable systems built from primitives.**

**Complete and Working:**
- **Character Controller**: Physics + input → movement [90%]
  - Location: `src/character/controller.{h,cpp}`
  - Depends on: Physics sphere, collision, spring-damper
  - Includes: Jump, coyote time, jump buffering
  - Status: ⚠️ Needs repairs (see below)

- **Landing Animation**: Spring-based vertical compression [95%]
  - Location: `src/character/animation.{h,cpp}`
  - Depends on: Spring-damper, controller events
  - Status: ✅ Clean implementation

- **Orientation System**: Yaw toward input direction [85%]
  - Location: `src/character/orientation.{h,cpp}`
  - Depends on: Controller velocity/input
  - Status: ⚠️ Dual-reference violation, needs simplification

- **Tuning System**: Bidirectional parameter adjustment [80%]
  - Location: `src/character/tuning.{h,cpp}`
  - Pattern: `apply_to()` + `read_from()`
  - Status: ⚠️ Defaults mismatch (see repairs)

- **Rendering Scene**: Scene graph, camera, debug primitives [100%]
  - Location: `src/rendering/scene.{h,cpp}`, `renderer.{h,cpp}`
  - Status: ✅ Solid

- **GUI System**: ImGui panels for tuning [95%]
  - Location: `src/gui/`
  - Status: ✅ Working well

- **Game World**: High-level orchestration [95%]
  - Location: `src/app/game_world.{h,cpp}`
  - Integrates: Camera, character, scene, collision world
  - Status: ✅ Clean architecture

**Systems work but have known repairs needed.**

---

## REPAIR BACKLOG (Must Fix Before Expansion)

**Foundation: 90% → 95% Target**

These systems exist but block confident building until fixed:

### Critical (Blocks Foundation)
1. **Tuning Defaults Mismatch**
   - Violation: Inconsistent sources of truth
   - Impact: `controller.h` defaults ≠ `tuning.h` defaults (4x discrepancy)
   - Fix: Choose single source of truth
   - Complexity: 1 point (trivial)
   - Certainty: 100%
   - Next: IMMEDIATE

2. **Spring-Damper Validation**
   - Violation: Mathematical foundation unproven in isolation
   - Impact: Core primitive used everywhere
   - Fix: Add unit tests for critical damping formula
   - Complexity: 1 point
   - Certainty: 100%

### High (Technical Debt)
3. **Accumulated State Pattern**
   - Violation: Position/velocity accumulate errors over time
   - Location: `controller.{position, velocity}`
   - Fix options: Document + accept | Derive each frame | Verlet integration
   - Complexity: 2-3 points
   - Certainty: 80%

4. **Dual-Reference in Orientation**
   - Violation: Target yaw references smoothed yaw (circular)
   - Location: `orientation.{h,cpp}`
   - Fix: Separate target from current (principle: dual reference)
   - Complexity: 2 points
   - Certainty: 90%

5. **Controller Mixed Concerns**
   - Violation: Controller combines physics + animation + orientation
   - Fix: Extract subsystems more clearly (partially done)
   - Complexity: 3 points
   - Certainty: 85%

**Priority:** Fix in order listed. Don't pull from Layer 4 until Layer 3 is 95%+

---

### Layer 4: VARIATIONS [Current Certainty: <50%]
**Variations on proven systems. Add breadth.**

**Waiting on Repairs (Foundation <90% for these):**

- **Skeletal Animation System**
  - Depends on: Character controller (90%), debug viz (98%)
  - Enables: Procedural animation, IK
  - Certainty needed: 95%
  - Complexity: 5-8 points
  - Status: Foundation not ready

- **Walk/Run Speed States**
  - Depends on: Controller (90%), tuning (80%)
  - Enables: Movement variation
  - Certainty needed: 95%
  - Complexity: 3-5 points
  - Status: Tuning must be fixed first

- **Wall Detection & Sliding**
  - Depends on: Collision (100%), controller (90%)
  - Enables: Better navigation
  - Certainty needed: 95%
  - Complexity: 3-5 points

- **Dash Mechanic**
  - Depends on: Controller (90%), input (95%)
  - Enables: Fast movement option
  - Certainty needed: 95%
  - Complexity: 3-5 points

- **IK Systems**
  - Depends on: Skeletal system (0%), math utils (100%)
  - Enables: Foot placement, hand reaching
  - Certainty needed: 95%
  - Complexity: 8 points

**Status:** Don't build these until repairs complete (foundation 90% → 95%)

---

### Layer 5: POLISH [Current Certainty: <50%]
**Visual/audio refinement. Only after mechanics proven.**

**Far Future (Don't Detail Yet):**
- Audio system
- UI/HUD
- Visual effects
- Terrain system
- Combat system

**Status:** Keep in liquid pool. Don't detail until Layer 4 solid.

---

## Recently Completed

**2025-10-17: Debug Visualization System**
- Layer: 2 (Primitives)
- Time: ~1 day
- Complexity: 2.5 points (estimated 3)
- Emerged:
  - F3 toggle pattern (zero-cost when disabled)
  - Axes visualization (XYZ = RGB)
  - Startup validation framework
  - 5 new visualization opportunities
- Foundation impact: 95% → 98%
- Emergence count: 5 new enabled systems
- Review: APPROVED (exemplary engineering)

**2025-10-17: Magic Number Documentation**
- Layer: 2-3 (Foundation + Systems)
- Time: ~2 hours
- Complexity: 1 point
- Documented: 27 constants across 12 files
- Discoveries:
  - Tuning defaults mismatch
  - Bidirectional tuning pattern
  - 75/25 friction decomposition
- Foundation impact: 79% → 89% (+10 from understanding)
- Emergence count: 3 reusable patterns

**2025-10-17: Remove Ground Plane Special Case**
- Layer: 3 (Systems)
- Violation: Special case for ground (not general)
- Removed: Ground plane entity
- Simplified: All collision now uniform
- Principle restored: Consistency (no special cases)

**2025-10-17: Fix Framerate-Dependent Yaw Smoothing**
- Layer: 3 (Systems)
- Violation: Frame-rate coupled behavior
- Fixed: Proper delta-time integration
- Principle restored: Consistency (predictable behavior)

---

## Deferred Systems

**Locomotion System** (2025-10-17)
- Reason: Too complex, tried to prescribe outcomes
- Learning: Character works better with pure physics
- Emergence: Simpler is better
- If retry: Build from physics, not from animation states

**Explicit State Machine** (Not started)
- Reason: Reactive systems work better than prescriptive
- Principle: Let behavior emerge from continuous math
- Alternative: Spring-damper + thresholds

---

## Notes

**Patterns Observed:**
- Debug visualization is force multiplier (3-day bugs → 30 seconds)
- Documentation increases certainty (mental model solidifies)
- Special cases always indicate design smell
- Simpler systems more reliable than complex

**Dependencies Discovered:**
- All future animation needs debug visualization (now available)
- Tuning system blocks confident parameter work (fix next)
- Accumulated state acceptable if documented
- Dual-reference pattern appears in multiple systems (shared fix)

**Serendipitous Ideas:**
- F3 toggle pattern reusable for all debug systems
- RGB axes immediately intuitive (XYZ = RGB)
- Bidirectional tuning enables GUI experimentation
- Spring-damper applicable beyond animation

---

## Usage

### When Foundation Improves
After completing repairs:
1. Recalculate layer certainties
2. Move systems from "Waiting" to "Buildable"
3. Update cascade (multiply down from foundation)

### When Adding New Idea
```markdown
- **System Name**: One-line description
  - Depends on: <existing systems>
  - Enables: <future possibilities>
  - Certainty needed: <%>
  - Complexity: <points>
```

### When Selecting System
1. Check foundation certainty at layer
2. Verify all dependencies ≥90%
3. Create `PLANS/<name>_SYSTEM.md`
4. Move to "In Progress"
5. Begin system-building cycle

### After Completion
Move to "Recently Completed" with:
- Time taken
- Emerged behaviors
- Foundation impact
- Discoveries made

---

## Current Status: REPAIR MODE

**Don't pull from Layer 4+ until repairs complete.**

**Next actions:**
1. Fix tuning defaults (1 point, immediate)
2. Validate spring-damper math (1 point)
3. Document/fix accumulated state (2-3 points)
4. Fix orientation dual-reference (2 points)
5. **Then** foundation → 95%+ and Layer 4 becomes buildable

**Philosophy:** Strengthen foundation before expansion. Repairs aren't failures—they're discovered knowledge that makes future work certain.

---

**Build from certainty. Repair before expanding. Keep it liquid.**
