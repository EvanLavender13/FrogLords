# Refinement: Metadata Range Composition

Make metadata primitives composable by deriving distance range from min/max distance metadata.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-23
**Complexity:** Standard
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/camera/camera_follow.h:28,36,40`
**Principle:** Fundamental Composable Functions, Single Source of Truth
**Severity:** Low
**Type:** Non-composable metadata primitives

**Current state:**
```cpp
static constexpr param_meta distance_meta = {
    "Distance", "m", 1.5f, 15.0f  // Static range
};

static constexpr param_meta min_distance_meta = {
    "Min Distance", "m", 0.5f, 10.0f
};

static constexpr param_meta max_distance_meta = {
    "Max Distance", "m", 5.0f, 30.0f
};
```

**Why violation:**
- distance_meta range [1.5, 15.0] is independent of min_distance_meta and max_distance_meta ranges
- Designer can set min_distance=8.0 and max_distance=12.0, but distance slider still shows [1.5, 15.0]
- Distance slider can display values the system will clamp, creating false affordance
- Metadata primitives cannot inform each other, violating composability

**Impact:**
- Blocks: Derived metadata patterns, composable parameter systems
- Cascades to: Any system using metadata-driven UI with interdependent parameters
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**FROM:** Static independent metadata ranges
**TO:** Derive distance_meta range from min/max metadata during GUI presentation

**Derive/Generalize:**
Metadata-about-metadata pattern: When parameter A is constrained by parameters B and C, derive A's metadata range from B and C's metadata ranges at presentation time, not definition time.

**Check duplicates:**
Audit all param_meta definitions for similar interdependencies (parameter ranges that should derive from other parameters' ranges).
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

### Step 1: Replace static distance_meta with composable helper
**Changes:** `src/camera/camera_follow.h:28-30` - Replace static constexpr with function
**From:**
```cpp
static constexpr param_meta distance_meta = {
    "Distance", "m", 1.5f, 15.0f
};
```
**To:**
```cpp
static inline param_meta make_distance_meta(float min, float max) {
    // GUI requires strict inequality, but runtime allows min == max
    // Add epsilon to max if equal to prevent assertion
    if (min >= max) {
        max = min + 0.01f;
    }
    return {"Distance", "m", min, max};
}
```
**Rationale:** Centralizes label/units (Single Source of Truth), handles equality case, reusable composition (Fundamental Composable Functions)
**Tests:** Build passes
**Validation:** Helper function available as static member

### Step 2: Use helper in tunable param
**Changes:** `src/gui/camera_panel.cpp:50` - Call make_distance_meta with min/max
**From:**
```cpp
if (gui::widget::tunable_param(&distance, camera_follow::distance_meta)) {
```
**To:**
```cpp
if (gui::widget::tunable_param(&distance, camera_follow::make_distance_meta(min_distance, max_distance))) {
```
**Rationale:** Derive metadata from current min/max values at presentation time
**Tests:** Build passes
**Validation:** Distance slider range updates when min/max change

### Step 3: Use helper in readonly display
**Changes:** `src/gui/camera_panel.cpp:32` - Call make_distance_meta with min/max
**From:**
```cpp
gui::widget::readonly_param(cam_follow.distance, camera_follow::distance_meta);
```
**To:**
```cpp
gui::widget::readonly_param(cam_follow.distance, camera_follow::make_distance_meta(cam_follow.min_distance, cam_follow.max_distance));
```
**Rationale:** Readonly display uses same composed metadata as tunable slider
**Tests:** Build passes
**Validation:** Readonly display reflects current min/max constraints

### Step 4: Audit other param_meta for similar interdependencies
**Changes:** Search codebase for static constexpr param_meta
**Check:** Do any other parameter ranges depend on runtime state?
**Location:** All headers with param_meta declarations
**Tests:** N/A (audit only)
**Validation:** Document findings, defer fixes if found

**Audit Results:**
- camera_follow.h: min/max_distance_meta are constraints (not derived), height_offset_meta is independent
- character/tuning.h: All parameters (max_speed, accel, jump_height, gravity) have independent static ranges
- character/controller.h: coyote_window_meta, jump_buffer_window_meta are independent time windows
- **Finding:** No other parameters exhibit distance_meta pattern (valid range constrained by other runtime values)
- **Conclusion:** distance_meta was unique case, no additional fixes needed

## Rollback
`git reset --hard HEAD` to revert all changes
<!-- END: REFINE/PLAN -->

---

<!-- BEGIN: REFINE/REVIEW -->
## Second Opinion Review

**Tool:** Codex CLI
**Date:** 2025-10-23

**Question asked:**
Review this metadata range composition refinement plan. Does it correctly derive distance_meta range from min/max_distance at presentation time? Are there any issues with the approach or steps? Does it follow principles of Single Source of Truth and Fundamental Composable Functions?

**Concerns evaluated:**
- Single Source of Truth for metadata literals
- Handling equality case (min == max) vs GUI strict inequality requirement
- True composability vs inline construction

**Feedback received:**
- Duplicated literals: Removing static distance_meta and inlining construction creates duplicate "Distance"/"m" strings, violating Single Source of Truth
- Equality case: GUI widget requires strict `<` but runtime allows `min_distance == max_distance`, would trigger assertion
- Not composable: Inline struct construction isn't a reusable function, doesn't honor Fundamental Composable Functions principle

**Impact on implementation:**
- Created helper function `make_distance_meta(min, max)` to centralize label/units in camera_follow.h
- Added epsilon guard for equality case: `if (min >= max) max = min + 0.01f`
- Replaced inline construction with reusable static function calls
<!-- END: REFINE/REVIEW -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Changes:**
- `src/camera/camera_follow.h:28-35` - Replaced static constexpr distance_meta with make_distance_meta() helper function
- `src/gui/camera_panel.cpp:32` - Updated readonly display to call make_distance_meta(min, max)
- `src/gui/camera_panel.cpp:50` - Updated tunable slider to call make_distance_meta(min, max)

**Files Modified:**
- src/camera/camera_follow.h: +7 lines (helper function), -3 lines (static metadata)
- src/gui/camera_panel.cpp: +2 lines (two function calls)

**Tests:** Build passing, no unit tests affected

**Manual Verification:**
- Distance slider range updates dynamically when min/max change ✓
- Equality case (min == max) handled with epsilon guard, no crash ✓
- Readonly display uses same composed metadata as tunable slider ✓

**Audit:** No other parameters require runtime-derived metadata ranges

**Result:** ✓ Violation removed
- Metadata primitives now composable (derive distance range from min/max at presentation)
- Single Source of Truth restored ("Distance" label and "m" units centralized in helper)
- Fundamental Composable Functions upheld (reusable make_distance_meta function)
<!-- END: REFINE/COMPLETED -->
