# Physics Integration: The Nature of Motion Through Time

## The Paradox of Integration

Physics integration stands apart from all other state management. It is the one place where accumulation is not compromise—it is mathematical necessity.

**The paradox:** We forbid accumulated state because it drifts from truth. Yet physics integration *is* accumulated state, and it *is* truth.

**The resolution:** Some truths cannot be known directly. They can only be discovered through integration over time.

## The Nature of Integration

### Two Forms of State

State exists in two forms, but integration reveals a third:

**Derivable Truth** - Calculable from authoritative sources each frame
**Accumulated Falsehood** - Built up through conditional updates, drifting toward lies
**Integrated Truth** - Built up through unconditional integration, solving equations that have no closed form

**The distinction is mathematical, not philosophical.**

A cache that accumulates to avoid recalculation is falsehood waiting to diverge.
An integral that accumulates to solve a differential equation is truth being discovered.

### The Test

Ask: "Can this value be calculated from source data alone?"

**If yes:** Derive it. Accumulation is cache rot.
**If no:** Integrate it. Accumulation is the solution method.

Position cannot be derived from current velocity alone—it requires integrating velocity over the entire trajectory.
Velocity cannot be derived from current acceleration alone—it requires integrating acceleration over time.

**These are not approximations. They are numerical solutions to continuous equations.**

## The Boundary Between Integration and Cache

Integration and caching both accumulate. The difference is necessity.

**Integration:** Solves differential equations
- Form: dx/dt = f(x, t)
- Cannot be derived from instantaneous state
- Accumulation is the algorithm itself
- Truth emerges from the process

**Caching:** Stores derivable results
- Can be recalculated from source
- Accumulation is optimization, not requirement
- Truth exists elsewhere, cache is convenience
- Divergence is inevitable without synchronization

**Examples of Integration (Truth):**
- Position from velocity over trajectory
- Velocity from acceleration over time
- Energy from power over duration

**Examples of Caching (Falsehood in waiting):**
- Running totals that could sum a collection
- Smoothed values that reference themselves
- Memoized calculations with stale inputs

**The principle:** If you can derive it, derive it. If you must integrate it, integrate it unconditionally.

## The Philosophy of Integration Methods

Numerical integration is approximation. We trade mathematical purity for computational feasibility.

**The question:** Which approximation preserves the properties we care about?

### The Stability Hierarchy

**Explicit methods** use old values to compute new ones. Energy can grow unbounded. Instability is the failure mode.

**Implicit methods** use new values in their own computation. Energy dissipates naturally. Stability is inherent but cost is high.

**Semi-implicit methods** split the difference. Update velocity explicitly, then use that new velocity for position. Stability emerges from ordering.

**The choice is not arbitrary—it determines whether simulation diverges or converges.**

### The Order-Accuracy Trade-off

**First-order methods** (Euler) have error proportional to timestep. Simple, fast, "good enough" for most games.

**Second-order methods** (Verlet) have error proportional to timestep squared. Better energy conservation, used in molecular dynamics.

**Fourth-order methods** (RK4) have error proportional to timestep to the fourth. Engineering-grade accuracy at 4× cost.

**The law of diminishing returns:** For damped systems with frequent resets (platformers), first-order is sufficient. Higher orders buy accuracy we don't need at costs we can't afford.

### Why Semi-Implicit Wins for Gameplay

Platformer physics is damped (friction), bounded (gravity, input limits), and reset (collisions, landing).

