# Physics-First Tuning: Parameters from Reality, Feel from Tuning

## The Backwards Derivation Anti-Pattern

**Wrong:** Start with desired behavior, reverse-engineer physics parameters.

**Right:** Start with physical parameters, derive behavior through tuning.

### The Problem

When you work backwards from desired feel to physics constants, you create:
- Non-physical parameters that obscure tuning (friction ratio instead of coefficient)
- Inflated values that compensate for backwards math (80 m/s² acceleration)
- Magic numbers with no real-world interpretation
- Systems that silently disagree when tuned

**Example violation (friction_model before fix):**
```cpp
// Desired: 75% friction, 25% net acceleration
constexpr float FRICTION_RATIO = 0.75f;
constexpr float NET_FRACTION = 0.25f;

// Backwards derivation - starts from ratio
ground_accel = desired_accel / NET_FRACTION;  // 20 / 0.25 = 80 m/s²
friction = (ground_accel * FRICTION_RATIO) / gravity_mag;  // μ ≈ 6.12 (!!)
```

**Result:** Friction coefficient of 6.12. Real-world max is ~1.0. No intuitive tuning knob.

### The Correct Approach

**Start with physical reality:**
```cpp
// Physical parameter with real-world meaning
friction_coefficient = 0.7f;  // Rubber on asphalt (dimensionless, range [0, 1])

// Apply standard physics
ground_accel = desired_accel;  // Direct assignment
friction = friction_coefficient;  // No inflation, no derivation
```

**Result:** μ = 0.7. Physically valid. Tunable from ice (0.0) to maximum grip (1.0).

## The Principle

**Physics drives feel. Feel does not drive physics.**

Parameters should have:
1. **Physical meaning** - Corresponds to real-world quantity
2. **Valid range** - Bounds match reality (friction ∈ [0, 1])
3. **Intuitive tuning** - Changing value has predictable effect
4. **Standard formulas** - Uses proven equations, not invented math

## Common Violations

### Ratio-Based Parameters

**Wrong:**
```cpp
float acceleration_ratio = 0.25f;  // 25% of what?
float effective_accel = base / acceleration_ratio;  // Backwards math
```

**Right:**
```cpp
float ground_acceleration = 20.0f;  // m/s² - direct physical quantity
```

### Self-Compensating Inflation

**Wrong:**
```cpp
// Inflate to compensate for friction we'll subtract later
float inflated_force = desired_force / (1.0f - friction_loss);
```

**Right:**
```cpp
// Apply forces independently, let physics combine them
float applied_force = input_force;
float friction_force = friction_coefficient * normal_force;
```

### Derived Constants Without Physical Grounding

**Wrong:**
```cpp
constexpr float MAGIC_DAMPING = 0.83f;  // "Feels good"
```

**Right:**
```cpp
// Critical damping ratio: 1.0 = critically damped
// <1.0 = underdamped (bouncy), >1.0 = overdamped (sluggish)
float damping_ratio = 0.7f;  // Slightly underdamped for snappy response
```

## The Method

### 1. Identify the Physical System

What are you modeling?
- Friction: Kinetic friction between surfaces
- Springs: Harmonic oscillator with damping
- Drag: Air/water resistance
- Gravity: Constant downward acceleration

### 2. Find the Standard Formula

Don't invent physics. Use proven equations.

**Friction:**
```
F_friction = μ * F_normal
a_friction = μ * |g|  (for horizontal ground)
```

**Spring-Damper:**
```
F_spring = -k * displacement - c * velocity
where c = 2 * sqrt(k * m) * damping_ratio
```

**Quadratic Drag:**
```
F_drag = -0.5 * ρ * C_d * A * v²
Simplified: F_drag = -drag_coefficient * v²
```

### 3. Expose Physical Parameters

Make tunable values correspond to real quantities.

**Friction system:**
- `friction_coefficient` (μ): [0.0, 1.0] dimensionless
- `gravity_magnitude`: 9.8 m/s² default

**Spring system:**
- `spring_constant` (k): N/m (stiffness)
- `damping_ratio`: dimensionless [0.0, 2.0]

**Drag system:**
- `drag_coefficient`: kg/m (simplified)
- Reference velocity for tuning

### 4. Derive Feel Through Tuning

Adjust physical parameters until behavior feels right.

**Friction too slippery?** Increase μ (0.3 → 0.7).
**Spring too bouncy?** Increase damping ratio (0.5 → 0.8).
**Drag too weak?** Increase drag coefficient.

**The parameters have meaning. Tuning has direction.**

## Examples from Codebase

### Friction (After Fix)

```cpp
// tuning.h - Physical parameter
float friction_coefficient = 0.7f;  // [0, 1] - rubber-like

// tuning.cpp - Direct assignment
c.friction = std::clamp(friction_coefficient, 0.0f, 1.0f);

// controller.cpp - Standard formula
friction_decel = friction * std::abs(gravity) * dt;  // μ * |g| * dt
```

**Tuning:** Decrease μ for ice (0.1), increase for grip (0.9).

### Spring-Damper (camera_follow)

```cpp
// camera_follow.h - Physical constants
float spring_constant = 200.0f;    // N/m - stiffness
float damping_ratio = 0.7f;        // Dimensionless - slightly underdamped

// camera_follow.cpp - Standard formula
float damping_coeff = 2.0f * std::sqrt(spring_constant) * damping_ratio;
// Uses semi-implicit integration (see PHYSICS_INTEGRATION_PATTERNS.md)
```

**Tuning:** Increase k for snappier (300), decrease for floaty (100). Adjust damping for oscillation control.

## When to Deviate

Sometimes game feel requires non-physical parameters. **Document the trade-off explicitly.**

**Example: Coyote time**
```cpp
// PRINCIPLE TRADE-OFF: Coyote time violates Consistency for Prime Directive.
// Allows jumping briefly after leaving ground to forgive near-miss inputs.
// Prime Directive (player intent) > Consistency (strict physics).
float coyote_time = 0.1f;  // seconds - grace period after leaving ledge
```

**Key:** The parameter (0.1s) has physical meaning even if the mechanic doesn't follow strict physics.

## The Test

Ask: "Can I explain this parameter to a physicist?"

**Good:**
- "Friction coefficient between rubber and asphalt: 0.7"
- "Spring constant for camera lag: 200 N/m"
- "Damping ratio: 0.7 (slightly underdamped)"

**Bad:**
- "Acceleration ratio: 0.25" (ratio of what to what?)
- "Friction loss factor: 0.75" (how does this relate to μ?)
- "Magic damping: 0.83" (why this value?)

**If you can't explain it in physical terms, you're tuning the wrong parameter.**

## The Rule

**Start physical. Stay physical. Tune within physical bounds.**

When gameplay demands deviation from physics:
1. Document the principle trade-off
2. Use parameters with physical units/meaning
3. Explain why physics was insufficient

**Physics is the foundation. Game feel is built on top, not instead of.**

---

**See also:** @TASKS/CONTEXT/PHYSICS_INTEGRATION_PATTERNS.md - How to integrate physics over time
