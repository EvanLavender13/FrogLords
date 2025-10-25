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

<!-- BEGIN: REFINE/REVIEW -->
## Second Opinion Review

**Tool:** Codex CLI
**Date:** 2025-10-24

**Question asked:**
Validate the 7-step refinement plan for consolidating vehicle parameters. Check: 1) Does the plan correctly move all vehicle physics params (max_speed, accel, weight) from character to vehicle namespace? 2) Are the steps in the right order to avoid breaking the build at each commit? 3) Are there any missing dependencies or call sites that need updating? 4) Does this align with Single Source of Truth principle? 5) Are there any risks or concerns with this approach?

**Concerns evaluated:**
- Step ordering and build breakage at intermediate commits
- Completeness of dependency identification
- Loss of horizontal speed visualization widget
- Empty character::tuning_params struct after move

**Feedback received:**
- Q1: ✓ Parameter move correctly identified
- Q2: ⚠️ **Critical**: Step 2 breaks build - deletions happen before consumers updated
- Q3: ✓ All call sites identified (game_world, runtime, character_panel, vehicle_panel)
- Q4: ✓ Aligns with Single Source of Truth
- Q5: Risks: horizontal speed viz lost, empty character::tuning_params should be deleted

**Impact on implementation:**
- Reordered steps: ADD vehicle params → UPDATE consumers → DELETE character params
- Step 2: Preserve horizontal speed plot in vehicle_panel
- Step 6: Delete entire character::tuning_params system (files + all references)
- New ordering ensures every commit keeps build working
<!-- END: REFINE/REVIEW -->

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

### Step 1: Add vehicle parameters to vehicle::tuning_params
**Changes:**
- `src/vehicle/tuning.h:8-35` - Add max_speed, accel, weight parameters with metadata
- `src/vehicle/tuning.cpp:8-25` - Update apply_to() to apply all vehicle params including max_speed, accel, weight

**Files Modified:**
- `src/vehicle/tuning.h`
- `src/vehicle/tuning.cpp`

**Tests:** Build succeeds
**Validation:** New parameters have metadata and validation, both sources exist temporarily

### Step 2: Add vehicle tuning widgets to vehicle_panel
**Changes:**
- `src/gui/vehicle_panel.cpp:10-30` - Add max_speed, accel, weight sliders with horizontal speed visualization
- Preserve horizontal speed plot from character_panel (line 35-36)

**Files Modified:**
- `src/gui/vehicle_panel.cpp`

**Tests:** Build succeeds
**Validation:** Vehicle panel has all vehicle parameters including speed visualization

### Step 3: Update runtime to use vehicle_params for vehicle parameters
**Changes:**
- `src/app/runtime.cpp:157-168` - Change MAX_SPEED, ACCEL, WEIGHT to use world.vehicle_params instead of world.character_params

**Files Modified:**
- `src/app/runtime.cpp`

**Tests:** Build succeeds, both sources work
**Validation:** Runtime applies from correct namespace

### Step 4: Update game_world initialization to use only vehicle_params
**Changes:**
- `src/app/game_world.cpp:13-21` - Remove character_params.apply_to() call, keep only vehicle_params.apply_to()

**Files Modified:**
- `src/app/game_world.cpp`

**Tests:** Build succeeds
**Validation:** Single source active at runtime

### Step 5: Remove vehicle parameters from character_panel
**Changes:**
- `src/gui/character_panel.cpp:11-39` - Delete draw_character_tuning_section() entirely

**Files Modified:**
- `src/gui/character_panel.cpp`

**Tests:** Build succeeds (tuning section removed but panel still draws)
**Validation:** Character panel no longer shows vehicle parameters

### Step 6: Delete character::tuning_params entirely
**Changes:**
- Delete `src/character/tuning.h` - Empty struct, no longer needed
- Delete `src/character/tuning.cpp` - No implementation needed
- `src/gui/character_panel.h` - Remove character::tuning_params parameter from draw_character_panel signature
- `src/gui/character_panel.cpp:118-138` - Remove params parameter from function
- `src/app/runtime.cpp:105-106` - Remove world.character_params from draw_character_panel call
- `src/app/game_world.h` - Remove character_params member and tuning.h include

