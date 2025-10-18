// Spring-Damper Validation Tests
// Mathematical verification of critical damping formula and behavior

#include "foundation/spring_damper.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>

// Test utilities
#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            printf("FAIL: %s\n  at %s:%d\n  condition: %s\n", msg, __FILE__, __LINE__, #cond); \
            exit(1); \
        } \
    } while (0)

#define TEST_ASSERT_NEAR(actual, expected, epsilon, msg) \
    do { \
        float diff = fabsf((actual) - (expected)); \
        if (diff > (epsilon)) { \
            printf("FAIL: %s\n  at %s:%d\n  expected: %f\n  actual: %f\n  diff: %f (epsilon: %f)\n", \
                   msg, __FILE__, __LINE__, (float)(expected), (float)(actual), diff, (float)(epsilon)); \
            exit(1); \
        } \
    } while (0)

#define RUN_TEST(test_func) \
    do { \
        printf("Running %s...\n", #test_func); \
        test_func(); \
        printf("  PASS\n"); \
    } while (0)

// Test 1: Critical Damping Formula Verification
// Verify that critical_damping() produces ζ=1 for various parameter combinations
void test_critical_damping_formula() {
    const float epsilon = 0.0001f;

    // Test cases: (k, m, expected_c)
    struct TestCase {
        float k;
        float m;
        float expected_c;
    } cases[] = {
        {100.0f, 1.0f, 20.0f},      // c = 2√(100·1) = 20
        {400.0f, 1.0f, 40.0f},      // c = 2√(400·1) = 40
        {100.0f, 2.0f, 28.284271f}, // c = 2√(100·2) = 2√200 ≈ 28.28
        {1.0f, 1.0f, 2.0f},         // c = 2√(1·1) = 2
        {1000.0f, 10.0f, 200.0f},   // c = 2√(1000·10) = 200
    };

    for (const auto& test : cases) {
        float c = critical_damping(test.k, test.m);
        TEST_ASSERT_NEAR(c, test.expected_c, epsilon, "Critical damping formula");

        // Verify ζ = c/(2√(km)) = 1.0
        float damping_ratio = c / (2.0f * sqrtf(test.k * test.m));
        TEST_ASSERT_NEAR(damping_ratio, 1.0f, epsilon, "Damping ratio should be exactly 1.0");
    }
}

// Test 2: No Overshoot Behavior
// Verify that critically damped spring never exceeds target
void test_no_overshoot() {
    spring_damper spring;
    spring.stiffness = 100.0f;
    spring.damping = critical_damping(spring.stiffness, 1.0f); // ζ=1
    spring.position = 0.0f;
    spring.velocity = 0.0f;

    const float target = 10.0f;
    const float dt = 0.016f; // 60 FPS
    const int num_steps = 200; // ~3.2 seconds

    float max_position = spring.position;

    for (int i = 0; i < num_steps; i++) {
        spring.update({target, dt});

        // Track maximum position reached
        if (spring.position > max_position) {
            max_position = spring.position;
        }

        // Critical damping: position should NEVER exceed target
        TEST_ASSERT(spring.position <= target + 0.001f, "Position should never overshoot target");
    }

    // Should approach target closely
    TEST_ASSERT_NEAR(spring.position, target, 0.1f, "Should settle near target");

    // Maximum position should not exceed target (no overshoot)
    TEST_ASSERT(max_position <= target + 0.001f, "Maximum position should not exceed target");
}

// Test 3: Monotonic Approach to Target
// Verify that critically damped spring approaches target monotonically (from below)
void test_monotonic_approach() {
    spring_damper spring;
    spring.stiffness = 100.0f;
    spring.damping = critical_damping(spring.stiffness, 1.0f);
    spring.position = 0.0f;
    spring.velocity = 0.0f;

    const float target = 10.0f;
    const float dt = 0.016f;
    const int num_steps = 200;

    float prev_distance = fabsf(target - spring.position);

    for (int i = 0; i < num_steps; i++) {
        spring.update({target, dt});

        float distance = fabsf(target - spring.position);

        // Distance to target should decrease monotonically (or stay same near equilibrium)
        TEST_ASSERT(distance <= prev_distance + 0.001f, "Distance to target should decrease monotonically");

        prev_distance = distance;
    }
}

// Test 4: Parameter Range Validation
// Test critical_damping() across wide range of physically reasonable parameters
void test_parameter_ranges() {
    const float epsilon = 0.001f;

    float stiffness_values[] = {1.0f, 10.0f, 100.0f, 1000.0f};
    float mass_values[] = {0.5f, 1.0f, 2.0f, 10.0f};

    for (float k : stiffness_values) {
        for (float m : mass_values) {
            float c = critical_damping(k, m);

            // Verify formula: c = 2√(km)
            float expected_c = 2.0f * sqrtf(k * m);
            TEST_ASSERT_NEAR(c, expected_c, epsilon, "Critical damping formula across parameter ranges");

            // Verify produces ζ=1
            float zeta = c / (2.0f * sqrtf(k * m));
            TEST_ASSERT_NEAR(zeta, 1.0f, epsilon, "Should produce ζ=1");

            // Critical damping coefficient should be positive
            TEST_ASSERT(c > 0.0f, "Damping coefficient should be positive");
        }
    }
}

// Test 5: Underdamped vs Critical vs Overdamped Comparison
// Verify that only critical damping (ζ=1) produces no overshoot
void test_damping_regimes() {
    const float k = 100.0f;
    const float m = 1.0f;
    const float c_critical = critical_damping(k, m);
    const float target = 10.0f;
    const float dt = 0.016f;
    const int num_steps = 200;

    // Test 1: Underdamped (ζ=0.5) should overshoot
    {
        spring_damper spring;
        spring.stiffness = k;
        spring.damping = c_critical * 0.5f; // ζ=0.5
        spring.position = 0.0f;
        spring.velocity = 0.0f;

        float max_position = 0.0f;
        for (int i = 0; i < num_steps; i++) {
            spring.update({target, dt});
            if (spring.position > max_position) {
                max_position = spring.position;
            }
        }

        // Underdamped should overshoot
        TEST_ASSERT(max_position > target + 0.1f, "Underdamped spring should overshoot target");
    }

    // Test 2: Critically damped (ζ=1.0) should NOT overshoot
    {
        spring_damper spring;
        spring.stiffness = k;
        spring.damping = c_critical; // ζ=1.0
        spring.position = 0.0f;
        spring.velocity = 0.0f;

        float max_position = 0.0f;
        for (int i = 0; i < num_steps; i++) {
            spring.update({target, dt});
            if (spring.position > max_position) {
                max_position = spring.position;
            }
        }

        // Critical damping should NOT overshoot
        TEST_ASSERT(max_position <= target + 0.001f, "Critically damped spring should NOT overshoot");
    }

    // Test 3: Overdamped (ζ=2.0) should NOT overshoot but settle slower
    {
        spring_damper spring;
        spring.stiffness = k;
        spring.damping = c_critical * 2.0f; // ζ=2.0
        spring.position = 0.0f;
        spring.velocity = 0.0f;

        float max_position = 0.0f;
        float critical_position_at_100 = 0.0f;

        // Run critical damping to 100 steps for comparison
        {
            spring_damper spring_crit;
            spring_crit.stiffness = k;
            spring_crit.damping = c_critical;
            spring_crit.position = 0.0f;
            spring_crit.velocity = 0.0f;
            for (int i = 0; i < 100; i++) {
                spring_crit.update({target, dt});
            }
            critical_position_at_100 = spring_crit.position;
        }

        // Run overdamped to 100 steps
        for (int i = 0; i < 100; i++) {
            spring.update({target, dt});
            if (spring.position > max_position) {
                max_position = spring.position;
            }
        }

        // Overdamped should NOT overshoot
        TEST_ASSERT(max_position <= target + 0.001f, "Overdamped spring should NOT overshoot");

        // Overdamped should settle slower than critical (position should be less at step 100)
        TEST_ASSERT(spring.position < critical_position_at_100 - 0.1f, "Overdamped should settle slower than critical");
    }
}

int main() {
    printf("=== Spring-Damper Validation Tests ===\n\n");

    RUN_TEST(test_critical_damping_formula);
    RUN_TEST(test_no_overshoot);
    RUN_TEST(test_monotonic_approach);
    RUN_TEST(test_parameter_ranges);
    RUN_TEST(test_damping_regimes);

    printf("\n=== All tests passed! ===\n");
    return 0;
}
