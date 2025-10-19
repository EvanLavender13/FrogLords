# System: Walk/Run Speed States

**Discrete speed tiers with distance-based phase output**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-19
**Layer:** 4
**Foundation:** 98%+
**Complexity:** Medium (3-5 points)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**

1. **David Rosen GDC Talk - "Surveyor Wheel" Technique** (NOTES/GDC/)
   - Core concept: cyclic motion has a **phase** (normalized position within cycle)
   - Phase calculated from distance traveled: `phase = (distance % cycle_length) / cycle_length`
   - Phase is 0-1 normalized value representing "where in the cycle are we?"
   - Different speeds = different cycle lengths
   - Phase is an **output** computed from movement, not an input
   - Future systems consume phase to drive their behavior (animation, audio, etc.)

2. **Distance Matching** (Modern Game Engines)
   - Same mathematical concept: map distance traveled to position in animation
   - Key insight: distance-based selection eliminates drift/desync
   - Animation playback position = f(distance), not f(time)

**Visual references:**
- GDC Vault: David Rosen demonstrates the concept ~5:30 in talk
- Overgrowth footage showing speed transitions with synchronized movement

**Conceptual Foundation:**

**What IS Phase?**
Phase is a normalized scalar (0-1) representing position within a repeating cycle.
- Phase 0.0 = start of cycle
- Phase 0.5 = halfway through cycle
- Phase 1.0 = end of cycle (wraps to 0.0)

**Why Distance-Based (not Time-Based)?**
- Time-based: `phase += rate * dt` → phase rate must change with speed → complex
- Distance-based: `phase = f(distance)` → automatically adapts to any speed → simple

**Mathematical Core:**
```
cycle_length = f(speed_tier)     // different speeds = different cycle lengths
distance += velocity.magnitude * dt
phase = fmod(distance, cycle_length) / cycle_length  // always 0-1
```

**What Can Future Systems Do With Phase?**
Phase is a fundamental output. Systems that don't exist yet will consume it:
- Visual representation systems (animation, when we have one)
- Audio systems (footstep timing)
- Gameplay systems (movement state queries)
- Debug visualization (show the cycle)

**At Graybox Level:**
- Character is a box with color-coded speed states (walk=green, run=yellow, sprint=red)
- Visualize phase with rotating "+" sign (surveyor wheel):
  - Wheel SIZE = cycle_length (walk=2m, run=3m, sprint=4m - faster gaits = longer strides = larger wheels)
  - Wheel ROTATION = phase (0° = phase 0, 180° = phase 0.5)
  - Rotates in direction of movement
- Debug overlay: state name, speed, phase, cycle_length, distance
- That's it. No feet. No stride. Just the pure math made visible.

**Constraints/Requirements:**
- Phase must be OUTPUT computed from movement state
- Phase must never DRIVE physics (physics drives phase, not reverse)
- Must support smooth transitions when cycle_length changes (speed tier changes)
- Debug visualization must make phase immediately visible and understandable
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required (≥90%):**
- Controller: 97% ✅
- Spring-Damper: 100% ✅
- Input: 95% ✅

**Enables (will become buildable):**
- Animation systems (consume state + phase for keyframe selection)
- Audio systems (footstep timing from phase)
- Stamina/energy systems (drain based on state)
- Movement effects (particles, trails based on state + phase)
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Speed classifies into discrete locomotion states (walk/run/sprint). Distance traveled calculates phase within cycle. System outputs: current state enum + phase value (0-1). Remove states → no speed variation. Remove phase → no cyclic motion data. Remove distance-based calculation → time-based complexity.

