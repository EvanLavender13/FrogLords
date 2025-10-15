# Principle Review: Secondary Motion Joint Limits

**Date:** 2025-10-14
**Status:** APPROVED

### Summary

Plan is tightly scoped and aligned with principles. It adds simple, local clamps on existing spring offsets to stop elbow/knee hyperextension without changing core control or broader animation flow. Ownership is correct (lives in character animation; UI in character panel). Recommend one minor improvement at the clamp boundary to avoid limit jitter: zero outward velocity when clamped. Proceed with graybox implementation.

### Violations (Must Fix)

**If none:** No violations found.

### Risks & Concerns

- **Hard clamp jitter at limits:** Springs may hit the hard limit and continue integrating outward velocity for a frame, causing micro-jitter at the boundary (visible at high stiffness).  
  - **Mitigation:** When `offset` is clamped and `velocity` is pushing further out (sign matches the clamp boundary), set `velocity = 0`. Keeps motion stable at the boundary.  
  - **Principle:** Million-repetition thinking; do no harm.

- **Radians↔degrees UI conversion drift:** Presenting limits in degrees but storing radians risks double-conversion bugs if not done carefully.  
  - **Mitigation:** Convert for display only: copy rad→deg to local vars, use sliders on copies, write back only if changed (deg→rad).  
  - **Principle:** Clarity over cleverness; simplicity over sophistication.

- **Axis correctness across joints:** Current secondary-motion axes are Y (elbows) and X (knees). If future pose authoring changes bone local axes, limit ranges might not match perceived bend direction.  
  - **Mitigation:** Add a one-line comment near each update_spring call documenting assumed bend axis; revisit if skeleton rest pose changes.  
  - **Principle:** Clarity; capture assumptions.

### Actionable Items

- [ ] Add per-joint limit fields in `secondary_motion_state` (radians), with defaults: min=0, max≈2.618 (150°)
- [ ] In `update_secondary_motion()` clamp AFTER integration and BEFORE applying rotation: `offset = clamp(offset, min, max)`
- [ ] At clamp boundary, if clamped and `velocity` would push further outward, set `velocity = 0`
- [ ] Thread `min/max` through the `update_spring` lambda and wire all four joints
- [ ] Character Panel: expose limits in degrees with robust rad↔deg conversion (display-only copies)
- [ ] Add short code comments: where clamp sits in the flow; assumed hinge axes per joint
- [ ] Quick smoke checks: jump mid-stride and rapid walk↔run toggles; verify no hyperextension and no visible snapping

### Recommendation

**Reasoning:**
- Aligns with “Graybox first” and “Simplicity over sophistication” by choosing a hard clamp over IK cones.  
- Respects layered architecture: reactive layer reads motion and applies visual-only changes—no control theft.  
- Scope is minimal (few lines + UI), with clear insertion points and no cross-system churn.  
- Builds directly on a stable dependency (secondary motion springs), matching the dependency stack’s certainty.

**Next Steps:**
- Proceed to EXECUTE with the velocity-zeroing tweak and careful UI conversion.
