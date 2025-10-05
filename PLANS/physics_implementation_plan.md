# Physics and Character Implementation Plan

## Status (2025-10-05)

The current implementation provides a solid foundation that is well-aligned with the project's core principles of creating a responsive, physics-driven character controller with a procedural animation layer.

**Verified Components:**
- **Core Collision:** Simple sphere-AABB collision system (`foundation/collision.cpp`)
- **Spring-Damper Primitive:** Reusable, critically-damped spring system (`foundation/spring_damper.cpp`)
- **Procedural Locomotion:** "Surveyor wheel" system with cubic interpolation for walk/run cycles (`character/locomotion.cpp`)
- **Procedural Orientation:** Smooth rotation to face velocity direction (`character/orientation.cpp`)

## Next Steps

Prioritize core procedural animation techniques from design documents. Complete foundational layer before advanced features.

### 1. Implement Acceleration Tilt

**Priority:** High  
**Rationale:** Simple, high-impact reactive animation. Core "Do No Harm" technique—interprets physics state without affecting it.

**Approach:**
1. Create new `character/animation` system for reactive animation layer
2. Calculate tilt angles from controller's horizontal acceleration vector
3. Apply exponential smoothing (defer spring-damper until needed)
4. Generate rotation matrix applied to character transform during rendering

**Notes:**
- Pure visual layer—never affects controller physics
- Currently renders as sphere; tilt verifiable via debug draw until mesh/capsule added
- See `PLANS/acceleration_tilt_implementation.md` for detailed implementation

### 2. Integrate Spring-Damper for Landings

**Priority:** High  
**Rationale:** Primary use case for existing `spring_damper` primitive. Eliminates landing keyframes via procedural simulation.

**Approach:**
1. Detect airborne→grounded transitions in controller
2. Apply downward impulse to vertical spring proportional to fall velocity
3. Spring motion drives vertical offset or crouch blend factor
4. Result: automatic "crouch and recover" animation

**Prerequisite:** Requires visible character body (not just sphere) to show crouch effect. Consider implementing after basic capsule/mesh rendering.

### 3. Implement Basic Inverse Kinematics (IK) for Foot Placement

**Priority:** Medium (defer until terrain complexity demands it)  
**Rationale:** Solves foot clipping/floating on uneven terrain. Currently ground is flat—revisit when adding terrain variation.

**Approach:**
1. Simple two-bone IK solver in `foundation` (basic trigonometry)
2. Raycast from foot positions after locomotion calculated
3. IK adjusts joints for ground contact
4. Smooth IK transitions to avoid snapping

**Prerequisite:** Requires skeletal system with foot/knee/hip joints. Current locomotion uses simple pose offsets.

### 4. Future Considerations (Design Backlog)

- **Active Ragdolls:** Extend spring-damper for dynamic death/impact reactions
- **"Solving for Stupid":** Profile visual quality, fix worst artifacts procedurally (wall slide → wall run)
- **Secondary Motion:** Bone "softness" parameters for wobble/follow-through
- **Speed-Based Animation:** Tilt magnitude scales with speed (like surveyor wheel bounce)
