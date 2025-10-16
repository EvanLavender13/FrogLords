/// Quaternion Math Validation Suite
///
/// Purpose: Validate quaternion mathematical operations (construction, slerp,
/// conjugate, swing-twist decomposition) in isolation before integrating into
/// production skeletal animation system.
///
/// Usage: Enable via CMake flag -DENABLE_QUATERNION_TESTS=ON, run executable.
/// Program runs tests at startup, outputs results to console, then exits.
///
/// References:
/// - NOTES/Math/QuaternionDecomp.md (mathematical theory)
/// - NOTES/Math/RealTimeProcedural.md (implementation guidance)
/// - PLANS/quaternion_math_validation_FEATURE.md (feature description)

#ifdef ENABLE_QUATERNION_TESTS

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <iomanip>

// Floating-point comparison tolerance
constexpr float EPSILON = 1e-5f;

/// Main entry point for quaternion validation suite.
/// Returns true if all tests pass, false if any fail.
/// Logs detailed results to stdout.
bool run_quaternion_validation_suite();

// ============================================================================
// Test Utility Functions
// ============================================================================

/// Compare two floats with epsilon tolerance.
bool compare_floats(float a, float b, float epsilon) {
    return std::abs(a - b) < epsilon;
}

/// Compare two vec3s component-wise with epsilon tolerance.
bool compare_vec3(glm::vec3 a, glm::vec3 b, float epsilon) {
    return compare_floats(a.x, b.x, epsilon) && compare_floats(a.y, b.y, epsilon) &&
           compare_floats(a.z, b.z, epsilon);
}

/// Compare two quaternions with epsilon tolerance.
/// Handles q and -q equivalence (both represent the same rotation).
bool compare_quat(glm::quat a, glm::quat b, float epsilon) {
    bool direct_match = compare_floats(a.w, b.w, epsilon) &&
                        compare_vec3(glm::vec3(a.x, a.y, a.z), glm::vec3(b.x, b.y, b.z), epsilon);
    bool negated_match =
        compare_floats(a.w, -b.w, epsilon) &&
        compare_vec3(glm::vec3(a.x, a.y, a.z), glm::vec3(-b.x, -b.y, -b.z), epsilon);
    return direct_match || negated_match;
}

/// Extract rotation angle in radians from quaternion.
float extract_angle_from_quat(glm::quat q) {
    // Clamp w to [-1, 1] to handle floating-point errors
    float w_clamped = glm::clamp(q.w, -1.0f, 1.0f);
    return 2.0f * std::acos(w_clamped);
}

/// Extract normalized rotation axis from quaternion.
/// Returns zero vector for identity quaternion (angle ~0).
glm::vec3 extract_axis_from_quat(glm::quat q) {
    float angle = extract_angle_from_quat(q);
    float sin_half_angle = std::sin(angle * 0.5f);

    // Handle singularity near identity rotation
    if (std::abs(sin_half_angle) < EPSILON) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 axis(q.x, q.y, q.z);
    return axis / sin_half_angle;
}

/// Log quaternion with components and extracted angle/axis.
void log_quat(const char* label, glm::quat q) {
    float angle_rad = extract_angle_from_quat(q);
    float angle_deg = glm::degrees(angle_rad);
    glm::vec3 axis = extract_axis_from_quat(q);

    std::cout << "  " << label << ": [w=" << std::fixed << std::setprecision(4) << q.w
              << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << "]" << std::endl;
    std::cout << "    Angle: " << angle_deg << "°, Axis: [" << axis.x << ", " << axis.y << ", "
              << axis.z << "]" << std::endl;
}

/// Log test result with consistent formatting.
void log_test_result(const char* test_name, bool passed) {
    std::cout << "  [" << (passed ? "PASS" : "FAIL") << "] " << test_name << std::endl;
}

// ============================================================================
// Test Cases: Quaternion Construction from Axis-Angle
// ============================================================================

