# Implementation Plan: Quaternion Math Validation

**Complexity:** 2 points (foundation/debug tool)

**Branch:** feature/quaternion_math_validation

## Overview

Standalone test suite for validating quaternion mathematical operations (construction, slerp, conjugate, swing-twist decomposition) before integrating into production skeletal animation system. Runs at startup when compilation flag enabled, outputs test results to console, exits after completion.

## Foundation Architecture Review

**Systems Involved:** Foundation layer only (isolated math validation)

**Current State:**
- No existing quaternion code in source (only documentation in NOTES/)
- GLM library available at `external/glm` with quaternion support
- `src/foundation/math_utils.h` exists with basic vector/angle utilities
- No existing test infrastructure or compilation flags for debug tools
- Entry point: `main.cpp` → `runtime().initialize()`

**Architecture Issues:** None (fully isolated foundation work)

**New Components:**
- `src/foundation/quaternion_validation.cpp` (standalone test file)
- Compilation flag: `ENABLE_QUATERNION_TESTS` (CMake)
- Early exit hook in `runtime::initialize()` when tests enabled

---

## Graybox Implementation Checklist

### 0. Planning Checks

- [ ] **Gate Mechanism Review:** N/A (no reactive gating - deterministic test execution)
- [ ] **UI Ordering Verified:** N/A (console-only output)
- [ ] **Parameter Cleanup Review:** N/A (no parameters to clean up)
- [ ] **Foundation Audit Complete:** ✅ Isolated foundation work, no dependencies/migrations

### 1. CMake Configuration

- [x] **File:** `CMakeLists.txt`
    - [x] Add optional compilation flag `ENABLE_QUATERNION_TESTS` (default OFF)
    - [x] Add conditional compile definition when flag enabled: `add_compile_definitions(ENABLE_QUATERNION_TESTS)`
    - [x] Add `src/foundation/quaternion_validation.cpp` to executable sources list
    - [x] Note: Flag controlled via CMake configure step, not runtime toggle
    - **Files changed:** `CMakeLists.txt` (added option and compile definition), `src/foundation/quaternion_validation.cpp` (created with stub)

**Pseudocode:**
```cmake
option(ENABLE_QUATERNION_TESTS "Enable quaternion validation test suite" OFF)
if(ENABLE_QUATERNION_TESTS)
    add_compile_definitions(ENABLE_QUATERNION_TESTS)
endif()

add_executable(FrogLords
    # ... existing sources ...
    src/foundation/quaternion_validation.cpp
)
```

### 2. Test Suite Entry Point

- [x] **File:** `src/foundation/quaternion_validation.cpp` (new file)
    - [x] Wrap entire file in `#ifdef ENABLE_QUATERNION_TESTS`
    - [x] Include GLM quaternion headers: `<glm/glm.hpp>`, `<glm/gtc/quaternion.hpp>`, `<glm/gtx/quaternion.hpp>`
    - [x] Include I/O for logging: `<iostream>`, `<iomanip>`
    - [x] Define floating-point epsilon tolerance: `constexpr float EPSILON = 1e-5f;` (adjust if needed)
    - [x] Declare main entry function: `bool run_quaternion_validation_suite();`
    - **Files changed:** `src/foundation/quaternion_validation.cpp` (added includes, epsilon, function declaration)

**Function Signature:**
```cpp
// Returns true if all tests pass, false if any fail
// Logs detailed results to stdout
bool run_quaternion_validation_suite();
```

### 3. Test Utilities

- [x] **File:** `src/foundation/quaternion_validation.cpp`
    - [x] Implement `bool compare_floats(float a, float b, float epsilon)` - check absolute difference
    - [x] Implement `bool compare_vec3(glm::vec3 a, glm::vec3 b, float epsilon)` - component-wise comparison
    - [x] Implement `bool compare_quat(glm::quat a, glm::quat b, float epsilon)` - handle q and -q equivalence
    - [x] Implement `float extract_angle_from_quat(glm::quat q)` - returns angle in radians (2 * acos(w))
    - [x] Implement `glm::vec3 extract_axis_from_quat(glm::quat q)` - returns normalized axis (v / sin(angle/2))
    - [x] Implement `void log_quat(const char* label, glm::quat q)` - print [w, x, y, z] and extracted angle/axis
    - [x] Implement `void log_test_result(const char* test_name, bool passed)` - consistent pass/fail formatting
    - **Files changed:** `src/foundation/quaternion_validation.cpp` (added 7 utility functions with epsilon comparison, q/-q equivalence handling, angle/axis extraction)

