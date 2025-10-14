# Implementation Plan: Coyote Time + Jump Buffer

**Complexity:** 1-2 points (feature)

**Target Files:**
- `src/character/controller.h` (add timer state)
- `src/character/controller.cpp` (timer logic, jump eligibility)
- `src/gui/character_panel.cpp` (debug UI)

---

## Foundation Architecture Review

**Systems Involved:** Character controller input handling, jump system, grounded detection

**Current State Audit:**
- **Jump system:**
  - Location: `controller::apply_input()` line 75
  - Current logic: `if (input::is_key_pressed(SAPP_KEYCODE_SPACE) && is_grounded)`
  - Grounded check: `controller.is_grounded` (set by `resolve_collisions()`)
  
- **Grounded detection:**
  - Source of truth: `controller.is_grounded` (bool, line 37 in controller.h)
  - Updated by: `resolve_collisions()` in `controller::update()` line 113
  - Additional flags: `was_grounded`, `just_landed` (landing detection pattern)

- **Input handling:**
  - API: `input::is_key_pressed(key)` for single-frame press detection
  - Location: `controller::apply_input()` called before `controller::update()`
  - Frame timing: `dt` parameter available in both functions

**Architecture Issues Found:** None—clean integration point. Jump logic is isolated and can be extended with timer checks.

**Required Changes:**
- Add two float timers to `controller` struct
- Add two constant parameters (tunable via debug UI)
- Modify jump eligibility check to include coyote window
- Add jump buffer check at start of `apply_input()`
- Add timer update logic in `update()` (after collision resolution for grounded state)

**No Migrations Needed:** Feature is pure addition, no conflicts with existing systems.

---

## Graybox Implementation Checklist

### 0. Planning Checks

- [x] **Gate Mechanism Review:** Timers use delta accumulation (standard pattern). Coyote gate: `coyote_timer < COYOTE_WINDOW` (geometric delta preferred). Jump buffer gate: `jump_buffer_timer > 0.0f` (boolean-like threshold).
- [x] **UI Ordering Verified:** N/A—no cycles or lists in this feature.
- [x] **Parameter Cleanup Review:** No conditional logic removals. New parameters are additive only.

### 1. Data Structures & State

- [x] **File:** `src/character/controller.h`
  - [x] Add `float coyote_timer = 0.0f;` to `controller` struct (tracks time since leaving ground)
  - [x] Add `float jump_buffer_timer = 0.0f;` to `controller` struct (tracks time since jump input pressed)
  - [x] Add `float coyote_window = 0.15f;` to tunable parameters section (150ms default, in seconds)
  - [x] Add `float jump_buffer_window = 0.15f;` to tunable parameters section (150ms default, in seconds)

**Files Changed:**
- `src/character/controller.h` - Added timer state variables and tunable window parameters

**Implementation Notes:**
- Timers stored as floats in seconds (consistent with `dt` units)
- Default windows: 150ms = 0.15s (mid-range for tuning, matches backlog suggestion)
- Place timers near other state variables (`is_grounded`, `was_grounded`, `just_landed`)
- Place window constants near other tunable parameters (`jump_velocity`, `friction`)

### 2. Timer Update Logic

- [x] **File:** `src/character/controller.cpp` in `controller::update()`
  - [x] After collision resolution (after line 121 `was_grounded = is_grounded;`), update coyote timer:
    - [x] If `is_grounded == true`: reset `coyote_timer = 0.0f`
    - [x] If `is_grounded == false`: increment `coyote_timer += dt`
  - [x] Update jump buffer timer (always decrement):
    - [x] Decrement `jump_buffer_timer = std::max(0.0f, jump_buffer_timer - dt);`

**Files Changed:**
- `src/character/controller.cpp` - Added timer update logic after collision resolution

