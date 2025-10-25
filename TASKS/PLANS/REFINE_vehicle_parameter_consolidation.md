# Refinement: Vehicle Parameter Consolidation

Unify vehicle physics parameters under vehicle::tuning_params

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-24
**Complexity:** Standard (3-8 pts)
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** System: Vehicle Tuning (`src/character/tuning.h`, `src/vehicle/tuning.h`, `src/gui/character_panel.cpp`)
**Principle:** Single Source of Truth, Systems Not Features
**Severity:** High
**Type:** State duplication across namespaces after incomplete system pivot

**Current state:**
- Vehicle physics parameters (`max_speed`, `accel`, `weight`) live in `character::tuning_params`
- Separate `vehicle::tuning_params` exists but incomplete
- GUI splits presentation across `character_panel` and `vehicle_panel`
- Truth scattered across multiple locations

**Why violation:**
- Single Source of Truth: Vehicle parameters exist in character namespace but define vehicle physics behavior
- Systems Not Features: Character vs vehicle split creates feature-based organization instead of system-based
- Confusion: Future maintainers won't know where vehicle parameters belong

**Impact:**
- Blocks: Clean vehicle tuning system development
- Cascades to: GUI panels, parameter application logic, future vehicle variations
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**FROM:** Split vehicle parameters between character and vehicle namespaces with fragmented GUI

**TO:** All vehicle physics parameters in `vehicle::tuning_params`, single vehicle panel

**Derive/Generalize:**
- Move `max_speed`, `accel`, `weight` from `character::tuning_params` to `vehicle::tuning_params`
- Update game_world to use vehicle params directly
- Remove vehicle sections from character_panel
- Consolidate all vehicle tuning in vehicle_panel
- Verify no other character state references vehicle params

**Check duplicates:**
- Audit both tuning headers for overlapping parameters
- Verify GUI isn't duplicating parameter widgets
- Ensure runtime applies vehicle params from single source
<!-- END: SELECT/FIX -->

---

## Validation

**Success Criteria:**
- All vehicle physics params in `vehicle::tuning_params` only
- `character::tuning_params` contains only character-specific state
- Single vehicle_panel for all vehicle tuning
- Runtime applies vehicle params from vehicle namespace
- Build succeeds, vehicle control unchanged

**Test:**
- Build and run
- Verify vehicle movement responds to tuning changes
- Confirm no character_panel vehicle widgets remain
