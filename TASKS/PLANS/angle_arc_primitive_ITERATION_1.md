# Angle Arc Primitive - ITERATION 1

**System:** Generic angle visualization primitive (Layer 2)

---

<!-- BEGIN: ITERATE/CONTEXT -->
**Starting from:** GRAYBOX/RESULTS - crude but working implementation

**Graybox delivered:**
- `generate_arc()` primitive in `procedural_mesh.{h,cpp}`
- Slip angle visualization working at half-radius
- Debug assertions validating inputs
- First consumer proves reusability

**Refinements requested:**
- World-space debug text showing angle degrees
- Arc styling (gradient colors, severity indication)
- Additional consumers (steering authority, counter-steer)
<!-- END: ITERATE/CONTEXT -->

---

<!-- BEGIN: ITERATE/INTERNAL_REVIEW -->
## Internal Review Findings

**Architecture: ✅ Correct Layer 2 Pattern**
- Pure function, no state, returns data (wireframe_mesh)
- Follows `generate_circle()` and `generate_arrow()` patterns
- Consumer decides visualization policy (color, when to render)

**Mathematics: ✅ Correct Implementation**
- Signed angle via atan2 with cross product (procedural_mesh.cpp:275-277)
- Orthonormal frame construction: X=start_dir, Y=cross(UP, X), Z=UP (lines 290-291)
- Arc parameterization: P(θ) = center + radius*(cos(θ)*X + sin(θ)*Y) (line 298)

**Issues Identified:**

1. **Missing Validation at Foundation (PATTERNS.md violation)**
   - Location: procedural_mesh.cpp:291
   - Issue: `glm::normalize(glm::cross(math::UP, frame_x))` assumes non-zero cross product
   - Root cause: Precondition requires horizontal (Y≈0), so cross product should be valid, but no assertion validates this
   - Fix: Add postcondition assertion that frame_y is unit length after normalization

2. **Arbitrary Fallback Constants (PATTERNS.md pattern)**
   - Location: procedural_mesh.cpp:280
   - Issue: `ANGLE_EPSILON = 0.001f` lacks justification
   - Root cause: Magic number without derivation from requirements
   - Fix: Document why 0.001 radians (~0.057°) or derive from visual/mathematical constraint

3. **Tolerance Inconsistency**
   - Location: procedural_mesh.cpp:266-272
   - Issue: Unit vector tolerance (0.01f), horizontal tolerance (0.01f), angle epsilon (0.001f) use different scales
   - Root cause: No consistent tolerance policy across related validations
   - Observation: Not necessarily wrong, but lacks coherent rationale

4. **Clarity: Parameterization lacks comment**
   - Location: procedural_mesh.cpp:297
   - Issue: Variable `t` sweeps [0,1] but purpose not documented
   - Fix: Add comment explaining parameter interpolation

**Consumer Analysis (debug_generation.cpp:79-90):**
- ✅ Uses primitive correctly
- ✅ Threshold check (2.9°) prevents noise from tiny slip angles
- ✅ Safe normalization with fallback prevents degenerate arcs
- Question: Should threshold be tunable/exposed?

**Anti-Pattern Check:**
- ✅ Derive, Don't Accumulate: Pure function
- ✅ Use Primitives: Leverages existing math utilities
- ✅ Single Source for Configuration: Follows pattern
- ❌ Missing Validation at Foundation: Cross product normalization unchecked
- ⚠️ Arbitrary Fallback Constants: ANGLE_EPSILON needs justification
<!-- END: ITERATE/INTERNAL_REVIEW -->

---

<!-- BEGIN: ITERATE/EXTERNAL_REVIEW -->
## External Review Findings (Codex)

**Critical Issues:**

1. **Missing finiteness checks** (Foundation validation pattern violation)
   - Location: procedural_mesh.cpp:265-273
   - Issue: No `FL_ASSERT_FINITE` on vectors and scalars at foundation boundary
   - Fix: Add finiteness checks for all inputs

2. **Inconsistent assertion style** (Convention violation)
   - Location: procedural_mesh.cpp:266-268
   - Issue: Ad hoc `abs(length-1) < 0.01f` instead of `FL_ASSERT_NORMALIZED`
   - Issue: Uses 0.01f tolerance instead of `FL_EPSILON` (0.001f)
   - Fix: Use project assertion macros consistently

3. **Arbitrary epsilon duplication** (Single Source for Configuration pattern)
   - Location: procedural_mesh.cpp:280
   - Issue: `ANGLE_EPSILON = 0.001f` defined inline instead of using `FL_EPSILON`
   - Fix: Replace with `FL_EPSILON` constant

