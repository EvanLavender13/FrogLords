#pragma once

/**
 * Debug Visualization Helpers
 *
 * Convenience functions for common debug visualization patterns.
 * These helpers wrap the existing debug primitive system with
 * math-validated, purpose-specific functions.
 *
 * Philosophy:
 * - Thin wrappers around existing debug::debug_primitive_list
 * - Assertion-validated inputs (preconditions/postconditions)
 * - Read-only visualization (never affects gameplay)
 * - Immediate understanding through color conventions
 *
 * Usage:
 *   debug::debug_primitive_list list;
 *   debug::add_axes(list, transform, 1.0f);
 *   debug::add_velocity_arrow(list, position, velocity, 0.5f);
 *   debug::add_collision_sphere(list, center, radius);
 */

#include "rendering/debug_primitives.h"
#include "foundation/debug_assert.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace debug {

//==============================================================================
// Color Constants
//==============================================================================

// RGB color convention for coordinate axes
constexpr glm::vec4 RGB_RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);   // X axis
constexpr glm::vec4 RGB_GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Y axis
constexpr glm::vec4 RGB_BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);  // Z axis

// Common visualization colors
constexpr glm::vec4 RGB_YELLOW = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);  // Acceleration
constexpr glm::vec4 RGB_CYAN = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);    // Normals
constexpr glm::vec4 RGB_MAGENTA = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f); // Collision
constexpr glm::vec4 RGB_WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);   // Input/Intent
constexpr glm::vec4 RGB_ORANGE = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);  // Warnings

//==============================================================================
// Coordinate Frame Visualization
//==============================================================================

/**
 * Add RGB axes visualization for a transform.
 *
 * Visualizes the local coordinate frame as three colored lines:
 * - X axis: Red (right)
 * - Y axis: Green (up)
 * - Z axis: Blue (forward)
 *
 * This is the primary tool for validating coordinate system assumptions
 * and understanding entity orientation.
 *
 * Preconditions:
 * - transform must be valid (orthonormal, right-handed)
 * - length must be positive
 *
 * @param list The primitive list to add to
 * @param transform The transformation matrix (rotation + translation)
 * @param length The length of each axis line (world units)
 */
inline void add_axes(debug_primitive_list& list, const glm::mat4& transform, float length = 1.0f) {
    FL_PRECONDITION(length > 0.0f, "axis length must be positive");
    fl::verify_transform_matrix(transform, "add_axes transform");

    // Extract axes and position from transform
    glm::vec3 x_axis = glm::vec3(transform[0]);
    glm::vec3 y_axis = glm::vec3(transform[1]);
    glm::vec3 z_axis = glm::vec3(transform[2]);
    glm::vec3 position = glm::vec3(transform[3]);

    // Draw three colored lines from origin along each axis
    list.lines.push_back({position, position + x_axis * length, RGB_RED});
    list.lines.push_back({position, position + y_axis * length, RGB_GREEN});
    list.lines.push_back({position, position + z_axis * length, RGB_BLUE});
}

/**
 * Add RGB axes visualization at a specific position and orientation.
 *
 * Convenience overload for when you have position and rotation separately.
 *
 * @param list The primitive list to add to
 * @param position World position for the axes origin
 * @param rotation Rotation matrix or quaternion (converted to mat3)
 * @param length The length of each axis line
 */
inline void add_axes(debug_primitive_list& list, const glm::vec3& position,
                     const glm::mat3& rotation, float length = 1.0f) {
    FL_PRECONDITION(length > 0.0f, "axis length must be positive");
    FL_ASSERT_FINITE(position, "axes position");

    // Build transform from position and rotation
    glm::mat4 transform = glm::mat4(rotation);
    transform[3] = glm::vec4(position, 1.0f);

    add_axes(list, transform, length);
}

//==============================================================================
// Motion Vector Visualization
//==============================================================================

/**
 * Add velocity arrow visualization.
 *
 * Visualizes a velocity vector as a green arrow. Arrow length is proportional
 * to velocity magnitude, providing immediate visual feedback about speed.
 *
 * No arrow is drawn for negligible velocities (< 0.01 units/s).
 *
 * Preconditions:
 * - position and velocity must be finite
 * - scale must be positive
 *
 * @param list The primitive list to add to
 * @param position Start position for the arrow (entity position)
 * @param velocity Velocity vector (direction and magnitude)
 * @param scale Scale factor for arrow length (default 0.5 for visibility)
 */
inline void add_velocity_arrow(debug_primitive_list& list, const glm::vec3& position,
                               const glm::vec3& velocity, float scale = 0.5f) {
    FL_PRECONDITION(scale > 0.0f, "scale must be positive");
    FL_ASSERT_FINITE(position, "velocity arrow position");
    FL_ASSERT_FINITE(velocity, "velocity vector");

    float speed = glm::length(velocity);

    // Don't render negligible velocities
    constexpr float min_speed = 0.01f;
    if (speed < min_speed) {
        return;
    }

    glm::vec3 end = position + velocity * scale;

    // Arrow head size: 20% of shaft length, clamped to reasonable size
    float arrow_length = speed * scale;
    float head_size = glm::clamp(arrow_length * 0.2f, 0.05f, 0.15f);

    list.arrows.push_back({position, end, RGB_GREEN, head_size});

    // Postcondition: verify we added a valid arrow
    FL_POSTCONDITION(!list.arrows.empty(), "arrow must have been added");
}

