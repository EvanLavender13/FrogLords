# Current Plan

**Status:** 🏗️ System - Angle Arc Primitive
**Branch:** system/angle-arc-primitive

---

## Current Work

**System:** Generic angle visualization primitive (Layer 2)
**Location:** @TASKS/PLANS/angle_arc_primitive_SYSTEM.md
**Next Task:** GRAYBOX

**Why This Matters:**
Reusable geometric primitive enabling 4-5 immediate visualizations:
- Slip angle (velocity vs heading)
- Steering angle (current vs max)
- Counter-steering detection
- Angular velocity cones
- Future: Front/rear axle slip angles

**Graybox Scope:**
- `generate_arc()` function in `procedural_mesh.{h,cpp}`
- Horizontal arcs only (XZ plane)
- First consumer: slip angle visualization in `debug_generation.cpp`

---

**Systems, not features. Build once, use everywhere.**