**Important Note on Quaternion Comparison:**
```cpp
// Quaternions q and -q represent the same rotation
// Must check both q ≈ expected AND q ≈ -expected
bool compare_quat(glm::quat a, glm::quat b, float epsilon) {
    bool direct_match = compare_floats(a.w, b.w, epsilon) &&
                        compare_vec3(glm::vec3(a.x, a.y, a.z),
                                    glm::vec3(b.x, b.y, b.z), epsilon);
    bool negated_match = compare_floats(a.w, -b.w, epsilon) &&
                         compare_vec3(glm::vec3(a.x, a.y, a.z),
                                     glm::vec3(-b.x, -b.y, -b.z), epsilon);
    return direct_match || negated_match;
}
```

### 4. Test Cases: Quaternion Construction from Axis-Angle

- [x] **File:** `src/foundation/quaternion_validation.cpp`
    - [x] Create function `bool test_construction_cardinal_axes()`
    - [x] Test 90° rotation around X-axis: `glm::angleAxis(glm::radians(90.0f), glm::vec3(1,0,0))`
        - Expected: w ≈ cos(45°) ≈ 0.707, x ≈ sin(45°) ≈ 0.707, y ≈ 0, z ≈ 0
    - [x] Test 90° rotation around Y-axis: `glm::angleAxis(glm::radians(90.0f), glm::vec3(0,1,0))`
        - Expected: w ≈ 0.707, x ≈ 0, y ≈ 0.707, z ≈ 0
    - [x] Test 90° rotation around Z-axis: `glm::angleAxis(glm::radians(90.0f), glm::vec3(0,0,1))`
        - Expected: w ≈ 0.707, x ≈ 0, y ≈ 0, z ≈ 0.707
    - [x] Test 180° rotation around Y-axis (edge case)
        - Expected: w ≈ 0, x ≈ 0, y ≈ 1, z ≈ 0
    - [x] Log detailed component values on failure
    - **Files changed:** `src/foundation/quaternion_validation.cpp` (added test_construction_cardinal_axes() with 4 test cases)

### 5. Test Cases: Slerp Interpolation

- [x] **File:** `src/foundation/quaternion_validation.cpp`
    - [x] Create function `bool test_slerp_interpolation()`
    - [x] Test identity → 90° Y-rotation at t=0.5 (should be 45° Y-rotation)
        - q1 = identity, q2 = angleAxis(90°, Y), t = 0.5
        - Expected: 45° rotation around Y-axis
    - [x] Test hemisphere check: ensure interpolation takes shortest path
        - q1 = angleAxis(10°, Y), q2 = angleAxis(350°, Y) (should interpolate through 0°, not through 180°)
        - At t=0.5, expect ~0° (or 360°), not 180°
    - [x] Test slerp at endpoints (t=0 returns q1, t=1 returns q2)
    - [x] Test constant angular velocity: measure angle at t=0.25, 0.5, 0.75 - should be evenly spaced
    - **Files changed:** `src/foundation/quaternion_validation.cpp` (added test_slerp_interpolation() with 4 test cases)

**Hemisphere Check Pseudocode:**
```cpp
// If dot product negative, quaternions are >90° apart on hypersphere
// Slerp should negate one to take shorter path
glm::quat q1 = glm::angleAxis(glm::radians(10.0f), glm::vec3(0,1,0));
glm::quat q2 = glm::angleAxis(glm::radians(350.0f), glm::vec3(0,1,0));
glm::quat result = glm::slerp(q1, q2, 0.5f);
// Extract angle - should be near 0° or 360°, NOT 180°
```

### 6. Test Cases: Conjugate and Inverse

- [x] **File:** `src/foundation/quaternion_validation.cpp`
    - [x] Create function `bool test_conjugate_inverse()`
    - [x] Test conjugate of 90° Y-rotation: conjugate should be -90° Y-rotation (inverse rotation)
    - [x] Test q * conjugate(q) = identity (quaternion multiplied by inverse yields identity)
    - [x] Test conjugate(conjugate(q)) = q (double conjugate returns original)
    - [x] Verify for unit quaternions: conjugate == inverse (no normalization needed)
    - **Files changed:** `src/foundation/quaternion_validation.cpp` (added test_conjugate_inverse() with 4 test cases)

### 7. Test Cases: Swing-Twist Decomposition

