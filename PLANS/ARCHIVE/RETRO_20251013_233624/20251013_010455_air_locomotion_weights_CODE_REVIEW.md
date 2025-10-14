# Code Review: Air Locomotion Weights

**Date:** 2025-10-13
**Status:** APPROVED

### Summary

Implementation aligns with plan and AGENTS.md: contact/air weights use dual-reference spring-damper, phase advances in air via horizontal distance, amplitude suppression blends gait toward T_POSE when airborne, and acceleration tilt scales by contact weight. Update order is correct in `game_world.cpp`. Debug UI exposes frequency and read-only values. No gameplay latency or control hijacking.

### Violations (Must Fix)

No violations found.

### Suggestions (Consider)

None.

### Code Quality Checks

- [x] Naming: snake_case
- [x] Formatting: 4-space indent, braces on same line
- [x] Dependency flow respected
- [x] Gameplay-first principles (input→acceleration, interruptible, physics-first)
- [x] Simplicity over sophistication

### Actionable Items

- [x] Option A: Remove `is_grounded` parameter from `locomotion_system::update` (header and source) and all call sites. ✅ COMPLETE

**Files changed:**
- `src/character/locomotion.h` (removed unused parameter)
- `src/character/locomotion.cpp` (removed unused parameter)
- `src/app/game_world.cpp` (updated call site)

### Recommendation

**Reasoning:** Changes are minimal, systemic, and follow validated patterns (dual-reference smoothing, slerp, surveyor-wheel). Code respects layering and style. Visual and build checks noted in the plan are consistent with the diffs.

**Next Steps:**
- If APPROVED: Proceed to FINALIZE_FEATURE
- If FIX_ISSUES: Address items, re-review
- If ROLLBACK: Defer feature, update backlog

