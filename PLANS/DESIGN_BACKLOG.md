# Design Backlog

**A liquid pool of unordered ideas. Pull only when foundation is certain.**

## The Rule

Ideas remain here until:
1. The foundation below them is 90%+ certain
2. They serve a clear gameplay need (not speculation)
3. They can be validated in isolation
4. They embody radical simplicity

---

## Immediate Priority (From Audit)

### 1. ✅ Debug Visualization System → COMPLETE (2025-10-17)
**Status:** ✅ IMPLEMENTED - Crisis resolved

**Delivered:**
- ✅ RGB axes for orientation (XYZ = RGB) via `debug::add_axes()`
- ✅ Velocity vector arrows via `debug::add_velocity_arrow()`
- ✅ Acceleration arrows via `debug::add_direction_arrow()`
- ✅ Collision sphere wireframe via `debug::add_collision_sphere()`
- ✅ Assertion framework for mathematical validation
- ✅ Startup validation of coordinate system and GLM assumptions
- ✅ F3 toggle for zero-impact control

**Actual Complexity:** 2.5 points (simpler than expected via reuse)
**Actual Certainty:** 98% (exceeded estimate)
**Result:** 3-day debugging crises now resolved in 30 seconds
**Gemini Review:** APPROVED - "exemplary engineering" with perfect principle adherence

**Emergence:** Completing debug visualization enabled 5 new possibilities:
- Skeletal animation debug visualization (see below)
- Physics force visualization (see below)
- IK chain debug visualization (see below)
- AI pathfinding visualization (see below)
- Gameplay state visualization (see below)

**Impact:** Foundation certainty increased 95% → 98%. System cascade improved 77% → 79%.

---

## Next Priority (From Updated Stack)

### 2. Fix Tuning System Defaults Mismatch
**Discovered:** During magic numbers documentation (2025-10-17)
**Violation:** Inconsistent sources of truth (controller.h vs tuning.h defaults)

**Issue:**
- Controller defaults imply `time_to_max_speed ≈ 1.6s`
- Tuning defaults specify `time_to_max_speed = 0.4s` (4x discrepancy)
- Root cause: Bidirectional system uses `read_from` at init (reverse-engineers)

**Options:**
- Apply tuning defaults at init (prescriptive approach)
- Align tuning defaults to match controller (descriptive approach)
- Choose single source of truth explicitly

**Complexity:** 1 point (small, localized fix)
**Certainty:** 100% (pure consistency fix, well-understood)
**Validation:** Both systems produce identical values

**Priority:** HIGH - Blocks confident tuning, foundation issue

### 3. Fix Accumulated State Pattern
**Violation Found:** Position/velocity accumulate errors

**Options:**
- Document and accept the accumulation (simplest)
- Calculate velocity from input each frame (harder)
- Use Verlet integration (position-based)

**Complexity:** 2-3 points
**Certainty:** 80% (multiple valid solutions)
**Validation:** Long-running stability test

### 3. ✅ Document Magic Numbers → COMPLETE (2025-10-17)
**Status:** ✅ ALL CONSTANTS DOCUMENTED

**Delivered:**
- 27 constants across 12 files with grep-able category tags
- All formulas verified (critical damping, jump kinematics, etc.)
- Units specified for every constant
- Relationships documented
- Findings report with critical discovery (tuning defaults mismatch)

**Complexity:** 1 point (as estimated)
**Certainty:** 100% (achieved)
**Impact:** Foundation certainty +10 points (79% → 89%)

**Emergent Discoveries:**
- Bidirectional tuning pattern (apply_to + read_from)
- 75/25 friction decomposition (reusable pattern)
- Documentation IS foundation repair (+10 certainty)

---

## Foundation Repairs (Next Priority)

### Verify Yaw Sign Convention
**Suspicion:** Right vector has negative X component

**Required:**
- Mathematical verification of yaw_to_right function
- Visual test with debug axes
- Unit test for rotation consistency

**Complexity:** 2 points
**Certainty:** 90% (math verification)
**Validation:** Visual and mathematical agreement

### Fix Dual-Reference Violation
**Violation Found:** Orientation smoothing references itself

**Required:**
- Ensure target_yaw never depends on current_yaw
- Implement proper dual-reference pattern
- Test for stability

**Complexity:** 1-2 points
**Certainty:** 95% (pattern is known)
**Validation:** No circular dependencies

### Separate Controller Concerns
**Violation Found:** Controller does physics + animation state

**Consider:**
- Move landing detection to animation system
- Animation queries physics, not vice versa
- Pure physics in controller

