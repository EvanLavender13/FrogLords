#pragma once

/**
 * Debug Visualization Startup Validation
 *
 * Verifies mathematical assumptions about the coordinate system,
 * transformation matrices, and GLM library behavior.
 *
 * These checks run once at startup in debug builds to catch
 * fundamental misconfigurations before they cause subtle bugs.
 *
 * Philosophy:
 * - Fail fast: crash immediately if assumptions are wrong
 * - Explicit verification: don't trust, validate
 * - Zero runtime cost: compile out in release builds
 * - Clear error messages: assertion failures explain what's wrong
 *
 * Usage:
 *   debug_viz::validation::run_startup_checks();
 */

namespace debug_viz::validation {

/**
 * Run all startup validation checks.
 *
 * Verifies:
 * - GLM uses right-handed coordinate system (X × Y = Z)
 * - Rotation matrices preserve orthonormality
 * - Transform matrices extract correctly
 * - Basis vectors are correct
 *
 * In debug builds: Runs checks, asserts on failure
 * In release builds: No-op (compiles out completely)
 *
 * Call this once at application startup, before any other
 * debug visualization code runs.
 */
void run_startup_checks();

} // namespace debug_viz::validation
