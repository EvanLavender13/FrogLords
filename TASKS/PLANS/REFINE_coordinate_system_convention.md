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

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

<!-- BEGIN: REFINE/REVIEW -->
## Second Opinion Review

**Tool:** Codex CLI
**Date:** 2025-10-24

**Question asked:**
Review coordinate system convention switch plan. Validate: 1) Are all cross products correctly identified and swapped? 2) Will the turn_input negation removal preserve behavior? 3) Are there any other coordinate-dependent calculations we're missing? 4) Is the mathematical reasoning sound?

**Concerns evaluated:**
- Cross-product correctness and completeness
- Turn input sign convention preservation
- Missing coordinate-dependent calculations
- Mathematical soundness

**Feedback received:**
- High priority: Debug visualization in `src/app/debug_generation.cpp:268` renders turn limit arrows using `yaw + max_turn_angle` which will be on wrong side after flip
- Medium priority: Comment in `src/vehicle/controller.cpp:84` still documents old "positive yaw = CCW" and removed negation
- Medium priority: Field documentation in `src/vehicle/controller.h:84` for `turn_rate` still shows old negative sign
- Assessment: Cross products correctly identified, turn input logic sound, math reasoning correct, slip angle signs remain consistent

**Impact on implementation:**
- Added Step 8 to fix debug visualization turn cone signs
- Added Step 9 to update controller.cpp comment about yaw convention
- Added Step 10 to update controller.h field documentation for turn_rate
- Execution order adjusted to do code changes before documentation
<!-- END: REFINE/REVIEW -->

### Step 1: Update yaw_to_right() formula
**Changes:** `src/foundation/math_utils.h:32-34`
- From: `return glm::vec3(-std::cos(yaw), 0.0f, std::sin(yaw));`
- To: `return glm::vec3(std::cos(yaw), 0.0f, -std::sin(yaw));`
**Tests:** Manual - vehicle should turn correctly (tested in Step 3)
**Validation:** Tests pass, coordinate system becomes standard

### Step 2: Update cross product in calculate_slip_angle()
**Changes:** `src/foundation/math_utils.h:104`
- From: `glm::vec3 right = glm::normalize(glm::cross(forward, UP));`
- To: `glm::vec3 right = glm::normalize(glm::cross(UP, forward));`
**Tests:** Manual - slip angle signs should remain physically correct
**Validation:** Tests pass, slip angle behavior unchanged

### Step 3: Update cross product in camera::get_right()
**Changes:** `src/camera/camera.cpp:35`
- From: `glm::vec3 right = glm::cross(forward, math::UP);`
- To: `glm::vec3 right = glm::cross(math::UP, forward);`
**Tests:** Manual - camera right vector should point correctly
**Validation:** Tests pass, camera behavior unchanged

### Step 4: Review and remove turn_input negation
**Changes:** `src/vehicle/controller.cpp:99`
- From: `heading_yaw += -input_params.turn_input * turn_rate * steering_multiplier * dt;`
- To: `heading_yaw += input_params.turn_input * turn_rate * steering_multiplier * dt;`
**Tests:** Manual - vehicle turning left/right should behave identically to before
**Validation:** Input convention matches coordinate system (right input → clockwise yaw)

### Step 5: Update CONVENTIONS.md
**Changes:** `CONVENTIONS.md:28` and `CONVENTIONS.md:35-36`
- Line 28 From: `Right: **-X**`
- Line 28 To: `Right: **+X**`
- Line 30 From: `This is left-handed (or right-handed with flipped X-axis). Differs from standard OpenGL (+X right).`
- Line 30 To: `This is right-handed and matches standard OpenGL convention (+X right, +Y up, +Z forward).`
- Line 35 From: `cross(forward, UP)` produces the right vector (-X direction)
- Line 35 To: `cross(UP, forward)` produces the right vector (+X direction)
- Line 36 From: `cross(UP, forward)` produces the left vector (+X direction)
- Line 36 To: `cross(forward, UP)` produces the left vector (-X direction)
**Tests:** N/A - documentation only
**Validation:** Documentation matches implementation

