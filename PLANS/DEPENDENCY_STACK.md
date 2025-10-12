# Dependency Stack

**Purpose:** Make cascading uncertainty visible so work proceeds bottom-up with confidence.

**Status:** Living document; trim when layers stabilize, expand when uncertainty returns.

---

## Dependency Tree

```
[Design Backlog | liquid pool | <50% certain]
|-- Near-term: Freeze Trail, Axis Gizmo, Extended Keyframes, Running Gait
|-- Deeper stack: IK Systems, Wall Detection, Ragdoll, Dash, Terrain, Combat, Audio, UI Polish
`-- Wild speculation: dY?, Frog Ideas dY?,
      |
      v
[Feature Pull | YOU ARE HERE | ~60% certain]
`-- Animation Systems (100%) COMPLETE
    `-- Locomotion Core (100%) COMPLETE
        `-- Foundation Systems (100%) COMPLETE
```

---

## Active Layer - Feature Pull (~60%)

- Focus: Backlog grooming to set up the next feature selection.
- Dependencies: Animation Systems layer is proven; only lightweight coordination needed.
- Next action: Once grooming is done, pull the highest-signal candidate from `PLANS/DESIGN_BACKLOG.md` that leverages the animation stack.

---

## Completed Layers (Collapsed Summaries)

### Animation Systems (100%) COMPLETE
- Scope covered: acceleration tilt, landing spring, walk/run transition, pose blending, skeleton/world separation, debug visual overhaul.
- Proven patterns: spring-damper reactive layering, `character::animation_state` as integration point, velocity injection instead of offset hacks, quaternion data validated independently before integration.
- Dependencies: Locomotion Core.
- Detailed retrospectives: `PLANS/ARCHIVE/dependency_stack_snapshot_2025-10-10.md`.

### Locomotion Core (100%) COMPLETE
- Scope covered: physics controller, procedural locomotion, orientation alignment, surveyor-wheel distance phasing.
- Proven patterns: dual-reference speed targets (immutable `run_speed` + smoothed `max_speed`), rotate visuals toward velocity for clarity, keep physics authoritative over animation.
- Dependencies: Foundation Systems.

### Foundation Systems (100%) COMPLETE
- Scope covered: runtime loop, rendering, input, camera, debug instrumentation.
- Proven patterns: parameters over assets, graybox-first workflow, bake instrumentation into systems from day one.
- Dependencies: none.

---

## Pull Guidance

- Confirm backlog certainty tags align with the proven layers above.
- Newly unblocked items: any feature that depended on animation blending, reactive springs, or locomotion stability now sits on a reliable base and can have certainty bumped.
- Keep the backlog breathable: archive lengthy notes in `PLANS/ARCHIVE/` once items close.

---

## Development Strategy Reminders

| Layer Certainty | Planning Horizon | Why |
| --------------- | ---------------- | --- |
| 90-100%         | Plan 3-8 points  | Derivative work on proven tech |
| 70-90%          | Plan 2-4 points  | Stable but still tuning |
| 50-70%          | Plan 1-2 points  | Validate assumptions quickly |
| <50%            | Do not plan      | Uncertainty multiplies upward |

- Build bottom-up; stabilize before layering polish.
- Accept roughness early to maximize iteration count.
- Keep backlog items non-interlocking; remove cruft aggressively.
- Graybox expensive content so mechanics stay legible.
- When taking big leaps, document the intent and return to tight loops after testing.

`P(unchanged) = C^N` remains the quick mental model (certainty `C` per layer, `N` layers of dependency).

---

## Meta Notes

- Historical retrospectives: `PLANS/ARCHIVE/dependency_stack_snapshot_2025-10-10.md`.
- Implementation specifics: `PLANS/implementation_*.md`, `PLANS/code_review_*.md`.
- Source inspiration: Tynan Sylvester's Designing Games dependency stack framing.

**Last Updated:** 2025-10-12 (post backlog grooming pass)