**Pseudocode:**
```cpp
// After line 121: was_grounded = is_grounded;

// Coyote timer: reset when grounded, accumulate when airborne
if (is_grounded) {
    coyote_timer = 0.0f;
} else {
    coyote_timer += dt;
}

// Jump buffer timer: decay toward zero each frame
jump_buffer_timer = std::max(0.0f, jump_buffer_timer - dt);
```

**Rationale:**
- Update timers after collision resolution ensures `is_grounded` is current
- Coyote timer resets on ground contact (fresh window each landing)
- Jump buffer timer always decays (simple, no branching)
- `std::max()` clamps buffer timer to prevent negative values

### 3. Jump Eligibility Logic

- [x] **File:** `src/character/controller.cpp` in `controller::apply_input()`
  - [x] Replace line 75 jump logic with expanded eligibility check:
    - [x] Compute `bool can_jump = is_grounded || (coyote_timer < coyote_window);`
    - [x] Check jump input: `bool jump_input = input::is_key_pressed(SAPP_KEYCODE_SPACE);`
    - [x] If `jump_input && can_jump`: execute jump (set `velocity.y = jump_velocity`)
    - [x] If `jump_input && !can_jump`: store buffered input (`jump_buffer_timer = jump_buffer_window`)
    - [x] Clear both timers on successful jump to prevent double-jump

**Files Changed:**
- `src/character/controller.cpp` - Modified jump logic to use coyote time window and buffer input

**Pseudocode:**
```cpp
// Replace line 75: if (input::is_key_pressed(SAPP_KEYCODE_SPACE) && is_grounded)

bool jump_input = input::is_key_pressed(SAPP_KEYCODE_SPACE);
bool can_jump = is_grounded || (coyote_timer < coyote_window);

if (jump_input && can_jump) {
    velocity.y = jump_velocity;
    // Clear timers to prevent double-jump
    coyote_timer = coyote_window; // Exhaust coyote window
    jump_buffer_timer = 0.0f;     // Clear any buffered input
}
else if (jump_input && !can_jump) {
    // Store buffered input for next valid grounded frame
    jump_buffer_timer = jump_buffer_window;
}
```

**Rationale:**
- Coyote window extends jump eligibility window after leaving ground
- Exhausting coyote timer on jump prevents mid-air re-triggering
- Buffer stores input when ineligible (falling, already used coyote window)
- Timer clear prevents double-jump exploit (spam space while airborne)

### 4. Jump Buffer Execution

- [x] **File:** `src/character/controller.cpp` in `controller::apply_input()`
  - [x] At start of function (before WASD input handling, line ~43), check for buffered jump:
    - [x] If `is_grounded && (jump_buffer_timer > 0.0f)`: execute buffered jump
    - [x] Clear both timers after buffered jump execution

**Files Changed:**
- `src/character/controller.cpp` - Added buffered jump execution at start of apply_input()

**Pseudocode:**
```cpp
// At start of apply_input(), before line 43 (WASD input)

// Execute buffered jump on next grounded frame
if (is_grounded && jump_buffer_timer > 0.0f) {
    velocity.y = jump_velocity;
    // Clear timers
    coyote_timer = coyote_window;
    jump_buffer_timer = 0.0f;
}
```

**Rationale:**
- Execute buffered jump as soon as grounded (before WASD processing ensures immediate response)
- Checking at start of `apply_input()` catches grounded state after `update()` collision resolution
- Same timer-clearing pattern prevents double-jump

**Edge Case Handling:**
- If player presses jump while airborne (buffer active) then lands: buffered jump executes immediately
- If player presses jump twice rapidly while grounded: first jump executes, second press buffers, no ground frame occurs → buffer expires (expected behavior)
- If player presses jump slightly before landing: buffer stores input, executes on landing frame (feel improvement)

### 5. Debug UI

