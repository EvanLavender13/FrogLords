# Refinement: Coordinate System Convention Switch

Switch from non-standard -X right to industry-standard +X right convention.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-24
**Complexity:** Standard (5 pts)
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**System:** Foundation (Layer 2)
**Principle:** Mathematical Foundation, Systems Not Features
**Severity:** High
**Type:** Non-standard convention causing integration friction

**Current state:**
Project uses -X as right direction. `math::yaw_to_right()` returns `(-cos(yaw), 0, sin(yaw))`, and cross products use `cross(forward, UP)` to generate right vectors pointing in -X direction.

**Why violation:**
- Industry standard is +X right (OpenGL, Unity, most game engines)
- Non-standard convention emerged organically without explicit decision
- Creates confusion during code reviews and external validation
- Complicates integration with external math/physics libraries
- Violates **Mathematical Foundation**: conventions should follow established standards
- Violates **Systems Not Features**: coordinate system is foundational infrastructure

**Impact:**
- Blocks: Clean integration with standard physics references
- Cascades to: All directional calculations, slip angle signs, debug visualizations
- Urgency: Do now while codebase is small—exponentially harder as gameplay code grows
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**From:** Non-standard -X right convention
**To:** Industry-standard +X right convention

**Changes:**

1. `math_utils.h:yaw_to_right()` - Update formula from `(-cos(yaw), 0, sin(yaw))` to `(cos(yaw), 0, -sin(yaw))`
2. `math_utils.h:calculate_slip_angle()` - Reverse cross product from `cross(forward, UP)` to `cross(UP, forward)`
3. `camera.cpp:35` - Reverse cross product from `cross(forward, UP)` to `cross(UP, forward)`
4. `vehicle/controller.cpp:99` - Review negation of `turn_input`, likely removable after convention switch
5. Documentation - Update `CONVENTIONS.md`, `math_utils.h` comments, `VEHICLE_DYNAMICS_TERMINOLOGY.md`

**Derive/Generalize:**
All right vectors now consistently derived from standard right-handed +X convention. Cross products follow standard right-hand rule without negation hacks.

**Check duplicates:**
Search all `cross(` calls and directional calculations. Verify slip angle sign conventions remain physically correct after switch.

**Validation:**
- Test vehicle turning both directions (left and right turns behave identically)
- Verify slip angle signs match physical reality (right slide = positive)
- Check all debug visualizations (arrows, cones, trails) point correctly
- Run existing unit tests if any reference coordinate system
<!-- END: SELECT/FIX -->

---

## Implementation Steps

1. Update `yaw_to_right()` formula
2. Update cross products in slip angle and camera
3. Test vehicle steering both directions
4. Review and potentially remove `turn_input` negation
5. Update documentation
6. Verify all debug visualizations

---

**Simplify foundation. Follow standards. Remove confusion.**
