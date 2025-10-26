#include "vehicle/controller.h"
#include "foundation/collision.h"
#include "foundation/math_utils.h"
#include "foundation/debug_assert.h"
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <algorithm>
#include <cmath>

namespace {
// Single-sphere collision configuration (experiment branch)
// Primary sphere handles all collision resolution

// TUNED: Collision sphere radius (character physical size)
// World scale: 0.5m radius = 1.0m diameter sphere (human-scale)
// Defines capsule/sphere collision volume for all character interactions
// Used in: controller constructor to initialize collision_sphere.radius
constexpr float BUMPER_RADIUS = 0.50f; // meters

// CALCULATED: Initial spawn height for sphere resting on ground plane
// Geometric derivation: Sphere resting on Y=0 has center at Y = radius
// Contact point: center.y - radius = 0 → center.y = radius
// Used in: controller constructor to set initial position.y
constexpr float STANDING_HEIGHT = BUMPER_RADIUS; // meters (= 0.5m)

} // namespace

controller::controller()
    : position(0.0f, STANDING_HEIGHT, 0.0f)
    , velocity(0.0f)
    , acceleration(0.0f) {
    // Validate steering_reduction_factor bounds (ensures monotonic steering decrease)
    FL_PRECONDITION(steering_reduction_factor >= 0.0f && steering_reduction_factor <= 1.0f,
                    "steering_reduction_factor must be in [0, 1]");
    FL_ASSERT_FINITE_SCALAR(steering_reduction_factor, "steering_reduction_factor");

    // Initialize single collision sphere
    collision_sphere.center = position;
    collision_sphere.radius = BUMPER_RADIUS;
}

float controller::compute_steering_multiplier(float horizontal_speed) const {
    FL_PRECONDITION(horizontal_speed >= 0.0f, "speed must be non-negative");
    FL_PRECONDITION(std::isfinite(horizontal_speed), "speed must be finite");
    FL_PRECONDITION(max_speed > 0.0f, "max_speed must be positive");

    // Compute speed ratio and clamp to [0, 1] to handle overspeed
    float speed_ratio = horizontal_speed / max_speed;
    speed_ratio = glm::clamp(speed_ratio, 0.0f, 1.0f);

    // Calculate steering multiplier: full authority at zero speed, reduced at high speed
    float multiplier = 1.0f - (speed_ratio * steering_reduction_factor);

    FL_POSTCONDITION(multiplier >= 0.0f && multiplier <= 1.0f,
                     "steering multiplier must be in [0, 1]");
    return multiplier;
}

float controller::calculate_slip_angle() const {
    glm::vec3 horizontal_velocity = math::project_to_horizontal(velocity);
    glm::vec3 forward = math::yaw_to_forward(heading_yaw);
    return math::calculate_slip_angle(horizontal_velocity, forward);
}

float controller::calculate_lateral_g_force() const {
    float horizontal_speed = glm::length(math::project_to_horizontal(velocity));
    return math::calculate_lateral_g_force(horizontal_speed, angular_velocity);
}

void controller::apply_input(const controller_input_params& input_params,
                             const camera_input_params& cam_params, float dt) {
    // Integrate heading from turn input with speed-dependent steering limits
    // Time-independent heading integration: heading_yaw += -turn_input * turn_rate *
    // steering_multiplier * dt Coordinate system: Y-up right-handed (+X right), positive yaw = CCW
    // rotation from above Input convention: positive = right turn, negative = left turn Yaw
    // convention: positive = CCW rotation, so negate input to map right → -yaw (CW)
    FL_PRECONDITION(dt > 0.0f && std::isfinite(dt),
                    "dt must be positive and finite for time-independent integration");
    FL_PRECONDITION(std::isfinite(turn_rate), "turn_rate must be finite");
    FL_PRECONDITION(std::isfinite(input_params.turn_input), "turn_input must be finite");
    FL_PRECONDITION(std::isfinite(heading_yaw), "heading_yaw must be finite before integration");

    // Calculate speed-dependent steering multiplier
    glm::vec3 horizontal_velocity = math::project_to_horizontal(velocity);
    float horizontal_speed = glm::length(horizontal_velocity);
    float steering_multiplier = compute_steering_multiplier(horizontal_speed);

    // Store previous heading before integration
    previous_heading_yaw = heading_yaw;

    // Integrate heading
    heading_yaw += -input_params.turn_input * turn_rate * steering_multiplier * dt;
    heading_yaw = math::wrap_angle_radians(heading_yaw);

    FL_POSTCONDITION(std::isfinite(heading_yaw),
                     "heading_yaw must remain finite after integration");
    FL_POSTCONDITION(heading_yaw >= -glm::pi<float>() && heading_yaw <= glm::pi<float>(),
                     "heading_yaw must be wrapped to [-π, π]");

    // Derive angular velocity from heading change (wrap-safe)
    // CRITICAL: Must use angle_difference to handle ±π wrap boundaries
    // Raw subtraction would spike to ±2π when wrapping
    float heading_delta = math::angle_difference_radians(heading_yaw, previous_heading_yaw);
    angular_velocity = heading_delta / dt;

    FL_POSTCONDITION(std::isfinite(angular_velocity), "angular_velocity must be finite");

    // Convert 2D input to 3D acceleration (basis-relative)
    // Basis provided by composition layer: camera vectors or heading vectors
    glm::vec3 forward = cam_params.forward;
    glm::vec3 right = cam_params.right;

    input_direction =
        forward * input_params.move_direction.y + right * input_params.move_direction.x;

    // Direct acceleration (instant response, no ground/air distinction)
    acceleration = input_direction * accel;

    // Update handbrake state from input
    handbrake.update(input_params.handbrake);
}

