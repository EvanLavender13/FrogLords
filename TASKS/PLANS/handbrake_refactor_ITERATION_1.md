# Handbrake Refactor - Iteration 1

**Refine crude but correct implementation into quality code.**

---

<!-- BEGIN: ITERATE/INTERNAL_REVIEW -->

## Internal Review Findings

**Architecture - GOOD:**
- Single source of truth: `controller.handbrake` owns state
- Unidirectional flow: controller updates handbrake, passes to consumers
- Multi-system consumption enabled: GUI (vehicle_panel.cpp:114), tuning (tuning.cpp:58), friction model (controller.cpp:179)

**Interface Design - ISSUE:**

**Finding 1: Broken Encapsulation at friction_model::compute_total_drag()**

Location: `friction_model.h:26`, `friction_model.cpp:4`, `controller.cpp:179`

Current signature:
```cpp
float compute_total_drag(float accel, float max_speed, bool handbrake_active, float brake_rate)
```

Problem: `handbrake_active` and `brake_rate` are both properties of `handbrake_system` but are unpacked and passed separately. This breaks encapsulation.

Call site reveals coupling:
```cpp
float k = friction.compute_total_drag(accel, max_speed, handbrake.is_active(), handbrake.brake_rate);
```

If `handbrake_system` evolves (gradual engagement, temperature-dependent braking, etc.), every call site must change.

**Better alternatives:**

A) Pass entire handbrake system (query pattern):
```cpp
float compute_total_drag(float accel, float max_speed, const handbrake_system& handbrake)
```

B) Handbrake provides drag contribution (tell-don't-ask):
```cpp
// In handbrake_system:
float get_drag_contribution() const { return active ? brake_rate : 0.0f; }

// In friction_model:
float compute_total_drag(float accel, float max_speed, float handbrake_drag)
```

C) Keep bool + rate separate but document why (current approach requires justification)

**Recommendation:** Option B aligns with "primitives return data, systems decide" (PRIMITIVES_GUIDE.md). Handbrake system knows how to compute its drag contribution; friction_model just adds contributions.

**Finding 2: Comment Accuracy**

Location: `friction_model.h:16`

Comment claims "query pattern" but implementation uses parameter passing with broken encapsulation. If keeping current approach, update comment to reflect actual pattern.

**Positive Observations:**
- Tuning system integration clean: `c.handbrake.brake_rate` accessible for metadata-driven UI
- GUI integration clean: `vehicle.handbrake.is_active()` query pattern
- Physics unchanged: drag coefficient calculation identical to pre-refactor
- Call sites updated correctly: all references migrated from `friction.handbrake` to `controller.handbrake`

<!-- END: ITERATE/INTERNAL_REVIEW -->

---

<!-- BEGIN: ITERATE/EXTERNAL_REVIEW -->

## External Review (Codex)

**Single Source of Truth:**
- ✓ Controller owns handbrake state (controller.h:102)
- ✓ Tuning config flows correctly (tuning.cpp:58)
- ⚠️ **Config default duplication:** `handbrake_system.h:21` has `brake_rate = 2.0f`, `tuning.h:47` also defines default. Violates "Single Source for Configuration" pattern.

**Query Pattern:**
- ✓ Clean separation: controller composes both friction and handbrake
- ✓ Explicit parameter passing (controller.cpp:179)
- ✓ GUI queries correctly (vehicle_panel.cpp:114)

**Anti-Patterns Detected:**

1. **Stale Documentation (Contract/Implementation Mismatch):**
   - `handbrake_system.h:7` — Comment says "Composed into friction_model" but now composed into controller
   - `tuning.h:46` — Comment references `controller.friction.handbrake.brake_rate`, should be `controller.handbrake.brake_rate`

2. **Plan Section Divergence:**
   - System plan describes passing only `handbrake_active`, implementation passes both `active` and `brake_rate`

