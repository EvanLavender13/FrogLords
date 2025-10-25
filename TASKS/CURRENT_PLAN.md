# Current Plan

**Status:** 🏗️ System - Vehicle Tilt
**Started:** 2025-10-24
**Branch:** system/vehicle-tilt
**Complexity:** Small
**Phase:** SELECT → GRAYBOX

---

## Current Work

**System:** Visual weight transfer through model tilting based on lateral acceleration and pitch
**Location:** @TASKS/PLANS/vehicle_tilt_SYSTEM.md
**Layer:** 4 (Variation)
**Complexity:** Small
**Next:** GRAYBOX

**Dependencies:**
- Lateral G-Force Calculator (Layer 2)
- Vehicle Movement System (Layer 3)

**Key Requirements:**
- Change debug box from tall (character) to long (vehicle) proportions
- New vehicle visual system (separate from legacy character code)
- Lean in corners based on lateral g-force
- Pitch during acceleration/braking