**Complexity:** 3-4 points
**Certainty:** 70% (refactor risk)
**Validation:** Clean dependency flow

---

## Mathematical Validation Suite

### Spring-Damper Validation
**From Audit:** Verify critical damping formula

**Tests:**
- Convergence time
- Overshoot behavior
- Stability at various dt
- Energy conservation

**Complexity:** 2-3 points
**Certainty:** 90% (math is known)

### Friction Formula Verification
**Suspicion:** friction * abs(gravity) seems arbitrary

**Required:**
- Derive from first principles
- Or find physical justification
- Or simplify to direct friction

**Complexity:** 2 points
**Certainty:** 70% (physics research needed)

---

## Removed Systems (Learn From)

### What Failed (2025-10-16)
- **Acceleration Tilt:** Coordinate confusion, sign errors
- **Contact Springs:** Complexity without purpose
- **Locomotion System:** Over-engineered for current needs
- **Walk/Run Blending:** Unnecessary state multiplication

### Why They Failed
Not because the ideas were bad, but because:
1. Mathematical foundation wasn't validated
2. Complexity preceded necessity
3. Couldn't debug what couldn't be seen
4. Built on assumptions, not verified truth

### Lessons
These may return when:
- Debug visualization exists
- Mathematical foundations are proven
- Simpler versions prove insufficient
- Never before

---

## Character Systems (Now Possible)

### Skeletal Animation Debug Visualization
**Status:** NOW ENABLED (debug visualization complete)

**What's Possible:**
- Use `debug::add_axes()` for bone orientation visualization
- Use `debug::add_line()` for bone hierarchy connections
- Visualize joint rotations, IK chains, blend trees
- Validate forward kinematics visually

**Complexity:** 1-2 points (primitives exist, just apply them)
**Certainty:** 95% (proven pattern, just new domain)
**Priority:** Medium-High (when skeletons implemented)

### Skeletal Animation System
**Prerequisites:**
1. ✅ Debug visualization (COMPLETE 2025-10-17)
2. Forward kinematics validation
3. Isolated rotation tests
4. Clear parent-child conventions

**Current Status:** Foundation ready. Visualization exists. Can proceed when needed.

### Movement Abilities
Dash, wall-jump, ledge-grab, climbing, swimming, crouching.

**Deferred because:** Current single-speed sphere is sufficient. Complexity must be earned.

---

## Physics Extensions (Now Possible)

### Physics Force Visualization
**Status:** NOW ENABLED (debug visualization complete)

**What's Possible:**
- Use `debug::add_velocity_arrow()` for forces, torques, impulses
- Visualize spring forces, damping, friction
- See contact normals, collision responses
- Debug physics interactions in real-time

**Complexity:** 1 point (primitives exist, trivial application)
**Certainty:** 98% (proven pattern)
**Priority:** High (next physics feature should include this)

### IK Chain Debug Visualization
**Status:** NOW ENABLED (debug visualization complete)

**What's Possible:**
- Axes for joint orientations
- Arrows for target offsets and desired directions
- Spheres for reach limits and constraint boundaries
- Visual validation of IK solver convergence

**Complexity:** 1-2 points (primitives exist)
**Certainty:** 95% (proven pattern)
**Priority:** Medium (when IK implemented)

### Frame-Independent Physics
**From Audit:** Current integration accumulates errors

**Options:**
- Fixed timestep with interpolation
- Verlet integration
- RK4 integration
- Or document and accept current state

**Complexity:** 4-6 points
**Certainty:** 60% (significant change)

### Collision Evolution
- Dual-sphere (bumper + weightlifter)
- Slope handling
- Moving platform interaction

**Current:** Single sphere works. Add complexity only when single sphere fails.

---

## New Patterns (Discovered 2025-10-17)

### Bidirectional Parameter Systems
**Discovery:** Tuning system is intentionally bidirectional

**Pattern:**
- `apply_to`: Designer intent → runtime params (prescriptive)
- `read_from`: Runtime params → designer view (descriptive)
- Enables exploration (adjust and see) + comprehension (observe and understand)

**Application Opportunities:**
- Camera parameters (preset → values, values → preset)
- Animation blend weights (high-level → blend tree, blend tree → visualization)
- Difficulty tuning (feel → numbers, numbers → feel)

**Certainty:** 90% (pattern proven in tuning system)
**Complexity:** 2-3 points per application

### Mathematical Pattern Library
**Discovery:** Reusable formulas identified during documentation

**Patterns Worth Documenting:**
- Critical damping: `c = 2√(k·m)` (harmonic oscillator)
- Friction decomposition: 75% friction + 25% net = tunable responsiveness
- Jump kinematics: `v = √(2gh)` (energy conservation)
- Exponential smoothing: `factor = 1 - exp(-rate·dt)` (framerate independence)