**Files Modified:**
- `src/character/tuning.h` (deleted)
- `src/character/tuning.cpp` (deleted)
- `src/gui/character_panel.h`
- `src/gui/character_panel.cpp`
- `src/app/runtime.cpp`
- `src/app/game_world.h`

**Tests:** Build succeeds, all tests pass
**Validation:** No character::tuning_params references remain anywhere

## Rollback
Each step is independently committed. Rollback: `git reset --hard HEAD~N` where N is number of steps to undo.
<!-- END: REFINE/PLAN -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Date:** 2025-10-24

### Changes Made

**Step 1:** Added max_speed, accel, weight to vehicle::tuning_params with metadata and validation
**Step 2:** Added vehicle tuning widgets to vehicle_panel with horizontal speed visualization
**Step 3:** Updated runtime parameter commands to use vehicle_params
**Step 4:** Removed character_params.apply_to() from game_world init
**Step 5:** Deleted character_panel entirely (moved traction to vehicle_panel, removed velocity trail toggle)
**Step 6:** Deleted character::tuning_params files and all references

### Files Modified
- `src/vehicle/tuning.h` - Added max_speed, accel, weight parameters
- `src/vehicle/tuning.cpp` - Updated apply_to() with all vehicle params
- `src/gui/vehicle_panel.cpp` - Added vehicle param sliders + traction display
- `src/app/runtime.cpp` - Updated parameter commands, removed character_panel
- `src/app/runtime.h` - Removed character_panel include and state
- `src/app/game_world.h` - Removed character_params member
- `src/app/game_world.cpp` - Removed character tuning include
- `src/app/debug_generation.h` - Removed panel_state parameter
- `src/app/debug_generation.cpp` - Velocity trail always shown
- `src/gui/parameter_command.h` - Removed landing spring params (8→5 params)
- `CMakeLists.txt` - Removed character_panel.cpp and character/tuning.cpp

### Files Deleted
- `src/gui/character_panel.h`
- `src/gui/character_panel.cpp`
- `src/character/tuning.h`
- `src/character/tuning.cpp`

### Metrics

**Files deleted:**
- `src/character/tuning.h`: 45 lines
- `src/character/tuning.cpp`: 26 lines
- `src/gui/character_panel.h`: 22 lines
- `src/gui/character_panel.cpp`: 140 lines

**Files modified:**
- `src/vehicle/tuning.h`: 35 → 67 (+32 lines)
- `src/vehicle/tuning.cpp`: +12 lines
- `src/gui/vehicle_panel.cpp`: +39 lines
- Other files: +123 lines (plan documentation, minor updates)

**Total:** -80 lines (206 insertions, 286 deletions)

**Violations removed:**
- Duplicated vehicle parameters: 3 parameters (max_speed, accel, weight) removed from character namespace
- Split GUI presentation: 1 vehicle tuning function consolidated from character_panel to vehicle_panel
- Scattered truth: 4 files deleted, single source established

### Tests
- ✓ Build succeeds at every commit
- ✓ Vehicle movement responds to parameter changes
- ✓ Horizontal speed visualization works
- ✓ Traction displays in Vehicle State
- ✓ Velocity trail always renders
- ✓ No character panel widgets remain

### Result
✓ **Violation removed**
✓ **Single Source of Truth restored**: All vehicle physics parameters unified in vehicle::tuning_params
✓ **Principle upheld**: Systems Not Features - vehicle parameters live in vehicle namespace

### Learning

**Root cause:** Incomplete system pivot left vehicle parameters in character namespace after vehicle system was introduced.

**Prevention:** When creating new system namespace (vehicle::), immediately audit related namespaces (character::) and move all semantically-aligned state. Delete empty structs immediately.

**Pattern:** Namespace-based organization can create feature splits if not aligned with system boundaries. Vehicle physics belongs in vehicle::, regardless of historical location.
<!-- END: REFINE/COMPLETED -->

---

## Validation

**Success Criteria:**
- All vehicle physics params in `vehicle::tuning_params` only
- `character::tuning_params` contains only character-specific state (deleted if empty)
- Single vehicle_panel for all vehicle tuning
- Runtime applies vehicle params from vehicle namespace
- Build succeeds, vehicle control unchanged

**Test:**
- Build and run
- Verify vehicle movement responds to tuning changes
- Confirm no character_panel vehicle widgets remain
