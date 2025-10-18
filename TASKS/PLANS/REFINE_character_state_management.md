# Refinement: Character State Management

**Unidirectional data flow for character tuning**

---

## Selected

**Date:** 2025-10-18
**Complexity:** Standard (5 points)
**Path:** B (Standard)

---

## Violation

**Location:** `src/gui/character_panel.cpp:17-21`
**Principle:** Consistency | Score: 7/10
**Severity:** High
**Type:** Multiple Truth Sources, Bidirectional Data Flow

**Current state:**
```cpp
// character_panel.cpp - GUI modifies controller directly AND params
bool changed = false;
changed |= gui::widget::slider_float("Max Speed (m/s)", &character.max_speed, 1.0f, 15.0f);
changed |= gui::widget::slider_float("Time to Max Speed (s)", &params.time_to_max_speed,
                                     0.1f, 2.0f);
changed |= gui::widget::slider_float("Jump Height (m)", &params.jump_height, 0.5f, 3.0f);
changed |= gui::widget::slider_float("Gravity (m/s^2)", &character.gravity, -20.0f, -5.0f);
```

```cpp
// tuning.h - Bidirectional flow
struct tuning_params {
    float time_to_max_speed = 0.4f;
    float jump_height = 1.3f;

    void apply_to(controller& c) const;     // params → controller
    void read_from(const controller& c);    // controller → params
};
```

**Why violation:**
- GUI modifies both `controller` directly (`max_speed`, `gravity`) AND `tuning_params` (`time_to_max_speed`, `jump_height`)
- Creates two paths for state changes - violates single source of truth
- Changing `max_speed` bypasses `apply_to()` recalculation → inconsistent derived values
- Bidirectional flow (`apply_to` + `read_from`) creates circular dependency
- No clear ownership: is controller or params the source of truth?

**Impact:**
- Blocks: Clean state management patterns
- Cascades to: Character controller, tuning system, GUI
- Foundation: Layer 3 (Systems) at 96%

---

## Fix

**Approach:** Simplify - Establish unidirectional data flow

**SIMPLIFY:**

**From:** Bidirectional flow with multiple truth sources
- GUI modifies controller fields directly
- GUI modifies tuning_params fields
- tuning_params has both `apply_to()` and `read_from()`
- Unclear which is source of truth

**To:** Unidirectional flow with single source of truth
- GUI modifies ONLY tuning_params fields
- tuning_params is single source of truth for high-level parameters
- After GUI changes, call `params.apply_to(controller)` to update derived values
- Remove `read_from()` - no longer needed

**Changes:**

1. **Add missing fields to tuning_params** (src/character/tuning.h):
   - Add `max_speed` field (currently only in controller)
   - Add `gravity` field (currently only in controller)
   - These become the authoritative high-level parameters

2. **Update apply_to()** (src/character/tuning.cpp):
   - Copy `max_speed` → `controller.max_speed`
   - Copy `gravity` → `controller.gravity`
   - Existing calculations already use `c.max_speed` and `c.gravity`

3. **Remove read_from()** (src/character/tuning.{h,cpp}):
   - Delete function entirely
   - Unidirectional flow - params flows TO controller only

4. **Update GUI** (src/gui/character_panel.cpp):
   - Change `&character.max_speed` → `&params.max_speed`
   - Change `&character.gravity` → `&params.gravity`
   - After any change: `if (changed) params.apply_to(character);`

5. **Update initialization** (wherever controller is created):
   - Create tuning_params with defaults
   - Call `params.apply_to(controller)` to initialize derived values

**Derive/Generalize:**
- `tuning_params` = high-level gameplay parameters (what designers tune)
- `controller` = low-level runtime state (what physics uses)
- Flow: Designer tunes params → `apply_to()` → Derived physics constants
- Pattern: Unidirectional transformation from intent to implementation

---

## Completed

**Date:** 2025-10-18

### Changes Made

**Step 1: Add fields to tuning_params** (`src/character/tuning.h`)
- Added `max_speed` field (8.0 m/s default)
- Added `gravity` field (-9.8 m/s² default)
- These are now the authoritative high-level parameters

**Step 2: Update apply_to()** (`src/character/tuning.cpp`)
- Copy `params.max_speed` → `controller.max_speed`
- Copy `params.gravity` → `controller.gravity`
- Use `params.max_speed` and `params.gravity` in calculations (not controller fields)

**Step 3: Remove read_from()** (`src/character/tuning.{h,cpp}`)
- Deleted `read_from()` function entirely
- Removed bidirectional flow - now unidirectional only
- Updated comments to remove references

**Step 4: Update GUI** (`src/gui/character_panel.cpp`)
- Changed `&character.max_speed` → `&params.max_speed`
- Changed `&character.gravity` → `&params.gravity`
- GUI now modifies tuning_params only (single source of truth)

**Step 5: Update initialization** (`src/app/game_world.cpp`)
- Changed `character_params.read_from(character)` → `character_params.apply_to(character)`
- Initialization now flows params → controller (unidirectional)

### Validation

**Tests:** All passing
- Build: ✓ Successful
- Runtime: ✓ No crashes
- Validation checks: ✓ All passed

**Lines changed:**
- Added: 12 lines (new fields + comments)
- Deleted: 16 lines (read_from function + bidirectional refs)
- Net: -4 lines

**Principle Validation:**

**Principle:** Consistency
**Before:** 7/10 | Violations: Bidirectional flow, multiple truth sources
**After:** 9/10 | Violations: None in this system
**Improvement:** +2 points
**Evidence:**
- Single source of truth: tuning_params owns high-level parameters
- Unidirectional flow: params → controller via apply_to()
- No circular dependencies: read_from() removed
- Clear ownership: params = intent, controller = runtime state

**Verdict:** ✓ Principle restored

### Success Criteria Met

- [x] Violation resolved - bidirectional flow removed
- [x] Principle score improved - Consistency 7/10 → 9/10 (+2)
- [x] Tests passing - build and runtime successful
- [x] No regressions - all validation checks pass

**Metrics:**
- Before: Consistency 7/10, 2 truth sources, bidirectional flow
- After: Consistency 9/10 (+2), 1 truth source, unidirectional flow

**Expected Impact:**
- Layer 3: 96% → 97% (+1%)
- Overall: 95.5% → 96% (+0.5%)
- Cascade: 91.2% → 91.5% (+0.3%)

**Result:** ✓ Violation removed, principle restored

---

## Risk

**Could go wrong:**
- Initialization order - controller might be used before `apply_to()` called
- Existing code might read from controller fields expecting them to be authoritative
- Tests might expect `read_from()` to exist

**Mitigation:**
- Search codebase for all controller initialization points
- Search for `read_from` usage
- Verify tests after changes
- Add assertion in controller that derived values are set (non-zero)

**Rollback:**
```bash
git reset --hard HEAD
# Or if committed:
git revert <commit-hash>
```

---

**One source of truth. Unidirectional flow. Simplicity wins.**