**Purpose:** Reference for future physics/tuning work
**Certainty:** 100% (already validated)
**Complexity:** 0.5 points (just documentation)

### Systematic Constant Review
**Discovery:** Category-based grep enables systematic improvements

**Enabled Operations:**
- `grep "// TUNED:"` → Find all empirical values for focused playtesting
- `grep "// CALCULATED:"` → Verify derived values match formulas
- `grep "// DERIVED:"` → Review mathematical correctness
- `grep "// COEFFICIENT:"` → Check dimensionless multipliers

**Application:** Regular constant audits, physics validation, tuning sessions
**Certainty:** 100% (infrastructure exists)
**Complexity:** 0 points (already complete)

---

## Code Quality Improvements

### Explicit State Machine
**From Audit:** Grounded/airborne state is implicit

**Consider:**
- Explicit state enum
- Clear transitions
- State-specific behavior

**Complexity:** 2-3 points
**Certainty:** 70% (may add complexity without benefit)

### Constants Configuration
**From Audit:** Magic numbers scattered

**Consider:**
- Central constants file
- Runtime tuning for all values
- Save/load tuning presets

**Complexity:** 2-3 points
**Certainty:** 80% (improves iteration speed)

---

## Camera & Input (Working)

### Potential Refinements
- Wall collision prevention
- Smoother follow modes
- Gamepad support (already in sokol)
- Rebindable keys

**Priority:** Low. Current systems work and don't violate principles.

---

## AI Systems (Now Possible)

### AI Pathfinding Visualization
**Status:** NOW ENABLED (debug visualization complete)

**What's Possible:**
- Lines for paths and navigation graphs
- Spheres for waypoints and decision points
- Arrows for desired velocity and steering forces
- Visualize pathfinding, line of sight, behavior trees

**Complexity:** 1-2 points (primitives exist)
**Certainty:** 90% (proven pattern, new domain)
**Priority:** Low (speculative, no AI yet)

---

## Gameplay Systems (Now Possible)

### Gameplay State Visualization
**Status:** NOW ENABLED (debug visualization complete)

**What's Possible:**
- Player-visible forces and motion vectors
- Targeting indicators and attack ranges
- Interactive physics feedback
- Could become actual game mechanic (showing player their physics state)

**Complexity:** 2-3 points (design + implementation)
**Certainty:** 70% (design exploration needed)
**Priority:** Medium (polish phase, emergent gameplay possibility)

---

## World Systems (Graybox)

### Terrain
- Heightmaps
- Uneven surfaces
- Moving geometry
- Water volumes

**Current:** Flat boxes sufficient for testing movement. Defer complexity.

---

## Polish Layer (Premature)

### Audio
Footsteps, impacts, ambience, music, 3D positioning.

### Visual Effects
Particles, screen shake, post-processing, time dilation.

**Status:** Polish serves proven mechanics. Current mechanics not proven.

---

## UI Systems

### Debug UI Improvements
**From Audit:** Need better visibility

**Consider:**
- Physics state panel (grounded, velocity, etc.)
- Coordinate system indicator
- Performance profiler
- State history graph

**Complexity:** 3-4 points
**Priority:** Medium-High (tools accelerate everything)

---

## Frog Concepts 🐸

**Pure speculation, kept for inspiration:**

Tongue grapple, hop charging, sticky surfaces, water advantages, fly catching, inflation mechanics, egg spawning, croak communication, metamorphosis, lily pads.

**Certainty:** ~0%. Most will never exist.

---

## Selection Criteria

Before pulling from backlog, ask:

1. **Does it serve immediate gameplay?** (Not hypothetical future gameplay)
2. **Can it be validated in isolation?** (Test before integration)
3. **Is it the simplest solution?** (Complexity must justify itself)
4. **Are its dependencies certain?** (Don't build on sand)
5. **Can we explain why it exists?** (Purpose must be clear)

If any answer is "no," leave it in the pool.

---

## The Discipline

This backlog is deliberately:
- **Unordered:** No false priorities (except critical fixes)
- **Liquid:** Ideas can merge, split, die
- **Mostly wrong:** Most ideas won't survive contact with reality
- **Always growing:** Capture everything, build almost nothing

The backlog is not a todo list. It is a pool of possibilities waiting for the right foundation.

**Update when:** Ideas emerge, items pulled, systems removed, lessons learned.

**Never:** Create dependencies between backlog items. Plan beyond the foundation's certainty.