# Dependency Stack

**Living document tracking cascading uncertainty through system dependencies.**

## The Fundamental Truth

Uncertainty multiplies upward: **P(survival) = C^N**

Where C = certainty of each layer, N = number of dependent layers.

---

## Current Stack

```
THE UNKNOWN (<50% certain)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Liquid pool - no dependencies assumed:

[Skeletal System] [IK Systems] [Wall Detection] [Ragdoll]
[Bounce Gravity] [Dash] [Terrain] [Combat] [Audio] [UI]
[Acceleration Tilt] [Contact Springs] [Locomotion System]
[Walk/Run Blending] [Surveyor Wheel] [Stride Calculations]
[Frame-Independent Physics] [Explicit State Machine]

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                            ↑
                     PULL BOUNDARY
                            ↓
────────────────────────────────────────────────────────
NEEDS REPAIR (From Audit - 85% certain)
────────────────────────────────────────────────────────

**Critical Issues Found:**
• No debug visualization (can't see physics state)
• Accumulated state in position/velocity
• Magic numbers without justification
• Dual-reference violation in orientation
• Mixed concerns in controller

**Must Fix Before Expansion**

────────────────────────────────────────────────────────
                            ↓
────────────────────────────────────────────────────────
CURRENT STATE (90% certain)
────────────────────────────────────────────────────────

What exists (and mostly works):
• Physics sphere with WASD movement
• Single max_speed (no state complexity)
• Yaw orientation toward input (has issues)
• Landing spring (vertical compression)
• Jump with timing forgiveness
• Orbit/follow camera
• Debug trails (partial visualization)

Visual: Oriented box + collision sphere + spring offset

────────────────────────────────────────────────────────
                            ↓
────────────────────────────────────────────────────────
THE MECHANICS (90% certain)
────────────────────────────────────────────────────────

              ┌─────────────────┐
              │ Character State │ 90%
              │ • Landing Spring│
              │ • Yaw Rotation  │ ← Issues found
              └────────┬────────┘
                       │
              ┌────────▼────────┐
              │ Physics Control │ 90%
              │ (sphere + force)│ ← Accumulation issue
              └────────┬────────┘
                       │
         ┌─────────────┼─────────────┐
         ▼             ▼             ▼
   ┌──────────┐ ┌──────────┐ ┌──────────┐
   │Collision │ │  Camera  │ │  Input   │
   │  (AABB)  │ │  System  │ │  (WASD)  │
   └──────────┘ └──────────┘ └──────────┘
       95%          95%           95%

────────────────────────────────────────────────────────
                            ↓
────────────────────────────────────────────────────────
THE FOUNDATION (95% certain)
────────────────────────────────────────────────────────

         ┌─────────────────────────┐
         │   Mathematical Truth    │ 95%
         │ • Spring-damper math    │ ← Needs validation
         │ • Collision primitives  │
         │ • Quaternion operations │ ✓ Validated
         │ • Vector mathematics    │
         │ • Coordinate system     │ ✓ Documented
         └────────────┬────────────┘
                      │
         ┌────────────▼────────────┐
         │    Rendering Core       │ 100%
         │ • Scene graph           │
         │ • Debug visualization   │ ← Incomplete
         └────────────┬────────────┘
                      │
         ┌────────────▼────────────┐
         │    Runtime Loop         │ 100%
         │ • Frame timing          │
         │ • Event handling        │
         └─────────────────────────┘
```

---

## Post-Audit Status

### What Changed
The audit revealed the stack is mostly solid but has critical gaps:

1. **Debug Visualization Missing** - The lack of visual debugging caused the coordinate confusion crisis. This is the highest priority fix.

2. **Mathematical Issues** - Some operations aren't validated (spring-damper, friction), some accumulate error (position/velocity).

3. **Certainty Adjustments**:
   - Foundation: 100% → 95% (math needs validation)
   - Mechanics: 95% → 90% (issues found)
   - Current State: 95% → 90% (repairs needed)

### The Repair Layer
A new layer emerged from the audit: systems that exist but need fixing before expansion. These aren't in the Unknown because they're implemented, but they block progress until repaired.

---

## Cascade Impact

Current cascade with repairs needed:
- Foundation 95% × Mechanics 90% × Current 90% = **77% survival chance**

After repairs:
- Foundation 100% × Mechanics 95% × Current 95% = **90% survival chance**

**Implication**: Fix the foundation before building anything new.

---

## Priority Order (From Audit)

1. **Debug Visualization** - Cannot verify anything without seeing it
2. **Mathematical Validation** - Foundation must be solid
3. **Fix Accumulation** - Errors compound over time
4. **Document Constants** - Can't reason about magic
5. **Fix Circular References** - Violates principles
6. **Separate Concerns** - Cleaner dependencies

Only after these repairs should anything be pulled from the Unknown.

---

## When Dependencies Change

### The Audit Revealed
- Coordinate system IS documented (good)
- But can't be verified without visualization (bad)
- Mathematical operations seem correct (good)
- But aren't validated in isolation (bad)

### The Response
1. **Build visualization first** (see the truth)
2. **Validate mathematics** (verify the truth)
3. **Fix violations** (establish the truth)
4. **Then expand** (build on truth)

---

## The Planning Horizon

| Certainty | Horizon | Why |
|-----------|---------|-----|
| 100% | 3-8 points | Truth permits confident steps |
| 90% | 1-2 points | Minor adjustments only |
| 85% | Repair only | Fix before expanding |
| <70% | Do not build | Foundation too uncertain |

**Current Status: 85% - Repair Mode**

---

## The Discipline

**Build bottom-up**: Foundation must be perfect before mechanics.

**Validate in isolation**: Test the math before building on it.

**Accept the cascade**: When foundation shifts, everything above shifts.

**Visualize everything**: Cannot debug what cannot be seen.

**Track truthfully**: The audit found issues. Record them. Fix them.

---

## Meta-Structure

This stack now shows:
- What exists (Current State)
- What's broken (Needs Repair)
- What could exist (The Unknown)
- How they depend (The arrows)

The audit proved the value of this structure: it revealed exactly where certainty was false and what must be fixed.

When the territory changes, update the map.
When the map shows problems, fix the territory.

**This is the way.**