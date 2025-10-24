# Current Plan

**Status:** ✅ Completed Parameter Cleanup
**Branch:** system/circle-turning (merged cleanup only)
**Result:** Circle-based turning DEFERRED - incompatible with physics-first architecture

---

## Previous Work Summary

**Attempted:** Circle-based turning (direct position calculation)
**Problem:** Fundamentally incompatible with force → acceleration → velocity → position pipeline
**Resolution:**
- Removed unused parameters (wheelbase, grip_coefficient, max_steering_angle)
- Documented physics-first approach in PhysicsFirst_Research.md
- Updated BACKLOG_SYSTEMS.md to remove incompatible tasks

---

## Recommended Next Work

**System:** Dynamic FOV System
**Location:** New system to create
**Layer:** 4 (Systems)
**Complexity:** Small
**Why:** Zero physics impact, massive feel improvement, no dependencies

**Alternative:** Slip Angle Calculator
**Layer:** 2 (Primitives)
**Why:** Pure math primitive, enables drift detection, measurements only
