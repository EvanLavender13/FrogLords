# Vehicle Dynamics Terminology

Purpose: Precise definitions for vehicle physics terms where intuition can mislead.

## Steering vs Sliding

**Steering direction:** Which way the wheels/heading rotate
- "Turn left" = wheels/heading rotate counter-clockwise (viewed from above)
- "Turn right" = wheels/heading rotate clockwise

**Slide/slip direction:** Which way the vehicle moves relative to heading
- Vehicle momentum lags behind steering changes
- When turning left, vehicle slides RIGHT relative to new heading
- When turning right, vehicle slides LEFT relative to new heading

**Critical:** Steering left causes right slip. This is not a contradiction—it's physics.

## Slip Angle

**Definition:** Angle between velocity vector and forward direction

**Sign convention (this codebase):**
- Positive slip angle = velocity points right of forward direction
- Negative slip angle = velocity points left of forward direction
- Zero = moving straight ahead (velocity parallel to forward)

**Physics:**
- Turn left → heading rotates left → velocity lags right of heading → positive slip
- Turn right → heading rotates right → velocity lags left of heading → negative slip

## Drift vs Slide vs Slip

**Slip:** Any deviation between velocity and heading (measured by slip angle)

**Slide:** Uncontrolled lateral movement, usually unwanted

**Drift:** Controlled slide maintained intentionally for cornering
- Initiated when slip angle exceeds threshold (typically 10-20°)
- Maintained through throttle and counter-steering
- Physics: reduced rear friction, maintained front grip

## Directional Components

**Longitudinal:** Along the vehicle's forward/backward axis
- Forward velocity, acceleration, braking forces

**Lateral:** Perpendicular to forward axis (side-to-side)
- Lateral velocity = velocity component perpendicular to heading
- Lateral acceleration = centripetal force during turning
- Lateral G-forces = lateral acceleration / gravity

## Coordinate Frame

**Vehicle-local:** Relative to vehicle orientation
- Forward = vehicle's front direction
- Right = vehicle's right side
- Used for: slip calculations, force application

**World-space:** Global coordinate system
- Fixed axes regardless of vehicle orientation
- This codebase: +X right, +Y up, +Z forward (see CONVENTIONS.md)
- Used for: position, velocity integration

## Common Misunderstandings

**"Turning causes sliding in same direction"** - Wrong. Turning left causes right slide due to momentum.

**"Positive slip = left slide"** - Wrong. Positive = velocity right of heading.

**"Drift is just extreme sliding"** - Partially true. Drift is controlled via friction modification and counter-steering.

**"Slip angle measures steering angle"** - Wrong. Slip angle measures velocity vs heading, not wheel angle.