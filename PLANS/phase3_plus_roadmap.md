# Phase 3+ Roadmap

## Baseline Snapshot (2025-10-04)
- Physics core uses weightlifter-first collision resolution, landing impact tracking, and friction-limited velocities (`src/character/character_controller.cpp`).
- Orientation and locomotion interpret physics via velocity-facing yaw and distance-phased strides with spring-driven vertical offsets (`src/character/orientation.cpp`, `src/character/locomotion.cpp`).
- Runtime cleanly separates simulation, GUI, and rendering while pushing landing impulses into locomotion (`src/app/runtime.cpp`).
- Missing capabilities: slope response, step-up negotiation, IK rig, aerial motion primitives, and ragdoll layers.

## Phase 3: Contact-Aware Transitions
Goal: make ground interaction trustworthy across uneven terrain before layering reactive systems.

Deliverables:
- Slope handling that projects velocity onto the contact plane and sheds speed when the incline exceeds `max_slope_angle`.
- Step-up probe using offset weightlifter sweeps to climb ledges up to `step_height` without tunneling.
- Contact diagnostics: debug overlay for weightlifter trajectory, contact normals, and slope grade history.

Iteration outline:
1. Extend collision resolution to store contact point, normal, and penetration depth per frame for reuse by higher layers.
2. Implement slope projection and configurable slide response; add ramp meshes to `scene` for graybox testing.
3. Build step-up detection with a forward capsule or ray query, including cooldowns to prevent oscillation and GUI tuning hooks.
4. Ship contact debug views (ImGui graphs + world overlays) to verify data while playtesting.

Exit criteria:
- Character walks up and down ramps without jitter or unintended sliding.
- Step-up transitions feel instantaneous and deterministic on 0.4m ledges.
- Debug overlays confirm continuous contact data with no frame gaps.

### Progress Log
- 2025-10-05: Iteration step 1 complete - sphere collisions now expose contact point/normal/penetration and the controller caches a per-frame contact_frame for higher layers.
- 2025-10-05: Iteration step 2 (core response) implemented - grounded velocity projects onto the contact plane, uphill motion beyond max_slope_angle is culled, and ground friction skips on steep grades.
- 2025-10-05: Iteration step 3 refined - weightlifter sphere rises onto ledges when its top clears the edge; removed probe/cooldown tuning and kept the ramp stair stack for quick testing. Slope contacts now clamp horizontal speed to pre-impact values and the debug panel plots horizontal/vertical velocity history instead of spring compression.
- Upcoming: Contact debug overlays (Iteration step 4).

## Phase 4: Environmental Awareness
Goal: reactive pose systems read physics and adapt without overriding the core controller.

Deliverables:
- `contact_frame` API exposing feet/hand targets derived from weightlifter history and surface normals.
- Lightweight skeleton description (hips, knees, ankles, spine anchors) consumable by procedural systems.
- Two-bone IK solver with spring-based settling for feet and hands.
- Foot placement that biases locomotion poses toward terrain while keeping center of mass governed by physics.

Iteration outline:
1. Promote contact sampling into `foundation` with cached positions, normals, and reliability flags.
2. Author `ik_chain` utilities (law-of-cosines solver + pole vector handling) and integrate unit tests via debug overlays.
3. Drive locomotion offsets through IK targets, blending stride phase with surface adaptation.
4. Add visualization (target markers, error vectors) and run focused playtests on uneven terrain.

Exit criteria:
- Feet land flush on sloped and stepped surfaces with <2 cm error.
- IK stays within mechanical limits without yanking the controller.
- Core position/velocity remain identical to raw physics integration.

## Phase 5: Aerial Control
Goal: maintain responsiveness mid-air using procedural curves instead of canned animations.

Deliverables:
- Airborne state pipeline (takeoff capture, hang time estimation, landing preparation).
- Cubic-interpolated rotation and pose curves parameterized by jump arc and player input.
- Spring-damper hooks for landing preloads and recovery timing.
- Debug tooling: apex predictor, rotational velocity plots, and landing alignment guides.

Iteration outline:
1. Capture takeoff data (velocity, impulse time) inside the controller and expose to reactive layers.
2. Define aerial pose spans (neutral, tuck, flail) with runtime-tweakable parameters and blend by hang time fraction.
3. Blend orientation system between velocity-driven yaw and scripted flips using controllable damping.
4. Validate through drop scenarios and repeated jump sequences; adjust parameters based on observed feel.

Exit criteria:
- Player retains directional influence mid-air without violating physics source of truth.
- Landing prepose aligns with ground contact, avoiding velocity discontinuities.
- Tooling highlights apex predictions within +/- 1 frame.

## Phase 6: Passive Ragdoll
Goal: add secondary motion that reacts to impulses while respecting the controller.

Deliverables:
- Procedural bone chains driven by spring-damper constraints (spine, arms, accessories).
- Impact filters deciding when to trigger or amplify secondary motion based on landing or collision magnitude.
- Blending layer that applies offsets post-animation without feeding back into core physics.

Iteration outline:
1. Reuse spring_damper primitives to model per-bone offsets with tunable mass and damping.
2. Attach chains to locomotion/IK outputs, ensuring offsets remain additive and bounded.
3. Instrument with energy graphs and visual gizmos to tune decay rates.
4. Playtest heavy landings and collisions to validate natural wobble without destabilizing control.

Exit criteria:
- Secondary motion activates on significant impacts and settles within designed time windows.
- No observable feedback into controller position/velocity.
- Debug graphs show bounded energy that decays monotonically.

## Phase 7: Active Ragdoll
Goal: couple physics bodies with control targets for expressive, high-interaction scenarios.

Deliverables:
- Hybrid rig where key bones have dynamic bodies constrained toward animation goals.
- Constraint solver parameters exposed for gameplay tuning (compliance, max force).
- Fail-safe transitions back to passive control to avoid instability.
- Scenario scripts (shoves, grapples) for manual evaluation.

Iteration outline:
1. Prototype limited active ragdoll on upper body using existing spring infrastructure plus torque constraints.
2. Integrate with collision events to trigger partial ragdoll and recover via procedural drives.
3. Build visualization of constraint error, applied forces, and recovery timelines.
4. Iterate through targeted playtests (push tests, landing recoveries) to validate stability.

Exit criteria:
- Active ragdoll sequences recover to standard locomotion without popping.
- Constraint errors remain within acceptable thresholds (displayed in debug HUD).
- System can be disabled per scenario without code churn.

## Supporting Practices
- Continue graybox validation before polish; add test ramps, ledges, and drop towers as needed.
- Record parameter baselines before each iteration to detect regressions quickly.
- Schedule focused playtest loops (self-test daily, external observation every 6-12 sessions) per Iteration Protocol.