/**
 * Add acceleration arrow visualization.
 *
 * Similar to velocity arrow but uses yellow color to distinguish
 * acceleration from velocity.
 *
 * @param list The primitive list to add to
 * @param position Start position for the arrow
 * @param acceleration Acceleration vector
 * @param scale Scale factor for arrow length (default 1.0)
 */
inline void add_acceleration_arrow(debug_primitive_list& list, const glm::vec3& position,
                                   const glm::vec3& acceleration, float scale = 1.0f) {
    FL_PRECONDITION(scale > 0.0f, "scale must be positive");
    FL_ASSERT_FINITE(position, "acceleration arrow position");
    FL_ASSERT_FINITE(acceleration, "acceleration vector");

    float magnitude = glm::length(acceleration);

    constexpr float min_magnitude = 0.01f;
    if (magnitude < min_magnitude) {
        return;
    }

    glm::vec3 end = position + acceleration * scale;
    float arrow_length = magnitude * scale;
    float head_size = glm::clamp(arrow_length * 0.2f, 0.05f, 0.15f);

    list.arrows.push_back({position, end, RGB_YELLOW, head_size});
}

/**
 * Add generic directional arrow.
 *
 * General-purpose arrow for visualizing any directional quantity.
 *
 * @param list The primitive list to add to
 * @param start Start position
 * @param direction Direction (will be normalized)
 * @param length Arrow length
 * @param color Arrow color
 */
inline void add_direction_arrow(debug_primitive_list& list, const glm::vec3& start,
                                const glm::vec3& direction, float length,
                                const glm::vec4& color = RGB_WHITE) {
    FL_PRECONDITION(length >= 0.0f, "length must be non-negative");
    FL_ASSERT_FINITE(start, "arrow start");
    FL_ASSERT_FINITE(direction, "arrow direction");

    if (length < 0.001f) {
        return;
    }

    float dir_len = glm::length(direction);
    if (dir_len < 0.001f) {
        return; // Can't normalize zero vector
    }

    glm::vec3 normalized_dir = direction / dir_len;
    glm::vec3 end = start + normalized_dir * length;
    float head_size = glm::clamp(length * 0.2f, 0.05f, 0.15f);

    FL_POSTCONDITION(glm::epsilonEqual(glm::length(normalized_dir), 1.0f, FL_EPSILON),
                     "direction must be normalized");

    list.arrows.push_back({start, end, color, head_size});
}

//==============================================================================
// Collision Boundary Visualization
//==============================================================================

/**
 * Add collision sphere wireframe.
 *
 * Visualizes a spherical collision boundary with magenta wireframe.
 * The radius should exactly match the physics collision radius.
 *
 * Preconditions:
 * - center must be finite
 * - radius must be positive
 *
 * @param list The primitive list to add to
 * @param center Sphere center (entity position)
 * @param radius Sphere radius (must match physics collision radius)
 * @param segments Wireframe resolution (4/6/8, default 8)
 */
inline void add_collision_sphere(debug_primitive_list& list, const glm::vec3& center, float radius,
                                 int segments = 8) {
    FL_PRECONDITION(radius > 0.0f, "sphere radius must be positive");
    FL_ASSERT_FINITE(center, "sphere center");
    FL_PRECONDITION(segments >= 4 && segments <= 8, "segments must be 4, 6, or 8");

    list.spheres.push_back({center, radius, RGB_MAGENTA, segments});

    // Postcondition: verify sphere was added with correct parameters
    FL_POSTCONDITION(list.spheres.back().radius == radius,
                     "sphere radius must match requested radius");
}

/**
 * Add sphere wireframe with custom color.
 *
 * @param list The primitive list to add to
 * @param center Sphere center
 * @param radius Sphere radius
 * @param color Wireframe color
 * @param segments Wireframe resolution
 */
inline void add_sphere(debug_primitive_list& list, const glm::vec3& center, float radius,
                       const glm::vec4& color, int segments = 8) {
    FL_PRECONDITION(radius > 0.0f, "sphere radius must be positive");
    FL_ASSERT_FINITE(center, "sphere center");

    list.spheres.push_back({center, radius, color, segments});
}

//==============================================================================
// Line Helpers
//==============================================================================

/**
 * Add simple line between two points.
 *
 * @param list The primitive list to add to
 * @param start Line start point
 * @param end Line end point
 * @param color Line color
 */
inline void add_line(debug_primitive_list& list, const glm::vec3& start, const glm::vec3& end,
                     const glm::vec4& color = RGB_WHITE) {
    FL_ASSERT_FINITE(start, "line start");
    FL_ASSERT_FINITE(end, "line end");

    list.lines.push_back({start, end, color});
}

} // namespace debug
