# Refinement: Accumulated State Pattern

**Document the rationale for position/velocity accumulation in physics integration**

---

## Selected

**Date:** 2025-10-18
**Complexity:** Trivial
**Path:** A (trivial - document only, no code changes)

---

## Violation Details

**Location:**
- File: `src/character/controller.{h,cpp}`
- Lines: 17-18 (position, velocity members), 96-112 (integration)
- System: Character Controller (Layer 1 - Core Physics)

**Principle Violated:**
- Primary: **Principled Development**
- Score before: 9.0/10 (undocumented trade-off)

**Severity:** Medium

**Type:** Undocumented trade-off

---

## Current State

**What exists now:**

```cpp
struct controller {
    // State (source of truth)
    glm::vec3 position;  // ← accumulated state
    glm::vec3 velocity;  // ← accumulated state
    glm::vec3 acceleration;
    // ...
};

void controller::update(const collision_world* world, float dt) {
    // Apply gravity
    acceleration.y += gravity;

    // Integrate velocity (ACCUMULATES)
    velocity += acceleration * dt;

    // Apply friction (if grounded)
    if (is_grounded) {
        // ... friction logic ...
    }

    // Integrate position (ACCUMULATES)
    position += velocity * dt;

    // ...
}
```

**Why this violates principles:**

- **Undocumented trade-off:** No explanation of why accumulation is acceptable
- **Principle unclear:** PRINCIPLES.md says "Pure Functions Over Accumulated State" but physics *requires* accumulation
- **Long-term stability:** No documentation of drift risks or mitigation
- **Integration method:** Euler integration not explicitly named or justified

**Impact:**
- Blocks: Understanding of when accumulated state is acceptable
- Cascades to: Future systems that might copy this pattern without understanding
- Foundation impact: Keeps Layer 1 at 90% instead of 95%

---

## Fix Approach

**Primary approach:** DOCUMENT

**Why we must keep it:**

Physics simulation fundamentally requires accumulated state. Position and velocity MUST be integrated over time. This is not a violation - this is the correct implementation of physics.

**The trade-offs we're accepting:**

1. **Euler integration** (semi-implicit)
   - Method: velocity += a·dt; position += v·dt
   - Pros: Fast, simple, stable for damped systems
   - Cons: Energy drift for undamped systems (not a concern - we have friction and collision)
   - Alternatives: Verlet (needs refactor), RK4 (overkill for platformer)

2. **Accumulated state** (position, velocity)
   - Required: Yes - physics integration demands accumulation
   - Drift risk: Mitigated by friction, collision resolution, and grounded state resets
   - Acceptable: Yes - this is the *correct* pattern for physics

3. **Frame-rate independence**
   - All rates multiplied by dt
   - Acceleration in m/s²
   - Velocity in m/s
   - Position in m
   - Unit analysis: Correct

**What needs documentation:**

1. **In controller.h comments:**
   - Mark position/velocity as accumulated state
   - Note: "Required for physics integration - acceptable use of accumulated state"
   - Reference: PRINCIPLES.md exception for physics simulation

2. **In PRINCIPLES.md:**
   - Add exception clause: "Physics simulation requires accumulated state for position/velocity"
   - Document when accumulation is acceptable vs. when to use derived state
   - Clarify: Integration over time is different from caching/memoization

3. **In controller.cpp comments:**
   - Document integration method (semi-implicit Euler)
   - Note stability properties
   - Explain why Euler is acceptable for this use case

---

## Success Criteria

**This refinement succeeds when:**
- [x] Position/velocity marked as accumulated state in controller.h
- [x] PRINCIPLES.md updated with physics integration exception
- [x] Integration method documented in controller.cpp
- [x] Trade-offs explained (why Euler, not Verlet/RK4)
- [x] Clear guidance on when accumulated state is acceptable
- [x] All tests still passing (no code changes)
- [x] No behavior changes

---

## Estimated Metrics

**Before:**
- LOC controller.h: 115
- LOC controller.cpp: 139
- LOC PRINCIPLES.md: ~200
- Principle score (Principled Development): 9.0/10
- Documentation completeness: 85% (physics pattern undocumented)

**After (estimated):**
- LOC controller.h: ~120 (+5, comments)
- LOC controller.cpp: ~145 (+6, integration method docs)
- LOC PRINCIPLES.md: ~230 (+30, accumulated state section)
- Principle score (Principled Development): 10.0/10 (+1.0)
- Documentation completeness: 100% (physics pattern now documented)