4. **Consumer gating mismatch**
   - Location: debug_generation.cpp:80-90
   - Issue: Gate uses `calculate_slip_angle()` (based on `heading_yaw`) but arc renders from `visuals.orientation.get_yaw()`
   - Impact: Under transient smoothing, gate/render may be inconsistent
   - Fix: Either compute angle from exact vectors used for rendering, or use `heading_yaw` consistently

**Improvement Opportunities:**

5. **Missing postcondition checks**
   - Issue: No assertions on returned mesh structure (vertex/edge counts)
   - Fix: Assert `vertices.size() == segments+1`, `edges.size() == segments`

6. **No reserve() for capacity**
   - Location: procedural_mesh.cpp:293-305
   - Issue: Vertices/edges vectors reallocate during generation
   - Fix: `vertices.reserve(segments+1); edges.reserve(segments);`

7. **Release behavior mismatch with generate_circle**
   - Location: procedural_mesh.cpp:240-242 vs 265-273
   - Issue: `generate_circle` has runtime guards (`if (segments <= 0)`), `generate_arc` only has debug assertions
   - Fix: Add minimal runtime guards for parity and resilience

8. **180° ambiguity undocumented**
   - Location: procedural_mesh.cpp:275-281
   - Issue: Opposite vectors always select +π sweep (one of two valid semicircles)
   - Fix: Document as intentional behavior

**Architecture Observations:**

9. **World-space vs local-space pattern split**
   - Observation: `generate_arc` and `generate_circle` emit world-space vertices (ignoring transform), while `generate_box` uses local-space + transform
   - Recommendation: Document pattern decision to avoid drift (world-space for "placed" primitives, local-space for "shapes")

10. **Extract signed angle helper**
    - Recommendation: `math::signed_angle_horizontal(a,b)` to avoid duplicating atan2+cross+dot logic
    - Benefits: Reusability across consumers, consistent behavior

11. **Future reusability suggestions**
    - Arbitrary plane normal overload (deferred but easy to add)
    - `generate_arc(center, start_dir, angle, radius)` variant for direct angle input

**Consumer-Specific:**

12. **Segment count coupling**
    - Location: debug_generation.cpp:75-78, 87-90
    - Issue: Arc and ring both use default 32 segments implicitly
    - Recommendation: Pass explicit `segments` to both for consistency guarantee

13. **Plan/documentation divergence**
    - Location: SYSTEM.md GRAYBOX/RESULTS
    - Issue: Claims "Arc connects yellow heading arrow to green orientation arrow" but code shows different arrow colors/configuration
    - Fix: Update documentation to match actual implementation
<!-- END: ITERATE/EXTERNAL_REVIEW -->

---

<!-- BEGIN: ITERATE/SCOPE -->
## Refinement Scope

**Priority: Foundation Hygiene (Critical)**

1. **Replace ad hoc assertions with project macros** (procedural_mesh.cpp:265-272)
   - Use `FL_ASSERT_NORMALIZED` instead of manual length checks
   - Use `FL_ASSERT_FINITE` for all vector/scalar inputs
   - Use `FL_ASSERT_POSITIVE` for radius
   - Use `FL_ASSERT_IN_RANGE` for segments with reasonable upper bound
   - Use `glm::epsilonEqual(v.y, 0.0f, FL_EPSILON)` for horizontal checks

2. **Replace ANGLE_EPSILON with FL_EPSILON** (procedural_mesh.cpp:280)
   - Single source for configuration (PATTERNS.md)
   - Document why this threshold is appropriate

3. **Add postcondition assertions** (procedural_mesh.cpp:293-307)
   - Assert `vertices.size() == segments + 1`
   - Assert `edges.size() == segments`
   - Optionally use `fl::verify_coordinate_frame(frame_x, frame_y, math::UP)` after frame construction

4. **Add reserve() calls** (procedural_mesh.cpp:293)
   - `vertices.reserve(segments + 1)`
   - `edges.reserve(segments)`

**Priority: Consistency and Clarity**

5. **Add runtime guards matching generate_circle pattern** (procedural_mesh.cpp:263)
   - Early return for invalid inputs that would bypass debug assertions in release

6. **Add parameterization comment** (procedural_mesh.cpp:297)
   - Document that `t ∈ [0,1]` sweeps from start to end direction

7. **Document 180° behavior** (procedural_mesh.cpp:275-281)
   - Add comment explaining opposite vectors always select +π sweep

8. **Fix consumer gating mismatch** (debug_generation.cpp:80-90)
   - Use `heading_yaw` consistently for both gate and arc start direction
   - Eliminates visual smoothing inconsistency

**Deferred (Architectural, non-critical for iteration 1):**

- Extract `math::signed_angle_horizontal()` helper (future refactor when second consumer needs it)
- World-space vs local-space pattern documentation (broader primitive API concern)
- Segment count explicit passing (improvement, not a bug)
- Plan documentation divergence (cosmetic, not blocking)

