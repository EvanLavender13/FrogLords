# Acceleration Tilt Implementation Plan

## Overview
Implement procedural tilt animation that reacts to character acceleration, creating a sense of weight and momentum. Character tilts forward when accelerating, backward when braking, and sideways when turning.

**Priority:** High  
**Complexity:** Low  
**Expected Duration:** 2-4 hours  
**Dependencies:** None (uses existing controller state)

## Design Rationale

### Why This Feature?
From GDC Technical Whitepaper:
> "Acceleration Tilt: A procedural tilt is added in the direction of the character's current acceleration. This gives the character a sense of weight and momentum, making it feel more like a dynamic vehicle (akin to a Segway) that leans into its movements."

**Elegance Heuristics:**
- ✅ Stupidly simple (napkin-describable: "tilt toward acceleration")
- ✅ Reused constantly (every frame during movement)
- ✅ Leverages conventions (real physics: objects lean when accelerating)
- ✅ Reactive layer (interprets core state, doesn't control it)
- ✅ Parameters > assets (stiffness/damping, no keyframes)

### Alignment with Principles
- **Do No Harm:** Pure reactive system—never affects controller physics
- **Procedural Foundation:** Animation derives from physics state
- **Simple Core:** Just rotation calculation + smoothing
- **Iteration-Friendly:** Single tunable parameter for feel testing

## Technical Specification

### System Location
Create new file: `src/character/animation.h/cpp`

**Rationale:**
- Dedicated namespace for reactive animation systems
- Clear separation: `controller` owns physics, `animation` owns visual response
- Foundation for future animation features (landing springs, secondary motion)
- Follows dependency flow: Character (physics) → Character (animation)

### Data Structure
```cpp
// character/animation.h
#pragma once
#include <glm/glm.hpp>

namespace character {

struct animation_state {
    // Acceleration tilt
    glm::vec3 tilt_angles;      // Current smoothed tilt (pitch, roll in radians)
    float tilt_smoothing = 8.0f; // Response speed (higher = snappier)
    float tilt_magnitude = 0.3f; // Max tilt angle in radians (~17 degrees)
    
    void update(glm::vec3 acceleration, float dt);
    glm::mat4 get_tilt_matrix() const;
};

} // namespace character
```

### Implementation Algorithm

**Core Logic:**
1. Extract horizontal acceleration from controller (ignore vertical/gravity)
2. Calculate target tilt angles:
   - Forward/back acceleration → pitch rotation
   - Left/right acceleration → roll rotation
3. Smooth current angles toward target (exponential decay)
4. Build rotation matrix from smoothed angles

**Mathematical Approach:**
```cpp
// Forward acceleration tilts forward (negative pitch)
// Right acceleration tilts right (positive roll)

glm::vec3 horizontal_accel = glm::vec3(accel.x, 0.0f, accel.z);
float accel_magnitude = glm::length(horizontal_accel);

if (accel_magnitude > 0.01f) {
    glm::vec3 accel_dir = horizontal_accel / accel_magnitude;
    
    // Map acceleration to tilt angles
    // Forward (Z-) → negative pitch, Right (X+) → positive roll
    float target_pitch = -accel_dir.z * tilt_magnitude * (accel_magnitude / 20.0f);
    float target_roll = accel_dir.x * tilt_magnitude * (accel_magnitude / 20.0f);
    
    // Smooth toward target (exponential decay)
    float blend = 1.0f - glm::exp(-tilt_smoothing * dt);
    tilt_angles.x = glm::mix(tilt_angles.x, target_pitch, blend);
    tilt_angles.z = glm::mix(tilt_angles.z, target_roll, blend);
} else {
    // Return to neutral when no acceleration
    float blend = 1.0f - glm::exp(-tilt_smoothing * dt);
    tilt_angles = glm::mix(tilt_angles, glm::vec3(0.0f), blend);
}
```

**Matrix Generation:**
```cpp
glm::mat4 animation_state::get_tilt_matrix() const {
    // Build rotation matrix: roll around Z, then pitch around X
    glm::mat4 tilt = glm::mat4(1.0f);
    tilt = glm::rotate(tilt, tilt_angles.z, glm::vec3(0, 0, 1)); // Roll
    tilt = glm::rotate(tilt, tilt_angles.x, glm::vec3(1, 0, 0)); // Pitch
    return tilt;
}
```

### Integration Points

**Controller Enhancement:**
```cpp
// controller.h - add member
#include "character/animation.h"

struct controller {
    // ... existing members ...
    character::animation_state animation;
    
    // ... existing methods ...
};

// controller.cpp - update animation state
void controller::update(const scene* scn, float dt) {
    // ... existing physics update ...
    
    // Update reactive animation (after physics resolved)
    animation.update(acceleration, dt);
}
```

**Rendering Integration:**
```cpp
// rendering/scene.cpp or wherever character is drawn
glm::mat4 character_transform = glm::translate(glm::mat4(1.0f), controller.position);
character_transform *= orientation.get_rotation_matrix();
character_transform *= controller.animation.get_tilt_matrix(); // Add tilt
character_transform *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)); // Character scale

// Draw character model with tilted transform
```

### Parameter Tuning Guide

**tilt_smoothing (recommended: 5.0-12.0):**
- Lower values (3.0-6.0): Sluggish, vehicle-like feel
- Mid values (6.0-10.0): Responsive, grounded feel
- High values (10.0-15.0): Twitchy, hyper-responsive feel

**tilt_magnitude (recommended: 0.2-0.5 radians / 11°-29°):**
- Lower values (0.1-0.25): Subtle, realistic
- Mid values (0.25-0.4): Noticeable, game-feel emphasis
- High values (0.4-0.6): Exaggerated, arcade feel

**Acceleration Scaling:**
Current formula divides by 20.0f (matches ground_accel from tuning).
Adjust this constant to match how acceleration maps to visible tilt intensity.

### Testing Protocol

**Visual Verification:**
1. Move forward → character pitches forward
2. Move backward → character pitches backward
3. Strafe right → character rolls right
4. Strafe left → character rolls left
5. Diagonal movement → combined pitch + roll
6. Stop input → smooth return to neutral

**Feel Testing:**
1. Does movement feel more dynamic/weighty?
2. Is tilt response timing natural (not laggy, not instant)?
3. Does tilt magnitude read as intentional (not broken)?
4. Test with different speeds (walk vs run if implemented)

**Edge Cases:**
- Zero acceleration (idle) → no tilt
- Air movement → still tilts (by design, shows intent)
- Wall collision → tilt shows blocked input direction (feature, not bug)

## Implementation Steps

### Step 1: Create Animation System (30 min)
1. Create `src/character/animation.h`
2. Create `src/character/animation.cpp`
3. Add to CMakeLists.txt
4. Implement `animation_state` structure
5. Implement `update()` function with smoothing logic
6. Implement `get_tilt_matrix()` function

### Step 2: Integrate with Controller (15 min)
1. Add `#include "character/animation.h"` to `controller.h`
2. Add `character::animation_state animation;` member to controller
3. Call `animation.update(acceleration, dt);` at end of `controller::update()`
4. Verify compilation

### Step 3: Wire to Rendering (30 min)
1. Locate where character is drawn (likely `scene.cpp` or `renderer.cpp`)
2. Apply `animation.get_tilt_matrix()` to character transform
3. Verify matrix multiplication order (translate → orient → tilt → scale)
4. Test basic rendering

### Step 4: Tune and Polish (1-2 hours)
1. Adjust `tilt_smoothing` until response feels right
2. Adjust `tilt_magnitude` until visual tilt is noticeable but not excessive
3. Test with different movement patterns
4. Iterate until it "feels right"

### Step 5: Add Debug Visualization (Optional, 30 min)
1. Draw acceleration vector from character position
2. Draw target tilt angle as wireframe cone/wedge
3. Add GUI sliders for real-time parameter tuning

## Success Criteria

- [ ] Character visibly tilts in direction of acceleration
- [ ] Tilt smoothly returns to neutral when acceleration stops
- [ ] No impact on controller physics (passes existing movement tests)
- [ ] Tunable parameters accessible and responsive
- [ ] Feels more dynamic than pre-implementation baseline

## Future Extensions (Not in Scope)

- Separate air/ground tilt parameters
- Speed-based tilt scaling (faster = less tilt, like surveyor wheel bounce)
- Tilt affects IK targets (feet shift to compensate for lean)
- Animation blending (tilt affects which locomotion frame is selected)

## Notes

**Why Not Spring-Damper?**
Exponential smoothing is simpler for this use case. Spring-damper adds value for multi-state transitions (stand→crouch) but overkill for continuous value smoothing. If tilt behavior needs more "springy" overshoot later, upgrade to spring_damper.

**Current Rendering Limitation:**
Project currently renders character as colored sphere. Tilt won't be visible until wireframe capsule or mesh is added. However, tilt matrix can be verified via debug draw of local axes. Consider adding simple capsule wireframe before tuning feel.

**Coordinate System:**
Assumes Y-up, Z-forward (OpenGL convention). Verify acceleration vector axes match rendering coordinate space.