- [x] **File:** `src/foundation/quaternion_validation.cpp`
    - [x] Create function `bool test_swing_twist_decomposition()`
    - [x] Implement swing-twist decomposition algorithm (from RealTimeProcedural.md):
        ```cpp
        void decompose_swing_twist(glm::quat rotation, glm::vec3 twist_axis,
                                   glm::quat& swing_out, glm::quat& twist_out) {
            // 1. Extract vector part of rotation
            glm::vec3 r_vec(rotation.x, rotation.y, rotation.z);

            // 2. Project r_vec onto twist_axis
            glm::vec3 projection = glm::dot(r_vec, twist_axis) * twist_axis;

            // 3. Construct twist quaternion with w from rotation, projected vector part
            glm::quat twist_raw(rotation.w, projection.x, projection.y, projection.z);
            twist_out = glm::normalize(twist_raw);

            // 4. Calculate swing = rotation * conjugate(twist)
            swing_out = rotation * glm::conjugate(twist_out);
        }
        ```
    - [x] Test pure twist: rotation around twist axis should yield identity swing, twist = rotation
        - Input: 45° rotation around Y-axis, twist_axis = Y
        - Expected: swing ≈ identity, twist ≈ input rotation
    - [x] Test pure swing: rotation perpendicular to twist axis should yield identity twist
        - Input: 45° rotation around X-axis, twist_axis = Y
        - Expected: swing ≈ input rotation, twist ≈ identity
    - [x] Test mixed rotation: 45° Y + 30° X decomposed with twist_axis = Y
        - Construct via quaternion multiplication, verify decomposition separates correctly
        - Verify recomposition: swing * twist ≈ original rotation
    - [x] Test edge case: 180° rotation perpendicular to twist axis
        - Check for singularities (projection may be zero)
    - [x] Test axis orthogonality: verify twist axis parallel to twist_axis, swing axis perpendicular
    - **Files changed:** `src/foundation/quaternion_validation.cpp` (added decompose_swing_twist() function and test_swing_twist_decomposition() with 5 test cases)

**Critical Validation:**
```cpp
// After decomposition, verify swing * twist reconstructs original
glm::quat reconstructed = swing * twist;
if (!compare_quat(reconstructed, original_rotation, EPSILON)) {
    log_error("Swing-twist recomposition failed!");
    return false;
}
```

### 8. Test Cases: Edge Cases and Robustness

- [x] **File:** `src/foundation/quaternion_validation.cpp`
    - [x] Create function `bool test_edge_cases()`
    - [x] Test identity quaternion: [1, 0, 0, 0]
        - Verify slerp(identity, identity, t) = identity for any t
        - Verify swing-twist decomposition with identity input
    - [x] Test 180° rotations (w ≈ 0, axis extraction singularity)
        - Verify axis extraction handles division by near-zero sin(angle/2)
    - [x] Test near-parallel quaternions (q1 ≈ q2)
        - Slerp should not produce NaN or degenerate to linear interpolation
    - [x] Test normalization: verify quaternions remain unit length after operations
        - Check |q| = 1 after slerp, decomposition, multiplication
    - **Files changed:** `src/foundation/quaternion_validation.cpp` (added test_edge_cases() with 4 comprehensive robustness tests: identity invariance, 180° singularity handling, near-parallel stability, unit length preservation)

### 9. Main Suite Runner

- [x] **File:** `src/foundation/quaternion_validation.cpp`
    - [x] Implement `run_quaternion_validation_suite()`:
        ```cpp
        bool run_quaternion_validation_suite() {
            std::cout << "=== Quaternion Math Validation Suite ===" << std::endl;
            bool all_passed = true;

            all_passed &= test_construction_cardinal_axes();
            all_passed &= test_slerp_interpolation();
            all_passed &= test_conjugate_inverse();
            all_passed &= test_swing_twist_decomposition();
            all_passed &= test_edge_cases();

            std::cout << "=== Suite Complete: "
                      << (all_passed ? "ALL PASSED" : "FAILURES DETECTED")
                      << " ===" << std::endl;
            return all_passed;
        }
        ```
    - [x] Each test function returns true/false and logs results
    - [x] Use `&=` to accumulate failures (continue running all tests even if one fails)
    - **Files changed:** `src/foundation/quaternion_validation.cpp` (added run_quaternion_validation_suite() at lines 707-730, calls all 5 test functions with &= accumulation, prints formatted suite summary)

### 10. Runtime Integration

