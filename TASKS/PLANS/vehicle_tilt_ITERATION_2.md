# Iteration 2: Vehicle Tilt - Single Source of Truth

**Started:** 2025-10-25
**Previous:** [ITERATION_1.md](vehicle_tilt_ITERATION_1.md)
**Status:** Ready for VALIDATE

---

<!-- BEGIN: ITERATE/CONTEXT -->
## Context from Previous Iteration

**Decision:** REVISE (from ITERATION_1)

**Required changes:**
1. Eliminate `tilt_stiffness` member variable from vehicle_visual_systems.h
2. Expose spring stiffness directly via getters: `lean_spring.stiffness` and `pitch_spring.stiffness`
3. OR add setter: `void set_tilt_stiffness(float k)` that updates both springs and their damping
4. Update assertions to validate actual spring stiffness values
5. Update any tuning/GUI code to access springs directly or use setter

**Baseline contract:** See ITERATION_1/CONTRACT - all items previously validated
<!-- END: ITERATE/CONTEXT -->

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Eliminate stiffness duplication to enforce Single Source of Truth. All other contract items validated in iteration 1.

### Single Source of Truth (Parameter Management)
- [x] `tilt_stiffness` member variable removed from vehicle_visual_systems.h
- [x] Spring stiffness accessed directly from spring objects (single source)
- [x] Assertions validate actual spring stiffness values (not redundant member)
- [x] GUI/tuning code accesses springs directly (no divergence possible)

### Regression Validation
- [x] All iteration 1 behaviors still work (lean, pitch, edge cases)
- [x] No new bugs introduced by refactoring
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->
### Playtest 1

**Date:** 2025-10-25
**Tester:** EvanUhhh

**All behaviors verified:**

✅ **Lean direction:**
- Turn left → leans RIGHT (away from turn, physically correct)
- Turn right → leans LEFT (away from turn, physically correct)
- Smooth spring-damped motion

✅ **Pitch direction:**
- Accelerating forward → nose pitches UP (weight to rear)
- Decelerating → nose pitches DOWN (weight to front)
- Smooth spring-damped motion

✅ **Edge cases:**
- Stationary: no rotation/tilt ✓
- High-speed straight: stays upright, no drift ✓
- Sharp corners: smooth lean, no overshoot ✓
- Rapid direction changes: no jitter ✓

✅ **GUI Tuning (Single Source of Truth validation):**
- Tilt Stiffness slider responds correctly
- Lower values → slower/softer tilt response
- Higher values → faster/snappier tilt response
- Changes apply immediately (confirms springs are actual source, no stale member)

**Emergent discovery:** Camera default longitude was 180° off, discovered during spatial reasoning verification. Fixed camera_follow.h longitude from 0° to 180° (camera now behind target, aligned with +Z forward convention). Separate commit.

✅ VERIFIED: All contract items proven through playtest
<!-- END: ITERATE/PLAYTEST -->

---

<!-- BEGIN: ITERATE/VALIDATION -->
## Validation Work

### Code Changes

**vehicle_visual_systems.h:**
- Removed `float tilt_stiffness` member (line 38)
- Added `get_tilt_stiffness()` getter returning `lean_spring.stiffness`
- Single source of truth: springs own their parameters

**vehicle_visual_systems.cpp:**
- Constructor: Use local `constexpr float default_tilt_stiffness = 150.0f` instead of member
- Assertions: Validate `lean_spring.stiffness` and `pitch_spring.stiffness` directly (lines 24-27)
- No longer validates removed member

**vehicle/tuning.cpp:**
- Removed redundant assignment: `visuals.tilt_stiffness = tilt_stiffness` (line 59)
- Springs still updated correctly (lines 62-65 unchanged)
- Added clarifying comment: "Single source of truth: springs own their stiffness values"

**vehicle/tuning.h:**
- No changes required - `tilt_stiffness` correctly exists here as configuration source
- Metadata and apply_to function work correctly

**gui/vehicle_panel.cpp:**
- No changes required - reads from tuning_params, not vehicle_visual_systems

### Architectural Verification

**Data flow (Single Source of Truth enforced):**
1. `tuning_params.tilt_stiffness` = configuration source (user input)
2. `tuning_params.apply_to()` = copies to springs directly
3. `lean_spring.stiffness` / `pitch_spring.stiffness` = runtime truth
4. No redundant member to diverge from springs

**Before (violation):**
- `tuning_params.tilt_stiffness` → `visuals.tilt_stiffness` → springs
- Runtime tuning could diverge: member vs spring values
- Assertions validated wrong value (member, not springs)

**After (correct):**
- `tuning_params.tilt_stiffness` → springs directly
- Single source at runtime: spring objects
- Assertions validate actual spring values
- Getter exposes spring value (read-only, no divergence)

### Build Verification

✅ Build successful - all files compile without errors
<!-- END: ITERATE/VALIDATION -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Single Source of Truth restored:**
- Removed redundant `tilt_stiffness` member from vehicle_visual_systems
- Springs own their stiffness values (single source)
- Assertions validate actual spring parameters
- GUI tuning confirmed working (runtime changes apply immediately)

**Properties validated:**
- All iteration 1 behaviors unchanged (lean, pitch, edge cases)
- GUI tuning functional (slider adjusts spring stiffness correctly)
- No parameter divergence possible

**Playtests:** 1 (full validation, all pass)

**Emergent discovery:** Camera longitude coordinate mismatch (180° off) found during spatial reasoning check. Fixed in camera_follow.h - camera now behind target as expected.

**Status:**
- [x] Contract proven
- [x] Single Source of Truth enforced
- [x] Ready for VALIDATE
<!-- END: ITERATE/COMPLETE -->

---
