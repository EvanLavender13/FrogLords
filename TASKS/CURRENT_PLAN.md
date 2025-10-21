# Current Plan

**Status:** 🏗️ System - Car-Like Control Scheme
**Started:** 2025-10-20
**Branch:** system/car_control
**Complexity:** Small (2-3 pts)
**Phase:** SELECT → GRAYBOX

---

## Current Work

**System:** Transform WASD input from world-space strafe to character-relative forward/back and turn
**Location:** @TASKS/PLANS/CAR_CONTROL_SYSTEM.md
**Layer:** 2 (Primitives)
**Complexity:** Small
**Next:** GRAYBOX

**Dependencies:**
- Character orientation system (Layer 3)
- Input system (Layer 1)
- Character controller (Layer 3)

**Enables:**
- Orientation-locked camera mode (deferred prerequisite)
- Drift movement mechanics