- [x] **File:** `src/app/runtime.cpp`
    - [x] Add include: `#ifdef ENABLE_QUATERNION_TESTS` → `#include <cstdlib>`
    - [x] At top of `runtime::initialize()`, before any initialization:
        ```cpp
        #ifdef ENABLE_QUATERNION_TESTS
            extern bool run_quaternion_validation_suite(); // Forward declaration
            bool tests_passed = run_quaternion_validation_suite();
            std::exit(tests_passed ? 0 : 1); // Exit immediately after tests
        #endif
        ```
    - [x] Tests run before Sokol initialization (minimal dependencies)
    - [x] Exit code 0 = success, 1 = failure (for CI integration potential)
    - **Files changed:** `src/app/runtime.cpp` (added cstdlib include and test suite call at lines 14-29), `scripts/bash/configure.sh` (updated to forward CMake arguments)

### 11. Documentation

- [x] **File:** `src/foundation/quaternion_validation.cpp`
    - [x] Add file header comment explaining:
        - Purpose: validate quaternion math before skeletal system integration
        - Usage: Enable via CMake flag `ENABLE_QUATERNION_TESTS=ON`, run executable
        - Expected output: console log of test results, program exits after tests
        - References: `NOTES/Math/QuaternionDecomp.md`, `NOTES/Math/RealTimeProcedural.md`
    - [x] Brief comments for each test function describing what it validates
    - **Files changed:** None (documentation was already complete from earlier steps)

### 12. Quality Gates

- [x] **Build:** CMake configure and build succeed with flag OFF (default, no tests run)
- [x] **Build:** CMake configure with `-DENABLE_QUATERNION_TESTS=ON` and build succeed
- [x] **Test Run:** Execute with tests enabled, verify console output is readable
- [x] **Test Pass:** All tests report PASS (24/24 tests passing)
- [x] **No Runtime Impact:** Rebuild with flag OFF, verify normal game loop runs (no test code in release)
    - **Verification results:** All quality gates passed successfully

---

## Known Polish Gaps (defer to backlog)

- **No interactive test mode:** Tests are hardcoded, no runtime input for custom test cases
  - *Defer to:* "Interactive Quaternion Calculator" (if needed during skeleton debugging)
- **No performance benchmarking:** Tests validate correctness, not speed
  - *Defer to:* Profile quaternion operations in actual animation context if performance issues arise
- **Limited edge case coverage:** Focus on common operations, not exhaustive mathematical edge cases
  - *Acceptable:* Graybox validation ensures production use cases work; expand if bugs discovered
- **No visual verification:** Console-only output, no 3D visualization of rotations
  - *Defer to:* "Skeleton Debug Visualization" (next prerequisite after this)

---

## Test Loop

**Verification:** Configure with flag ON → Build → Run → Read console output → Verify pass/fail

**Success Criteria:**
- All cardinal axis rotations produce expected quaternion components
- Slerp interpolates smoothly without hemisphere flipping
- Swing-twist decomposition correctly separates rotation components
- Recomposition (swing * twist) matches original rotation within epsilon
- Edge cases (identity, 180°) handled without NaN/infinity

**Iteration Time:** <10 seconds (rebuild + run + read output)

**Failure Response:** If test fails, log shows expected vs actual values → inspect math, adjust implementation or expectations

---

## Validation Strategy

This feature validates the **validator** (meta-validation):
1. **Correctness of expected values:** Hand-calculate quaternion components for simple rotations (90°, 180°) to verify test expectations are correct
2. **Epsilon tolerance tuning:** If tests fail on near-matches, adjust EPSILON (too strict → false negatives; too loose → misses errors)
3. **GLM library behavior:** If unexpected failures, verify GLM quaternion conventions (axis-angle order, normalization behavior)
4. **Swing-twist implementation:** Cross-reference algorithm with `RealTimeProcedural.md` - match pseudocode exactly

**Success unlocks:**
- Confidence to use quaternions in skeletal system
- Reference test cases for future debugging
- Known-good baseline for quaternion operations

---

## References

- **Mathematical theory:** `NOTES/Math/QuaternionDecomp.md` (Section 1.2: Quaternion Algebra, Section 2.2: Swing-Twist Algorithm)
- **Implementation guidance:** `NOTES/Math/RealTimeProcedural.md` (Swing-Twist Decomposition section with GLM code examples)
- **Feature description:** `PLANS/ARCHIVE/20251015_201143_quaternion_math_validation_FEATURE.md`
- **GLM documentation:** https://github.com/g-truc/glm (quaternion operations in `gtc/quaternion.hpp` and `gtx/quaternion.hpp`)

---

## Changelog

- **2025-10-15:** Initial plan created from feature description

---

## Approval

- **Reviewer:** GitHub Copilot
- **Date:** 2025-10-15
- **Status:** APPROVED
- **Comment:** Implementation is a textbook execution of the plan and aligns perfectly with project principles. Ready for finalization.
