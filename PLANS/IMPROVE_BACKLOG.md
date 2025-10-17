# Improve Backlog

**A record of principle violations awaiting simplification.**

**Last Review:** 2025-10-16 23:15 (Gemini-Assisted Deep Review)

---

## The Rule

Only track violations that:
1. Break one of the six principles
2. Can be fixed by removal or simplification
3. Have been verified through audit
4. Make the codebase simpler when fixed

---

## Review Summary (2025-10-16 Gemini Deep Review)

**Method:** Gemini CLI full-codebase analysis with Claude interpretation
**Result:** Foundation is solid. Found 3 principle violations requiring attention.

**Files Scanned:** All files in src/ (complete codebase)
**Critical Violations:** 0 (1 deferred - renderer redesign out of scope)
**High Priority:** 2 (mathematical correctness violations)
**Medium Priority:** 1 (architectural simplification)
**Low Priority:** 1 (documentation)

**Key Finding:** Post-simplification baseline is clean. New violations are subtle but real - mathematical correctness (framerate dependency) and consistency (special-case ground plane).

---

## Critical Violations

**NONE.** Post-simplification baseline is clean.

---

## High Priority

### Special-Case Ground Plane Collision
**Principle Violated:** Consistency, Radical Simplicity
**File:** `src/foundation/collision.cpp:60`, `src/foundation/collision.cpp:113`
**Current:** Hardcoded ground plane at y=0 with special-case `resolve_ground_collision()` function
**Problem:** Ground is treated as special case separate from box collision, creating ordering dependency and inconsistency. Special cases violate simplicity.
**Solution:** Remove `resolve_ground_collision()` entirely. Add ground as large AABB to `setup_test_level()` like any other collision object.
**Complexity:** 3 points (remove function, add ground AABB to level setup)
**Why High:** Consistency principle violation. System simpler without special cases.
**Tags:** #consistency #simplification #batch-candidate

---

## Medium Priority

### Duplicated World Geometry Representation
**Principle Violated:** Radical Simplicity, Composable Functions
**File:** `src/app/game_world.h:16`, `src/app/game_world.cpp:76-108`
**Current:** Maintains both `scene scn` (rendering) and `collision_world world_geometry` (physics) with manual duplication in `setup_test_level()`
**Problem:** Two sources of truth for same information. Every collision box added twice: `scn.add_collision_box()` and `world_geometry.boxes.push_back()`. Violates simplicity and composability.
**Solution:** Remove `collision_world` from `game_world`. Make `scene` single source of truth. Physics receives reference to `scene` collision shapes.
**Complexity:** 5 points (architectural change affecting multiple systems)
**Why Medium:** Works correctly now, purely architectural improvement. Requires careful refactoring of physics dependencies.
**Tags:** #architecture #simplification

### Redundant Camera Distance State
**Principle Violated:** Radical Simplicity
**File:** `src/camera/camera.h:70-75`
**Current:** Separate variables for ORBIT mode (`distance`, `min_distance`, `max_distance`) and FOLLOW mode (`follow_distance`, `min_follow_distance`, `max_follow_distance`)
**Problem:** Duplicates state. Mode switch and zoom logic must handle both sets. Unnecessary complexity.
**Solution:** Unify to single `distance` variable with one set of min/max limits. Mode only affects how `center` is updated, not distance management.
**Complexity:** 3 points (refactor camera state and zoom logic)
**Why Medium:** Camera working correctly, simplification not urgent.
**Tags:** #simplification

---

## Low Priority (Cosmetic/Documentation)

### Unexplained Math Constants
**Principle Violated:** Mathematical Foundations (document derivations)
**Files:**
- `src/character/controller.cpp:13-14` - `BUMPER_RADIUS = 0.50f`, `STANDING_HEIGHT = BUMPER_RADIUS`
- `src/character/tuning.cpp:6-7` - `FRICTION_RATIO = 0.75f`, `NET_FRACTION = 1.0f - FRICTION_RATIO`
**Current:** Constants used without derivation explanation
**Solution:** Add comments explaining physical basis or empirical tuning
**Complexity:** 1 point (documentation only)
**Why Low:** Code works, just needs documentation
**Tags:** #documentation

---

## Patterns Verified Clean

**✅ No Circular Dependencies:** Controller → Animation/Orientation, no reverse refs
**✅ No Code Duplication:** No logic appears 3+ times
**✅ No Dead Code:** All files/functions used
**✅ Naming Consistent:** All snake_case
**✅ Layer Violations Clean:** Foundation never imports from character/app
**✅ No TODO/FIXME Comments:** All found and addressed

