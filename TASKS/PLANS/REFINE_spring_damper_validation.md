# Refinement: Spring-Damper Validation

**Add unit tests to mathematically validate critical damping formula**

---

## Selected

**Date:** 2025-10-17
**Complexity:** Standard (3-5 points)
**Path:** B (standard - full workflow)

---

## Violation Details

**Location:**
- File: `src/foundation/spring_damper.cpp`
- Lines: 24-34 (critical_damping function)
- System: Primitives (Layer 2)

**Principle Violated:**
- Primary: Solid Mathematical Foundations
- Score before: 9.8/10

**Severity:** High

**Type:** Unproven (critical damping formula untested)

---

## Current State

**What exists now:**
```cpp
float critical_damping(float stiffness, float mass) {
    // DERIVED: Critical damping formula from harmonic oscillator theory
    // Equation: c = 2√(k·m)
    // Derivation:
    //   - Spring-damper ODE: mẍ + cẋ + kx = 0
    //   - Characteristic equation: mλ² + cλ + k = 0
    //   - Critical damping (ζ=1): Discriminant = 0 → c² = 4km → c = 2√(km)
    // Coefficient: 2.0 is the mathematical constant from the derivation
    // Result: Fastest return to equilibrium without overshoot
    return 2.0f * sqrtf(stiffness * mass); // DERIVED coefficient: 2.0 (dimensionless)
}
```

**Why this violates principles:**
- Formula is documented but never validated with tests
- Mathematical correctness unproven in practice
- No verification that ζ=1 actually produces no overshoot
- Foundation primitive should have highest certainty (100%)

**Impact:**
- Blocks: Full confidence in Layer 2 (Primitives)
- Cascades to: Landing animation, future spring-based systems
- Foundation impact: Layer 2 at 98% instead of 100%

---

## Fix Approach

**Primary approach:** Document (add validation tests)

**Test Strategy:**

**Test 1: Critical Damping Formula Verification**
- Given: k=100, m=1
- Calculate: c = 2√(100·1) = 20
- Verify: ζ = c/(2√(km)) = 20/(2√(100·1)) = 1.0
- Assert: Damping ratio equals exactly 1.0

**Test 2: No Overshoot Behavior**
- Setup: Spring at position=0, target=10, with critical damping
- Simulate: 100 steps over 2 seconds
- Verify: Position never exceeds target (no overshoot)
- Verify: Position monotonically approaches target

**Test 3: Settling Time**
- Setup: Spring displaced from target
- Measure: Time to settle within 1% of target
- Verify: Matches theoretical prediction for ζ=1

**Test 4: Parameter Ranges**
- Test critical_damping() with various k and m values
- Verify formula correctness across ranges:
  - k: [1, 10, 100, 1000]
  - m: [0.5, 1.0, 2.0, 10.0]

**Test 5: Underdamped vs Critical vs Overdamped**
- Setup three springs: ζ<1, ζ=1, ζ>1
- Verify: Only critical shows no overshoot + fastest settle
- Verify: Underdamped overshoots
- Verify: Overdamped slower than critical

**Implementation:**
- Location: Create `tests/foundation/test_spring_damper.cpp`
- Framework: Decide between catch2, doctest, or simple asserts
- Integration: Add to CMakeLists.txt
- Run: Include in build script validation

---

## Success Criteria

**This refinement succeeds when:**
- [x] Critical damping formula mathematically verified
- [x] No overshoot behavior confirmed
- [x] Tests pass across parameter ranges
- [x] Tests integrated into build system
- [x] All existing tests still passing
- [x] No regressions in landing animation

---

## Estimated Metrics

**Before:**
- LOC: 35 (spring_damper.h + .cpp)
- Principle score: 9.8/10
- Test coverage: 0%
- Validation: Documentation only

**After (estimated):**
- LOC: 35 + ~150 test code
- Principle score: 10/10 (+0.2)
- Test coverage: 100% for critical_damping()
- Validation: Mathematical proof + empirical tests

**Foundation Impact:**
- Layer 2: 98% → 100% (+2%)
- Overall: 90% → 91% (+1%)

---

## Risk Assessment

**What could go wrong:**
1. Tests reveal formula is incorrect
   - Mitigation: Formula is from harmonic oscillator theory (proven)
   - Likely: Implementation matches theory
2. Test framework integration issues
   - Mitigation: Start simple, add complexity gradually
3. Floating point precision issues in tests
   - Mitigation: Use reasonable epsilon for comparisons
4. Performance impact of tests
   - Mitigation: Tests only run in debug/test builds

**Rollback plan:**
- Tests are additive (no code changes to spring_damper)
- If tests fail: Fix implementation or test expectations
- No existing functionality at risk

---

## Implementation Questions

**To decide during planning:**
1. Test framework choice:
   - Option A: Simple assert-based tests (minimal deps)
   - Option B: Catch2 (full featured)
   - Option C: Doctest (lightweight, header-only)

2. Test location:
   - Option A: `tests/foundation/` (separate directory)
   - Option B: `src/foundation/spring_damper_test.cpp` (co-located)

3. Integration:
   - Option A: Separate test executable
   - Option B: Tests run on startup in debug builds
   - Option C: Manual test runner

**Prefer:** Simple assert-based, separate test directory, manual runner (Path A for each)

---

## Notes

**Why this matters:**
- Spring-damper is foundation primitive
- Used by critical systems (landing animation)
- Will be used by many future systems
- Mathematical correctness must be certain, not assumed

**Why now:**
- Priority #2 in backlog (after tuning defaults)
- Foundation repair mode (targeting 95%)
- Low risk (additive only)
- Enables 100% certainty in Layer 2

**Related violations:**
- None directly blocked by this
- Improves foundation for all future spring-based systems

**Learning opportunity:**
- Establish testing patterns for mathematical primitives
- Create template for future validation tests
- Prove that documentation + tests = 100% certainty

---

**Validate before trusting. Test before shipping. Prove before proclaiming.**