/// Test quaternion construction from axis-angle for cardinal axes.
bool test_construction_cardinal_axes() {
    std::cout << "\nTest: Quaternion Construction (Cardinal Axes)" << std::endl;
    bool all_passed = true;

    // Test 1: 90° rotation around X-axis
    // Expected: w = cos(45°) ≈ 0.707, x = sin(45°) ≈ 0.707, y = 0, z = 0
    {
        glm::quat q = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat expected(0.707107f, 0.707107f, 0.0f, 0.0f);
        bool passed = compare_quat(q, expected, EPSILON);

        if (!passed) {
            std::cout << "  FAILED: 90° X-axis rotation" << std::endl;
            log_quat("Expected", expected);
            log_quat("Actual", q);
        }
        log_test_result("90° X-axis rotation", passed);
        all_passed &= passed;
    }

    // Test 2: 90° rotation around Y-axis
    // Expected: w = cos(45°) ≈ 0.707, x = 0, y = sin(45°) ≈ 0.707, z = 0
    {
        glm::quat q = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat expected(0.707107f, 0.0f, 0.707107f, 0.0f);
        bool passed = compare_quat(q, expected, EPSILON);

        if (!passed) {
            std::cout << "  FAILED: 90° Y-axis rotation" << std::endl;
            log_quat("Expected", expected);
            log_quat("Actual", q);
        }
        log_test_result("90° Y-axis rotation", passed);
        all_passed &= passed;
    }

    // Test 3: 90° rotation around Z-axis
    // Expected: w = cos(45°) ≈ 0.707, x = 0, y = 0, z = sin(45°) ≈ 0.707
    {
        glm::quat q = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::quat expected(0.707107f, 0.0f, 0.0f, 0.707107f);
        bool passed = compare_quat(q, expected, EPSILON);

        if (!passed) {
            std::cout << "  FAILED: 90° Z-axis rotation" << std::endl;
            log_quat("Expected", expected);
            log_quat("Actual", q);
        }
        log_test_result("90° Z-axis rotation", passed);
        all_passed &= passed;
    }

    // Test 4: 180° rotation around Y-axis (edge case)
    // Expected: w = cos(90°) = 0, x = 0, y = sin(90°) = 1, z = 0
    {
        glm::quat q = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat expected(0.0f, 0.0f, 1.0f, 0.0f);
        bool passed = compare_quat(q, expected, EPSILON);

        if (!passed) {
            std::cout << "  FAILED: 180° Y-axis rotation" << std::endl;
            log_quat("Expected", expected);
            log_quat("Actual", q);
        }
        log_test_result("180° Y-axis rotation (edge case)", passed);
        all_passed &= passed;
    }

    return all_passed;
}

// ============================================================================
// Test Cases: Slerp Interpolation
// ============================================================================

