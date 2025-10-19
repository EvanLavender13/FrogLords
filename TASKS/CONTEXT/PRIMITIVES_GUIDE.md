# Primitives: The Boundary Between Truth and Interpretation

## The Nature of Primitives

A primitive is a mathematical truth expressed in code.

It knows geometry, forces, and mathematics.
It does not know gameplay, intent, or meaning.

The line between primitive and system is the line between **what happened** and **what it means**.

## The Dual Reference Pattern

**Primitives return rich data.**
**Systems interpret that data.**

A collision primitive reports: contact occurred, normal direction, penetration depth, contact location.

It does not report: "player is grounded," "surface is walkable," "landing happened."

Those are interpretations. Interpretations belong to systems, not primitives.

## The Separation

**Primitives operate in the domain of physics:**
- Positions, velocities, forces
- Normals, penetrations, contacts
- Distances, angles, magnitudes
- Mathematical correctness

**Systems operate in the domain of gameplay:**
- Grounded state, slope limits, jump buffering
- Movement modes, state transitions
- Intent, behavior, response
- Game rules and logic

**The boundary is sacred.**

When a primitive knows about "grounded," it has crossed into gameplay.
When a system performs collision math, it has descended into physics.

Both are violations of orthogonality.

## The Coupling Test

Ask: "Can I reuse this primitive for a different gameplay system?"

**If the answer is no, gameplay has leaked into the primitive.**

A collision resolver that sets "is_grounded" cannot serve:
- Wall-running (different grounded logic)
- Magnetic boots (inverted gravity)
- Flying creatures (no grounded concept)

A collision resolver that returns contact normals can serve all of them.

**Primitives are reusable. Systems are specific.**

## The Direction of Knowledge

**Allowed:** Systems know about primitives (they call them, interpret their results)
**Forbidden:** Primitives know about systems (gameplay concepts, state interpretation)

The dependency arrow points up the stack:
```
System (gameplay logic)
  ↓ calls
Primitive (mathematical truth)
```

Never reverse this arrow. Primitives that reach up into gameplay become non-reusable.

## The Parameter Test

Look at function signatures.

**Primitive parameters:**
- Geometry (positions, radii, extents)
- Physics state (velocities, forces)
- Mathematical constants (epsilon, damping)

**System parameters:**
- Gameplay state (is_grounded, can_jump)
- Game rules (max_slope_angle, coyote_time)
- Interpretation flags (out-parameters for state)

**If a primitive accepts gameplay parameters, the boundary is violated.**

`max_slope_angle` is a game rule. It interprets normals to determine "walkable."
That interpretation belongs in systems, not primitives.

The primitive returns the normal. The system compares it to `max_slope_angle`.

## The Return Value Principle

**Primitives return data, not decisions.**

Return: contact normal (data)
Don't return: is_grounded (decision)

Return: distance to surface (data)
Don't return: is_too_far_to_interact (decision)

Return: velocity after collision (data)
Don't return: should_play_impact_sound (decision)

Data is objective. Decisions are interpretations. Primitives deal in objectivity.

## The Warning Signs

Primitives violate the boundary when they:

- Accept game rule parameters (`max_slope_angle`, `jump_buffer_time`)
- Set gameplay state via out-parameters (`is_grounded&`, `can_dash&`)
- Make decisions about game behavior ("if slope > limit, slide")
- Know domain-specific terminology (grounded, dashing, falling)

When you see these, interpretation has leaked into truth.

## The Correction

**Move interpretation up the stack.**

Extract gameplay logic from primitives.
Return richer data structures instead.
Let systems interpret that data using their own rules.

**Before (coupled):**
```
resolve_collision(position, velocity, is_grounded&, max_slope_angle)
  // Primitive sets is_grounded based on slope check
```

**After (orthogonal):**
```
contact = resolve_collision(position, velocity)
  // Returns: normal, penetration, contact_box

is_grounded = (contact.normal.y >= cos(max_slope_angle))
  // System interprets contact using game rules
```

The primitive knows geometry. The system knows gameplay. Each stays in its domain.

## The Benefit

**Primitives become mathematical utilities.**

Testable in isolation against mathematical correctness.
Reusable across different gameplay systems.
Composable into new behaviors without modification.

**Systems become game logic.**

Free to implement custom interpretations.
Can change rules without touching primitives.
Express gameplay intent clearly.

**The boundary enables both.**

## The Principle

**Primitives are truth. Systems are meaning.**

Keep truth pure. Let meaning live above it.

**When physics knows about gameplay, orthogonality dies.**
