# Angle Arc Primitive System

**Layer 2: Primitive** - Generic angle visualization infrastructure

---

<!-- BEGIN: SELECT/REFERENCES -->
## Internal References

**Existing Primitives:**
- `src/foundation/procedural_mesh.{h,cpp}` - Wireframe mesh generation pattern
  - `generate_circle()` - Full 360° ring (radius, segments)
  - `generate_arrow()` - Directional vector visualization
  - `wireframe_mesh` - Vertices, edges, position, rotation, scale
- `src/app/debug_generation.cpp:54-78` - Speed gradient ring visualization
  - Uses `generate_circle()` with speed-based color interpolation
  - Pattern: procedural mesh → `mesh_to_debug_lines()` → debug primitives
- `src/foundation/math_utils.h` - Vector math utilities
  - `calculate_slip_angle()` - Angle between velocity and heading (lines 80-118)
  - `project_to_horizontal()` - XZ plane projection (line 24)
  - `yaw_to_forward()`, `yaw_to_right()` - Direction vectors from yaw (lines 29-38)
  - `UP` constant - World Y-up axis (line 20)

**Integration Points:**
- `src/vehicle/controller.h:115-120` - `calculate_slip_angle()` exists, returns radians
- `src/vehicle/controller.h:88-90` - `heading_yaw` (car-like control heading)
- `src/vehicle/controller.cpp` - Velocity, heading, angular velocity state
- `src/app/debug_generation.cpp` - Consumer of debug primitives

**Coordinate System (CONVENTIONS.md):**
- World: +Y up, +Z forward, +X right (right-handed, OpenGL)
- Horizontal plane: XZ (Y=0)
- Rotations: heading-relative (not world-absolute)

## External References

**3D Angle Arc Visualization Research:**

From web search "3D debug drawing angle between vectors arc visualization techniques":
- **Coordinate frame method**: Create orthonormal basis {X, Y, Z} where:
  - X = unit vector in first direction
  - Z = unit vector perpendicular to arc plane (X × second direction)
  - Y = Z × X (completes right-handed frame)
  - Arc parameterization: P(θ) = origin + r·cos(θ)·X + r·sin(θ)·Y
- **Segment-based smoothness**: More segments = smoother arc appearance
- **Local 2D projection**: Generate arc in plane containing both vectors

From web search "racing game debug visualization steering angle drift angle display":
- **Steering angle display**: Common in sim racing telemetry (degrees from neutral)
- **Slip angle visualization**: Shows angle between rolling direction and movement direction
- **Counter-steering detection**: Visualize input vs required correction angle
- **Peak lateral force**: Occurs at slip angles 5°-8° (visualization can highlight thresholds)
- **Telemetry overlays**: Steering input graphs plot angle over time/distance

From web search "game physics debug angular velocity rotation visualization":
- **Angular velocity**: Vector direction = axis, magnitude = rad/s
- **Rotation cones**: Field of possible steering/drift angles
- **Debug drawing patterns**: Arc primitives fundamental to physics debugging

**Key Insight:** Arc is reusable primitive, not single-purpose visualization. Racing games use angle arcs for steering input, slip angle, counter-steer detection, FOV cones, and turn authority display.

<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/SELECTED -->
## System Decision

**Selected:** Angle Arc Primitive (Layer 2)

**Why Layer 2:** Pure geometric primitive. No state, no side effects. Generates wireframe mesh from mathematical inputs. Composes with existing debug visualization infrastructure.

**Foundational Value:** Enables 4-5 immediate visualizations from single primitive:
1. **Slip angle arc** - velocity vs heading (drift detection)
2. **Steering angle arc** - current vs max turn authority (speed-dependent limits)
3. **Counter-steering arc** - input direction vs slip correction
4. **Angular velocity cone** - field of possible rotation
5. **Future: Front/rear axle slip angles** - when tire physics arrive

## Dependencies

