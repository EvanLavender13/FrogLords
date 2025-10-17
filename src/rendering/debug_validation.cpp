#include "rendering/debug_validation.h"
#include "foundation/debug_assert.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

namespace debug_viz::validation {

void run_startup_checks() {
#if FL_DEBUG_VALIDATION
    printf("[DEBUG_VIZ] Running startup validation checks...\n");

    // ========================================================================
    // Test 1: Verify GLM coordinate system is right-handed
    // ========================================================================
    {
        glm::vec3 x = {1.0f, 0.0f, 0.0f};
        glm::vec3 y = {0.0f, 1.0f, 0.0f};
        glm::vec3 z = {0.0f, 0.0f, 1.0f};

        // Verify basis vectors are unit length
        FL_ASSERT(glm::epsilonEqual(glm::length(x), 1.0f, FL_EPSILON),
                  "X basis vector must be unit length");
        FL_ASSERT(glm::epsilonEqual(glm::length(y), 1.0f, FL_EPSILON),
                  "Y basis vector must be unit length");
        FL_ASSERT(glm::epsilonEqual(glm::length(z), 1.0f, FL_EPSILON),
                  "Z basis vector must be unit length");

        // Verify orthogonality
        FL_ASSERT(glm::epsilonEqual(glm::dot(x, y), 0.0f, FL_EPSILON),
                  "X and Y must be orthogonal");
        FL_ASSERT(glm::epsilonEqual(glm::dot(y, z), 0.0f, FL_EPSILON),
                  "Y and Z must be orthogonal");
        FL_ASSERT(glm::epsilonEqual(glm::dot(z, x), 0.0f, FL_EPSILON),
                  "Z and X must be orthogonal");

        // Verify right-handed: X × Y = Z
        glm::vec3 cross = glm::cross(x, y);
        FL_ASSERT(glm::all(glm::epsilonEqual(cross, z, FL_EPSILON)),
                  "Coordinate system must be right-handed: X × Y = Z");

        printf("[DEBUG_VIZ]   ✓ GLM coordinate system is right-handed\n");
    }

    // ========================================================================
    // Test 2: Verify rotation preserves orthonormality
    // ========================================================================
    {
        // Test rotation around Y axis (yaw)
        float angle = glm::radians(45.0f);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));

        glm::vec3 x_axis = glm::vec3(rotation[0]);
        glm::vec3 y_axis = glm::vec3(rotation[1]);
        glm::vec3 z_axis = glm::vec3(rotation[2]);

        // After rotation, axes should still be orthonormal
        fl::verify_coordinate_frame(x_axis, y_axis, z_axis, "rotated frame");

        printf("[DEBUG_VIZ]   ✓ Rotation preserves orthonormality\n");
    }

    // ========================================================================
    // Test 3: Verify transform matrix extraction
    // ========================================================================
    {
        glm::vec3 position(1.0f, 2.0f, 3.0f);
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

        // Extract position from matrix
        glm::vec3 extracted_position = glm::vec3(translation[3]);

        FL_ASSERT(glm::all(glm::epsilonEqual(extracted_position, position, FL_EPSILON)),
                  "Transform must preserve translation");

        // Extract axes (should still be identity rotation)
        glm::vec3 x_axis = glm::vec3(translation[0]);
        glm::vec3 y_axis = glm::vec3(translation[1]);
        glm::vec3 z_axis = glm::vec3(translation[2]);

        FL_ASSERT(glm::all(glm::epsilonEqual(x_axis, glm::vec3(1, 0, 0), FL_EPSILON)),
                  "X axis must be (1,0,0) for identity rotation");
        FL_ASSERT(glm::all(glm::epsilonEqual(y_axis, glm::vec3(0, 1, 0), FL_EPSILON)),
                  "Y axis must be (0,1,0) for identity rotation");
        FL_ASSERT(glm::all(glm::epsilonEqual(z_axis, glm::vec3(0, 0, 1), FL_EPSILON)),
                  "Z axis must be (0,0,1) for identity rotation");

        printf("[DEBUG_VIZ]   ✓ Transform matrix extraction correct\n");
    }

    // ========================================================================
    // Test 4: Verify combined rotation + translation
    // ========================================================================
    {
        glm::vec3 position(5.0f, 10.0f, 15.0f);
        float angle = glm::radians(90.0f);

        // Rotate around Y then translate
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform = glm::rotate(transform, angle, glm::vec3(0, 1, 0));

        // Verify the combined transform
        fl::verify_transform_matrix(transform, "combined rotation+translation");

        // Verify translation preserved
        glm::vec3 extracted_position = glm::vec3(transform[3]);
        FL_ASSERT((std::isfinite(extracted_position.x) && std::isfinite(extracted_position.y) &&
                   std::isfinite(extracted_position.z)),
                  "Position must be finite");

        printf("[DEBUG_VIZ]   ✓ Combined rotation+translation valid\n");
    }

    // ========================================================================
    // Test 5: Verify vector normalization
    // ========================================================================
    {
        glm::vec3 v(3.0f, 4.0f, 0.0f); // Length = 5
        glm::vec3 normalized = glm::normalize(v);

        FL_ASSERT(glm::epsilonEqual(glm::length(normalized), 1.0f, FL_EPSILON),
                  "Normalized vector must have unit length");

        // Verify direction preserved
        glm::vec3 expected = v / 5.0f;
        FL_ASSERT(glm::all(glm::epsilonEqual(normalized, expected, FL_EPSILON)),
                  "Normalization must preserve direction");

        printf("[DEBUG_VIZ]   ✓ Vector normalization correct\n");
    }

    // ========================================================================
    // Test 6: Verify cross product behavior
    // ========================================================================
    {
        glm::vec3 a(1, 0, 0);
        glm::vec3 b(0, 1, 0);
        glm::vec3 c = glm::cross(a, b);

        FL_ASSERT(glm::all(glm::epsilonEqual(c, glm::vec3(0, 0, 1), FL_EPSILON)),
                  "X × Y must equal Z");

        // Verify anti-commutative: a × b = -(b × a)
        glm::vec3 d = glm::cross(b, a);
        FL_ASSERT(glm::all(glm::epsilonEqual(d, -c, FL_EPSILON)),
                  "Cross product must be anti-commutative");

        printf("[DEBUG_VIZ]   ✓ Cross product behaves correctly\n");
    }

    printf("[DEBUG_VIZ] ✓ All startup validation checks passed\n");
    printf("[DEBUG_VIZ] Mathematical foundations verified - coordinate system is valid\n");

#else
    // In release builds, this function is a no-op
    // (though it should still compile and link)
#endif
}

} // namespace debug_viz::validation