**Net change:**
- LOC total: +41 (documentation only)
- Code change: 0 (pure documentation)
- Behavioral change: 0 (clarification only)

**Layer 1 impact:**
- Physics adherence: 90% → 100% (+10%)
- Layer 1 certainty: 90% → 95% (+5%)
- Overall foundation: 92.5% → 95% (+2.5%)

---

## Risk Assessment

**What could go wrong:**
- Documentation could be unclear or misleading
- Might encourage inappropriate use of accumulated state elsewhere

**Mitigation:**
- Be explicit about when accumulation is acceptable (physics integration ONLY)
- Provide counter-examples (caching, memoization = bad accumulated state)
- Link to mathematical foundations (Euler integration)

**Rollback plan:**
- Revert documentation commits
- No code changes to rollback

---

## Documentation Plan

### 1. Update controller.h

Add comments to position/velocity members:

```cpp
// State (source of truth)
// NOTE: Position and velocity are ACCUMULATED STATE (integrated over time).
// This is the CORRECT pattern for physics simulation - see PRINCIPLES.md.
// Exception: Physics integration requires accumulation; this is not a violation.
glm::vec3 position;  // meters
glm::vec3 velocity;  // meters/second
glm::vec3 acceleration;  // meters/second²
```

### 2. Update controller.cpp

Add integration method documentation:

```cpp
void controller::update(const collision_world* world, float dt) {
    // Physics integration using semi-implicit Euler method:
    //
    // Semi-implicit Euler (Symplectic Euler):
    //   v(t+dt) = v(t) + a(t)·dt     (velocity first)
    //   x(t+dt) = x(t) + v(t+dt)·dt  (then position using NEW velocity)
    //
    // Properties:
    //   - Stable for damped systems (friction, collision)
    //   - Conserves energy better than explicit Euler
    //   - Sufficient for platformer physics (no rigid body dynamics)
    //
    // Alternatives considered:
    //   - Verlet: Better energy conservation, but requires refactor
    //   - RK4: Overkill for platformer (4x cost for minimal gain)
    //
    // Trade-off: Euler is fast, stable, and sufficient for our use case.

    // Apply gravity
    acceleration.y += gravity;

    // Integrate velocity (accumulate - required for physics)
    velocity += acceleration * dt;

    // ... rest of update ...
}
```

### 3. Update PRINCIPLES.md

Add section on accumulated state:

```markdown
## The Patterns of Truth

### Pure Functions Over Accumulated State

**General rule:** Derive state from inputs rather than accumulating it over time.

**Why:** Accumulated state drifts from truth. Calculated state remains correct.

**Exception: Physics Integration**

Physics simulation REQUIRES accumulated state for position and velocity. This is not a violation - this is the correct implementation.

**Acceptable accumulated state:**
- Position integrated from velocity (physics)
- Velocity integrated from acceleration (physics)
- Time-based integration (numerical methods)

**Unacceptable accumulated state:**
- Cached values that can be derived
- Memoized results that could be recalculated
- Running totals that could be computed from source data

**The distinction:**

Integration over time (physics):
```
velocity += acceleration * dt  // CORRECT - physics requires this
position += velocity * dt      // CORRECT - integration over time
```

Accumulated cache (anti-pattern):
```
cached_value += delta  // WRONG - derive from source instead
total += increment     // WRONG - compute from collection
```

**When in doubt:** Can you derive this from source data? If yes, derive it. If no (physics integration), accumulate it.

**See:** `character/controller.{h,cpp}` for correct use of accumulated state in physics.
```

---

## Complexity Justification

**Why this is Trivial:**

1. **No code changes** - pure documentation
2. **No tests to update** - behavior unchanged
3. **Clear fix path** - add comments and docs
4. **Low risk** - can't break anything
5. **<1 hour** - quick documentation task

**Can skip PLAN step, go directly to REFINE.**

---

## Philosophy

**This refinement clarifies an apparent contradiction:**

PRINCIPLES.md says "Pure Functions Over Accumulated State" but controller accumulates position/velocity. This seems like a violation.

**The truth:** Physics integration is the *exception*, not the violation.

**The problem:** The exception was not documented.

**The fix:** Document the exception, explain the trade-offs, provide guidance.

**The result:** Principle adherence improved by making the exception explicit. Not all accumulated state is bad - physics integration REQUIRES it. The principle should guide toward derived state *except when integration demands accumulation*.

---

**Document the truth. Explain the trade-offs. Make the exceptions explicit.**
