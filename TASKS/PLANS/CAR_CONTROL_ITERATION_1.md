# Iteration 1: Car-Like Control Scheme

**Started:** 2025-10-21
**Status:** In Progress

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Define what must be proven about this system. Choose categories that serve validation.

### Time Independence
- [x] Heading integration produces identical rotation regardless of frame rate (c7a8e91)
- [x] Turn rate scaling by dt prevents frame-dependent behavior (c7a8e91)
- [x] Angle wrapping maintains correctness across time steps (c7a8e91)

### Mathematical Correctness
- [x] Heading yaw stays within valid range [-π, π] after wrapping (c7a8e91)
- [x] Basis vectors (forward/right) are orthonormal when derived from heading (c7a8e91)
- [ ] Turn direction matches input sign (A increases yaw, D decreases yaw)

### Input Transformation
- [ ] CAR_LIKE mode: lateral input (x) controls turn rate, not lateral acceleration
- [ ] CAR_LIKE mode: forward input (y) drives motion along heading direction
- [ ] FREE_STRAFE mode: existing camera-relative behavior unchanged

### Edge Cases
- [ ] Zero turn rate input maintains stable heading
- [ ] Maximum turn rate input produces stable rotation
- [ ] Rapid control scheme toggle preserves physics state
- [ ] Heading wraps correctly at ±π boundary
<!-- END: ITERATE/CONTRACT -->

---