void controller::update(const collision_world* world, float dt) {
    FL_PRECONDITION(dt > 0.0f, "dt must be positive for frame-rate independence");
    FL_PRECONDITION(std::isfinite(dt), "dt must be finite");

    update_physics(dt);
    update_collision(world, dt);
}

void controller::update_collision(const collision_world* world, float dt) {
    // Derive wall threshold from max_slope_angle (single source of truth)
    float wall_threshold = glm::cos(glm::radians(max_slope_angle));

    sphere_collision contact =
        resolve_collisions(collision_sphere, *world, position, velocity, wall_threshold);

    // Store collision debug info
    collision_contact_debug.active = contact.hit;
    collision_contact_debug.normal = contact.normal;
    collision_contact_debug.penetration = contact.penetration;

    // Interpret contact to determine grounded state (controller logic)
    // Use contacted_floor flag to handle simultaneous floor+wall contacts
    is_grounded = false;
    if (contact.contacted_floor) {
        is_grounded = true;
    }
}

void controller::update_physics(float dt) {
    // Frame-rate independent physics integration
    //
    // Horizontal velocity: Exponential drag model (exact solution)
    //   Solves: dv/dt = a - k*v  where k = accel/max_speed
    //   Solution: v(t+dt) = v(t)*exp(-k*dt) + (a/k)*(1 - exp(-k*dt))
    //
    //   Guarantees:
    //   - Equilibrium at max_speed when a = accel (full input)
    //   - Frame-rate independent: identical behavior at any dt
    //   - Allows overspeed: dash mechanics can exceed max_speed, decay back naturally
    //   - Exponential convergence: smooth approach to equilibrium
    //
    // Vertical velocity: Standard Euler integration (weight only)
    //   No drag on vertical component - constant downward force
    //
    // See PRINCIPLES.md: Time-Independence, Solid Mathematical Foundations

    // Apply weight force as acceleration (F=ma: weight_force/mass = acceleration)
    // Weight force: F = m * g (downward)
    // Weight acceleration: a = F/m = (m * g)/m = g
    // Sign: negative because Y-up coordinate system (gravity pulls down)
    float weight_accel = (mass * -math::GRAVITY) / mass; // Simplifies to -GRAVITY
    acceleration.y += weight_accel;

    // Unified drag coefficient from friction model
    // Composes: base drag (equilibrium) + handbrake drag + future modifiers
    // Time-independent: single exponential integrator with correct particular solution
    float k = friction.compute_total_drag(accel, max_speed, handbrake.is_active(), handbrake.brake_rate);

    FL_POSTCONDITION(k > 0.0f && std::isfinite(k), "drag coefficient must be positive and finite");

    // Extract horizontal components
    glm::vec3 horizontal_accel = math::project_to_horizontal(acceleration);
    glm::vec3 horizontal_velocity = math::project_to_horizontal(velocity);

    // Store pre-update horizontal speed for assertion
    float speed_before = glm::length(horizontal_velocity);

    // Apply frame-rate independent horizontal update
    if (k < 1e-6f) {
        // Fallback for degenerate case (near-zero accel or very large max_speed)
        // Standard Euler integration when drag is negligible
        horizontal_velocity += horizontal_accel * dt;
    } else {
        // Exact exponential solution
        float decay = std::exp(-k * dt);
        horizontal_velocity = horizontal_velocity * decay + (horizontal_accel / k) * (1.0f - decay);
    }

    // Integrate vertical velocity (semi-implicit Euler - no drag)
    // Semi-implicit: v += a*dt, then x += v*dt (uses updated velocity)
    // This is the project standard per TASKS/CONTEXT/PHYSICS_INTEGRATION_PATTERNS.md
    // Chosen for: stability, simplicity, speed in damped gameplay systems
    velocity.y += acceleration.y * dt;

    // Reconstruct full velocity (horizontal + vertical)
    velocity.x = horizontal_velocity.x;
    velocity.z = horizontal_velocity.z;

    // Zero-velocity tolerance: exponential decay never fully stops
    // When horizontal speed drops below perceptible threshold AND no input applied,
    // snap to zero to prevent residual drift from numerical precision
    //
    // CRITICAL: Only apply when no input present (Prime Directive)
    // If input is active, velocity must accumulate even if below epsilon
    // Otherwise low acceleration prevents movement from standstill
    constexpr float VELOCITY_EPSILON = 0.01f; // m/s (imperceptible at 60fps)
    constexpr float ACCEL_EPSILON = 0.01f;    // m/s² (negligible acceleration)
    float horizontal_speed = glm::length(horizontal_velocity);
    float accel_magnitude = glm::length(horizontal_accel);

    // Only clamp when decelerating (no input), not when accelerating
    if (horizontal_speed < VELOCITY_EPSILON && accel_magnitude < ACCEL_EPSILON) {
        velocity.x = 0.0f;
        velocity.z = 0.0f;
        horizontal_speed = 0.0f;
    }

    // Validate frame-rate independent drag behavior
    // When no input applied, drag should decrease speed (or maintain zero)
    if (accel_magnitude < ACCEL_EPSILON) {
        // No input: speed must decrease or stay at zero
        // Allow small epsilon for numerical precision (1% tolerance)
        FL_POSTCONDITION(horizontal_speed <= speed_before * 1.01f,
                         "horizontal speed must decrease when no input applied");
    }

    // Integrate position (accumulate - required for physics)
    position += velocity * dt;

    // Reset acceleration for next frame
    acceleration = glm::vec3(0, 0, 0);
}