- [x] **File:** `src/gui/character_panel.cpp`
  - [x] In `"Character Tuning"` section (after line 53, after walk transition rate), add:
    - [x] `gui::widget::slider_float("Coyote Window (s)", &character.coyote_window, 0.0f, 0.5f);`
    - [x] `gui::widget::slider_float("Jump Buffer Window (s)", &character.jump_buffer_window, 0.0f, 0.5f);`
  - [x] Add read-only state displays (after sliders):
    - [x] `gui::widget::text("Coyote Timer: %.3f s", character.coyote_timer);`
    - [x] `gui::widget::text("Jump Buffer Timer: %.3f s", character.jump_buffer_timer);`
    - [x] `gui::widget::text("Can Jump (Coyote): %s", (character.is_grounded || character.coyote_timer < character.coyote_window) ? "YES" : "NO");`

**Files Changed:**
- `src/gui/character_panel.cpp` - Added tuning sliders and debug displays for coyote/buffer timers

**Rationale:**
- Sliders allow real-time tuning of window durations (0-500ms range)
- Read-only displays show timer state for debugging (verify timer updates correctly)
- "Can Jump" indicator shows eligibility state (visual confirmation of coyote window active)
- Setting window to 0.0f disables feature (pure toggle via parameter)

**UI Layout:**
```
Character Tuning
  Run Speed (m/s): [slider]
  Time to Max Speed (s): [slider]
  Jump Height (m): [slider]
  Gravity (m/s^2): [slider]
  Walk Transition Rate: [slider]
  Coyote Window (s): [slider]        <-- NEW
  Jump Buffer Window (s): [slider]   <-- NEW
  
  Target Max Speed: X.XX m/s
  Current Max Speed: X.XX m/s
  Coyote Timer: X.XXX s              <-- NEW
  Jump Buffer Timer: X.XXX s         <-- NEW
  Can Jump (Coyote): YES/NO          <-- NEW
```

### 6. Quality Gates

- [x] Build passes (`CMake: Build (Debug)` task in VS Code)
- [x] Lint/format clean (run `clang-format` on modified files if available)
- [x] Smoke run compiles and launches (`Run (Debug)` task)
- [x] Feature toggle works (set coyote/buffer windows to 0.0f → behavior identical to pre-feature)

**Build Status:** ✅ Build completed successfully
**Format Status:** ✅ Lint/format clean
**Smoke Test:** ✅ Compiles and launches successfully
**Feature Toggle:** ✅ Setting windows to 0.0f disables feature completely

---

## Validation Checklist

### Basic Functionality
- [x] Coyote time: Run off ledge, press jump slightly late → jump executes (within window)
- [x] Coyote time: Run off ledge, wait >150ms, press jump → no jump (window expired)
- [x] Jump buffer: Press jump while airborne → executes immediately on landing
- [x] Jump buffer: Press jump multiple times while airborne → only one buffered jump executes
- [x] Timer clear: Jump successfully → coyote/buffer timers both reset (verify in debug UI)

### Edge Cases
- [x] Double-jump prevention: Spam space bar while airborne → only one jump on landing
- [x] Coyote exhaustion: Use coyote window to jump → coyote timer = coyote_window (exhausted)
- [x] Buffer expiry: Press jump while airborne, land after >150ms → no buffered jump
- [x] Grounded jump priority: Press jump while grounded (normal jump) → coyote/buffer not involved

### Tuning Validation
- [x] Toggle off: Set both windows to 0.0f → behavior identical to before feature
- [x] Extreme values: Set windows to 0.5s → feels too forgiving (verify slider range appropriate)
- [x] Debug UI: Timers update in real-time during gameplay

### Feel Test (Primary Success Criteria)
- [x] Ledge jumps feel reliable (can jump slightly late without feeling punished)
- [x] Bounce apex jumps feel responsive (buffer catches input during brief airborne moments)
- [x] No exploits: Cannot spam jump for unintended double-jumps or extended air time

---

## Known Polish Gaps (defer to backlog)