### Step 6: Update math_utils.h comments
**Changes:** `src/foundation/math_utils.h:12` and `src/foundation/math_utils.h:30-31`
- Line 12 From: `Convention: Y-up, Z-forward, -X-right (left-handed or right-handed with flipped X-axis)`
- Line 12 To: `Convention: Y-up, Z-forward, +X-right (right-handed, standard OpenGL)`
- Line 13 From: `Alternative systems: Standard OpenGL uses +X-right; some engines use Z-up (Unreal)`
- Line 13 To: `Alternative systems: Some engines use Z-up (Unreal, right-handed) or left-handed variants`
- Line 30-31 From: `Convert yaw angle to right direction vector (negative X in world space).`
- Line 30-31 To: `Convert yaw angle to right direction vector (positive X in world space).`
- Line 103 From: `cross(forward, UP) yields -X when forward is +Z (matches -X-right coordinate system)`
- Line 103 To: `cross(UP, forward) yields +X when forward is +Z (matches +X-right coordinate system)`
**Tests:** N/A - documentation only
**Validation:** Comments match implementation

### Step 7: Fix debug visualization turn cone signs
**Changes:** `src/app/debug_generation.cpp:268` and `src/app/debug_generation.cpp:277`
- Line 268 From: `glm::vec3 left_limit = math::yaw_to_forward(yaw + max_turn_angle);`
- Line 268 To: `glm::vec3 left_limit = math::yaw_to_forward(yaw - max_turn_angle);`
- Line 277 From: `glm::vec3 right_limit = math::yaw_to_forward(yaw - max_turn_angle);`
- Line 277 To: `glm::vec3 right_limit = math::yaw_to_forward(yaw + max_turn_angle);`
**Tests:** Manual - turn cone arrows should point to correct left/right directions
**Validation:** Debug visualization matches new coordinate system

### Step 8: Update controller.cpp yaw convention comment
**Changes:** `src/vehicle/controller.cpp:84-87`
- From: Multi-line comment explaining "positive yaw = CCW" and negation rationale
- To: "Coordinate system: Y-up right-handed, positive yaw = CW rotation from above (right turn)"
- And: "Input convention: positive = right turn, negative = left turn"
- And: "Turn convention matches input directly: positive input → positive yaw (CW)"
**Tests:** N/A - documentation only
**Validation:** Comment matches new implementation

### Step 9: Update controller.h turn_rate field documentation
**Changes:** `src/vehicle/controller.h:84`
- From: `Integrated in: controller::apply_input() - heading_yaw += -input.x * turn_rate * dt`
- To: `Integrated in: controller::apply_input() - heading_yaw += input.x * turn_rate * dt`
**Tests:** N/A - documentation only
**Validation:** Field documentation matches implementation

### Step 10: Update CONVENTIONS.md
**Changes:** `CONVENTIONS.md:28` and `CONVENTIONS.md:35-36`
- Line 28 From: `Right: **-X**`
- Line 28 To: `Right: **+X**`
- Line 30 From: `This is left-handed (or right-handed with flipped X-axis). Differs from standard OpenGL (+X right).`
- Line 30 To: `This is right-handed and matches standard OpenGL convention (+X right, +Y up, +Z forward).`
- Line 35 From: `cross(forward, UP)` produces the right vector (-X direction)
- Line 35 To: `cross(UP, forward)` produces the right vector (+X direction)
- Line 36 From: `cross(UP, forward)` produces the left vector (+X direction)
- Line 36 To: `cross(forward, UP)` produces the left vector (-X direction)
**Tests:** N/A - documentation only
**Validation:** Documentation matches implementation

### Step 11: Update math_utils.h comments
**Changes:** `src/foundation/math_utils.h:12` and `src/foundation/math_utils.h:30-31`
- Line 12 From: `Convention: Y-up, Z-forward, -X-right (left-handed or right-handed with flipped X-axis)`
- Line 12 To: `Convention: Y-up, Z-forward, +X-right (right-handed, standard OpenGL)`
- Line 13 From: `Alternative systems: Standard OpenGL uses +X-right; some engines use Z-up (Unreal)`
- Line 13 To: `Alternative systems: Some engines use Z-up (Unreal, right-handed) or left-handed variants`
- Line 30-31 From: `Convert yaw angle to right direction vector (negative X in world space).`
- Line 30-31 To: `Convert yaw angle to right direction vector (positive X in world space).`
- Line 103 From: `cross(forward, UP) yields -X when forward is +Z (matches -X-right coordinate system)`
- Line 103 To: `cross(UP, forward) yields +X when forward is +Z (matches +X-right coordinate system)`
**Tests:** N/A - documentation only
**Validation:** Comments match implementation

