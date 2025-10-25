# System: Dynamic FOV

Reactive system that modulates camera field of view based on vehicle speed and lateral g-force to enhance speed sensation.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-25
**Layer:** 4 (VARIATION)
**Complexity:** Small
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/INTERNAL_REFERENCES -->
## Internal References

**Research notes:**
- NOTES/CarMechanics/PhysicsFirst_Research.md:20-26 (Dynamic FOV formula, ranges, exponential curves)

**Source files:**
- src/camera/camera.h:50 (fov_degrees storage)
- src/camera/camera.cpp:9 (FOV used in projection matrix)
- src/foundation/math_utils.h:126-145 (calculate_lateral_g_force primitive)
- src/vehicle/controller.h:62 (max_speed parameter)
- src/gui/camera_panel.cpp:36 (FOV currently displayed read-only)

**Existing patterns:**
- Reactive systems: vehicle_visual_systems.{h,cpp} reads controller state, applies visual transforms
- Unidirectional flow: GUI emits commands, systems apply
- Metadata-driven tuning: parameter_meta pattern for exposing tunables

**Integration points:**
- Reads: controller (speed, lateral g-force)
- Writes: camera (fov_degrees)
- Display: camera_panel (tuning parameters)
<!-- END: SELECT/INTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/EXTERNAL_REFERENCES -->
## External References

**Technical documentation:**
- Flatout 2: "Tunnel vision" trick - FOV adjusts dynamically with speed
- Need for Speed Heat: Dynamic FOV + motion blur + camera shake combination
- Industry standard: 30° low speed → 100° high speed (linear mapping common)
- Research finding: Default FOV causes speed underestimation (perceived = 0.5x actual)

**Research findings:**
- Most impactful speed sensation technique (more than motion blur or particles)
- Exponential curves feel more natural than linear
- G-force multipliers: 0.5-2.0 typical range
- Base FOV: 75-90° arcade feel vs 40-60° sim racing
- Max FOV: 100-110° at top speed
- Peripheral vision critical for speed perception

**Constraints/Requirements:**
- Pure reactive system - no state accumulation
- Reads physics state, writes camera parameter
- Must not be embedded in camera or vehicle controller
- Should support smooth interpolation (spring-damper pattern)
<!-- END: SELECT/EXTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- Lateral G-Force Calculator (Layer 2, exists in src/foundation/math_utils.h:126-145)
- Camera System (Layer 1, exists in src/camera/camera.{h,cpp})
- Vehicle Controller (Layer 3, exists in src/vehicle/controller.{h,cpp})
- Parameter Metadata (Layer 2, exists in src/foundation/param_meta.h)

**Enables (will become buildable):**
- Camera Shake on Boost (Layer 4, visual polish)
- Lower Camera Positioning (Layer 4, visual polish)
- Dynamic motion blur systems (Layer 5)
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Reactive system that reads speed and lateral g-force from vehicle controller, calculates target FOV using formula `FOV = base + (speed/max_speed) * range + lateral_g * g_multiplier`, and writes result to camera.

**Why a system, not a feature:**
Enables category of speed-reactive camera behaviors. Foundation for camera shake, dynamic positioning, motion blur. Demonstrates pattern: reactive visual systems read physics state, write presentation parameters. Separates perception from physics.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- FOV calculation produces values within valid range [min_fov, max_fov]
- Spring-damper convergence to target FOV (if smoothing applied)
- Linear vs exponential speed mapping correctness

**Known uncertainties:**
- Optimal g-force multiplier value (research suggests 0.5-2.0, needs playtesting)
- Linear vs exponential speed curve preference
- Smoothing necessary or does instant response feel better?
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
- New file: `src/camera/dynamic_fov.{h,cpp}`
- Struct with tunable parameters (base_fov, max_fov_range, g_multiplier)
- Pure function: `calculate_fov(speed, max_speed, lateral_g, params) -> float`
- Update function: reads controller, writes camera
- No smoothing initially (validate instant response first)
- Metadata for all tunables
- GUI panel integration (camera_panel or new fov_panel)

**Validation:**
- Debug text overlay showing: current speed, lateral_g, calculated FOV, actual camera FOV
- Assert FOV always within valid range [base, base+range]
- Test at: zero speed, max speed, high g-force corners, combined conditions
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Core functional in graybox
- [ ] Reactive system reads controller state (speed, lateral_g, max_speed)
- [ ] Calculates target FOV using documented formula
- [ ] Writes to camera.fov_degrees
- [ ] Debug visualization shows all inputs and output
- [ ] Tunable parameters exposed via metadata
- [ ] FOV clamped to valid range at all times
- [ ] Speed sensation noticeably enhanced during playtesting
- [ ] System architecture: standalone, reactive, no state accumulation
- [ ] Principles upheld: Single source of truth (physics state), derive don't accumulate (FOV calculated per-frame), systems not features (pattern for reactive camera behaviors)
<!-- END: SELECT/SUCCESS -->
