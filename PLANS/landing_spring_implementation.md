# Landing Spring Implementation Plan

## Overview
Integrate the existing `spring_damper` primitive to create a procedural landing animation. When the character transitions from an airborne to a grounded state, a vertical spring will be activated to simulate a "crouch and recover" motion, eliminating the need for landing keyframes.

**Priority:** High
**Complexity:** Medium
**Expected Duration:** 2-3 hours
**Dependencies:** `spring_damper` primitive, character controller with ground detection.

## Design Rationale

### Why This Feature?
From `physics_implementation_plan.md`:
> "Primary use case for existing `spring_damper` primitive. Eliminates landing keyframes via procedural simulation."

This feature directly supports the core design principle of creating a responsive, physics-driven character controller. It replaces a static animation with a dynamic simulation that naturally responds to the character's fall velocity.

### Alignment with Principles
- **Do No Harm:** The landing spring is a reactive animation that interprets physics state (fall velocity, ground contact) without affecting the controller's position or velocity.
- **Procedural Foundation:** The animation is generated from a simulation, not pre-authored data.
- **Parameters > Assets:** The landing behavior is controlled by tunable parameters (stiffness, damping, impulse scale) rather than keyframes.
- **Elegance Heuristics:**
    - ✅ **Stupidly simple:** "On landing, push a spring down."
    - ✅ **Reused constantly:** Every time the character lands.
    - ✅ **Leverages conventions:** Simulates the real-world physics of absorbing impact.

## Technical Specification

### System Location
The logic will be integrated into the existing `character/animation`, `character/controller`, and `app/runtime` systems.

**Rationale:**
- **`character/controller`:** The controller is the source of truth for ground state.
- **`character/animation`:** The landing spring is a reactive animation, fitting alongside the acceleration tilt logic.
- **`app/runtime`:** The main loop is responsible for orchestrating updates between systems, ensuring a clean data flow from physics to animation.

### Data Structure Changes

**1. `character/controller.h`**
A new boolean flag is needed to track the previous frame's ground state.

```cpp
// character/controller.h
struct controller {
    // ... existing members ...
    bool is_grounded = false;
    bool was_grounded = false; // ADD THIS
    float vertical_velocity_on_land = 0.0f; // ADD THIS
    // ...
};
```

**2. `character/animation.h`**
The `animation_state` will be extended to include the landing spring and its parameters.

```cpp
// character/animation.h
#pragma once
#include <glm/glm.hpp>
#include "foundation/spring_damper.h"

namespace character {

struct animation_state {
    // Acceleration tilt
    glm::vec3 tilt_angles;
    float tilt_smoothing = 8.0f;
    float tilt_magnitude = 0.3f;

    // Landing spring (ADD THIS SECTION)
    spring_damper landing_spring;
    float landing_impulse_scale = 0.5f;

    animation_state();

    void update_acceleration_tilt(glm::vec3 acceleration, float orientation_yaw, float dt);
    void update_landing_spring(bool just_landed, float vertical_velocity, float dt); // MODIFIED
    glm::mat4 get_tilt_matrix() const;
    glm::mat4 get_vertical_offset_matrix() const;
    float get_vertical_offset() const; // ADD THIS for GUI
};

} // namespace character
```

### Implementation Algorithm

**1. Landing Detection (`controller.cpp`)**
In `controller::update()`, we'll detect the transition and store the vertical velocity at the moment of impact.

```cpp
// controller.cpp -> inside controller::update(), after collision resolution
    // ...
    bool just_landed = !was_grounded && is_grounded;
    if (just_landed) {
        vertical_velocity_on_land = velocity.y;
    }

    was_grounded = is_grounded;
    //...
```

**2. Update Orchestration (`app/runtime.cpp`)**
The main loop will trigger the animation update after the controller state is final for the frame.

```cpp
// app/runtime.cpp -> in frame() function
    // ...
    g_character_controller.update(g_scene.get(), dt);
    g_character_orientation.update(g_character_controller, dt);

    // Update reactive animation systems
    bool just_landed = !g_character_controller.was_grounded && g_character_controller.is_grounded;
    g_character_controller.animation.update_landing_spring(
        just_landed,
        g_character_controller.vertical_velocity_on_land,
        dt);
    // ...
```

**3. Spring Activation and Update (`animation.cpp`)**
The `update_landing_spring` function will manage the spring's state.

