# Vehicle Mass Property System - Iteration 1

---

<!-- BEGIN: ITERATE/INTERNAL_REVIEW -->

## Internal Review

**Strengths:**
1. **Mathematical correctness** - F=ma properly implemented at controller.cpp:173. Mass cancels correctly: `(mass * -GRAVITY) / mass = -GRAVITY`
2. **Single source of truth** - GRAVITY constant established at math_utils.h:13, no duplication
3. **Metadata-driven pattern** - Mass follows existing pattern with mass_meta at tuning.h:68
4. **Full integration** - Mass flows correctly: tuning → controller → GUI → command handler
5. **Validation** - Preconditions and assertions properly applied in tuning.cpp:16-28
6. **Time-independence** - Weight force calculation integrates properly with existing physics

**Code locations verified:**
- math_utils.h:13 - GRAVITY constant (single source of truth)
- tuning.h:26 - Mass parameter with comment
- tuning.h:68 - Mass metadata [50, 500] kg range
- tuning.cpp:55 - Mass copy in apply_to()
- controller.h:62 - Mass state member
- controller.cpp:173 - Weight force F=ma calculation
- parameter_command.h:22 - MASS enum
- vehicle_panel.cpp:33-34 - Mass GUI slider
- runtime.cpp:163-165 - Mass command handler

**Observations:**
- Implementation is mathematically sound and architecturally clean
- No anti-pattern violations detected
- Code follows conventions (snake_case, comments explain why)
- Mass currently has no observable effect (expected - cancels in weight calculation)
- Foundation properly laid for future force-based lateral physics

<!-- END: ITERATE/INTERNAL_REVIEW -->

---

<!-- BEGIN: ITERATE/EXTERNAL_REVIEW -->

## External Review (Codex)

**F=ma Integration:**
- Correct force-to-acceleration mapping for gravity (controller.cpp:169-174)
- Mass validated and nonzero to avoid division hazards (tuning.cpp:16-17, 28)
- Expression intentionally explicit to keep F=ma semantics visible for upcoming lateral force usage

**Single Source of Truth (Gravity):**
- Single constant centralized in foundation (math_utils.h:13)
- Used in g-force calc (math_utils.h:147) and weight calc (controller.cpp:173)
- No stray hardcoded 9.8f in code

**Parameter Metadata Pattern:**
- Metadata lives with data (tuning.h:26, 68)
- GUI uses metadata (vehicle_panel.cpp:33)
- Runtime applies via unidirectional commands (runtime.cpp:163-166)
- Units present and consistent (kg)

**Time-Independence:**
- Horizontal dynamics: exact exponential integrator, frame-invariant (controller.cpp:154-199)
- **Vertical dynamics: semi-implicit Euler (explicit for v, then x += v·dt)**
  - Current: controller.cpp:201-203, 236-237
  - **Issue:** For constant acceleration, semi-implicit Euler is stable but not strictly framerate-identical
  - Exact would use: `x += v_old·dt + 0.5·a·dt²`
  - **Recommendation:** Either add exact vertical position update OR document exception referencing PHYSICS_INTEGRATION_PATTERNS

**Anti-Patterns Check:**
- Single source for configuration: satisfied (GRAVITY centralized)
- Missing invariant enforcement: none found (dt validated, bounds enforced)
- Arbitrary fallback constants: acceptable with rationale (k near-zero fallback guarded)
- Contract/Implementation mismatch: none material
- Dead state: none (mass flows through UI → params → controller → physics)

**Semantic Correctness:**
- Weight treated as force conceptually; acceleration derived via F/m
- No remaining "weight as acceleration" parameter
- Comments and units consistent

**Recommendations:**
1. If strict time-independence required for vertical motion, update position to use `x += v_old·dt + 0.5·a·dt²` for vertical axis
2. Otherwise add exception comment referencing PHYSICS_INTEGRATION_PATTERNS where semi-implicit Euler is default for gameplay stability
3. Optional: Centralize numeric tolerances if they start multiplying (currently acceptable)

<!-- END: ITERATE/EXTERNAL_REVIEW -->

---

<!-- BEGIN: ITERATE/SCOPE -->

## Refinement Scope

**Decision on vertical integration:**
- Read PHYSICS_INTEGRATION_PATTERNS.md - clarifies project policy
- **"Semi-implicit Euler as default for gameplay: stable, simple, fast"**
- Current vertical integration (v_y += a·dt; y += v_y·dt) is CORRECT per project architecture
- External review concern resolved: semi-implicit Euler is documented project standard

**Refinement action:**
1. Add clarifying comment to vertical integration referencing PHYSICS_INTEGRATION_PATTERNS
   - Location: controller.cpp:201-203 (vertical velocity update)
   - Purpose: Document intentional semi-implicit choice, prevent future confusion
   - Reference: TASKS/CONTEXT/PHYSICS_INTEGRATION_PATTERNS.md line 14

**Optional improvements (deferred):**
- Centralizing numeric tolerances - defer until pattern emerges across multiple files
- Current inline rationale acceptable for single-file usage

**Out of scope:**
- Mass implementation architecturally sound, no changes needed
- No refactoring needed for metadata pattern
- No changes to GRAVITY constant handling

<!-- END: ITERATE/SCOPE -->

---

<!-- BEGIN: ITERATE/REFINEMENT -->

## Refinement

**Change: Clarified vertical integration method**
- Added comment documenting semi-implicit Euler choice at controller.cpp:201-205
- References PHYSICS_INTEGRATION_PATTERNS.md as authority for integration policy
- Clarifies intentional choice for stability in damped gameplay systems
- Resolves external review concern about time-independence

**Result:**
- Build succeeded with no errors
- No behavior changes (comment-only addition)
- Code now explicitly documents integration method choice

<!-- END: ITERATE/REFINEMENT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->

## Playtest Session 1

**Tests performed:**
- Baseline behavior verification
- Mass parameter tunability (50-500 kg range)
- Edge case testing at parameter bounds
- Integration validation during varied gameplay

**Results:**
- All tests passed
- Zero assertion failures
- No console errors
- GUI responsive and functional
- Behavior unchanged across mass values (expected - mass cancels in weight)
- System stable at extreme values (50 kg, 500 kg)

**Status:** ✅ Stable - ready for commit

<!-- END: ITERATE/PLAYTEST -->

---