**Why a system, not a feature:**
Phase is a fundamental primitive for cyclic motion. State classification enables gameplay variation (stamina, movement modes). Together they form the mathematical foundation for future locomotion systems (animation, audio, effects) without those systems needing to understand speed or distance directly. They consume state + phase, not raw velocity.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Core equations:**
```
// State classification
speed = velocity.magnitude
if (speed < walk_threshold) state = WALK
else if (speed < run_threshold) state = RUN
else state = SPRINT

// Cycle length per state
cycle_length = state_to_cycle_length[state]  // walk=2.0m, run=3.0m, sprint=4.0m

// Phase calculation (distance-based)
distance_traveled += speed * dt
phase = fmod(distance_traveled, cycle_length) / cycle_length  // 0-1

// Output
locomotion_state { state, phase, cycle_length }
```

**Needs proof:**
- Phase calculation is frame-rate independent (distance accumulation correct)
- Phase wraps cleanly at cycle boundaries (0.99 → 0.0 without jumps)
- State transitions are immediate (discrete, not smoothed)
- Cycle_length change causes phase discontinuity (this is correct - distance is source of truth)

**Resolved decisions (ITERATION_2, ITERATION_3):**
- Speed thresholds: walk < 3 m/s, run < 6 m/s, sprint ≥ 6 m/s
- Cycle lengths: walk=2m, run=3m, sprint=4m (faster = longer stride)
- Phase continuity: Phase RECALCULATES from distance when cycle_length changes (value discontinuous)
- Distance accumulator: Continuous forever (no reset on state change)
- Float precision: Acceptable limitation at ~1e7 meters (10,000 km of travel)
- Output contract: {state, phase, cycle_length} (distance_traveled is internal only)
- Wheel visualization: Uses distance_traveled for rotation angle (continuous), not phase (discontinuous)
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Visual:**
- Box character with color-coded states (walk=green, run=yellow, sprint=red)
- Rotating "+" sign (surveyor wheel) attached to character:
  - Size scales with cycle_length (walk=2m smallest, run=3m medium, sprint=4m largest)
  - Rotates in direction of movement
  - Rotation position shows phase (0° = phase 0, 180° = phase 0.5, 360° = phase 1.0)
- Grid floor for distance reference

**Debug overlay:**
- State: WALK | RUN | SPRINT
- Speed: X.XX m/s
- Phase: 0.XXX (0-1)
- Cycle length: X.XX m
- Distance traveled: XXX.XX m

**Validation:**
- Box color changes instantly when crossing speed thresholds
- Wheel size changes when state changes
- Wheel rotation speed adapts to actual movement speed (distance-based, not time-based)
- Phase wraps cleanly at 1.0 → 0.0
- Can reach all states by varying input magnitude
- Wheel rotates in movement direction (forward/backward/strafe)
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox Scope

**Visual:**
- Core: Colored box character (state-coded), rotating "+" surveyor wheel (phase)
- Debug: ImGui overlay with state/speed/phase/cycle/distance values
- Input: Keyboard WASD primitives (existing controller)

**Included:**
- [x] Implementation plan
- [ ] Core mechanic (state classification, phase calculation)
- [ ] Math implementation (distance accumulation, phase wrapping)
- [ ] Debug visualization (surveyor wheel, color coding)
- [ ] Build verification (compiles, runs, shows data)

**Excluded:**
- Art/models/audio
- Polish/juice
- Optional features (stamina, effects)
- Variations (multiple locomotion modes)

**Complexity:** Medium (3-5 points)
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Approach:**
Add new `locomotion_state` struct to track speed state and phase. Integrate into controller as a separate concern. Phase is calculated FROM velocity (output), never drives velocity (input).

**Files to modify:**

1. **src/character/controller.h** (add state tracking)
   - Add `enum class locomotion_speed_state { walk, run, sprint }`
   - Add `struct locomotion_state { speed_state state; float phase; float distance_traveled; }`
   - Add `locomotion_state locomotion;` member to controller
   - Add thresholds: `float walk_threshold, run_threshold`
   - Add cycle lengths: `float walk_cycle_length, run_cycle_length, sprint_cycle_length`
   - Add helper: `float get_cycle_length(locomotion_speed_state state) const;` (pure function)

