# Current Plan

**Status:** 🏗️ System - Slip Angle Calculator
**Started:** 2025-10-24
**Branch:** system/slip-angle
**Complexity:** Small
**Phase:** SELECT → GRAYBOX

---

## Current Work

**System:** Pure mathematical primitive measuring angle between velocity vector and forward direction
**Location:** @TASKS/PLANS/SLIP_ANGLE_SYSTEM.md
**Layer:** 2 (Primitives)
**Complexity:** Small
**Next:** GRAYBOX

**Dependencies:**
- GLM (exists)
- math::project_to_horizontal (Layer 2, exists)

**Enables:**
- Physics-Based Drift System (Layer 4)
- Drift Detection (Layer 4)
- Slip Angle Visualization (Layer 5)
- Individual Tire Simulation (future)