---

## Batch Opportunities

### Post-Simplification Cleanup (1 item)
**Item:** Unused GLM Headers in Animation System
**Rationale:** Leftover includes from deleted acceleration tilt code
**Solution:** Remove unused quaternion headers
**Complexity:** 1 point
**Tags:** #batch #cleanup

---

## Deferred Violations (Foundation Not Ready)

### Per-Draw Buffer Creation in Renderer
**Principle Violated:** Radical Simplicity (performance is simplicity)
**File:** `src/rendering/renderer.cpp:78`
**Current:** Creates and destroys vertex/index buffers every draw call with `sg_make_buffer()` and `sg_destroy_buffer()`
**Problem:** Critical performance issue. `sg_make_buffer` is heavyweight resource creation, not per-draw operation. Severely degrades with object count.
**Solution:** Implement batching renderer with single dynamic vertex/index buffer per frame. Standard technique.
**Complexity:** 8 points (renderer redesign)
**Why Deferred:** Requires complete renderer architecture change. Out of current scope. Performance acceptable for current object counts.
**Tags:** #performance #architecture #deferred

---

## Recently Completed (Learn From)

**2025-10-16 Fix Framerate-Dependent Yaw Smoothing (2 points):**
- Pattern: Framerate-dependent mathematical formula
- Fixed: Linear lerp replaced with exponential smoothing using exp()
- Solution: `float smoothing_factor = 1.0f - std::exp(-yaw_smoothing * dt);`
- Result: Mathematically correct, framerate-independent orientation smoothing
- Lesson: Mathematical violations are easy to miss but critical for consistency. Exponential smoothing is the correct formula for framerate-independent behavior.

**2025-10-16 Gemini-Assisted Deep Review:**
- Method: Gemini CLI analyzed entire src/ directory against Six Lenses
- Found: 7 violations (1 deferred, 3 accepted, 3 rejected as debug-only)
- Lesson: Gemini excellent for bulk analysis. Claude essential for gate filtering and priority assessment.
- Pattern: Mathematical violations (framerate dependency) easy to miss in manual review

**2025-10-16 Encapsulate Controller Subsystems (3 points):**
- Pattern: Leaky abstraction / responsibility diffusion
- Fixed: game_world reaching into controller internals (orientation, animation, flags)
- Solution: Created `controller::update_reactive_systems(float dt)`
- Result: 83% coupling reduction (6 internals → 1 interface)
- Lesson: Reactive systems should be owned by the entity they react to, not orchestrated externally

**2025-10-16 Radical Simplification removed:**
- Acceleration tilt system (80 lines, coordinate confusion, false validation)
- Contact weight springs (unnecessary complexity)
- Locomotion system (`locomotion.cpp/h` deleted entirely)
- Walk/run blending (unnecessary state management)

**Result:** animation.cpp reduced from 120 → 37 lines (69% reduction)

**Lesson:** Complexity without mathematical validation always fails. False validation (tests passing but not testing production code) is worse than no validation.

---

## Selection Criteria

Before pulling from this backlog:

1. **Is foundation ≥90% certain?** (Check DEPENDENCY_STACK.md) - Raised from 85% post-simplification
2. **Will fixing make things simpler?** (Not just different)
3. **Can we test the fix in isolation?**
4. **Does it serve immediate gameplay?**

If any answer is no, do not select.

---

## The Discipline

This backlog tracks violations, not improvements.
Every item here represents complexity to remove, not features to add.
The best fix is deletion.
The second best is simplification.
Adding is last resort.

**Update when:**
- Audit finds new violations
- Violations are fixed (remove them)
- Foundation certainty changes

**Never add:**
- Feature requests
- "Nice to haves"
- Performance optimizations without proof
- Anything that adds complexity

---

## Current Status

**Foundation Certainty:** 93% (Post-Mathematical-Fix - 2025-10-16)
**Critical Violations:** 0 (1 deferred - renderer needs redesign)
**High Priority:** 1 (consistency violation)
**Medium Priority:** 2 (architectural simplifications)
**Low Priority:** 1 (documentation only)

**Assessment:** Mathematical correctness restored. Special-case ground plane remains as consistency violation.

**Next Actions (Priority Order):**
1. Remove special-case ground plane (3 points) - Consistency + Simplification
3. Consider: Unify camera distance state (3 points) - Simplification
4. Consider: Deduplicate world geometry (5 points) - Architectural (complex)
5. Batch cleanup: Remove unused includes (1 point)
6. Document: Add comments to math constants when time permits (1 point)

**No blockers to new feature development.** Foundation is solid.

**This is the way.**