**Out of Scope (Future features):**

- Arbitrary plane normal overload
- Direct angle parameter variant
- Additional consumers (steering authority, counter-steer)
- Arc styling (gradients, severity colors)
- World-space debug text showing degrees
<!-- END: ITERATE/SCOPE -->

---

<!-- BEGIN: ITERATE/REFINEMENT -->
## Refinement Changes

**Files modified:**
- `src/foundation/procedural_mesh.cpp` (generate_arc implementation)
- `src/app/debug_generation.cpp` (slip angle arc consumer)

**Changes applied:**

1. **Replaced ad hoc assertions with project macros** (procedural_mesh.cpp:270-281)
   - `FL_ASSERT_NORMALIZED` for unit vector validation
   - `FL_ASSERT_FINITE` for all vector/scalar inputs (center, directions)
   - `FL_ASSERT_POSITIVE` for radius
   - `FL_ASSERT_IN_RANGE` for segments (3-1024)
   - `glm::epsilonEqual(v.y, 0.0f, FL_EPSILON)` for horizontal checks
   - Consistent use of `FL_EPSILON` throughout

2. **Replaced ANGLE_EPSILON with FL_EPSILON** (procedural_mesh.cpp:290)
   - Single source for tolerance values (PATTERNS.md compliance)
   - Added comment documenting 180° behavior for opposite vectors

3. **Added postcondition assertions** (procedural_mesh.cpp:325-328)
   - Assert `vertices.size() == segments + 1`
   - Assert `edges.size() == segments`
   - Added `fl::verify_coordinate_frame()` validation after frame construction (line 303)

4. **Added reserve() calls** (procedural_mesh.cpp:306-307)
   - `vertices.reserve(segments + 1)`
   - `edges.reserve(segments)`
   - Prevents reallocation during generation

5. **Added runtime guards** (procedural_mesh.cpp:265-268)
   - Early return for invalid inputs (segments <= 0, radius <= 0.0f)
   - Matches `generate_circle` pattern for release build resilience

6. **Added documentation comments** (procedural_mesh.cpp:288-310)
   - Documented 180° ambiguity behavior (line 289)
   - Documented orthonormal frame construction (lines 295-298)
   - Documented arc parameterization (line 309)
   - Added comment explaining t parameter interpolation (line 310)

7. **Fixed consumer gating mismatch** (debug_generation.cpp:83-84)
   - Changed from `visuals.orientation.get_yaw()` to `character.heading_yaw`
   - Now consistent with `calculate_slip_angle()` implementation
   - Eliminates visual smoothing inconsistency
   - Added comment explaining the consistency requirement

**Build status:** ✅ Clean compile (no warnings)

**Patterns addressed:**
- ✅ Missing Validation at Foundation (fixed)
- ✅ Arbitrary Fallback Constants (fixed - using FL_EPSILON)
- ✅ Single Source for Configuration (fixed - consistent tolerance)
<!-- END: ITERATE/REFINEMENT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->
## Playtest Results

**Status:** ✅ STABLE - All scenarios passed

**Scenarios tested:**

1. **Basic slip angle visualization** - ✅ PASS
   - Arc appears during turns when slip > threshold
   - Direction matches turn input (left/right)
   - Magnitude scales correctly with slip angle

2. **Handbrake drift (high slip angle)** - ✅ PASS
   - Large arcs render correctly during handbrake turns
   - No rendering artifacts or visual glitches
   - Arc smoothly represents full drift angle

3. **Straight-line motion (zero slip)** - ✅ PASS
   - No arc appears when driving straight
   - Threshold gating working correctly
   - No spurious arcs at zero slip

4. **Slow speed maneuvering** - ✅ PASS
   - Small arcs appear during low-speed turns
   - Correct orientation at all speeds
   - Works at edge of speed threshold

5. **Consistency check (heading_yaw fix)** - ✅ PASS
   - Arc appears/disappears consistently with actual slip
   - No flicker from visual smoothing mismatch
   - Consumer gating fix eliminates inconsistency

6. **Assertion validation (debug build)** - ✅ PASS
   - No assertion failures during normal gameplay
   - All preconditions validated correctly
   - Postconditions hold after generation
   - Coordinate frame validation passes

**Observations:**
- Arc primitive behaves identically to graybox (no behavioral regression)
- Assertions provide immediate validation without performance impact
- Consumer fix (heading_yaw consistency) eliminates subtle visual inconsistencies
- Code quality significantly improved while preserving functionality

**Conclusion:** Refinement successful. Ready for commit and validation phase.
<!-- END: ITERATE/PLAYTEST -->

---
