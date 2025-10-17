#pragma once

#include <cassert>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

/**
 * FrogLords Debug Assertion Framework
 *
 * Provides math-aware assertions for validating preconditions, postconditions,
 * and invariants throughout the codebase. All assertions compile out in release
 * builds for zero overhead.
 *
 * Philosophy:
 * - Catch violations at the exact point they occur
 * - Document contracts explicitly in code
 * - Enable fearless refactoring with immediate feedback
 * - Prevent compound errors from propagating through systems
 *
 * Usage:
 *   FL_PRECONDITION(speed >= 0.0f, "speed must be non-negative");
 *   FL_ASSERT_NORMALIZED(direction, "movement direction");
 *   FL_POSTCONDITION(result.is_valid(), "result must be valid");
 */

// Master debug flag (matches CMake _DEBUG builds)
#ifdef _DEBUG
#define FL_DEBUG_VALIDATION 1
#else
#define FL_DEBUG_VALIDATION 0
#endif

// Epsilon for floating point comparisons
#define FL_EPSILON 0.001f

//==============================================================================
// Core Assertion Macros
//==============================================================================

#if FL_DEBUG_VALIDATION

// Basic assertion with message
#define FL_ASSERT(expr, msg) assert((expr) && (msg))

// Contract assertions (semantically meaningful)
#define FL_PRECONDITION(expr, msg) FL_ASSERT(expr, "PRECONDITION: " msg)

#define FL_POSTCONDITION(expr, msg) FL_ASSERT(expr, "POSTCONDITION: " msg)

#define FL_INVARIANT(expr, msg) FL_ASSERT(expr, "INVARIANT: " msg)

#else
// No-ops in release builds
#define FL_ASSERT(expr, msg)
#define FL_PRECONDITION(expr, msg)
#define FL_POSTCONDITION(expr, msg)
#define FL_INVARIANT(expr, msg)
#endif

//==============================================================================
// Math-Aware Assertions
//==============================================================================

#if FL_DEBUG_VALIDATION

// Verify vector is normalized (unit length)
#define FL_ASSERT_NORMALIZED(v, msg)                                                               \
    FL_ASSERT(glm::epsilonEqual(glm::length(v), 1.0f, FL_EPSILON),                                 \
              "Vector must be normalized: " msg)

// Verify all components are finite (not NaN or Inf)
#define FL_ASSERT_FINITE(v, msg)                                                                   \
    FL_ASSERT((std::isfinite((v).x) && std::isfinite((v).y) && std::isfinite((v).z)),              \
              "Vector must be finite (no NaN/Inf): " msg)

// Verify scalar is finite
#define FL_ASSERT_FINITE_SCALAR(x, msg)                                                            \
    FL_ASSERT(std::isfinite(x), "Scalar must be finite (no NaN/Inf): " msg)

// Verify value is positive
#define FL_ASSERT_POSITIVE(x, msg) FL_ASSERT((x) > 0.0f, "Value must be positive: " msg)

// Verify value is non-negative
#define FL_ASSERT_NON_NEGATIVE(x, msg) FL_ASSERT((x) >= 0.0f, "Value must be non-negative: " msg)