/// Test spherical linear interpolation (slerp) for smooth rotation blending.
bool test_slerp_interpolation() {
    std::cout << "\nTest: Slerp Interpolation" << std::endl;
    bool all_passed = true;

    // Test 1: Identity → 90° Y-rotation at t=0.5 (should be 45° Y-rotation)
    {
        glm::quat q1 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity
        glm::quat q2 = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat result = glm::slerp(q1, q2, 0.5f);

        // Expected: 45° rotation around Y-axis
        glm::quat expected = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        bool passed = compare_quat(result, expected, EPSILON);

        if (!passed) {
            std::cout << "  FAILED: Identity → 90° Y at t=0.5" << std::endl;
            log_quat("Expected (45° Y)", expected);
            log_quat("Actual", result);
        }
        log_test_result("Identity → 90° Y at t=0.5", passed);
        all_passed &= passed;
    }

    // Test 2: Hemisphere check (shortest path)
    // 10° Y → 350° Y should interpolate through 0°, not through 180°
    {
        glm::quat q1 = glm::angleAxis(glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat q2 = glm::angleAxis(glm::radians(350.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat result = glm::slerp(q1, q2, 0.5f);

        // Extract angle - should be near 0° or 360°, NOT 180°
        float angle_deg = glm::degrees(extract_angle_from_quat(result));

        // Normalize angle to [0, 360)
        while (angle_deg < 0.0f)
            angle_deg += 360.0f;
        while (angle_deg >= 360.0f)
            angle_deg -= 360.0f;

        // Should be close to 0° or 360° (within 20° tolerance for shortest path)
        bool passed = (angle_deg < 20.0f || angle_deg > 340.0f);

        if (!passed) {
            std::cout << "  FAILED: Hemisphere check (shortest path)" << std::endl;
            std::cout << "  Expected: ~0° or ~360° (shortest path through 0°)" << std::endl;
            std::cout << "  Actual: " << angle_deg << "° (took long path?)" << std::endl;
            log_quat("Result", result);
        }
        log_test_result("Hemisphere check (shortest path)", passed);
        all_passed &= passed;
    }

    // Test 3: Slerp endpoints (t=0 returns q1, t=1 returns q2)
    {
        glm::quat q1 = glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat q2 = glm::angleAxis(glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::quat result_t0 = glm::slerp(q1, q2, 0.0f);
        glm::quat result_t1 = glm::slerp(q1, q2, 1.0f);

        bool passed_t0 = compare_quat(result_t0, q1, EPSILON);
        bool passed_t1 = compare_quat(result_t1, q2, EPSILON);

        if (!passed_t0) {
            std::cout << "  FAILED: Slerp t=0 should return q1" << std::endl;
            log_quat("Expected (q1)", q1);
            log_quat("Actual", result_t0);
        }
        if (!passed_t1) {
            std::cout << "  FAILED: Slerp t=1 should return q2" << std::endl;
            log_quat("Expected (q2)", q2);
            log_quat("Actual", result_t1);
        }

        bool passed = passed_t0 && passed_t1;
        log_test_result("Slerp endpoints (t=0, t=1)", passed);
        all_passed &= passed;
    }

    // Test 4: Constant angular velocity
    // Angles at t=0.25, 0.5, 0.75 should be evenly spaced
    {
        glm::quat q1 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity (0°)
        glm::quat q2 = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // 90° Y

        glm::quat result_025 = glm::slerp(q1, q2, 0.25f);
        glm::quat result_050 = glm::slerp(q1, q2, 0.50f);
        glm::quat result_075 = glm::slerp(q1, q2, 0.75f);

        float angle_025 = glm::degrees(extract_angle_from_quat(result_025));
        float angle_050 = glm::degrees(extract_angle_from_quat(result_050));
        float angle_075 = glm::degrees(extract_angle_from_quat(result_075));

        // Expected: 22.5°, 45°, 67.5° (evenly spaced)
        float expected_025 = 22.5f;
        float expected_050 = 45.0f;
        float expected_075 = 67.5f;

        bool passed_025 = compare_floats(angle_025, expected_025, 0.5f); // 0.5° tolerance
        bool passed_050 = compare_floats(angle_050, expected_050, 0.5f);
        bool passed_075 = compare_floats(angle_075, expected_075, 0.5f);

        if (!passed_025 || !passed_050 || !passed_075) {
            std::cout << "  FAILED: Constant angular velocity" << std::endl;
            std::cout << "  Expected: 22.5°, 45.0°, 67.5° (evenly spaced)" << std::endl;
            std::cout << "  Actual:   " << angle_025 << "°, " << angle_050 << "°, " << angle_075
                      << "°" << std::endl;
        }

        bool passed = passed_025 && passed_050 && passed_075;
        log_test_result("Constant angular velocity", passed);
        all_passed &= passed;
    }

    return all_passed;
}

// ============================================================================
// Test Cases: Conjugate and Inverse
// ============================================================================

/// Test quaternion conjugate and inverse operations.
bool test_conjugate_inverse() {
    std::cout << "\nTest: Conjugate and Inverse" << std::endl;
    bool all_passed = true;

    // Test 1: Conjugate of 90° Y-rotation should be -90° Y-rotation (inverse)
    {
        glm::quat q = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat q_conj = glm::conjugate(q);

        // Conjugate should represent inverse rotation (-90° around Y)
        glm::quat expected = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        bool passed = compare_quat(q_conj, expected, EPSILON);

        if (!passed) {
            std::cout << "  FAILED: Conjugate of 90° Y should be -90° Y" << std::endl;
            log_quat("Original (90° Y)", q);
            log_quat("Expected (-90° Y)", expected);
            log_quat("Actual conjugate", q_conj);
        }
        log_test_result("Conjugate as inverse rotation", passed);
        all_passed &= passed;
    }

    // Test 2: q * conjugate(q) = identity
    {
        glm::quat q =
            glm::angleAxis(glm::radians(45.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
        glm::quat q_conj = glm::conjugate(q);
        glm::quat result = q * q_conj;

        // Result should be identity quaternion
        glm::quat identity(1.0f, 0.0f, 0.0f, 0.0f);
        bool passed = compare_quat(result, identity, EPSILON);

        if (!passed) {
            std::cout << "  FAILED: q * conjugate(q) should equal identity" << std::endl;
            log_quat("Original q", q);
            log_quat("Conjugate", q_conj);
            log_quat("Expected (identity)", identity);
            log_quat("Actual q * conj(q)", result);
        }
        log_test_result("q * conjugate(q) = identity", passed);
        all_passed &= passed;
    }

    // Test 3: conjugate(conjugate(q)) = q (double conjugate returns original)
    {
        glm::quat q = glm::angleAxis(glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::quat q_conj = glm::conjugate(q);
        glm::quat q_double_conj = glm::conjugate(q_conj);

        bool passed = compare_quat(q_double_conj, q, EPSILON);

        if (!passed) {
            std::cout << "  FAILED: conjugate(conjugate(q)) should equal original q" << std::endl;
            log_quat("Original q", q);
            log_quat("Single conjugate", q_conj);
            log_quat("Double conjugate", q_double_conj);
        }
        log_test_result("conjugate(conjugate(q)) = q", passed);
        all_passed &= passed;
    }

    // Test 4: For unit quaternions, conjugate == inverse (no normalization needed)
    {
        glm::quat q =
            glm::angleAxis(glm::radians(75.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
        glm::quat q_conj = glm::conjugate(q);
        glm::quat q_inv = glm::inverse(q);

        // Verify q is unit length
        float q_length = glm::length(q);
        bool is_unit = compare_floats(q_length, 1.0f, EPSILON);

        // For unit quaternions, conjugate should equal inverse
        bool passed = is_unit && compare_quat(q_conj, q_inv, EPSILON);

        if (!passed) {
            std::cout << "  FAILED: For unit quaternions, conjugate should equal inverse"
                      << std::endl;
            std::cout << "  Quaternion length: " << q_length << " (should be 1.0)" << std::endl;
            log_quat("Conjugate", q_conj);
            log_quat("Inverse", q_inv);
        }
        log_test_result("Unit quaternion: conjugate = inverse", passed);
        all_passed &= passed;
    }

    return all_passed;
}

// ============================================================================
// Test Cases: Swing-Twist Decomposition
// ============================================================================

/// Decompose a rotation quaternion into swing and twist components.
/// twist: rotation purely around twist_axis
/// swing: rotation with axis perpendicular to twist_axis
/// Decomposition: rotation = swing * twist
void decompose_swing_twist(glm::quat rotation, glm::vec3 twist_axis, glm::quat& swing_out,
                           glm::quat& twist_out) {
    // Extract vector part of rotation quaternion
    glm::vec3 r_vec(rotation.x, rotation.y, rotation.z);

    // Project vector part onto twist_axis to isolate twist component
    glm::vec3 projection = glm::dot(r_vec, twist_axis) * twist_axis;

    // Construct twist quaternion: scalar from rotation, projected vector
    glm::quat twist_raw(rotation.w, projection.x, projection.y, projection.z);
    twist_out = glm::normalize(twist_raw);

    // Calculate swing = rotation * conjugate(twist)
    // This isolates the rotation component orthogonal to twist_axis
    swing_out = rotation * glm::conjugate(twist_out);
}

/// Test swing-twist decomposition for separating axial and directional rotation.
bool test_swing_twist_decomposition() {
    std::cout << "\nTest: Swing-Twist Decomposition" << std::endl;
    bool all_passed = true;

    // Test 1: Pure twist (rotation around twist axis → identity swing, twist = rotation)
    {
        glm::vec3 twist_axis = glm::vec3(0.0f, 1.0f, 0.0f); // Y-axis
        glm::quat rotation = glm::angleAxis(glm::radians(45.0f), twist_axis);

        glm::quat swing, twist;
        decompose_swing_twist(rotation, twist_axis, swing, twist);

        // Expected: swing ≈ identity, twist ≈ rotation
        glm::quat identity(1.0f, 0.0f, 0.0f, 0.0f);
        bool swing_is_identity = compare_quat(swing, identity, EPSILON);
        bool twist_is_rotation = compare_quat(twist, rotation, EPSILON);
        bool passed = swing_is_identity && twist_is_rotation;

        if (!passed) {
            std::cout << "  FAILED: Pure twist (45° around Y-axis)" << std::endl;
            log_quat("Original rotation", rotation);
            log_quat("Expected swing (identity)", identity);
            log_quat("Actual swing", swing);
            log_quat("Expected twist (= rotation)", rotation);
            log_quat("Actual twist", twist);
        }
        log_test_result("Pure twist → identity swing", passed);
        all_passed &= passed;
    }

    // Test 2: Pure swing (rotation perpendicular to twist axis → identity twist)
    {
        glm::vec3 twist_axis = glm::vec3(0.0f, 1.0f, 0.0f); // Y-axis
        glm::quat rotation =
            glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis

        glm::quat swing, twist;
        decompose_swing_twist(rotation, twist_axis, swing, twist);

        // Expected: swing ≈ rotation, twist ≈ identity
        glm::quat identity(1.0f, 0.0f, 0.0f, 0.0f);
        bool swing_is_rotation = compare_quat(swing, rotation, EPSILON);
        bool twist_is_identity = compare_quat(twist, identity, EPSILON);
        bool passed = swing_is_rotation && twist_is_identity;

        if (!passed) {
            std::cout << "  FAILED: Pure swing (45° around X-axis, twist on Y)" << std::endl;
            log_quat("Original rotation", rotation);
            log_quat("Expected swing (= rotation)", rotation);
            log_quat("Actual swing", swing);
            log_quat("Expected twist (identity)", identity);
            log_quat("Actual twist", twist);
        }
        log_test_result("Pure swing → identity twist", passed);
        all_passed &= passed;
    }

    // Test 3: Mixed rotation (45° Y + 30° X decomposed with twist_axis = Y)
    {
        glm::vec3 twist_axis = glm::vec3(0.0f, 1.0f, 0.0f); // Y-axis

        // Construct mixed rotation: first 30° around X (swing), then 45° around Y (twist)
        glm::quat swing_component =
            glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat twist_component = glm::angleAxis(glm::radians(45.0f), twist_axis);
        glm::quat rotation = twist_component * swing_component; // Apply swing first, then twist

        glm::quat swing, twist;
        decompose_swing_twist(rotation, twist_axis, swing, twist);

        // Verify recomposition: swing * twist ≈ original rotation
        glm::quat reconstructed = swing * twist;
        bool passed = compare_quat(reconstructed, rotation,
                                   EPSILON * 10.0f); // Looser tolerance for composition

        if (!passed) {
            std::cout << "  FAILED: Mixed rotation recomposition" << std::endl;
            log_quat("Original rotation", rotation);
            log_quat("Decomposed swing", swing);
            log_quat("Decomposed twist", twist);
            log_quat("Reconstructed (swing * twist)", reconstructed);
        }
        log_test_result("Mixed rotation: swing * twist = original", passed);
        all_passed &= passed;
    }

    // Test 4: Verify swing and twist are orthogonal
    // Extract axes from swing and twist, verify dot product ~0
    {
        glm::vec3 twist_axis = glm::vec3(0.0f, 1.0f, 0.0f); // Y-axis

        // Create a rotation with both swing and twist components
        glm::quat rotation =
            glm::angleAxis(glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

        glm::quat swing, twist;
        decompose_swing_twist(rotation, twist_axis, swing, twist);

        // Extract twist axis from twist component
        glm::vec3 twist_extracted_axis = extract_axis_from_quat(twist);
        glm::vec3 swing_extracted_axis = extract_axis_from_quat(swing);

        // Twist axis should be parallel to twist_axis
        // Swing axis should be perpendicular to twist_axis
        float twist_dot = std::abs(glm::dot(twist_extracted_axis, twist_axis));
        float swing_dot = std::abs(glm::dot(swing_extracted_axis, twist_axis));

        bool twist_parallel = twist_dot > 0.99f ||
                              glm::length(twist_extracted_axis) < EPSILON; // Parallel or identity
        bool swing_perpendicular = swing_dot < 0.1f || glm::length(swing_extracted_axis) <
                                                           EPSILON; // Perpendicular or identity

        bool passed = twist_parallel && swing_perpendicular;

        if (!passed) {
            std::cout << "  FAILED: Swing/twist axis orthogonality" << std::endl;
            std::cout << "  Twist axis · Y: " << twist_dot << " (should be ~1.0)" << std::endl;
            std::cout << "  Swing axis · Y: " << swing_dot << " (should be ~0.0)" << std::endl;
            log_quat("Swing", swing);
            log_quat("Twist", twist);
        }
        log_test_result("Swing/twist axes orthogonal", passed);
        all_passed &= passed;
    }

    // Test 5: Edge case - 180° rotation perpendicular to twist axis
    {
        glm::vec3 twist_axis = glm::vec3(0.0f, 1.0f, 0.0f); // Y-axis
        glm::quat rotation =
            glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // 180° X

        glm::quat swing, twist;
        decompose_swing_twist(rotation, twist_axis, swing, twist);

        // Verify recomposition still works for this edge case
        glm::quat reconstructed = swing * twist;
        bool passed = compare_quat(reconstructed, rotation, EPSILON * 10.0f);

        if (!passed) {
            std::cout << "  FAILED: 180° rotation edge case" << std::endl;
            log_quat("Original (180° X)", rotation);
            log_quat("Decomposed swing", swing);
            log_quat("Decomposed twist", twist);
            log_quat("Reconstructed", reconstructed);
        }
        log_test_result("Edge case: 180° perpendicular rotation", passed);
        all_passed &= passed;
    }

    return all_passed;
}

// ============================================================================
// Test Cases: Edge Cases and Robustness
// ============================================================================

/// Test edge cases and robustness of quaternion operations.
bool test_edge_cases() {
    std::cout << "\nTest: Edge Cases and Robustness" << std::endl;
    bool all_passed = true;

    // Test 1: Identity quaternion behavior
    {
        glm::quat identity(1.0f, 0.0f, 0.0f, 0.0f);

        // Slerp(identity, identity, t) should return identity for any t
        glm::quat result_025 = glm::slerp(identity, identity, 0.25f);
        glm::quat result_050 = glm::slerp(identity, identity, 0.50f);
        glm::quat result_075 = glm::slerp(identity, identity, 0.75f);

        bool passed_025 = compare_quat(result_025, identity, EPSILON);
        bool passed_050 = compare_quat(result_050, identity, EPSILON);
        bool passed_075 = compare_quat(result_075, identity, EPSILON);

        bool passed = passed_025 && passed_050 && passed_075;

        if (!passed) {
            std::cout << "  FAILED: Slerp(identity, identity, t) should return identity"
                      << std::endl;
            if (!passed_025)
                log_quat("Result t=0.25", result_025);
            if (!passed_050)
                log_quat("Result t=0.50", result_050);
            if (!passed_075)
                log_quat("Result t=0.75", result_075);
        }
        log_test_result("Identity quaternion: slerp invariance", passed);
        all_passed &= passed;

        // Swing-twist decomposition with identity input
        glm::vec3 twist_axis = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::quat swing, twist;
        decompose_swing_twist(identity, twist_axis, swing, twist);

        bool swing_identity = compare_quat(swing, identity, EPSILON);
        bool twist_identity = compare_quat(twist, identity, EPSILON);
        bool decomp_passed = swing_identity && twist_identity;

        if (!decomp_passed) {
            std::cout << "  FAILED: Swing-twist of identity should yield two identities"
                      << std::endl;
            log_quat("Swing", swing);
            log_quat("Twist", twist);
        }
        log_test_result("Identity quaternion: swing-twist decomposition", decomp_passed);
        all_passed &= decomp_passed;
    }

    // Test 2: 180° rotations (axis extraction singularity when w ≈ 0)
    {
        glm::quat rot_180_x = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat rot_180_y = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat rot_180_z = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // Verify axis extraction handles w ≈ 0 without NaN/infinity
        glm::vec3 axis_x = extract_axis_from_quat(rot_180_x);
        glm::vec3 axis_y = extract_axis_from_quat(rot_180_y);
        glm::vec3 axis_z = extract_axis_from_quat(rot_180_z);

        bool no_nan = !std::isnan(axis_x.x) && !std::isnan(axis_x.y) && !std::isnan(axis_x.z) &&
                      !std::isnan(axis_y.x) && !std::isnan(axis_y.y) && !std::isnan(axis_y.z) &&
                      !std::isnan(axis_z.x) && !std::isnan(axis_z.y) && !std::isnan(axis_z.z);

        bool no_inf = !std::isinf(axis_x.x) && !std::isinf(axis_x.y) && !std::isinf(axis_x.z) &&
                      !std::isinf(axis_y.x) && !std::isinf(axis_y.y) && !std::isinf(axis_y.z) &&
                      !std::isinf(axis_z.x) && !std::isinf(axis_z.y) && !std::isinf(axis_z.z);

        bool passed = no_nan && no_inf;

        if (!passed) {
            std::cout << "  FAILED: 180° rotation axis extraction produced NaN or Inf" << std::endl;
            std::cout << "  180° X axis: [" << axis_x.x << ", " << axis_x.y << ", " << axis_x.z
                      << "]" << std::endl;
            std::cout << "  180° Y axis: [" << axis_y.x << ", " << axis_y.y << ", " << axis_y.z
                      << "]" << std::endl;
            std::cout << "  180° Z axis: [" << axis_z.x << ", " << axis_z.y << ", " << axis_z.z
                      << "]" << std::endl;
        }
        log_test_result("180° rotations: no NaN/Inf in axis extraction", passed);
        all_passed &= passed;
    }

    // Test 3: Near-parallel quaternions (q1 ≈ q2)
    {
        glm::quat q1 = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat q2 =
            glm::angleAxis(glm::radians(45.001f), glm::vec3(0.0f, 1.0f, 0.0f)); // Very close

        // Slerp should not produce NaN or degenerate
        glm::quat result = glm::slerp(q1, q2, 0.5f);

        bool no_nan = !std::isnan(result.w) && !std::isnan(result.x) && !std::isnan(result.y) &&
                      !std::isnan(result.z);
        bool is_unit = compare_floats(glm::length(result), 1.0f, EPSILON);

        bool passed = no_nan && is_unit;

        if (!passed) {
            std::cout << "  FAILED: Near-parallel quaternion slerp" << std::endl;
            std::cout << "  No NaN: " << (no_nan ? "yes" : "no") << std::endl;
            std::cout << "  Unit length: " << (is_unit ? "yes" : "no")
                      << " (length = " << glm::length(result) << ")" << std::endl;
            log_quat("Result", result);
        }
        log_test_result("Near-parallel quaternions: slerp stability", passed);
        all_passed &= passed;
    }

    // Test 4: Normalization after operations
    {
        // Test quaternion multiplication maintains unit length
        glm::quat q1 = glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat q2 = glm::angleAxis(glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat result_mul = q1 * q2;

        bool mul_unit = compare_floats(glm::length(result_mul), 1.0f, EPSILON);

        // Test slerp maintains unit length
        glm::quat result_slerp = glm::slerp(q1, q2, 0.5f);
        bool slerp_unit = compare_floats(glm::length(result_slerp), 1.0f, EPSILON);

        // Test swing-twist decomposition maintains unit length
        glm::vec3 twist_axis = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::quat swing, twist;
        decompose_swing_twist(result_mul, twist_axis, swing, twist);
        bool swing_unit = compare_floats(glm::length(swing), 1.0f, EPSILON);
        bool twist_unit = compare_floats(glm::length(twist), 1.0f, EPSILON);

        bool passed = mul_unit && slerp_unit && swing_unit && twist_unit;

        if (!passed) {
            std::cout << "  FAILED: Quaternion normalization after operations" << std::endl;
            std::cout << "  Multiplication unit: " << (mul_unit ? "yes" : "no")
                      << " (length = " << glm::length(result_mul) << ")" << std::endl;
            std::cout << "  Slerp unit: " << (slerp_unit ? "yes" : "no")
                      << " (length = " << glm::length(result_slerp) << ")" << std::endl;
            std::cout << "  Swing unit: " << (swing_unit ? "yes" : "no")
                      << " (length = " << glm::length(swing) << ")" << std::endl;
            std::cout << "  Twist unit: " << (twist_unit ? "yes" : "no")
                      << " (length = " << glm::length(twist) << ")" << std::endl;
        }
        log_test_result("Normalization: unit length preserved", passed);
        all_passed &= passed;
    }

    return all_passed;
}

// ============================================================================
// Main Suite Runner
// ============================================================================

/// Run all quaternion validation tests.
/// Returns true if all tests pass, false if any fail.
/// Continues running all tests even if some fail to provide complete diagnostic output.
bool run_quaternion_validation_suite() {
    std::cout << "\n";
    std::cout << "========================================" << std::endl;
    std::cout << "  Quaternion Math Validation Suite" << std::endl;
    std::cout << "========================================" << std::endl;

    bool all_passed = true;

    // Run all test suites
    all_passed &= test_construction_cardinal_axes();
    all_passed &= test_slerp_interpolation();
    all_passed &= test_conjugate_inverse();
    all_passed &= test_swing_twist_decomposition();
    all_passed &= test_edge_cases();

    // Print final summary
    std::cout << "\n";
    std::cout << "========================================" << std::endl;
    std::cout << "  Suite Complete: " << (all_passed ? "ALL PASSED ✓" : "FAILURES DETECTED ✗")
              << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "\n";

    return all_passed;
}

#endif // ENABLE_QUATERNION_TESTS