2. **src/character/controller.cpp** (implement calculation)
   - In `controller::controller()`: Initialize locomotion state (walk, phase=0, distance=0)
   - In `controller::update()` AFTER velocity integration:
     ```
     // Classify speed state
     float speed = glm::length(math::project_to_horizontal(velocity));
     if (speed < walk_threshold) locomotion.state = walk;
     else if (speed < run_threshold) locomotion.state = run;
     else locomotion.state = sprint;

     // Set cycle length based on state
     switch(locomotion.state) {
       case walk: locomotion.cycle_length = walk_cycle_length; break;
       case run: locomotion.cycle_length = run_cycle_length; break;
       case sprint: locomotion.cycle_length = sprint_cycle_length; break;
     }

     // Accumulate distance traveled
     locomotion.distance_traveled += speed * dt;

     // Calculate phase (0-1)
     locomotion.phase = std::fmod(locomotion.distance_traveled, locomotion.cycle_length)
                        / locomotion.cycle_length;
     ```

3. **src/gui/character_panel.cpp** (debug overlay)
   - Add section after existing tuning sliders
   - Display: state name, speed, phase, cycle_length (computed via helper), distance_traveled
   - Format: state as string, others with 2-3 decimal precision

4. **src/app/debug_generation.cpp** (surveyor wheel visualization)
   - In debug generation function (where velocity trail is drawn):
     ```
     // Draw surveyor wheel at character position
     glm::vec3 wheel_pos = character_pos + glm::vec3(0, 0.5f, 0); // above character
     float cycle_length = controller.get_cycle_length(locomotion.state);
     float wheel_radius = cycle_length * 0.3f; // scale with cycle
     float rotation_angle = locomotion.phase * 2.0f * PI; // phase → radians

     // Draw circle
     debug_draw::circle(wheel_pos, wheel_radius, glm::vec3(1,1,1), math::UP);

     // Draw "+" rotated by phase
     glm::vec3 dir = glm::vec3(cos(rotation_angle), 0, sin(rotation_angle));
     glm::vec3 perp = glm::vec3(-sin(rotation_angle), 0, cos(rotation_angle));
     debug_draw::line(wheel_pos + dir * wheel_radius, wheel_pos - dir * wheel_radius, color);
     debug_draw::line(wheel_pos + perp * wheel_radius, wheel_pos - perp * wheel_radius, color);
     ```

5. **Color-code character by state:**
   - Walk = green (0.2, 1.0, 0.2)
   - Run = yellow (1.0, 1.0, 0.2)
   - Sprint = red (1.0, 0.2, 0.2)
   - Pass color to character rendering

**Call structure:**
```
game_world::update()
  → controller::update(dt)
    → velocity integration (existing)
    → [NEW] classify speed → set state
    → [NEW] accumulate distance
    → [NEW] calculate phase
  → debug_generation::generate()
    → [NEW] read locomotion.state, phase
    → [NEW] draw surveyor wheel
    → [NEW] set character color
  → character_panel::render()
    → [NEW] display locomotion state/phase
```

**Debug data flow:**
1. Controller computes: `speed → state → cycle_length → distance → phase`
2. Debug visualization reads: `state` (for color), `phase` (for wheel rotation), `cycle_length` (for wheel size)
3. GUI reads: all locomotion_state fields for numeric display

**Integration points:**
- **controller::update()** - Add phase calculation AFTER `velocity += acceleration * dt` and AFTER position integration (line ~140, after collision resolution)
- **src/app/debug_generation.cpp** - Add wheel drawing where velocity trail is drawn (existing debug viz area)
- **character_panel.cpp** - Add new ImGui section after existing parameter displays
- **No changes to input handling** - uses existing velocity, no new inputs required

**Validation checkpoints:**
1. Does it compile? (all files build)
2. Does state change with speed? (accelerate/decelerate and watch color)
3. Does phase wrap 0→1? (watch numeric display)
4. Does wheel rotate smoothly? (visual feedback matches motion)
5. Does wheel size change with state? (walk=smallest, run=medium, sprint=largest)
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-19

