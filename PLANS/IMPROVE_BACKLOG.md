# Improve Backlog

**A record of principle violations awaiting simplification.**

---

## The Rule

Only track violations that:
1. Break one of the six principles
2. Can be fixed by removal or simplification
3. Have been verified through audit
4. Make the codebase simpler when fixed

---

## Critical Violations (From Audit 2025-10-16)

### Missing Debug Visualization
**Principle Violated:** Mathematical Foundations (can't validate what can't see)
**Current:** No visual debugging for physics state, orientation, forces
**Problem:** This caused the coordinate confusion crisis
**Solution:** Add RGB axes, velocity vectors, collision spheres
**Complexity:** 3-4 points
**Why Critical:** Cannot verify any other fixes without seeing them

### Accumulated State Pattern
**Principle Violated:** Mathematical Foundations
**Current:** `velocity += acceleration * dt; position += velocity * dt;`
**Problem:** Errors compound over time, drift inevitable
**Solution:** Document and accept OR implement frame-independent physics
**Complexity:** 2-3 points if documented, 5-6 if fixed
**Why Critical:** Mathematical incorrectness compounds

---

## Principle Violations

### Dual-Reference Violation
**Principle Violated:** Consistency
**File:** `src/character/orientation.h`
**Current:** `current_yaw` references itself during smoothing
**Solution:** Implement proper dual-reference pattern
**Complexity:** 1-2 points

### Mixed Responsibilities
**Principle Violated:** Composable Functions
**File:** `src/character/controller.cpp`
**Current:** Controller manages physics + animation state
**Solution:** Move animation concerns to animation system
**Complexity:** 3-4 points

### Magic Numbers
**Principle Violated:** Mathematical Foundations
**Files:** Throughout codebase
**Current:** Unexplained constants (BUMPER_RADIUS = 0.5, epsilon = 0.0001)
**Solution:** Document derivation or remove if arbitrary
**Complexity:** 1 point

---

## Suspicious Patterns (Need Investigation)

### Friction Formula
**File:** `src/character/controller.cpp:89-96`
**Suspicion:** `friction * abs(gravity)` - why multiply by gravity?
**Action:** Derive from physics or simplify

### Yaw to Right Vector
**File:** `src/foundation/math_utils.h:26`
**Suspicion:** Negative X component for right vector
**Action:** Verify mathematically with debug visualization

---

## Deferred (Foundation Not Ready)

These violations exist but cannot be fixed until foundation is stable:

- Rendering buffer inefficiency (wait for stable render pipeline)
- Hardcoded test level (premature to extract)
- Mouse input location (works, don't touch)

---

## Recently Removed (Learn From)

**2025-10-16 Radical Simplification removed:**
- Acceleration tilt system (coordinate confusion)
- Contact weight springs (unnecessary complexity)
- Locomotion system (over-engineered)
- Walk/run blending (unnecessary states)

**Lesson:** Complexity without mathematical validation always fails.

---

## Selection Criteria

Before pulling from this backlog:

1. **Is foundation ≥85% certain?** (Check DEPENDENCY_STACK.md)
2. **Will fixing make things simpler?** (Not just different)
3. **Can we test the fix in isolation?**
4. **Does it serve immediate gameplay?**

If any answer is no, do not select.

---

## The Discipline

This backlog tracks violations, not improvements.
Every item here represents complexity to remove, not features to add.
The best fix is deletion.
The second best is simplification.
Adding is last resort.

**Update when:**
- Audit finds new violations
- Violations are fixed (remove them)
- Foundation certainty changes

**Never add:**
- Feature requests
- "Nice to haves"
- Performance optimizations without proof
- Anything that adds complexity

---

## Current Status

**Foundation Certainty:** 85% (Repair Mode)
**Action:** Fix critical violations before anything else
**Next:** Debug visualization (cannot verify other fixes without it)

**This is the way.**