**Interface Design:**
- Current signature works but risks parameter explosion as systems grow
- Recommended: `get_drag_contribution()` method for encapsulation (tell-don't-ask)
- Future: consider contributions struct for multi-system drag sources

**Documentation Accuracy:**
- `friction_model.h:16` — "query pattern" comment needs clarification with current implementation
- `handbrake_system.h:10` — Exponential integrator comment incomplete (missing particular solution term)

**Recommendation:** Add `handbrake_system::get_drag_contribution()` to encapsulate the `active ? brake_rate : 0.0f` logic, reducing coupling and future-proofing the interface.

<!-- END: ITERATE/EXTERNAL_REVIEW -->

---

<!-- BEGIN: ITERATE/SCOPE -->

## Refinement Scope

Both reviews converge on same core issue: **broken encapsulation** in friction_model interface.

**Actionable Changes:**

1. **Add encapsulation method to handbrake_system:**
   - Add `float get_drag_contribution() const` returning `active ? brake_rate : 0.0f`
   - Encapsulates handbrake logic, enables future evolution (gradual engagement, temperature, etc.)
   - Aligns with PRIMITIVES_GUIDE: "primitives return data, systems decide"

2. **Simplify friction_model::compute_total_drag() signature:**
   - Remove `bool handbrake_active` parameter
   - Rename `float brake_rate` → `float handbrake_drag`
   - New signature: `compute_total_drag(float accel, float max_speed, float handbrake_drag)`
   - Friction model agnostic to handbrake semantics

3. **Update call site in controller::update_physics():**
   - Change from: `friction.compute_total_drag(accel, max_speed, handbrake.is_active(), handbrake.brake_rate)`
   - Change to: `friction.compute_total_drag(accel, max_speed, handbrake.get_drag_contribution())`
   - Single coupling point; future handbrake changes don't affect caller

4. **Fix stale documentation:**
   - `handbrake_system.h:7` — Update comment to reflect controller ownership
   - `tuning.h:46` — Correct path to `controller.handbrake.brake_rate`
   - `handbrake_system.h:10` — Complete exponential integrator formula
   - `friction_model.h:16` — Update query pattern description

5. **Remove config default duplication:**
   - Remove `brake_rate = 2.0f` default from `handbrake_system.h:21`
   - Single source: tuning system provides default via initialization
   - Prevents drift between tuning defaults and member defaults

**Out of Scope (defer to future work):**
- Contributions struct for multi-system drag (not needed yet, only 1 contributor)
- Plan section divergence (acceptable variance from plan)

**Expected Impact:**
- Interface cleaner and more maintainable
- Future-proof for handbrake evolution
- Documentation accurate
- No behavior change (physics identical)

<!-- END: ITERATE/SCOPE -->

---

<!-- BEGIN: ITERATE/REFINEMENT -->

## Code Refinement

**Changes Made:**

**1. Added encapsulation method (handbrake_system.h:37)**
```cpp
float get_drag_contribution() const { return active ? brake_rate : 0.0f; }
```
- Encapsulates handbrake drag logic in single method
- Enables future evolution without changing call sites
- Aligns with "primitives return data" pattern

**2. Simplified friction_model interface (friction_model.h:25, friction_model.cpp:4)**
- Removed `bool handbrake_active` parameter
- Changed `float brake_rate` → `float handbrake_drag`
- New signature: `compute_total_drag(float accel, float max_speed, float handbrake_drag)`
- Friction model now agnostic to handbrake internals

**3. Updated call site (controller.cpp:179)**
- Changed from: `friction.compute_total_drag(accel, max_speed, handbrake.is_active(), handbrake.brake_rate)`
- Changed to: `friction.compute_total_drag(accel, max_speed, handbrake.get_drag_contribution())`
- Single coupling point: handbrake system responsible for computing its contribution

**4. Fixed stale documentation:**
- `handbrake_system.h:7` — Updated to reflect controller ownership and query pattern
- `handbrake_system.h:10` — Completed exponential integrator formula with particular solution
- `tuning.h:46` — Corrected path from `controller.friction.handbrake.brake_rate` to `controller.handbrake.brake_rate`
- `friction_model.h:16` — Updated to describe contribution pattern

**5. Removed config default duplication (handbrake_system.h:22)**
- Removed `= 2.0f` default initializer from `brake_rate`
- Tuning system provides default via initialization (single source)
- Prevents drift between duplicated defaults

**Build Result:** ✅ Success (13/13 files compiled, linked successfully)

**Impact:**
- Interface cleaner: single method call instead of unpacking two parameters
- Encapsulation preserved: handbrake logic changes don't affect callers
- Documentation accurate: reflects actual architecture
- Config hygiene: single source for brake_rate default
- Physics unchanged: drag calculation identical

<!-- END: ITERATE/REFINEMENT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->

## Playtest Results

**Status:** ✅ All scenarios passed

**Tested Scenarios:**

1. **Basic Handbrake Functionality:**
   - ✅ Handbrake engages on Space key
   - ✅ Rapid deceleration observed
   - ✅ GUI displays "Handbrake: ACTIVE"
   - ✅ Velocity trail shows strong braking effect

2. **Handbrake Release:**
   - ✅ Handbrake disengages on Space release
   - ✅ Returns to normal drag behavior
   - ✅ GUI displays "Handbrake: INACTIVE"

3. **Tuning Parameter Access:**
   - ✅ "Handbrake Brake Rate" parameter visible in tuning GUI
   - ✅ Parameter adjustable (tested increase to 5.0)
   - ✅ Stronger braking observed with higher values
   - ✅ Tuning system integration intact

4. **Zero-Speed Handbrake:**
   - ✅ Handbrake engageable while stationary
   - ✅ Reduced equilibrium speed when accelerating with handbrake
   - ✅ Physics behaves as expected

5. **Interface Stability:**
   - ✅ Behavior identical to pre-refinement
   - ✅ No crashes or assertion failures
   - ✅ Frame rate stable
   - ✅ No visual glitches

**Observations:**
- Physics unchanged: braking force identical to graybox implementation
- Interface cleaner: `get_drag_contribution()` encapsulation successful
- Tuning system unaffected by refactoring
- No regressions detected

**Conclusion:** Refinement successful. Code quality improved while preserving exact behavior.

<!-- END: ITERATE/PLAYTEST -->

