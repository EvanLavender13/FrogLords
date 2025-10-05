# Character Slow Sink Bug

**Status:** ✅ **RESOLVED**  
**Date:** October 5, 2025  
**Severity:** Gameplay-breaking (affects all landings)

---

## Summary

Character exhibited slow "settling" behavior after landing on flat surfaces. Instead of instant solid contact, the character would visibly sink ~10 cm over approximately 1 second, with debug readouts showing weightlifter penetration gradually increasing from 0.0 m to 0.10 m.

## Reproduction

1. Jump onto any flat platform
2. Observe character settling slowly into the surface over ~1 second
3. Debug panel shows:
   - Weightlifter contact active (should be bumper for flat ground)
   - Penetration starting at 0.0 m, gradually increasing to 0.10 m
   - Bumper contact remains inactive

## Root Cause

**Collision authority misassignment:** The weightlifter sphere was configured to handle **all** upward-facing surfaces (`normal.y > 0.5f`), including perfectly flat horizontal ground where the bumper sphere should have exclusive control.

### Why This Caused Settling

1. Character lands on flat ground
2. Weightlifter sphere detects contact first (`normal.y > 0.5f` includes flat ground)
3. Initial contact reports 0.0 m penetration (sphere just touching surface)
4. Each frame, gravity integration nudges character slightly downward
5. Weightlifter penetration increases gradually toward its 0.10 m burial target
6. Visual result: character "sinks" into surface over multiple frames

The bumper sphere—designed as the **primary collision volume** for clean surfaces—never engaged on flat ground, defeating the Overgrowth-style two-sphere design intent.

### Design Intent (From GDC 2013)

- **Bumper sphere:** Primary collision, handles clean horizontal surfaces, provides responsive solid contact
- **Weightlifter sphere:** Special handling for slopes, edges, step-ups where grip/stability matters

The bug violated this separation of concerns by giving the lifter universal authority over all ground contacts.

## Solution

**Split collision authority by surface normal angle:**

```cpp
// Phase 1a: Bumper collision (flat ground surfaces)
if (col.normal.y > 0.9f) {
    position += col.normal * col.penetration;  // Full push-out
    bumper.center = position;
    weightlifter.center = position + WEIGHTLIFTER_OFFSET;
    // Set grounded, kill velocity...
}

// Phase 1b: Weightlifter collision (sloped/transitional surfaces)
if (col.normal.y > 0.5f && col.normal.y <= 0.9f) {
    // Apply 0.10 m vertical burial logic for stability
    // Only for angled platforms, edges, transitions
}
```

**Threshold justification:**
- `normal.y > 0.9f` → ~25° max slope → flat ground
- `0.5f < normal.y ≤ 0.9f` → 25°-60° slope → transitional terrain
- `normal.y ≤ 0.5f` → walls (handled by bumper sliding phase)

**Additional changes:**
- Updated `resolve_ground_collision()` to test bumper sphere instead of weightlifter
- Adjusted spawn constants: `STANDING_HEIGHT = BUMPER_RADIUS`, `WEIGHTLIFTER_DROP = 0.15f`

### Files Modified

- `src/character/controller.cpp`
  - Split `resolve_box_collisions()` into Phase 1a (bumper/flat) and Phase 1b (lifter/slopes)
  - Updated `resolve_ground_collision()` to use bumper sphere
  - Corrected spawn pose constants

## Validation Results

✅ **Spawn pose:** Bumper center at 0.50 m, weightlifter at 0.35 m  
✅ **Landing on flat ground:** Instant contact, zero drift  
✅ **Bumper reports:** 0.0 m penetration on flat surfaces  
✅ **Jumping:** No sticky ground behavior  
✅ **Ground plane fallback:** Matches box collision behavior  
✅ **Extended standing:** No drift over 10+ seconds  

## Lessons Learned

### Design Principle Validated

**"Simple components, complex interactions"** (from AGENTS.md)

The two-sphere system achieves elegant emergent behavior when each sphere has a **distinct, non-overlapping role**:
- Bumper = primary collision (clean surfaces, instant response)
- Lifter = special handling (slopes, steps, edges, stability)

Initial implementation violated this by granting the lifter authority over all upward surfaces, creating role overlap and subtle emergent bugs. The fix restored proper separation of concerns.

### Overgrowth-Style Collision Confirmed

Character now exhibits the responsive ground contact behavior described in David Rosen's GDC 2013 talk:
- Bumper provides solid, immediate feel on flat ground
- Lifter enables smooth transitions on uneven terrain
- Simple spheres + surface normal thresholds = robust locomotion

### Investigation Notes

Initial hypothesis incorrectly blamed spawn pose misalignment and normal-space penetration calculations. While those adjustments were beneficial, the actual issue was simpler: wrong sphere claiming flat ground authority. Lesson: **test the simplest explanation first before diving into math.**

## Future Work

- Add sloped test geometry to validate weightlifter behavior in the 0.5-0.9 normal.y range
- Implement step-up logic using weightlifter sphere (Phase B of collision system plan)
- Consider multi-iteration resolution for stacked box stability
- Document the 0.9 threshold rationale in code comments for future maintainers
