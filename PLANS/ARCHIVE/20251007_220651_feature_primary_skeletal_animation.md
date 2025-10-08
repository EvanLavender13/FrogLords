# Primary Skeletal Animation (Locomotion-Driven Limb Cycles)

**Status:** Clarification Complete - Ready for Implementation Planning

---

## OVERVIEW

Distance-phased arm swing and leg stride synchronized to surveyor-wheel locomotion. Limbs animate proportional to distance traveled. Extends proven surveyor-wheel pattern from body bounce to skeletal joints—all animation driven by `locomotion.distance_traveled`.

**Unlocks:** Secondary motion, IK foot placement, limb reactions.

**Certainty:** ~80%. Direct extension of proven system; risk is tuning time, not architecture.

---

## KEYFRAME SYSTEM (RUN GAIT)

4 distinct keyframes: REACH-RIGHT → PASS-LEFT → REACH-LEFT → PASS-RIGHT (continuous cycle).  
Phase-driven by `locomotion.distance_traveled`.  
No interpolation—strict keyframe switching.

**Note:** Walk gait deferred to second iteration.

---

## VISUAL REFERENCES (GDC Talk - Bunny Animation)

### Terminology

**REACH:** Limbs at maximum extension. One leg fully forward, one back. Arms opposite to legs (left arm forward = right leg forward). Elbows locked 90°.

**PASS:** Legs crossing at body center. One knee bent 90° (will extend forward next), other leg extending. Transitional moment between REACH poses.

---

### Run Gait - 4 Keyframes

**Full cycle:** REACH-RIGHT → PASS-LEFT → REACH-LEFT → PASS-RIGHT → (repeat)

#### Frame 1: REACH-RIGHT
```
Right leg: Extended forward (40° hip rotation)
Left leg: Extended back, knee bent (80° knee bend)
Right arm: Back (50° shoulder behind body)
Left arm: Forward (50° shoulder ahead of body)
Elbows: Locked 90° (both arms)
Torso: Slight forward lean
```

#### Frame 2: PASS-LEFT  
```
Right leg: Moving back, beginning to extend
Left leg: At center, knee bent 90° (preparing to swing forward)
Arms: Transitioning through neutral
Torso: Upright
```

#### Frame 3: REACH-LEFT
```
Left leg: Extended forward (40° hip rotation)
Right leg: Extended back, knee bent (80° knee bend)
Left arm: Back (50° shoulder behind body)
Right arm: Forward (50° shoulder ahead of body)
Elbows: Locked 90°
Torso: Slight forward lean
```

#### Frame 4: PASS-RIGHT
```
Left leg: Moving back, beginning to extend
Right leg: At center, knee bent 90° (preparing to swing forward)
Arms: Transitioning through neutral
Torso: Upright
```

**Key Insight:** Frames 2 and 4 are distinct—opposite knee is bent in each.

---

### Limb Coordination Rules

**Opposite coordination (always):**
- Left arm forward → Right leg forward
- Right arm forward → Left leg forward

**Phase relationships:**
- Left/right arms: 180° out of phase
- Left/right legs: 180° out of phase
- Arms vs same-side legs: 180° out of phase

**Visual correctness test:** If left leg is forward, right arm must be forward.

---

## RESOLVED CLARIFICATIONS

### Joint Configuration
✅ Shoulders: Forward/back rotation (sagittal plane)  
✅ Elbows: Locked at 90° for run (constant)  
✅ Hips: Forward/back rotation (sagittal plane)  
✅ Knees: Bend varies per keyframe  
✅ Opposite limb coordination: Left arm forward = right leg forward  

### Keyframe Structure
✅ Run: 4 distinct keyframes (REACH-RIGHT → PASS-LEFT → REACH-LEFT → PASS-RIGHT)  
✅ REACH: Maximum stride, arms/legs at opposite extremes  
✅ PASS: Transitional moment, one knee bent 90°, legs at centerline  
✅ Each keyframe explicitly defines all joint angles  

### Implementation Approach
✅ Phase-driven: Surveyor wheel advances through keyframe cycle via `locomotion.distance_traveled`  
✅ No interpolation: Strict keyframe switching based on phase thresholds  
✅ Direct control: Set joint local transforms from current keyframe values  
✅ Unmentioned joints: Spine, head, hands, feet remain in T-pose  
✅ Stopping behavior: Limbs stop instantly with body  
✅ Implementation order: Run gait only (walk gait is second iteration)  
✅ Tuning: GUI with per-limb sliders, pose lock, phase scrubber, reference overlay  

### Success Criteria
✅ Visual coherence: Limbs match body speed (no sliding/skating)  
✅ Keyframe switching: Clean transitions between discrete poses  
✅ Phase accuracy: Arms/legs maintain opposite coordination  
✅ Test scenarios: Running in circles, straight lines, varying speeds, starting/stopping  

---

## IMPLEMENTATION

### Scope (Run Gait Only)

