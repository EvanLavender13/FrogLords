# Current Plan

**Status:** 🏗️ System - Angle Arc Primitive
**Branch:** system/angle-arc-primitive

---

## Current Work

**System:** Generic angle visualization primitive (Layer 2)
**Location:** @TASKS/PLANS/angle_arc_primitive_SYSTEM.md
**Next Task:** ITERATE

**Why This Matters:**
Reusable geometric primitive enabling 4-5 immediate visualizations:
- Slip angle (velocity vs heading) ✅ Working
- Steering angle (current vs max)
- Counter-steering detection
- Angular velocity cones
- Future: Front/rear axle slip angles

**Graybox Complete:**
- `generate_arc()` primitive in `procedural_mesh.{h,cpp}` ✅
- Slip angle visualization working at half-radius ✅
- Debug assertions validating inputs ✅

**Iterate Refinements:**
- World-space debug text showing angle degrees
- Arc styling (gradient colors, severity indication)
- Additional consumers (steering authority, counter-steer)

---

**Systems, not features. Build once, use everywhere.**
