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

### 1. Debug Visualization System
**Status:** CRITICAL - Root cause of coordinate confusion crisis

**Required:**
- RGB axes for orientation (XYZ = RGB)
- Velocity vector arrows (green)
- Acceleration vector arrows (yellow)
- Ground normal indicator (blue)
- Collision sphere wireframe (always visible)
- Input direction indicator (white)

**Complexity:** 3-4 points
**Certainty:** 95% (proven patterns exist)
**Validation:** Can see and understand all physics state visually
**Why First:** Cannot debug what cannot be seen. This caused the crisis.

### 2. Fix Accumulated State Pattern
**Violation Found:** Position/velocity accumulate errors

**Options:**
- Document and accept the accumulation (simplest)
- Calculate velocity from input each frame (harder)
- Use Verlet integration (position-based)

**Complexity:** 2-3 points
**Certainty:** 80% (multiple valid solutions)
**Validation:** Long-running stability test

### 3. Document Magic Numbers
**Violation Found:** Unexplained constants throughout

**Required:**
- Document BUMPER_RADIUS (why 0.5?)
- Document epsilon values (why 0.0001?)
- Document friction formula (why gravity multiplier?)
- Either justify or derive from first principles

**Complexity:** 1 point
**Certainty:** 100% (documentation task)
**Validation:** Can explain every constant

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

## Character Systems (Deferred)

### Skeletal Animation Prerequisites
**Cannot build without:**
1. Debug visualization (see bones, joints, axes) ← REQUIRED FIRST
2. Forward kinematics validation
3. Isolated rotation tests
4. Clear parent-child conventions

**Current Status:** Foundation exists (quaternions validated) but visualization missing.

### Movement Abilities
Dash, wall-jump, ledge-grab, climbing, swimming, crouching.

**Deferred because:** Current single-speed sphere is sufficient. Complexity must be earned.

---

## Physics Extensions (Deferred)

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