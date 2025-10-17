#pragma once

/**
 * Debug Visualization Toggle Control
 *
 * Provides global control for enabling/disabling debug visualization.
 * Toggle with F3 key (standard debug convention).
 *
 * Philosophy:
 * - Zero gameplay impact when enabled or disabled
 * - No performance cost when disabled (primitives not generated)
 * - Simple boolean state, no complex configuration
 * - Immediate visual feedback (on/off, nothing in between)
 *
 * Usage:
 *   if (debug_viz::is_enabled()) {
 *       debug::add_axes(list, transform, 1.0f);
 *   }
 */

namespace debug_viz {

/**
 * Enable or disable debug visualization.
 *
 * When disabled, all debug visualization helpers should skip
 * adding primitives to the list.
 *
 * @param enabled True to enable visualization, false to disable
 */
void set_enabled(bool enabled);

/**
 * Check if debug visualization is currently enabled.
 *
 * @return True if visualization is enabled, false otherwise
 */
bool is_enabled();

/**
 * Toggle debug visualization on/off.
 *
 * Called when F3 key is pressed.
 */
void toggle();

} // namespace debug_viz