```cpp
// character/animation.cpp
animation_state::animation_state() {
    landing_spring.stiffness = 400.0f;
    landing_spring.damping = critical_damping(landing_spring.stiffness);
}

void animation_state::update_landing_spring(bool just_landed, float vertical_velocity, float dt) {
    if (just_landed) {
        float impulse = -glm::abs(vertical_velocity) * landing_impulse_scale;
        landing_spring.add_impulse(impulse);
    }

    spring_step step{ .target = 0.0f, .delta_time = dt };
    landing_spring.update(step);
}
```

**4. Rendering Integration (`debug_draw.cpp`)**
The spring's position generates a vertical translation matrix, applied in the correct order.

```cpp
// rendering/debug_draw.cpp -> in draw_character_body()
// ...
glm::mat4 character_transform = glm::translate(glm::mat4(1.0f), controller.position);
character_transform *= orientation.get_rotation_matrix();
character_transform *= controller.animation.get_tilt_matrix();
character_transform *= controller.animation.get_vertical_offset_matrix(); // Correct order
// ...
```

## Implementation Steps

### Step 1: Update Controller State (30 min) ✅
1.  ✅ Add `bool was_grounded` and `float vertical_velocity_on_land` to `controller.h`.
2.  ✅ In `controller.cpp`'s `update()` method, after collision resolution, add logic to detect a landing (`!was_grounded && is_grounded`) and store `velocity.y` in `vertical_velocity_on_land`.
3.  ✅ Update `was_grounded = is_grounded;` at the end of the block.

**Implementation Notes:**
- Added `was_grounded` flag and `vertical_velocity_on_land` member to controller state
- Landing detection occurs after both box and ground collision resolution
- Captures `velocity.y` at moment of impact (negative value = downward speed)
- State update (`was_grounded = is_grounded`) happens after detection to ensure proper edge detection next frame

### Step 2: Extend Animation System (1 hour) ✅
1.  ✅ In `animation.h`, add `spring_damper landing_spring` and `landing_impulse_scale`.
2.  ✅ Update the constructor in `animation.cpp` to configure the spring's default `stiffness` and `damping`.
3.  ✅ Declare and implement `update_landing_spring()`, `get_vertical_offset_matrix()`, and `get_vertical_offset()`.
4.  ✅ Rename `update_tilt` to `update_acceleration_tilt` for clarity.

**Implementation Notes:**
- Added `spring_damper` include to animation.h
- Landing spring members: `spring_damper landing_spring` and `float landing_impulse_scale = 0.5f`
- Constructor initializes spring with stiffness=400.0 and critical damping
- `update_landing_spring()`: Applies impulse on landing, updates spring simulation each frame
- `get_vertical_offset_matrix()`: Returns translation matrix from spring position
- `get_vertical_offset()`: Exposes spring position for GUI display
- Renamed method to `update_acceleration_tilt()` with direct parameters (acceleration, yaw, dt)
- Added legacy `update()` wrapper for backward compatibility with existing code

### Step 3: Orchestrate Updates (30 min) ✅
1.  ✅ In `app/runtime.cpp`, after the `controller` and `orientation` updates, add the call to `animation.update_landing_spring()`.
2.  ✅ Pass the required state (`just_landed`, `vertical_velocity_on_land`, `dt`) from the controller to the animation system.

**Implementation Notes:**
- Added landing spring update in `update_simulation()` after controller and orientation updates
- Landing detection computed locally: `just_landed = !character.was_grounded && character.is_grounded`
- Data flows unidirectionally: Controller state → Animation system via explicit parameters
- Replaced legacy `animation.update()` call with explicit `update_acceleration_tilt()` and `update_landing_spring()` calls
- Update order: physics → orientation → landing spring → acceleration tilt → locomotion
- This ensures landing spring reacts to final grounded state before rendering

### Step 4: Wire to Rendering & GUI (30 min) ✅
1.  ✅ In `rendering/debug_draw.cpp`, apply `get_vertical_offset_matrix()` to the character's transform in the correct order.
2.  ✅ In `gui/character_panel.cpp`, add landing spring controls and state display.

**Implementation Notes:**
- **Character Body Rendering**: Applied `get_vertical_offset_matrix()` to character body transform
  - Transform order: position → orientation → landing offset → tilt
  - Character body now visually crouches when landing spring compresses
