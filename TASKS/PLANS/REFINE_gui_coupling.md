# Refinement: GUI Coupling

**GUI directly manipulates game state, violating unidirectional data flow architecture**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-18
**Complexity:** Standard (5 points)
**Path:** B (Standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/gui/character_panel.cpp:draw_character_panel`
**System:** GUI Panel
**Principles:** Composable Functions, Principled Development
**Severity:** High
**Type:** Bidirectional coupling, Architectural violation

**Current state:**
```cpp
void draw_character_panel(character_panel_state& state, controller& character,
                          character_reactive_systems& visuals, character::tuning_params& params) {
    // GUI takes non-const references and directly modifies them
    gui::widget::slider_float("Max Speed (m/s)", &params.max_speed, 1.0f, 15.0f);
    gui::widget::slider_float("Coyote Window (s)", &character.coyote_window, 0.0f, 0.5f);
    gui::widget::slider_float("Stiffness", &visuals.animation.landing_spring.stiffness, 100.0f, 1000.0f);
    // ... direct manipulation of 10+ parameters across 3 systems
}
```

**Why violation:**
- **Bidirectional coupling:** GUI both reads and writes game state, creating circular dependency
- **Architecture violation:** `character_reactive_systems.h:14` explicitly documents "Data flows unidirectionally: controller → reactive_systems → rendering"
- **Non-composable:** Systems cannot be tested/reused independently when tightly coupled to GUI
- **Source of truth ambiguity:** Game state can be modified from multiple paths (game logic AND GUI)

**Impact:**
- **Blocks:** Clean system separation, independent testing, future GUI refactoring
- **Cascades to:** Any system touched by GUI (controller, reactive_systems, tuning)
- **Foundation:** Layer 3 (Systems) - 97% (architectural pattern violation)
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**CRITICAL:** This is a conceptual design, not implementation code.

**SIMPLIFY:**

**From:** Bidirectional coupling (GUI ↔ game state)
```
GUI Widget → Direct modification of game state
          ← Reads current values for display
```

**To:** Unidirectional flow (game state → GUI for display, GUI → commands/events for changes)
```
Game State → GUI (const references for display)
GUI → Commands/Events → Game State (explicit modification path)
```

**Conceptual Design:**

**Option 1: Command Pattern**
```
Concept: GUI emits parameter change commands
- GUI widgets return "change request" when user interacts
- Commands contain: parameter ID, new value
- Game loop applies commands to game state
- GUI receives const references for display only
```

**Option 2: Event Pattern**
```
Concept: GUI fires events on parameter change
- GUI widgets trigger "parameter_changed" event with ID + value
- Event handler in game loop updates appropriate system
- GUI operates read-only on game state
```

**Chosen: Command Pattern** (simpler, explicit, easier to test)

**Pseudocode structure:**
```
struct ParameterCommand {
    enum Type { MaxSpeed, CoyoteWindow, LandingStiffness, ... }
    Type param_type
    float new_value
}

// GUI returns commands (no direct state modification)
draw_character_panel(state, const controller&, const reactive_systems&, const tuning_params&)
    → returns vector<ParameterCommand>

// Game loop applies commands explicitly
for each command:
    switch command.type:
        case MaxSpeed: params.max_speed = command.value; params.apply_to(controller)
        case CoyoteWindow: controller.coyote_window = command.value
        case LandingStiffness: reactive_systems.animation.landing_spring.stiffness = command.value
```

**Derive/Generalize:**
- **Principle:** GUI is observer and commander, never executor
- **Pattern:** Read-only display + command emission = unidirectional flow
- **Benefit:** Systems become independently testable (can apply commands programmatically)

**Steps (conceptual):**
1. Define `ParameterCommand` struct with enum for all tunable parameters
2. Change `draw_character_panel` signature to take const references, return command vector
3. Modify GUI widgets to accumulate commands instead of directly modifying state
4. Update call site (game loop) to apply commands after GUI rendering
5. Verify all parameter changes still work correctly

**Tests:**
- Verify slider interactions produce correct commands
- Verify commands applied to game state correctly
- Verify read-only display shows current values
- Verify no direct GUI → game state writes remain
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

### Step 1: Define ParameterCommand struct
**Changes:**
- Create `src/gui/parameter_command.h`
- Define enum for all tunable parameters (9 parameters)
- Define command struct with type and value

**Files Created:**
- `src/gui/parameter_command.h`

**Tests:** None (data structure only)
**Validation:** Compiles successfully

### Step 2: Update draw_character_panel signature
**Changes:**
- `src/gui/character_panel.h:15-16` - Change signature to take const references, return vector<ParameterCommand>
- `src/gui/character_panel.cpp:9-10` - Update function signature
- Add include for `parameter_command.h`

**Files Modified:**
- `src/gui/character_panel.h`
- `src/gui/character_panel.cpp`

**Tests:** None yet (function body not updated)
**Validation:** Compiles with build errors (call site not updated yet)

### Step 3: Modify widgets to emit commands
**Changes:**
- `src/gui/character_panel.cpp:9-66` - Replace direct modification with command emission
- Accumulate commands in vector, return at end
- Remove `changed` flag logic (no longer applies params directly)

**Files Modified:**
- `src/gui/character_panel.cpp`

**Tests:** None yet (call site not updated)
**Validation:** Compiles, but GUI won't modify state until Step 4

### Step 4: Update call site to apply commands
**Changes:**
- `src/app/runtime.cpp:108-109` - Capture returned commands
- Apply each command to appropriate system
- Implement switch/dispatch logic for each parameter type

**Files Modified:**
- `src/app/runtime.cpp`

**Tests:** Manual verification - adjust sliders, verify parameters change
**Validation:** All parameter changes work correctly, no regressions

### Step 5: Run tests and verify
**Changes:** None
**Tests:** Build and run application, test all sliders
**Validation:**
- Tests pass
- All parameters adjustable via GUI
- No behavioral regressions
- Architecture now unidirectional (GUI → commands → game state)

## Rollback
`git reset --hard HEAD` (each step commits independently)
<!-- END: REFINE/PLAN -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Change:** Implemented command pattern for unidirectional data flow
- Created `parameter_command.h` with command struct (36 LOC)
- Updated `draw_character_panel` to emit commands instead of modifying state (68→95 LOC, +27)
- Updated `runtime.cpp` call site to apply commands explicitly (+42 LOC)

**Architecture:**
- **Before:** Bidirectional coupling (GUI ↔ game state)
- **After:** Unidirectional flow (game state → GUI → commands → game state)

**Tests:** All passing
- All 9 parameter sliders functional
- Character behavior responds correctly to parameter changes
- No regressions in GUI interaction

**Metrics:**
- LOC: +105 total (36 new file + 69 modifications)
- Principle: Composable Functions 6/10 → 9/10 (+3)
- Principle: Principled Development improved (architecture compliance)

**Result:** ✓ Violation removed
- GUI no longer directly modifies game state
- Unidirectional data flow established
- Systems independently testable (can apply commands programmatically)
- Architecture matches documented pattern in `character_reactive_systems.h:14`

**Future Consideration:** Registry pattern if parameter count grows (per Codex feedback)
<!-- END: REFINE/COMPLETED -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved (GUI no longer directly modifies game state)
- [ ] Principle score improved (Composable Functions, Principled Development)
- [ ] Tests passing (parameter changes work correctly)
- [ ] No regressions (all GUI interactions functional)
- [ ] Architecture documented (unidirectional flow maintained)

**Metrics:**
- **Before:** character_panel.cpp ~68 LOC, Composable Functions 6/10, Bidirectional coupling
- **After:** character_panel.cpp ~80 LOC (+12 for command struct), Composable Functions 9/10 (+3), Unidirectional flow ✅
- **Complexity:** Standard (5 points) - Architectural refactor, moderate risk, isolated scope
<!-- END: SELECT/SUCCESS -->