**Question asked:**
Review the implementation plan for principle adherence (physics drives animation, single source of truth, pure functions), frame-rate independence, radical simplicity, orthogonality, distance accumulation correctness, and integration point concerns.

**Feedback received:**
- ✅ Plan honors "physics drives animation" and single source of truth
- ✅ Phase calculation (`distance += speed * dt`) is frame-rate independent
- ✅ Design stays orthogonal (controller owns truth, others read)
- ✅ Distance accumulation pattern is correct (blocked motion → zero distance → animation stalls naturally)
- ⚠️ Don't cache `cycle_length` in struct—compute as pure function to avoid divergence
- ⚠️ Add divide-by-zero guard for `cycle_length <= 0`
- ⚠️ File path correction: `src/app/debug_generation.cpp` (not `src/rendering/...`)

**Impact on implementation:**
- Remove `cycle_length` field from `locomotion_state` struct
- Add helper function `get_cycle_length(speed_state) -> float` (pure function)
- Add precondition assertion in phase calculation: `FL_PRECONDITION(cycle_length > 0.0f)`
- Correct file path in implementation notes
- All other aspects approved—proceed with confidence
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Graybox Results

**Status:** ✅ Core implemented, build successful
**Date:** 2025-10-19

**Implemented:**
- [x] State classification (walk/run/sprint based on horizontal speed)
- [x] Phase calculation (distance-based, frame-rate independent)
- [x] Pure function `get_cycle_length(state)` (no cached state)
- [x] Surveyor wheel visualization (rotating "+" shows phase, size shows cycle_length)
- [x] Character body color-coding (green=walk, yellow=run, red=sprint)
- [x] ImGui debug panel (state, speed, phase, cycle_length, distance_traveled)
- [x] Precondition assertion (cycle_length > 0)

**Works:**
- ✅ Runs without crashing
- ✅ All 3 states reachable (walk<3, run 3-6, sprint≥6 m/s at default max_speed=8)
- ✅ Phase wraps seamlessly (0→1→0, no visual snap)
- ✅ Character color-coding (green=walk, yellow=run, red=sprint, instant transitions)
- ✅ Surveyor wheel (vertical, bicycle-like, rolls forward)
- ✅ Wheel aligned with character orientation when stationary
- ✅ Wheel size scales with state (walk=smallest, sprint=largest)
- ✅ Debug panel displays all values correctly
- ✅ Thresholds correctly gate states (lowering max_speed prevents sprint)

**Doesn't Work:**
- None identified

**Surprises:**
- Wheel orientation required 3 attempts (horizontal→sideways fan→vertical bicycle)
- Initial cycle lengths backwards (walk>run>sprint, reversed to match physics)
- Wheel rotation fast but physically accurate

**Next:**
- ITERATE: Test edge cases (strafe, backward, collision during state change)
- Consider exposing thresholds in GUI for runtime tuning
- Tune cycle lengths by feel if needed (currently walk=2m, run=3m, sprint=4m)
<!-- END: GRAYBOX/RESULTS -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [x] Core functional in graybox
- [ ] State classification correct at all speeds (needs runtime testing)
- [x] Phase calculation mathematically correct (distance-based, frame-rate independent)
- [x] Phase wraps cleanly at cycle boundaries (via fmod)
- [x] Wheel visualization clearly shows state (size) and phase (rotation)
- [x] Debug visualization makes system behavior immediately obvious
- [x] Can query locomotion state from external systems
- [ ] Principle scores ≥7.0 average (deferred to ITERATE)
<!-- END: SELECT/SUCCESS -->

---

## Iterations

- [walk_run_speed_states_ITERATION_1.md](walk_run_speed_states_ITERATION_1.md) - REVISE
- [walk_run_speed_states_ITERATION_2.md](walk_run_speed_states_ITERATION_2.md) - REVISE
- [walk_run_speed_states_ITERATION_3.md](walk_run_speed_states_ITERATION_3.md) - In Progress
