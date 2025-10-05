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

## Progress Log

### 2025-10-05: Step 1 Complete - Animation System Created
**Status:** ✅ Complete

**Files Created:**
- `src/character/animation.h` - animation_state structure with tilt parameters
- `src/character/animation.cpp` - update() and get_tilt_matrix() implementation
- Updated `CMakeLists.txt` to include animation.cpp in build

**Implementation Details:**
- Exponential smoothing for tilt angles (tilt_smoothing = 8.0f)
- Default tilt magnitude of 0.3 radians (~17 degrees)
- Horizontal acceleration mapped to pitch/roll
- Returns to neutral when acceleration near zero
- Matrix generation uses roll then pitch order

**Build Status:** Compiled successfully with no errors

**Next Steps:** Proceed to Step 2 - Integrate with Controller

### 2025-10-05: Step 2 Complete - Controller Integration
**Status:** ✅ Complete

**Files Modified:**
- `src/character/controller.h` - Added animation.h include and animation_state member
- `src/character/controller.cpp` - Added animation.update() call after physics resolution

**Implementation Details:**
- Added `character::animation_state animation;` member to controller struct
- Called `animation.update(acceleration, dt);` at end of update() method
- Update happens after all physics resolved but before acceleration reset
- Animation system now receives controller's acceleration each frame

**Build Status:** Compiled successfully with no errors

**Next Steps:** Proceed to Step 3 - Wire to Rendering

### 2025-10-05: Step 3 Complete - Rendering Integration
**Status:** ✅ Complete

**Files Modified:**
- `src/rendering/debug_draw.h` - Added draw_character_body() declaration
- `src/rendering/debug_draw.cpp` - Implemented character body visualization with tilt
- `src/app/runtime.cpp` - Added draw_character_body() call to render loop

**Implementation Details:**
- Created tall rectangular box (0.4w × 0.8h × 0.3d) as character body stand-in
- Transform order: translate → orient (yaw) → tilt → vertices
- Applied tilt matrix from animation system between orientation and rendering
- Distinctive magenta color for character body visibility
- Vertices transformed in world space before rendering

