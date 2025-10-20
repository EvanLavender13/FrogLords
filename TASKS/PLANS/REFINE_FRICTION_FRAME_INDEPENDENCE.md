# Refinement: Friction/Acceleration Frame-Rate Independence

Frame-rate dependent terminal velocity violates mathematical foundations and blocks dash mechanic.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-19
**Complexity:** Standard
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/character/controller.cpp:68,151`
**Principle:** Solid Mathematical Foundations, Consistency, Time-Independence | Score: Critical Foundation Error
**Severity:** Critical
**Type:** Sequential update produces frame-rate dependent equilibrium

**Current state:**
```cpp
// Line 68: Apply ground acceleration
velocity.x += ground_accel.x * dt;
velocity.z += ground_accel.z * dt;

// Line 151: Apply friction (later in update)
velocity.x -= velocity.x * drag * dt;
velocity.z -= velocity.z * drag * dt;
```

**Why violation:**
- Sequential update (add acceleration, then subtract friction) produces frame-rate dependent terminal velocity
- Equilibrium occurs at `max_speed - ground_accel * dt`, not `max_speed`
- At 60fps: ~7.67 m/s, at 30fps: ~7.33 m/s
- Unaccounted `drag * ground_accel * dt²` error term
- Violates Time-Independence pattern from PRINCIPLES.md
- Creates hidden frame-rate dependency that compounds over time

**Impact:**
- Blocks: Dash mechanic, any burst movement system requiring overspeed
- Cascades to: All Layer 4 movement variations
- Foundation: 99% → potentially unstable for movement mechanics
- Mathematical correctness violated - equilibrium is not analytically proven
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**CRITICAL:** Describe the approach conceptually. Use pseudocode and high-level explanations. Do NOT write actual source code—that happens during REFINE.

**SIMPLIFY:**

**From:**
- Sequential updates: acceleration then friction
- Two separate operations with dt applied independently
- Frame-rate dependent equilibrium point

**To:**
- Combined friction/acceleration update with analytically proven frame-rate independence
- Single operation deriving exact equilibrium

**Derive/Generalize:**

Derive combined update from differential equation:
```
dv/dt = accel - drag * v
```

Solve analytically for discrete timestep update that:
1. Produces exact equilibrium at `max_speed` regardless of frame rate
2. Eliminates the `drag * accel * dt²` error term
3. Converges smoothly to terminal velocity

The solution involves:
- Deriving closed-form solution for velocity update
- Proving equilibrium point is exactly `accel / drag` (which equals `max_speed`)
- Implementing single update operation instead of sequential

**Check duplicates:**
- Verify no other systems use sequential acceleration/friction pattern
- Ensure tuning parameters remain physically meaningful after derivation
- Validate that existing movement feel is preserved (or improved)

**Validation:**
- Add automated cross-FPS verification tests (30/60/120fps)
- Verify equilibrium velocity matches `max_speed` exactly
- Test convergence behavior at different frame rates
- Ensure debug assertions catch any violations

<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved - frame-rate independent update implemented
- [ ] Principle score improved - mathematical correctness proven
- [ ] Tests passing - cross-FPS verification added
- [ ] No regressions - movement feel preserved

**Metrics:**
- Before: Sequential update, equilibrium varies with dt
- After: Analytical update, equilibrium exact at max_speed
- Mathematical proof: Yes
- Cross-FPS tests: 30/60/120fps verified identical
- Dash mechanic: Unblocked

<!-- END: SELECT/SUCCESS -->

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

### Analysis

**Current physics (frame-rate dependent):**

Sequential update creates frame-rate dependency:
1. Apply acceleration: `velocity += acceleration * dt`
2. Apply friction deceleration: `speed -= friction * |gravity| * dt`
3. Apply speed cap: `speed = min(speed, max_speed)`

**Issue:** System reaches equilibrium only through speed cap, not through balance of forces. Blocks dash mechanic (needs to exceed max_speed, then decay back naturally).

**Solution physics (frame-rate independent):**

Replace with combined drag/acceleration using derived drag coefficient:
- Derive `k = accel / max_speed` (equilibrium guaranteed)
- Apply combined update: `velocity_new = (accel / k) * (1 - exp(-k * dt)) + velocity * exp(-k * dt)`

This produces:
- Exact equilibrium at `accel / k = max_speed`
- Frame-rate independent convergence
- Smooth exponential approach to terminal velocity
- **Dash mechanics enabled:** velocity > max_speed decays naturally back to max_speed

**Key insight:** Remove friction as independent parameter. Derive drag coefficient from accel/max_speed to guarantee equilibrium.

### Step 1: Replace sequential updates with combined horizontal velocity update

**Changes:** `src/character/controller.cpp:132-149` - Replace sequential acceleration+friction with single frame-rate independent update

**Implementation:**
- Remove old horizontal acceleration application from line 136 (vertical gravity stays)
- Remove friction block (lines 139-146)
- Remove speed clamp (line 149) - equilibrium is now mathematically guaranteed
- Calculate drag coefficient: `float k = ground_accel / max_speed`
- Add k≈0 guard: `if (k < 1e-6f) { fallback to v += a*dt }`
- Apply combined horizontal update using exponential formula (see below)
- Keep vertical integration: `velocity.y += gravity * dt`

**Mathematical formula:**
```
For horizontal velocity components (x, z):
k = ground_accel / max_speed  // e.g., 20.0 / 8.0 = 2.5 s⁻¹
a_horizontal = input_direction * ground_accel