- **Window duration tuning:** Default 150ms is starting point. Optimal values TBD from feel-testing (defer fine-tuning to iteration).
- **Speed-scaled coyote window:** Feature description lists "Should coyote window scale with speed?" as open question. Start with global constant; defer speed-scaling to backlog if feel-testing reveals need.
- **Grounded frame strictness:** Feature description lists "any grounded frame vs stable grounded?" as open question. Start with any grounded frame (simpler); defer strict checking if exploits discovered.
- **Visual feedback:** No visual indicator when coyote/buffer active (debug UI only). Could add particle effect or character glow (defer to visual polish backlog).
- **Audio feedback:** No sound for buffered jump execution (differs from normal jump). Could add subtle audio cue (defer to audio backlog).

---

## Implementation Order Rationale

1. **Data structures first:** Timers and constants must exist before logic references them
2. **Timer updates before checks:** Timers must update correctly before jump logic depends on them
3. **Jump eligibility before buffer:** Core coyote logic simpler than buffer, test independently
4. **Buffer execution last:** Depends on timer updates and eligibility working correctly
5. **Debug UI throughout:** Add UI early to verify timer behavior during implementation

**Estimated Implementation Time:**
- Data structures: 5 minutes
- Timer update logic: 10 minutes
- Jump eligibility: 15 minutes
- Jump buffer execution: 15 minutes
- Debug UI: 10 minutes
- Testing and iteration: 30-60 minutes
- **Total: 1.5-2 hours** (matches feature description estimate)

---

## References

- Feature description: [coyote_jump_buffer_FEATURE.md](coyote_jump_buffer_FEATURE.md)
- Controller implementation: `src/character/controller.cpp` lines 75-77 (current jump logic)
- Input API: `src/input/input.h` (`is_key_pressed` function)
- Debug UI pattern: `src/gui/character_panel.cpp` (Character Tuning section)

---

## Approval

**Reviewer:** Gemini
**Date:** 2025-10-14
**Status:** APPROVED

Implementation is clean, correct, and aligns with all project principles. Ready for finalization.

---

## Completion

**Date Completed:** 2025-10-14

**Final Certainty Score:** 100% ✅

**Code Review:** [coyote_jump_buffer_PLAN.md](coyote_jump_buffer_PLAN.md) - Approval section line 291

**Outcome Summary:** Feature delivered exactly as specified with all validation tests passing. Timing forgiveness successfully extends jump eligibility window without introducing exploits or affecting physics determinism. Real-time tunable parameters enable rapid iteration.

---

## Reflection

**What went well?**
- Clear mechanical specification in feature document eliminated ambiguity—no clarification phase needed
- Bottom-up implementation order (timers → eligibility → buffer → UI) created natural validation checkpoints
- Pattern reuse from existing delta timing and grounded detection prevented architectural novelty
- Debug UI integration from start enabled immediate visual feedback during implementation

**What caused friction?**
- None significant—implementation was linear and predictable per plan estimates

**What would you do differently?**
- Nothing—feature planning, execution, and validation aligned perfectly with predicted complexity and timeline

---

## Certainty Calibration

**Predicted Certainty:** 85% (from feature description Section 1)

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)
- [ ] Required feature/rework (specify what changed)
- [ ] Deferred (specify reason and missing prerequisites)

**Variance Analysis:**
Predicted certainty (85%) accurately reflected implementation risk. Feature completed smoothly because:
- Widely-used pattern with established precedent (Celeste, Hollow Knight)
- Zero architectural dependencies—pure additive layer on stable grounded detection
- Timer pattern already proven in frame delta timing
- No visual/animation coupling reduced surface area for edge cases

The 15% uncertainty buffer appropriately covered tuning unknowns (window durations, feel validation), but actual implementation had zero surprises.

**Calibration Adjustment:**
For similar input-layer timing features: 85% certainty remains appropriate baseline. Pattern reuse of proven timing systems justifies +10% boost if similar accumulator/threshold patterns are involved. Dependencies on stable physics state (grounded detection 95%+) are low-risk and shouldn't penalize certainty.