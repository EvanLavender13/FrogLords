# Implementation Plan: Smoothed Walk Speed Transition

## Foundation Architecture Review

**Feature touches:** `controller` system (character physics/input)

**Current State Audit:**
- `controller` struct: Defined in `src/character/controller.h`, implemented in `src/character/controller.cpp`
- Walk speed handling: Hard velocity clamp at `controller.cpp:91-100` (lines 91-100)
- Current implementation: `is_walking` flag (line 54) drives instant speed lock via `effective_max_speed` calculation
- Max speed references:
  - `controller.h:47` - `float max_speed` member variable
  - `controller.h:48` - `float walk_speed` member variable
  - `controller.cpp:93-100` - Hard clamp logic
  - `gui/character_panel.cpp:33` - Max Speed tuning slider
  - Other references in `game_world.cpp`, `debug_draw.cpp`, `tuning.cpp`, `animation.cpp` (all read-only)

**Architecture Issues:** None - clean single ownership in `controller`

**Required Changes:**
- Replace hard clamp with smooth transition logic in `controller::update()`
- Add transition state variables to `controller` struct
- Add tuning parameter to GUI (`character_panel.cpp`)
- No migration/consolidation needed (single ownership)

---

## Graybox Implementation Checklist

### 1. Data Structures & State

- [x] **File:** `src/character/controller.h`
  - [x] Add `float target_max_speed` member to `controller` struct (after `max_speed` on line 47)
    - Comment: "Target speed based on walk/run input (smoothly interpolated)"
  - [x] Add `float walk_transition_rate = 10.0f` tuning parameter (after `walk_speed` on line 48)
    - Comment: "How quickly max_speed transitions between walk/run (larger = faster)"

### 2. Core Logic

- [x] **File:** `src/character/controller.cpp`

  - [x] **In `controller::constructor()` (around line 23):**
    - [x] Initialize `target_max_speed = max_speed` (set initial target to run speed)

  - [x] **In `controller::update()` - REPLACE hard clamp (lines 91-100):**

    **Remove existing code:**
    ```cpp
    // Apply max speed cap (walk speed if shift held)
    // Clamp walk_speed to never exceed max_speed
    float effective_max_speed = is_walking ? std::min(walk_speed, max_speed) : max_speed;
    glm::vec3 horizontal_velocity = math::project_to_horizontal(velocity);
    float speed = glm::length(horizontal_velocity);
    if (speed > effective_max_speed) {
        horizontal_velocity = horizontal_velocity * (effective_max_speed / speed);
        velocity.x = horizontal_velocity.x;
        velocity.z = horizontal_velocity.z;
    }
    ```

    **Replace with smooth transition logic:**
    - [x] Update target speed based on input: Set `target_max_speed` to `walk_speed` if `is_walking` is true, else `max_speed`
    - [x] Clamp walk_speed to never exceed max_speed (preserve existing safety check)
    - [x] Smooth `max_speed` toward `target_max_speed` using exponential decay
      - Formula: `max_speed = lerp(max_speed, target_max_speed, walk_transition_rate * dt)`
      - Use `glm::mix()` for lerp (built-in GLM function)
      - Clamp transition rate to prevent overshooting: `min(walk_transition_rate * dt, 1.0f)`
    - [x] Apply speed cap using smoothed `max_speed` value (preserve existing clamp logic structure)

    **Pseudocode:**
    ```
    // Set target based on walk input
    target_max_speed = is_walking ? min(walk_speed, max_speed) : max_speed

    // Smooth current max_speed toward target (exponential decay)
    float alpha = min(walk_transition_rate * dt, 1.0f)
    max_speed = mix(max_speed, target_max_speed, alpha)

    // Apply smoothed speed cap (existing clamp logic)
    horizontal_velocity = project_to_horizontal(velocity)
    speed = length(horizontal_velocity)
    if (speed > max_speed):
        horizontal_velocity = horizontal_velocity * (max_speed / speed)
        velocity.x = horizontal_velocity.x
        velocity.z = horizontal_velocity.z
    ```

### 3. Debug UI

- [x] **File:** `src/gui/character_panel.cpp`
  - [x] In "Character Tuning" section (after line 37, before line 39):
    - [x] Add slider for walk transition rate:
      ```cpp
      gui::widget::slider_float("Walk Transition Rate", &character.walk_transition_rate, 1.0f, 30.0f);
      ```
    - [x] Add read-only displays for debugging transition behavior:
      ```cpp
      gui::widget::text("Target Max Speed: %.2f m/s", character.target_max_speed);
      gui::widget::text("Current Max Speed: %.2f m/s", character.max_speed);
      ```
      (Shows target vs. smoothed value in real-time; helps visualize transition behavior during tuning)

