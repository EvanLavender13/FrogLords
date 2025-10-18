# Physics Integration Patterns

**Mathematical correctness in motion simulation.**

---

## Accumulated State in Physics

### The Exception to the Rule

PRINCIPLES.md states: "Pure Functions Over Accumulated State"

**Physics integration is the exception.** Position and velocity MUST be accumulated over time. This is not a violation—this is the correct implementation.

### Acceptable Accumulated State

**Physics integration requires accumulation:**
- Position integrated from velocity over time
- Velocity integrated from acceleration over time
- Any numerical integration of differential equations

**Why:** These values cannot be derived from source data alone. They represent the solution to differential equations integrated over time.

**Example (CORRECT):**
```cpp
// Physics integration - accumulation is required
velocity += acceleration * dt;  // Integrate acceleration → velocity
position += velocity * dt;      // Integrate velocity → position
```

### Unacceptable Accumulated State

**Caching and dual-reference violations:**
- Cached values that can be derived from source data
- Memoized results that could be recalculated
- Running totals that could be computed from collections
- Smoothed values that reference themselves (dual-reference)

**Example (WRONG):**
```cpp
// Caching - should derive from source instead
cached_value += delta;  // WRONG - derive from source

// Dual-reference - smoothed value references itself
smooth = smooth * 0.9f + target * 0.1f;  // WRONG - use spring-damper

// Running total - should compute from collection
total += increment;  // WRONG - sum the collection
```

### The Distinction

**Integration over time** (differential equations):
- Solves dx/dt = f(x, t) numerically
- Cannot be derived from source alone—requires history
- Accumulation is mathematically required
- **Pattern:** Physics simulation, motion, dynamics

**Accumulated cache** (memoization):
- Stores derived results for performance
- Can be recalculated from source data
- Accumulation is an optimization, not a requirement
- **Pattern:** Caching, totals, smoothing

**When in doubt:** Can you derive this from source data? If yes, derive it. If no (physics integration), accumulate it.

---

## Integration Methods

### Semi-Implicit Euler (Current)

**Method:**
```cpp
// Semi-implicit Euler (Symplectic Euler):
velocity += acceleration * dt;     // Integrate velocity FIRST
position += velocity * dt;          // Then position using NEW velocity
```

**Properties:**
- Stable for damped systems (friction, collision)
- Conserves energy better than explicit Euler
- First-order accurate (error ~O(dt))
- Fast and simple
- Sufficient for platformer physics

**When to use:** Damped systems, platformer movement, character controllers

**Trade-offs:**
- Energy drift for undamped systems (not a concern with friction)
- Less accurate than higher-order methods
- Good enough for games (not engineering simulation)

### Alternatives Considered

**Velocity Verlet:**
- Better energy conservation for undamped systems
- Second-order accurate
- Requires refactor (position-velocity-position pattern)
- **Decision:** Overkill for platformer with friction

**RK4 (Runge-Kutta 4th order):**
- High accuracy (error ~O(dt⁴))
- 4× computational cost
- Used in engineering simulations
- **Decision:** Overkill for platformer physics

**Explicit Euler:**
- Simplest method: velocity = v + a*dt; position = p + v*dt
- Uses OLD velocity for position (unstable)
- Energy gain over time
- **Decision:** Inferior to semi-implicit, same cost

### Why Semi-Implicit Euler Wins

For platformer physics with friction and collision:
- Stability matters more than energy conservation
- First-order accuracy is sufficient
- Speed and simplicity matter
- Higher-order methods provide minimal benefit

**See:** `src/character/controller.cpp` for implementation

---

## Frame-Rate Independence

### Units and Scaling

All physics rates must be multiplied by `dt` (delta time):

```cpp
// Correct - frame-rate independent
velocity += acceleration * dt;  // m/s² × s = m/s
position += velocity * dt;      // m/s × s = m

// Wrong - frame-dependent
velocity += acceleration;  // Would move faster at higher FPS
position += velocity;      // Would move faster at higher FPS
```

**Unit analysis:**
- Acceleration: m/s²
- Velocity: m/s
- Position: m
- Time step (dt): s

**Verification:** Dimensional analysis must be consistent.

### Tuning Constants

Constants must be in correct units:

```cpp
// Correct
float gravity = -9.8f;           // m/s² (acceleration)
float friction = 0.9f;           // dimensionless (multiplier)
float jump_velocity = 5.0f;      // m/s (velocity)
float max_speed = 8.0f;          // m/s (velocity)
float ground_accel = 80.0f;      // m/s² (acceleration)

// Apply with dt
acceleration.y += gravity;        // m/s²
velocity += acceleration * dt;    // (m/s²) × s = m/s
position += velocity * dt;        // (m/s) × s = m
```

**Key insight:** Accelerations and velocities are rates. They must be multiplied by time to produce changes.

---

## Drift and Stability

### Sources of Drift

**Numerical integration accumulates error:**
- Euler methods have O(dt) error per step
- Error compounds over time
- Can cause position/velocity drift

**Mitigation in platformer:**
- Friction continuously damps velocity
- Collision resolution resets to contact
- Grounded state provides reference frame
- Accumulated error is bounded by game mechanics

### Stability Analysis

**Semi-implicit Euler is stable when:**
- System is damped (friction, air resistance)
- Forces are bounded (gravity, input acceleration)
- Collisions reset state (ground contact)

**Our system has all three:**
- Friction: `velocity *= friction` (exponential decay)
- Bounded forces: Gravity constant, input clamped
- Collision: Resets to surface contact

**Result:** Drift is negligible for platformer physics.

### When to Worry About Drift

**High-precision simulations:**
- Space flight (long time scales)
- Orbital mechanics (energy conservation critical)
- Engineering (accuracy requirements)

**Platformer physics:**
- Short time scales (seconds, not hours)
- Constant resets (landing, collisions)
- Bounded states (friction, max speed)

**Verdict:** Semi-implicit Euler is stable and sufficient.

---

## Pattern Summary

**Accumulated state in physics:**
- ✅ Position/velocity integration (required)
- ✅ Differential equation solutions (required)
- ❌ Cached derived values (use derivation)
- ❌ Dual-reference smoothing (use spring-damper)

**Integration method:**
- ✅ Semi-implicit Euler (fast, stable, sufficient)
- ❌ Verlet (overkill for damped systems)
- ❌ RK4 (overkill for platformer)

**Frame independence:**
- ✅ All rates multiplied by dt
- ✅ Units consistent (m, m/s, m/s²)
- ✅ Dimensional analysis verified

**Stability:**
- ✅ Damped system (friction)
- ✅ Bounded forces (gravity, input)
- ✅ State resets (collision, grounded)

---

## References

**Implementation:** `src/character/controller.{h,cpp}`
**Theory:** PRINCIPLES.md - "Pure Functions Over Accumulated State"
**Testing:** `tests/foundation/test_spring_damper.cpp` (second-order systems)

---

**Accumulate physics. Derive everything else.**
