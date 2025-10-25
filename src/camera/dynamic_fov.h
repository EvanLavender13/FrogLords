#pragma once
#include "foundation/param_meta.h"

// Forward declarations
struct controller;
class camera;

/**
 * dynamic_fov_system
 *
 * Reactive system that modulates camera FOV based on vehicle speed and lateral g-force.
 * Reads physics state from controller, calculates target FOV, writes to camera.
 *
 * Data flow: controller (physics) → dynamic_fov (calculation) → camera (presentation)
 *
 * Formula: FOV = clamp(base + saturate(speed/max_speed) * range + |lateral_g| * g_multiplier,
 * base, base+range)
 */
struct dynamic_fov_system {
    // Tunable parameters (single source of truth for FOV)
    // BASE: Minimum FOV at zero speed (arcade feel: 75-90°)
    // Industry: Arcade racers use wider base than sim racers (40-60°)
    float base_fov = 75.0f; // degrees

    // RANGE: Maximum additional FOV at top speed (typical: 30-40°)
    // Research: Base 75° → Max 105° creates strong speed sensation
    float max_fov_range = 30.0f; // degrees

    // G_MULTIPLIER: FOV increase per g-force (typical: 0.5-2.0)
    // Enhances corner sensation by widening view during high-g turns
    float g_multiplier = 2.0f; // degrees per g

    /**
     * Update camera FOV based on current vehicle state
     * Call after vehicle physics update, before rendering
     *
     * @param ctrl Controller providing speed and lateral g-force
     * @param cam Camera to modify
     */
    void update(const controller& ctrl, camera& cam);

    // Metadata for tuning UI
    static constexpr param_meta base_fov_meta = {"Base FOV", "degrees", 60.0f, 90.0f};
    static constexpr param_meta max_fov_range_meta = {"Max FOV Range", "degrees", 0.0f, 40.0f};
    static constexpr param_meta g_multiplier_meta = {"G-Force Multiplier", "deg/g", 0.0f, 5.0f};
};