// Verify value is in range [min, max]
#define FL_ASSERT_IN_RANGE(x, min, max, msg)                                                       \
    FL_ASSERT((x) >= (min) && (x) <= (max), "Value must be in range [" #min ", " #max "]: " msg)

// Verify two vectors are orthogonal (perpendicular)
#define FL_ASSERT_ORTHOGONAL(v1, v2, msg)                                                          \
    FL_ASSERT(glm::epsilonEqual(glm::dot(v1, v2), 0.0f, FL_EPSILON),                               \
              "Vectors must be orthogonal: " msg)

// Verify two vectors are parallel
#define FL_ASSERT_PARALLEL(v1, v2, msg)                                                            \
    FL_ASSERT(glm::epsilonEqual(std::abs(glm::dot(glm::normalize(v1), glm::normalize(v2))), 1.0f,  \
                                FL_EPSILON),                                                       \
              "Vectors must be parallel: " msg)

#else
// No-ops in release builds
#define FL_ASSERT_NORMALIZED(v, msg)
#define FL_ASSERT_FINITE(v, msg)
#define FL_ASSERT_FINITE_SCALAR(x, msg)
#define FL_ASSERT_POSITIVE(x, msg)
#define FL_ASSERT_NON_NEGATIVE(x, msg)
#define FL_ASSERT_IN_RANGE(x, min, max, msg)
#define FL_ASSERT_ORTHOGONAL(v1, v2, msg)
#define FL_ASSERT_PARALLEL(v1, v2, msg)
#endif

//==============================================================================
// Helper Functions
//==============================================================================

#if FL_DEBUG_VALIDATION

namespace fl {

/**
 * Verify a coordinate frame is orthonormal and right-handed.
 *
 * Checks:
 * - All axes are unit length (normalized)
 * - All axes are mutually perpendicular (orthogonal)
 * - Right-handed: X × Y = Z
 *
 * This is the fundamental validation for any transform matrix.
 */
inline void verify_coordinate_frame(const glm::vec3& x_axis, const glm::vec3& y_axis,
                                    const glm::vec3& z_axis,
                                    const char* context = "coordinate frame") {
    // Verify normalized
    FL_ASSERT(glm::epsilonEqual(glm::length(x_axis), 1.0f, FL_EPSILON),
              "X axis must be normalized");
    FL_ASSERT(glm::epsilonEqual(glm::length(y_axis), 1.0f, FL_EPSILON),
              "Y axis must be normalized");
    FL_ASSERT(glm::epsilonEqual(glm::length(z_axis), 1.0f, FL_EPSILON),
              "Z axis must be normalized");

    // Verify orthogonal
    FL_ASSERT(glm::epsilonEqual(glm::dot(x_axis, y_axis), 0.0f, FL_EPSILON),
              "X and Y axes must be orthogonal");
    FL_ASSERT(glm::epsilonEqual(glm::dot(y_axis, z_axis), 0.0f, FL_EPSILON),
              "Y and Z axes must be orthogonal");
    FL_ASSERT(glm::epsilonEqual(glm::dot(z_axis, x_axis), 0.0f, FL_EPSILON),
              "Z and X axes must be orthogonal");

    // Verify right-handed: X × Y = Z
    glm::vec3 cross = glm::cross(x_axis, y_axis);
    FL_ASSERT(glm::all(glm::epsilonEqual(cross, z_axis, FL_EPSILON)),
              "Coordinate frame must be right-handed (X × Y = Z)");
}

/**
 * Verify a transform matrix is valid.
 *
 * Extracts the rotation component and verifies it forms a valid
 * orthonormal right-handed coordinate frame.
 */
inline void verify_transform_matrix(const glm::mat4& transform,
                                    const char* context = "transform matrix") {
    glm::vec3 x_axis = glm::vec3(transform[0]);
    glm::vec3 y_axis = glm::vec3(transform[1]);
    glm::vec3 z_axis = glm::vec3(transform[2]);

    verify_coordinate_frame(x_axis, y_axis, z_axis, context);

    // Verify translation is finite
    glm::vec3 position = glm::vec3(transform[3]);
    FL_ASSERT((std::isfinite(position.x) && std::isfinite(position.y) && std::isfinite(position.z)),
              "Transform position must be finite");
}

} // namespace fl

#else

// No-ops in release builds
namespace fl {
inline void verify_coordinate_frame(const glm::vec3&, const glm::vec3&, const glm::vec3&,
                                    const char* = "") {}
inline void verify_transform_matrix(const glm::mat4&, const char* = "") {}
} // namespace fl

#endif

//==============================================================================
// Usage Examples
//==============================================================================

/*

// Example 1: Function preconditions
void normalize_vector(glm::vec3& v) {
    FL_PRECONDITION(glm::length(v) > 0.0f, "cannot normalize zero vector");
    v = glm::normalize(v);
    FL_POSTCONDITION(glm::epsilonEqual(glm::length(v), 1.0f, FL_EPSILON),
                     "vector must be unit length after normalization");
}

// Example 2: Math-aware assertions
void compute_velocity(glm::vec3 direction, float speed) {
    FL_ASSERT_NORMALIZED(direction, "movement direction");
    FL_ASSERT_NON_NEGATIVE(speed, "speed");

    glm::vec3 velocity = direction * speed;

    FL_POSTCONDITION(glm::length(velocity) <= speed + FL_EPSILON,
                     "velocity magnitude must match speed");
}

// Example 3: Coordinate frame validation
void set_transform(const glm::mat4& transform) {
    fl::verify_transform_matrix(transform, "entity transform");
    // ... use transform
}

// Example 4: Invariant checking
void integrate_physics(float dt) {
    FL_INVARIANT(dt > 0.0f && dt < 1.0f, "timestep must be reasonable");
    FL_ASSERT_FINITE(position, "position before integration");
    FL_ASSERT_FINITE(velocity, "velocity before integration");

    position += velocity * dt;

    FL_POSTCONDITION(glm::all(glm::isfinite(position)),
                     "position must remain finite after integration");
}

*/