- **Spring Visualization**: Updated `draw_physics_springs()` with intuitive visual feedback
  - Bottom endpoint: bottom of collision sphere (acts as "feet")
  - Top endpoint: character body center (affected by spring compression)
  - Uses `generate_spring()` wireframe primitive (8 coils, 0.2m radius)
  - Always visible: subtle gray at rest, bright yellow/orange when compressed
  - Spring represents suspension between collision sphere and body (doesn't stretch during jumps)
- **GUI Controls**: Added "Landing Spring" collapsible panel with:
  - Stiffness slider (100.0 - 1000.0)
  - Damping slider (10.0 - 100.0)
  - Impulse Scale slider (0.1 - 1.5)
  - Read-only spring position display (meters)
  - Read-only spring velocity display (m/s)
- Debug visual provides immediate feedback on spring behavior during tuning

### Step 5: Tune and Polish (1-2 hours) ✅
1.  ✅ Adjust `stiffness`, `damping`, and `landing_impulse_scale` via the character tuning panel.
2.  ✅ Test by jumping from various heights to ensure the animation feels natural and responsive.

**Implementation Notes:**
- ✅ System fully functional - impulse application, spring simulation, rendering, GUI all working
- ✅ Spring visualization refined - bottom at collision sphere base ("feet"), top at body center
- ✅ Spring behavior validated - compresses on landing, extends during recovery, doesn't stretch during jumps
- ✅ GUI controls responsive - all parameters tunable in real-time with immediate visual feedback
- ✅ Default parameters provide good feel (stiffness=400.0, critical damping, impulse_scale=0.5)
- ✅ Spring visual provides clear feedback - subtle gray at rest, bright yellow/orange when compressed

## Principles Review

### ✅ Elegance Heuristics Met
- **Stupidly simple:** "On landing, push a spring down." The core logic is trivial.
- **Reused constantly:** Triggers on every landing.
- **Reactive layer:** Pure visual response; respects the "Do No Harm" principle.
- **Parameters > assets:** Behavior is controlled by three floats, not keyframes.
- **Distinct role:** Manages vertical landing compression; does not overlap with acceleration tilt.

### ✅ Core Tenets Followed
- **Clarity over cleverness:** The data flow is unidirectional and explicit: `Controller` state flows to `Animation` via the `Runtime` orchestrator.
- **Simplicity over sophistication:** Uses the existing `spring_damper` primitive without modification.
- **Graybox before polish:** Integrates with debug rendering and GUI panels for tuning, avoiding premature art asset creation.

### ✅ Procedural Foundation
- **Layer priority correct:** Core physics in the `Controller` is untouched. The animation system is a purely reactive layer that interprets the controller's state.
- **Technique appropriate:** Uses a spring-damper for a state transition (airborne to grounded), which is its ideal use case according to project docs.

## Tuning Guide
- **`landing_spring.stiffness` (recommended: 300.0-800.0):**
    - Lower values: A soft, bouncy landing.
    - Higher values: A stiff, quick recovery.
- **`landing_spring.damping` (recommended: `critical_damping(stiffness)`):**
    - Critically damped provides a smooth recovery with no oscillation.
    - Underdamped (lower value) will cause the character to bounce.
    - Overdamped (higher value) will make the recovery feel sluggish.
- **`landing_impulse_scale` (recommended: 0.3-0.7):**
    - Lower values: A subtle crouch, even from high falls.
    - Higher values: An exaggerated crouch, making landings feel heavy.

## Testing Protocol
1.  **Basic Jump:** Character crouches slightly on landing and quickly recovers.
2.  **High Fall:** Landing crouch is visibly deeper than a standard jump.
3.  **Repeated Jumps:** Spring handles being re-triggered before fully settling.
4.  **Walk off Edge:** Landing triggers correctly.
5.  **No-Op Cases:** Walking/running on flat ground does not trigger the spring. Character does not get stuck.

## Success Criteria
- [x] Character visibly crouches upon landing.
- [x] Crouch depth is proportional to fall velocity.
- [x] Character smoothly returns to an upright position.
- [x] System feels responsive and adds a sense of weight.
- [x] No impact on controller physics.

---

## ✅ IMPLEMENTATION COMPLETE

**Completion Date:** October 5, 2025

**Final Status:** All systems working as designed. Landing spring provides natural, procedurally-generated landing animation that responds proportionally to fall velocity. Spring visualization offers excellent debug feedback. GUI controls enable real-time parameter tuning.

**Key Learnings:**
- **Data flow critical:** Landing detection in controller, flag-based communication to animation system prevented timing bugs
- **Spring visualization matters:** Initial ground-to-body approach stretched during jumps; collision-sphere-to-body correctly represents suspension
- **Debug output invaluable:** Printf debugging revealed landing detection worked but animation system wasn't receiving the signal
- **Architecture validated:** Reactive animation layer pattern proven through second implementation (acceleration tilt was first)

**Next Steps:** Feature complete. Ready for gameplay iteration and playtesting. Consider adding to dependency stack as stable foundation for future reactive systems.