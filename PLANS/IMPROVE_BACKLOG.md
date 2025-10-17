# Improve Backlog

**A record of principle violations awaiting simplification.**

**Last Review:** 2025-10-16 22:43 (Post-Radical Simplification)

---

## The Rule

Only track violations that:
1. Break one of the six principles
2. Can be fixed by removal or simplification
3. Have been verified through audit
4. Make the codebase simpler when fixed

---

## Review Summary (2025-10-16 Post-Simplification)

**Result:** Codebase is remarkably clean after radical simplification.

**Files Scanned:** 21 .cpp files, 25 .h files (46 total)
**Critical Violations:** 0
**High Priority:** 0
**Medium Priority:** 2 (minor simplifications)
**Low Priority:** 0

**Key Finding:** The simplification was effective. Most violations from previous audit were addressed by deletion.

---

## Critical Violations

**NONE.** Post-simplification baseline is clean.

---

## High Priority

**NONE.**

---

## Medium Priority

### Game World Direct Orientation Update
**Principle Violated:** Composable Functions (leaky abstraction)
**File:** `src/app/game_world.cpp:56`
**Current:** `game_world::update()` directly calls `character.orientation.update(intended_velocity, dt)`
**Problem:** Game world reaches into character's orientation subsystem
**Solution:** Move to `controller::update()` or create `controller::update_orientation()` wrapper
**Complexity:** 2 points
**Why Medium:** Works but violates encapsulation - controller should manage its own subsystems
**Tags:** #architecture #encapsulation

### Game World Direct Animation Update
**Principle Violated:** Composable Functions (leaky abstraction)
**File:** `src/app/game_world.cpp:58-60`
**Current:** `game_world::update()` directly calls `character.animation.update_landing_spring()` and manages `just_landed` flag
**Problem:** Game world reaches into character's animation subsystem
**Solution:** Move to `controller::update()` or create `controller::update_animation()` wrapper
**Complexity:** 2 points
**Why Medium:** Works but violates encapsulation - controller should manage its own subsystems
**Tags:** #architecture #encapsulation #batch-candidate

**NOTE:** These two items could be batched - both involve game_world reaching into controller subsystems. Consider single "Encapsulate Controller Subsystems" improvement.

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

### Encapsulate Controller Subsystems (2 items)
**Combines:** Game World Direct Orientation Update + Game World Direct Animation Update
**Rationale:** Both are game_world reaching into controller subsystems
**Solution:** Create `controller::update_reactive_systems(bool just_landed, float vertical_velocity, float dt)` to encapsulate orientation and animation updates
**Complexity:** 3 points (combined)
**Tags:** #batch #architecture

### Post-Simplification Cleanup (1 item)
**Item:** Unused GLM Headers in Animation System
**Rationale:** Leftover includes from deleted acceleration tilt code
**Solution:** Remove unused quaternion headers
**Complexity:** 1 point
**Tags:** #batch #cleanup

---

## Recently Removed (Learn From)

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

**Foundation Certainty:** 90% (Post-Radical Simplification)
**Critical Violations:** 0
**High Priority:** 1 (unused includes from incomplete cleanup)
**Medium Priority:** 2 (encapsulation leaks in game_world)
**Low Priority:** 1 (documentation only)

**Assessment:** Codebase is clean. Simplification was effective.

**Next Actions:**
1. Batch cleanup: Remove unused includes (1 point)
2. Consider: Encapsulate controller subsystems if game_world complexity increases (3 points)
3. Document: Add comments to math constants when time permits (1 point)

**No blockers to new feature development.** Foundation is solid.

**This is the way.**