**In this context:**
- Stability matters more than energy conservation (we dissipate energy by design)
- Speed matters more than high-order accuracy (60fps+ is non-negotiable)
- Simplicity matters more than generality (we're not simulating orbits)

**Semi-implicit Euler is not a compromise. It is the correct tool for the problem.**

Explicit Euler would drift toward infinity.
Verlet would conserve energy we're trying to dissipate.
RK4 would compute precision we immediately throw away.

**Match the method to the mathematics of the domain.**

## The Principle of Frame-Rate Independence

Time is a parameter, not an assumption.

**A simulation that behaves differently at 30fps versus 60fps has failed mathematically.**

### The Nature of Rates

Rates are changes per unit time. To produce change, multiply by time.

**Velocity is meters per second.** To change position, multiply by seconds.
**Acceleration is meters per second per second.** To change velocity, multiply by seconds.

**This is not a performance trick—it is dimensional analysis.**

When rates are not scaled by time, the simulation runs faster at higher framerates. This is not a bug, it is a mathematical error. The units do not balance.

### The Verification

Check dimensions on every equation.

Position (m) = Position (m) + Velocity (m/s) × Time (s)
Velocity (m/s) = Velocity (m/s) + Acceleration (m/s²) × Time (s)

**If dimensions do not balance, the equation is wrong.**

No amount of tuning can fix an equation with incorrect units. It will fail under frame rate changes, time dilation, slow motion, or fast-forward.

**Frame-rate independence is not a feature. It is mathematical correctness.**

## The Question of Drift

Numerical integration accumulates error. Over infinite time, all integrators drift.

**The question is not "does it drift?" but "does the drift matter?"**

### Bounded Drift in Damped Systems

Platformer physics has three properties that bound error:

**Damping:** Friction exponentially decays velocity. Errors decay with it.
**Resets:** Collision resolution resets position to contact. Error cannot accumulate across landings.
**Bounded forces:** Gravity and input are constant or clamped. Unbounded error growth is impossible.

**Result:** Error is bounded by game mechanics, not integration order.

### When Drift Matters

**Undamped systems** (orbital mechanics, springs without damping) accumulate energy error that compounds forever.

**Long time scales** (space flight, astronomical simulation) integrate error over hours or years.

**High precision requirements** (engineering, scientific simulation) demand accuracy beyond gameplay.

**None of these describe platformer physics.**

We operate on second-to-minute timescales.
We reset state dozens of times per second.
We dissipate energy by design.

**In this domain, first-order integration is not just sufficient—it is optimal.**

## The Warning Signs

Not all accumulation is integration. Watch for cache disguised as physics.

**Conditional accumulation** - `if (grounded) { time_on_ground += dt; }` (derive from transition timestamp)
**Self-referential smoothing** - `smooth = smooth * 0.9 + target * 0.1;` (use spring-damper with separate state)
**Running totals** - `total_distance += speed * dt;` (integrate position, derive distance)
**Reset patterns** - `if (jumped) { accumulated_value = 0; }` (why does it need resetting?)

**When accumulation needs resetting, question whether it should accumulate at all.**

Integration over continuous time doesn't reset. Motion doesn't have a "clear history" button. If your accumulated state does, it's probably cache, not integration.

## The Principle

**Integration is the solution to equations that have no closed form.**

Position from velocity, velocity from acceleration, energy from power—these are differential equations solved numerically.

**Accumulation is the algorithm, not an optimization.**

Cache accumulates to avoid recalculation. Integration accumulates because calculation-from-source is impossible.

**The distinction is mathematical:**

Can you write the formula for position at time t given only current velocity? No—you need the integral of velocity over the trajectory.

Can you write the formula for total damage given the damage event log? Yes—sum the collection.

**One is integration. One is cache. Only one should accumulate.**

## The Direction of Truth

**Physics integration flows forward through time.**

Given initial conditions and forces, we integrate to discover future state. The arrow of causation is clear: past determines present, present determines future.

**Cache pretends to flow forward but actually references backward.**

A smoothed value that uses its own previous value creates circular dependency. The accumulation isn't solving an equation—it's approximating one while introducing lag and staleness.

**Integration discovers. Cache duplicates.**

Know which you're doing. Integrate when mathematics demands it. Derive when source truth exists.

**The best way to prevent accumulated state bugs is still to eliminate accumulated state—except where state is the solution itself.**