**Technical Approach:**
- Generated box mesh with `generate_box()`
- Built composite transform matrix manually
- Applied transform to vertices directly (since mesh.get_model_matrix() doesn't support custom matrices)
- Character body drawn before other debug visualizations for proper layering

**Build Status:** Compiled and ran successfully - character body with tilt now visible

**Next Steps:** Proceed to Step 4 - Tune and Polish (test movement and adjust parameters)

### 2025-10-05: Bug Fix - Character-Local Space Transformation
**Status:** ✅ Fixed

**Problem Identified:**
Tilt only worked correctly when moving forward (W key). When character rotated, tilt directions became incorrect because acceleration was in world-space, not character-local space.

**Root Cause:**
Animation system was using world-space acceleration (x, z) directly to calculate tilt. When character faced different directions, world-space acceleration didn't match character's local forward/right axes.

**Solution Implemented:**
1. **Added orientation_yaw parameter** to `animation_state::update()`
2. **Transform world-space to local-space** - Rotate acceleration by -yaw to get character's frame of reference
3. **Preserved acceleration value** - Added `last_acceleration` member to controller since acceleration resets before animation updates
4. **Moved animation update** - Called from `runtime.cpp` after orientation updates instead of inside controller

**Files Modified:**
- `src/character/animation.h` - Added orientation_yaw parameter
- `src/character/animation.cpp` - Transform acceleration to character-local space using yaw rotation
- `src/character/controller.h` - Added last_acceleration member
- `src/character/controller.cpp` - Save acceleration before reset, removed animation update call
- `src/app/runtime.cpp` - Call animation.update() after orientation updates with yaw parameter

**Technical Details:**
```cpp
// Transform world-space acceleration to character-local space
float cos_yaw = std::cos(-orientation_yaw);
float sin_yaw = std::sin(-orientation_yaw);
float local_forward = horizontal_accel.z * cos_yaw - horizontal_accel.x * sin_yaw;
float local_right = horizontal_accel.x * cos_yaw + horizontal_accel.z * sin_yaw;
```

**Result:** Tilt now correctly responds to character-relative movement regardless of facing direction. Forward acceleration always pitches forward, right acceleration always rolls right, etc.

**Build Status:** Compiled and tested successfully with all movement directions

**Next Steps:** Proceed to Step 4 - Tune and Polish (parameters should now be more evaluable with correct behavior)

### 2025-10-05: Step 4 Complete - Tune and Polish
**Status:** ✅ Complete

**Tuning Results:**
- Default parameters (tilt_smoothing=8.0, tilt_magnitude=0.3) feel natural and dynamic
- Tilt response timing is immediate but smooth - not laggy or twitchy
- Visual magnitude is noticeable without being exaggerated
- Works correctly in all directions (forward/back/left/right)
- Smooth return to neutral when stopping

**Testing Performed:**
- ✅ Forward movement - pitches forward correctly
- ✅ Backward movement - pitches backward correctly  
- ✅ Strafe right - rolls right correctly
- ✅ Strafe left - rolls left correctly
- ✅ Diagonal movement - combined pitch + roll
- ✅ Stop input - smooth return to neutral
- ✅ Direction changes while moving - responsive
- ✅ Wall collisions - tilt shows intent (feature, not bug)

**Feel Assessment:**
Character now has noticeable weight and momentum. The "Segway lean" effect successfully conveys acceleration intent. Movement feels more dynamic and alive compared to pre-implementation baseline.

**No Further Tuning Required:** Current parameters achieve design goals.

## Principles Review

### ✅ Elegance Heuristics Met
- **Stupidly simple:** 45 lines of code, napkin-describable ("tilt toward acceleration")
- **Reused constantly:** Every frame during movement
- **Leverages conventions:** Real physics - objects lean when accelerating
- **Reactive layer:** Pure visual response, never affects physics
- **Parameters > assets:** Two floats control all behavior, no keyframes
- **Distinct role:** First animation system - no overlap with existing systems

### ✅ Core Tenets Followed
- **Clarity over cleverness:** Straightforward math, clear variable names
- **Simplicity over sophistication:** Exponential smoothing beats spring-damper complexity
- **Iteration over planning:** Fixed coordinate space bug through testing, not upfront analysis
- **Graybox before polish:** Using rectangular box stand-in, not premature mesh work

### ✅ Procedural Foundation
- **Layer priority correct:** Core physics untouched, reactive system interprets state
- **Do No Harm:** Animation never modifies controller state
- **Parameters drive behavior:** tilt_smoothing/magnitude tune feel without code changes
- **Technique appropriate:** Exponential smoothing for continuous value interpolation

### ✅ Implementation Quality
- **Consistent naming:** snake_case throughout (`animation_state`, `tilt_angles`)
- **Proper namespace:** `character::` for domain-specific system
- **Clean separation:** Physics in controller, animation in animation, rendering in debug_draw
- **Dependency flow:** Foundation → Character (physics) → Character (animation) → Rendering

### 🎯 Serendipity Captured
Discovered coordinate space issue through testing - acceleration was world-space when it needed to be character-local. Solution emerged from observing "tilt works forward but not sideways" behavior. This is design through discovery, not pure authorship.

### 📊 Cascading Uncertainty Impact
- **Foundation solidified:** Animation system architecture now proven
- **Confidence increased:** 90%+ certainty this approach scales to landing springs, secondary motion
- **Backlog ready:** Can now pull "Spring-Damper Landings" with confidence
- **No restrictions added:** Tilt system imposes zero constraints on future art/level/mechanics

## Success Criteria

- [✅] Character visibly tilts in direction of acceleration
- [✅] Tilt smoothly returns to neutral when acceleration stops
- [✅] No impact on controller physics (passes existing movement tests)
- [✅] Tunable parameters accessible and responsive
- [✅] Feels more dynamic than pre-implementation baseline

### 2025-10-05: Step 5 Complete - Debug Visualization
**Status:** ✅ Complete (Simplified)

**Visualization Implemented:**
- Tall rectangular box (0.4w × 0.8h × 0.3d) serves as character body stand-in
- Magenta wireframe color for easy identification
- Tilt clearly visible during all movement directions
- Transform chain properly visualizes: position → orientation → tilt

**Scope Decision:**
Debug GUI sliders deemed unnecessary - would add clutter without value. Current box visualization is sufficient for:
- Verifying tilt correctness (all directions work)
- Judging tilt magnitude (visually appropriate)
- Testing smoothing response (feels natural)

**Rationale:**
Parameters already well-tuned (tilt_smoothing=8.0, tilt_magnitude=0.3). No need for real-time adjustment UI. If future tuning needed, can modify code directly (parameters > GUI complexity). Aligns with "Graybox before polish" and "Simplicity over sophistication" principles.

**Acceleration Vector Visualization:** Not implemented - tilt box provides sufficient visual feedback. Adding vector would clutter view without adding insight.

---

## Implementation Complete

**Total Duration:** ~3 hours (within 2-4 hour estimate)

**All Steps Complete:**
1. ✅ Create Animation System
2. ✅ Integrate with Controller  
3. ✅ Wire to Rendering
4. ✅ Tune and Polish
5. ✅ Debug Visualization (Simplified)

**Deliverables:**
- `src/character/animation.h` - Animation state structure
- `src/character/animation.cpp` - Tilt calculation and smoothing
- Character body visualization with tilt transform
- Fully tested across all movement directions
- Parameters tuned for natural feel

**Ready for:** Next procedural animation feature (spring-damper landings, secondary motion)

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
