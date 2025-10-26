# Current Plan

**Status:** 🏗️ System - Vehicle Mass Property
**Started:** 2025-10-26
**Branch:** system/vehicle-mass
**Complexity:** Small
**Phase:** SELECT → GRAYBOX

---

## Current Work

**System:** Vehicle Mass Property - Foundation for force-based physics

**Location:** @TASKS/PLANS/VEHICLE_MASS_SYSTEM.md

**Layer:** 4 (Variation within vehicle movement system)

**Complexity:** Small

**Next:** GRAYBOX

**Dependencies:**
- ✅ Parameter metadata system (Layer 2)
- ✅ Vehicle tuning system (Layer 3)
- ✅ Vehicle movement system (Layer 3)

**Why this system:**
- Zero blockers - all dependencies exist
- Enables entire physics-driven drift chain
- Fixes architectural lie: weight is acceleration, should be force
- Single source of truth: gravity constant currently duplicated

**Core change:**
- Add `mass` parameter to tuning system
- Replace `weight` (acceleration) with proper F=ma force calculation
- Establish gravity constant as single source of truth
- Enable future lateral force application (F/m = a)