### 4. Quality Gates

- [x] Build passes (`CMake: Build (Debug)`)
- [x] Run application and verify no crashes
- [x] **Functional tests:**
  - [x] Run at max speed, press SHIFT → smooth deceleration to walk (no instant lock)
  - [x] Hold SHIFT from standstill → accelerate directly to walk (no overshoot)
  - [x] Release SHIFT while walking → smooth acceleration to run
  - [x] Rapid SHIFT tapping → graceful handling (no oscillation)
- [x] **Tuning verification:**
  - [x] Adjust "Walk Transition Rate" slider → transition speed changes in real-time
  - [x] Default value (~10.0) feels responsive but smooth
  - [x] Visual coherence: Animation walk_factor matches velocity change

---

## Implementation Notes

**Why exponential decay over spring-damper:**
- Simpler implementation (one parameter vs. two)
- No overshoot risk (spring can overshoot target)
- Sufficient for speed transitions (spring better for spatial motion)
- Can upgrade to spring later if feel requires overshoot

**Integration order:**
- Transition happens AFTER friction, BEFORE velocity cap
- Preserves existing physics ordering (gravity → friction → speed cap → integration)
- `max_speed` becomes dynamic state variable (smoothed each frame)

**Safety:**
- Clamp `walk_speed` to never exceed `run_speed` (preserves existing guard)
- Clamp alpha to `[0, 1]` to prevent instability at high delta times
- No new state machine - simple lerp toward target

---

## Bug Fix: Circular Dependency in Speed State

**Problem discovered during implementation:**

The initial implementation had a critical bug where releasing SHIFT would not transition back to run speed. The root cause was a circular dependency in the speed state management.

**Root cause:**
The original design used `max_speed` for two conflicting purposes:
1. As the tunable "run speed" reference parameter (what the user sets in the GUI)
2. As the smoothed state variable (modified each frame by the lerp)

This created a circular dependency in the target calculation:
```cpp
// BUGGY CODE (original implementation):
target_max_speed = is_walking ? std::min(walk_speed, max_speed) : max_speed;
max_speed = glm::mix(max_speed, target_max_speed, alpha);
```

When SHIFT was released, the code would set `target_max_speed = max_speed`, but `max_speed` had already been lowered to walk speed by the smoothing. The system had lost its reference to the original run speed, so it could never transition back up.

**Solution:**
Separated the two concerns by introducing a new `run_speed` member variable:
- `run_speed`: The tunable reference parameter (constant between frames, user-adjustable)
- `max_speed`: The smoothed state variable (modified each frame)
- `target_max_speed`: The target for smoothing (set based on `is_walking` flag)

**Changes required:**
1. Added `float run_speed = 8.0f` member to `controller` struct (`controller.h:47`)
2. Updated `target_max_speed` calculation to use `run_speed` instead of `max_speed` (`controller.cpp:96`)
3. Initialize both `max_speed` and `target_max_speed` from `run_speed` in constructor (`controller.cpp:32-33`)
4. Changed GUI slider from "Max Speed" to "Run Speed" to control the tunable parameter (`character_panel.cpp:33`)

**Corrected implementation:**
```cpp
// Set target based on walk input (clamp walk_speed to never exceed run_speed)
target_max_speed = is_walking ? std::min(walk_speed, run_speed) : run_speed;

// Smooth current max_speed toward target (exponential decay)
float alpha = std::min(walk_transition_rate * dt, 1.0f);
max_speed = glm::mix(max_speed, target_max_speed, alpha);
```

Now when SHIFT is released, `target_max_speed` correctly gets set to the original `run_speed` value, and `max_speed` smoothly interpolates back up to it.

**Lessons learned:**
- Clearly separate tunable parameters from derived state variables
- Avoid using the same variable as both input (reference) and output (state)
- State variables modified by smoothing need immutable reference values to target

**Estimated implementation time:** 30-60 minutes
**Estimated tuning time:** 15-30 minutes (adjust `walk_transition_rate` slider)

**Line count delta:**
- Remove: ~9 lines (hard clamp logic)
- Add: ~15 lines (smooth transition + 2 state variables + GUI slider)
- Net: +6 lines total across 3 files

---

## Approval

**Status:** Approved

**Reviewer:** Claude (Code Review Agent)

**Date:** 2025-10-11

**Review Summary:** Implementation complete and adheres to project principles. No violations found. Two minor optional suggestions for code clarity (variable shadowing, safety comment). See [code_review_smoothed_walk_transition.md](code_review_smoothed_walk_transition.md) for full review.
