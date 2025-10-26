#pragma once

#include "vehicle/controller.h"
#include "foundation/param_meta.h"

// Forward declarations
struct vehicle_reactive_systems;

namespace vehicle {

struct tuning_params {
    // TUNED: Maximum horizontal movement speed (source of truth)
    // Real-world context: 8.0 m/s ≈ 17.9 mph (fast jogging pace)
    // Used in: apply_to to copy to controller.max_speed
    float max_speed = 8.0f; // m/s

    // TUNED: Horizontal acceleration (direct physical parameter)
    // Controls responsiveness - higher = snappier feel
    // Used in: apply_to to copy to controller.accel
    float accel = 5.0f; // m/s²

    // TUNED: Vehicle mass (source of truth for force-based physics)
    // Foundation for F=ma calculations in future lateral force systems
    // Real-world context: 150 kg ≈ 330 lbs (lightweight vehicle/go-kart)
    // Used in: apply_to to copy to controller.mass
    float mass = 150.0f; // kg

    // TUNED: Turn rate for car-like control heading (source of truth)
    // Controls rotational speed when using heading-based movement
    // Higher values = faster turning (arcade feel)
    // Real-world context: 3.0 rad/s ≈ 172°/s turning rate
    // Used in: apply_to to copy to controller.turn_rate
    float turn_rate = 3.0f; // radians/second

    // TUNED: Steering reduction factor for speed-dependent steering limits (source of truth)
    // Controls how much steering authority decreases at high speeds
    // Formula: multiplier = 1.0 - (speed/max_speed) * reduction_factor
    // At max_speed with reduction_factor=0.7: 30% steering authority remains
    // Used in: apply_to to copy to controller.steering_reduction_factor
    float steering_reduction_factor = 0.7f; // dimensionless [0,1]

    // TUNED: Handbrake drag coefficient (source of truth)
    // Drag coefficient contribution when handbrake engaged
    // Added to base drag in friction model for unified exponential integration
    // Higher values = stronger braking
    // Used in: apply_to to copy to controller.handbrake.brake_rate
    float brake_rate = 2.0f; // 1/s (drag coefficient)

    // NOTE: Friction removed - drag now derived from accel/max_speed
    // Drag coefficient k = accel / max_speed guarantees equilibrium at max_speed
    // See controller::update for exponential drag model implementation

    // TUNED: Visual tilt parameters (lean and pitch)
    // Controls how much the vehicle visual model tilts in response to forces
    float lean_multiplier = 0.3f;   // radians per g (lateral tilt in corners)
    float pitch_multiplier = 0.05f; // radians per m/s² (pitch during accel/brake)
    float tilt_stiffness = 150.0f;  // Spring stiffness for tilt response

    // TUNED: Orientation system parameters
    // Controls how smoothly the vehicle rotates to face velocity direction
    float orientation_stiffness = 25.0f; // Spring stiffness for yaw rotation

    // Parameter metadata for GUI presentation
    static constexpr param_meta max_speed_meta = {"Max Speed", "m/s", 1.0f, 15.0f};

    static constexpr param_meta accel_meta = {"Acceleration", "m/s²", 1.0f, 50.0f};

    static constexpr param_meta mass_meta = {"Mass", "kg", 50.0f, 500.0f};

    static constexpr param_meta turn_rate_meta = {"Turn Rate", "rad/s", 1.0f, 10.0f};

    static constexpr param_meta steering_reduction_factor_meta = {"Steering Reduction", "", 0.0f,
                                                                  1.0f};

    static constexpr param_meta brake_rate_meta = {"Brake Rate", "1/s", 0.5f, 10.0f};

    static constexpr param_meta lean_multiplier_meta = {"Lean Multiplier", "rad/g", 0.0f, 1.0f};

    static constexpr param_meta pitch_multiplier_meta = {"Pitch Multiplier", "rad/(m/s²)", 0.0f,
                                                         0.2f};

    static constexpr param_meta tilt_stiffness_meta = {"Tilt Stiffness", "N/m", 10.0f, 500.0f};

    static constexpr param_meta orientation_stiffness_meta = {"Orientation Stiffness", "N/m", 5.0f,
                                                              500.0f};

    void apply_to(controller& c, vehicle_reactive_systems& visuals) const;
};

} // namespace vehicle