**Keyframes:** REACH-RIGHT, PASS-LEFT, REACH-LEFT, PASS-RIGHT (hardcoded)

**Animated joints:**
- Shoulders: forward/back rotation (sagittal plane)
- Hips: forward/back rotation (sagittal plane)
- Knees: bend angle (varies per keyframe)
- Elbows: locked at 90° (constant)

**Phase system:**
- Phase 0.00-0.25: REACH-RIGHT
- Phase 0.25-0.50: PASS-LEFT  
- Phase 0.50-0.75: REACH-LEFT
- Phase 0.75-1.00: PASS-RIGHT

**Switching:** Instant transition at phase thresholds (no interpolation)

---

### Initial Angle Estimates

**REACH-RIGHT:**
- Right shoulder: -50° (back), Left shoulder: +50° (forward)
- Right hip: +40° (forward), Left hip: -40° (back)
- Right knee: 170° (nearly straight), Left knee: 100° (bent)

**PASS-LEFT:**
- Right shoulder: +20°, Left shoulder: -20°
- Right hip: +10°, Left hip: 0° (at center, knee bent)
- Right knee: 150°, Left knee: 90° (bent 90°)

**REACH-LEFT:**
- Right shoulder: +50° (forward), Left shoulder: -50° (back)
- Right hip: -40° (back), Left hip: +40° (forward)
- Right knee: 100° (bent), Left knee: 170° (nearly straight)

**PASS-RIGHT:**
- Right shoulder: -20°, Left shoulder: +20°
- Right hip: 0° (at center, knee bent), Left hip: +10°
- Right knee: 90° (bent 90°), Left knee: 150°

**Note:** Positive = forward, Negative = backward. Starting estimates for GUI refinement.

---

### GUI Controls

- Keyframe selector: Dropdown (REACH-RIGHT, PASS-LEFT, REACH-LEFT, PASS-RIGHT)
- Pose lock: Freeze animation at selected keyframe
- Phase scrubber: Manually advance through cycle
- Per-limb angle sliders: Right/Left Shoulder, Right/Left Hip, Right/Left Knee
- Reference overlay: Display reference image for comparison

**Tuning workflow:**
1. Select keyframe, lock pose
2. Load reference overlay
3. Adjust sliders to match reference
4. Repeat for all keyframes
5. Unlock and preview full cycle

---

### Code Structure

**Data:**
```cpp
struct keyframe {
    float right_shoulder, left_shoulder;
    float right_hip, left_hip;
    float right_knee, left_knee;
};

keyframe run_frames[4]; // REACH-RIGHT, PASS-LEFT, REACH-LEFT, PASS-RIGHT
```

**Update logic:**
```cpp
float phase = (locomotion.distance_traveled * frequency) % 1.0;
int frame_index = (int)(phase * 4.0f);

keyframe current = run_frames[frame_index];

// Apply to skeleton
skeleton.joints[RIGHT_SHOULDER].local_rotation = rotate_x(current.right_shoulder);
// ... etc
```

**Location:** `character::animation::update()` after orientation, before reactive systems

**Integration points:**
- `character::animation::update()` - main update loop
- `character::tuning` - keyframe angle parameters
- `gui::character_panel` - animation controls

**Estimate:** ~80-120 lines total

---

## TEST PROTOCOL

**Verify:**
- Limbs match body speed (no sliding/skating)
- Arms/legs maintain opposite coordination
- Clean keyframe switching (discrete pose changes)
- Instant stopping with body

**Test scenarios:**
- Straight line running
- Circular running
- Start from standstill
- Abrupt stops

**Iterate:** Self-test with GUI adjustments, compare against reference images.

---

## DEPENDENCIES

**Required (stable):**
- ✅ `locomotion.distance_traveled` (surveyor-wheel pattern)
- ✅ `skeleton` system with joint local transforms
- ✅ GUI panel infrastructure

**Blocks (waiting on this):**
- ❌ Secondary motion (bone softness, follow-through)
- ❌ IK foot placement
- ❌ Reactive limb animations (landings, impacts)
- ❌ Equipment/weapon holding poses

---

## DEFERRED TO SECOND ITERATION

- Walk keyframes (NEUTRAL, STRIDE-RIGHT, STRIDE-LEFT)
- Modified phase logic for repeating NEUTRAL frame
- `walk_factor` blending between walk/run
- Relaxed elbow angle for walk gait

---

## DESIGN NOTES

**Why 4 keyframes (not 2)?**  
PASS poses are explicit keyframes, not interpolated. Legs crossing with 90° knee bend is a distinct pose—without it, legs interpolate incorrectly between REACH poses.

**Why distance-phased (not time-phased)?**  
Ensures limb motion matches body movement speed. No sliding/skating artifacts. Character standing still = limbs frozen mid-cycle (intentional for graybox). Follows existing surveyor-wheel pattern.

**Why opposite limb coordination?**  
Natural human/animal locomotion. Balances momentum and center of gravity. Breaking this rule looks immediately wrong.