if (k < 1e-6f):
    v_horizontal_new = v_horizontal + a_horizontal * dt  // fallback
else:
    decay = exp(-k * dt)
    v_horizontal_new = v_horizontal * decay + (a_horizontal / k) * (1 - decay)

NO CLAMP - equilibrium naturally at max_speed
```

**Equilibrium proof:**
- At equilibrium (dv/dt = 0): `0 = a - k*v_eq`
- Therefore: `v_eq = a/k = (ground_accel) / (ground_accel/max_speed) = max_speed`
- When v > max_speed (dash): exponential decay brings it back to max_speed
- When v < max_speed (accelerating): exponential approach to max_speed

**Tests:** Compile, existing gameplay tests

**Validation:**
- Tests pass
- Movement feels responsive
- No frame-rate dependent behavior

**Runtime verification needed:**
- Run game and verify smooth acceleration to max_speed
- Verify drag brings character to stop when no input (decay to zero)
- Check no jitter or oscillation at equilibrium
- Verify dash mechanic (when re-implemented) exceeds max_speed and decays back

### Step 2: Add zero-velocity tolerance

**Changes:** `src/character/controller.cpp` - Zero out tiny velocities from exponential decay

**Implementation:**
- After combined update, check horizontal speed
- If speed < 0.01 m/s (below perceptible threshold), zero it
- Prevents exponential tail from leaving residual drift
- Document: numerical cleanup for exponential decay

**Tests:** Compile, existing gameplay tests

**Validation:**
- Character fully stops when no input (no drift)
- No perceptible change to movement feel

### Step 3: Add debug assertions for mathematical correctness

**Changes:** `src/character/controller.cpp` - Add postconditions verifying physics invariants

**Implementation:**
- Add assertion: drag coefficient k > 0 (guards against invalid accel/max_speed)
- Add assertion: if no input, horizontal speed decreases or stays at zero
- Document why: validate frame-rate independent behavior
- Use FL_POSTCONDITION with clear messages

**Tests:** Compile, run with assertions enabled

**Validation:**
- Assertions never trigger during normal play
- Clear error messages if violation occurs

### Step 4: Remove friction parameter from controller

**Changes:** `src/character/controller.h` - Delete obsolete friction parameter

**Implementation:**
- Delete `float friction = 0.9f;` and documentation (lines 48-52)
- Verify no remaining references to controller.friction
- Document: drag is now derived from accel/max_speed

**Tests:** Compile, grep confirms no references

**Validation:**
- Code compiles
- No references to controller.friction remain

### Step 5: Simplify tuning - add direct accel, remove time_to_max_speed and friction

**Changes:** `src/character/tuning.h` - Replace time_to_max_speed and friction_coefficient with direct accel

**Implementation:**
- Add `float accel = 20.0f; // m/s²` to tuning_params
- Document: direct physical parameter for horizontal acceleration
- Default 20.0 m/s² matches old time_to_max_speed=0.4s calculation
- Remove `float time_to_max_speed` and documentation (lines 14-18)
- Remove `float friction_coefficient` and documentation (lines 32-42)

**Tests:** Compile

**Validation:** Code compiles, parameter added, obsolete parameters removed

### Step 6: Simplify apply_to() - direct copy, no derivation

**Changes:** `src/character/tuning.cpp` - Remove all derivation logic

**Implementation:**
- Remove time_to_max_speed calculation (lines 17-25)
- Remove friction clamping and assignment (lines 12-14)
- Add direct copy: `c.ground_accel = accel;`
- Keep jump_velocity calculation (still derived from jump_height)
- Keep max_speed and gravity copies

**Tests:** Compile, existing gameplay tests

**Validation:**
- Tests pass
- Movement behavior unchanged (same values)

### Step 7: Remove air_accel special case from controller