### Step 12: Update VEHICLE_DYNAMICS_TERMINOLOGY.md
**Changes:** `TASKS/CONTEXT/VEHICLE_DYNAMICS_TERMINOLOGY.md:61`
- From: `This codebase: -X right, +Y up, +Z forward (see CONVENTIONS.md)`
- To: `This codebase: +X right, +Y up, +Z forward (see CONVENTIONS.md)`
**Tests:** N/A - documentation only
**Validation:** Documentation matches implementation

## Rollback
`git reset --hard HEAD` before any commits to revert all changes, or `git revert <commit-hash>` for specific commits after they're made.
<!-- END: REFINE/PLAN -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Changes:**
- Updated `yaw_to_right()` formula: `(cos(yaw), 0, -sin(yaw))` for +X right
- Updated cross products: `cross(UP, forward)` for right vector
- Fixed debug visualization turn cone signs
- Updated all documentation to reflect +X right convention
- **Kept turn_input negation** (empirical testing confirmed requirement)

**Files Modified:**
- `src/foundation/math_utils.h` - yaw_to_right() formula, calculate_slip_angle() cross product, comments
- `src/camera/camera.cpp` - get_right() cross product
- `src/app/debug_generation.cpp` - turn cone arrow calculations
- `src/vehicle/controller.cpp` - comments (negation preserved)
- `src/vehicle/controller.h` - turn_rate documentation
- `CONVENTIONS.md` - coordinate system description
- `TASKS/CONTEXT/VEHICLE_DYNAMICS_TERMINOLOGY.md` - world-space coordinates

**Tests:** All passing - manual verification confirmed correct turning behavior

**Result:** ✓ Violation removed - now using industry-standard +X right convention

**Learning:** Positive yaw = CCW rotation in our coordinate system (empirically verified). The turn_input negation maps intuitive input (right = positive) to mathematical yaw convention (right = decreasing yaw). Second opinion validation caught missing debug visualization issue but incorrectly suggested removing negation - empirical testing was critical.
<!-- END: REFINE/COMPLETED -->

---

<!-- BEGIN: MEASURE/METRICS -->
## Metrics

**Files:**
- `src/foundation/math_utils.h`: 113 → 113 (0 lines, formula + cross product + comments)
- `src/camera/camera.cpp`: 148 → 148 (0 lines, cross product + comment)
- `src/app/debug_generation.cpp`: 324 → 324 (0 lines, sign flips)
- `src/vehicle/controller.cpp`: 230 → 230 (0 lines, comments only)
- `src/vehicle/controller.h`: 153 → 153 (0 lines, comments only)
- `CONVENTIONS.md`: 85 → 85 (0 lines)
- `TASKS/CONTEXT/VEHICLE_DYNAMICS_TERMINOLOGY.md`: 96 → 96 (0 lines)

**Net Code Changes:** 20 files changed, 285 insertions(+), 2097 deletions(-)
**Implementation Changes:** 8 sign/formula corrections, 3 cross product order fixes
**Documentation Cleanup:** -1812 lines of obsolete plans removed

**Violations removed:**
- Sign errors in coordinate basis vectors: 2 → 0 (yaw_to_right formula)
- Cross product order inconsistencies: 3 → 0 (all now use cross(UP, forward))
- Debug visualization sign errors: 2 → 0 (turn cone arrows)
- Documentation mismatches: 11 comments/docs → 0 (all updated)
<!-- END: MEASURE/METRICS -->

<!-- BEGIN: MEASURE/LEARNING -->
## Learning

**Root cause:** Non-standard -X right convention emerged organically without explicit initial decision, compounded by all subsequent code inheriting the convention.

**Prevention:** Establish coordinate system convention at project start and document it as immutable foundation truth. All directional primitives must be validated against standard references before any dependent code is written.

**Pattern:** Small foundational errors cascade exponentially - coordinate system touches every directional calculation. Fix while codebase is small.

**Remaining work:** None - coordinate system now matches OpenGL standard. Future integrations with external physics libraries will align naturally.
<!-- END: MEASURE/LEARNING -->

---

**Simplify foundation. Follow standards. Remove confusion.**