**Required (all exist):**
- Layer 1: Debug visualization pipeline (`debug_primitives.h`, `debug_generation.cpp`)
- Layer 2: `procedural_mesh` patterns (`generate_circle`, `generate_arrow`)
- Layer 2: Vector math (`glm`, cross product, normalization)
- Layer 2: `math::UP` constant for horizontal plane projection

**Consumers Ready:**
- `controller::calculate_slip_angle()` exists (line 115, `controller.h`)
- `debug_generation.cpp` consumes wireframe meshes via `mesh_to_debug_lines()`

**No Blockers:** All dependencies satisfied.

## Core Mechanics

**Input:**
- Origin point (world position)
- Start direction (unit vector, horizontal)
- End direction (unit vector, horizontal)
- Radius (visualization scale)
- Segments (smoothness control)

**Output:**
- `wireframe_mesh` with arc geometry in horizontal (XZ) plane

**Algorithm:**
1. Validate inputs (unit vectors, horizontal, non-zero angle)
2. Compute signed angle between directions (atan2 with cross product)
3. Construct local orthonormal frame {X=start, Y=perpendicular, Z=UP}
4. Generate arc vertices: P(θ) = origin + r·cos(θ)·X + r·sin(θ)·Y, θ ∈ [0, angle]
5. Connect vertices with edges
6. Return `wireframe_mesh`

## Mathematical Foundation

**Orthonormal Frame Construction:**
```
X = normalize(start_direction)          // First basis vector
Z = UP                                   // Plane normal (horizontal arcs)
Y = normalize(cross(Z, X))              // Perpendicular in-plane direction
```

**Angle Calculation (signed):**
```
dot_product = dot(start, end)
cross_product = cross(start, end)
angle = atan2(dot(cross_product, UP), dot_product)  // Signed angle [-π, π]
```

**Arc Parameterization:**
```
for θ in [0, angle] stepped by (angle / segments):
    vertex = origin + radius * (cos(θ) * X + sin(θ) * Y)
```

**Time-Independence:** Pure function, no time dependence. Identical output for identical inputs.

**Edge Cases:**
- Zero angle: Return empty mesh (or radial line at start direction)
- 180° angle: Semicircle requires careful Y direction (cross product undefined)
- Negative angle: Arc sweeps opposite direction (clockwise vs counter-clockwise)

## Graybox Approach

**Simplest Possible Version:**

**Scope:**
- Single function: `generate_arc(origin, start_dir, end_dir, radius, segments)`
- Horizontal arcs only (Y=0 vertices, UP normal assumed)
- Fixed segment count (32 default, matches `generate_circle`)
- No color parameter (consumer applies color via debug primitives)

**First Consumer (validates primitive works):**
- Slip angle visualization in `debug_generation.cpp`
- Arc from heading direction to velocity direction
- Radius matches speed ring radius (visual consistency)
- Color: white or gradient (speed-dependent)

**Success Criteria:**
- Arc renders correctly for positive/negative slip angles
- Arc direction matches heading→velocity rotation
- Arc integrates with existing debug visualization (no flicker, correct depth)
- Pattern reusable for other angle visualizations

**Explicitly Deferred:**
- 3D arcs (arbitrary plane normals) - horizontal only for graybox
- Dynamic segment count tuning - use circle default
- Arc styling (dashed, double-line, thickness) - debug primitives handle rendering
- Multiple arc layers (inner/outer radii) - single radius sufficient

**Validation:**
- Visual inspection: arc connects heading to velocity arrows
- Sign test: right turn (negative angular velocity) shows clockwise arc
- Magnitude test: 90° slip angle produces quarter-circle
- Integration test: works alongside speed ring, orientation arrow, g-force arrow

<!-- END: SELECT/SELECTED -->

---

## Next Step

**GRAYBOX** - Build simplest version:
1. Add `generate_arc()` to `procedural_mesh.{h,cpp}`
2. Implement orthonormal frame, angle calculation, vertex generation
3. Add slip angle arc to `debug_generation.cpp` (first consumer)
4. Validate: visual inspection, sign/magnitude tests, integration

**Success = Reusable primitive enabling multiple visualizations.**

---

**Build systems, not features. One primitive, many uses.**