**Changes:** `src/character/controller.h` and `controller.cpp` - Unify acceleration

**Implementation:**
- Delete `float air_accel` declaration and documentation from controller.h (lines 75-81)
- In apply_input(): remove conditional `is_grounded ? ground_accel : air_accel` (line 71)
- Replace with: `acceleration = input_direction * ground_accel`
- Document: consistent physics everywhere, no ground/air distinction

**Tests:** Compile, existing gameplay tests

**Validation:**
- Tests pass
- Air movement now matches ground movement (consistent physics)

**Runtime verification needed:**
- Verify air control feels responsive

### Step 8: Rename ground_accel to accel

**Changes:** `src/character/controller.h` and `controller.cpp` - Remove "ground" prefix

**Implementation:**
- Rename `float ground_accel` → `float accel` in controller.h
- Update all documentation references
- Update usage in controller.cpp apply_input()
- Update usage in controller.cpp combined velocity update (Step 1)
- Update usage in tuning.cpp (Step 6)

**Tests:** Compile, existing gameplay tests

**Validation:**
- Tests pass
- All references updated

### Step 9: Update parameter documentation for new physics

**Changes:** `src/character/controller.h` - Document new drag-based physics

**Implementation:**
- Update accel documentation to note direct from tuning (not derived)
- Note equilibrium is mathematically proven at max_speed via k = accel/max_speed
- Reference Time-Independence principle from PRINCIPLES.md
- Document that friction parameter removed (drag now derived)

**Tests:** Documentation review

**Validation:** Documentation accurately reflects implementation

## Rollback

`git reset --hard HEAD~N` where N is number of completed steps, or `git revert` individual commits.

<!-- BEGIN: REFINE/REVIEW -->
## Second Opinion Review

**Tool:** Codex CLI
**Date:** 2025-10-19

**Question asked:**
Review friction/acceleration frame-rate independence refinement plan. Check: 1) Is exponential drag formula mathematically correct? 2) Does removing air_accel align with Consistency? 3) Edge cases or stability concerns? 4) Does direct accel parameter follow Radical Simplicity?

**Concerns evaluated:**
- Mathematical correctness of exponential update formula
- Equilibrium mismatch between accel and max_speed
- Division by zero when k≈0
- Double-application of acceleration
- Whether air_accel removal addresses real violation
- Whether time_to_max_speed removal increases coupling

**Feedback received:**
- ✓ Exponential formula mathematically correct for frame-rate independence
- ⚠ Equilibrium mismatch: accel=20 m/s² with k≈1.1 yields v_eq≈18 m/s, not 8 m/s
- ⚠ Need k≈0 guard for division by k
- ⚠ Need explicit guard for zero-velocity threshold (exponential tail never fully stops)
- ⚠ Plan unclear about removing old acceleration application
- ⚠ air_accel removal questioned (user-requested simplification)
- ⚠ time_to_max_speed removal questioned (user-requested simplification)

**Impact on implementation:**
- **Changed approach after user feedback**: Equilibrium mismatch identified as blocker for dash mechanic
- **Solution**: Derive k = accel/max_speed (not friction-based) to guarantee equilibrium at max_speed
- **Step 1**: Remove speed clamp - equilibrium is mathematically guaranteed, allows dash overspeed
- **Step 1**: Add k≈0 guard with fallback to Euler integration
- **Step 1**: Clarify removal of old acceleration/friction code
- **Step 2**: Zero-velocity tolerance to handle exponential decay tail
- **Step 3**: Assertions for drag coefficient validity and decay behavior
- **Step 4**: Remove friction parameter entirely (no longer needed)
- **Steps 5-9**: User-requested simplifications (direct accel, remove air_accel, remove time_to_max_speed) align with Radical Simplicity
<!-- END: REFINE/REVIEW -->

<!-- BEGIN: REFINE/DISCOVERY -->
## Discovery During Testing

**Emergent Mechanic: Drift Movement**

During Step 1 testing, lowering acceleration (increasing time to max speed to ~2s) creates a racing-game drift effect. The velocity vector lags behind character orientation, producing natural-feeling drift mechanics.

**Why it works:**
- Physics (velocity) and animation (orientation) are independent
- Low acceleration → velocity changes slowly
- Character rotates instantly to face input direction
- Velocity smoothly catches up → drift appearance

**Design potential:**
- Different movement modes (normal vs drift)
- Environmental modifiers (ice, mud)
- Character abilities (drift boost)

**Principle validated:** Emergent Behavior - "magic happens in the space between rules"

**Future exploration:** Consider dedicated drift mechanics exploration after movement foundation complete.
<!-- END: REFINE/DISCOVERY -->
<!-- END: REFINE/PLAN -->
