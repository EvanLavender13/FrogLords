# Unidirectional Flow

**Pattern:** Single source of truth with one-way data transformation.

**Anti-pattern:** Bidirectional sync, dual storage, circular dependencies.

---

## The Pattern

**Data flows in one direction:**
```
Source of Truth → Transformation → Derived State → Usage
       ↑                                              |
       └──────────── (No back flow) ─────────────────┘
```

**Single source of truth:**
- One struct owns the authoritative data
- Other structs derive or reference, never duplicate
- Changes happen at the source, flow downstream

**One transformation function:**
- `source.apply_to(derived)` - transforms source → derived
- No `derived.read_from(source)` - no reverse flow
- No `sync()` - if you need sync, you have two sources

---

## Examples from Refinements

### Character State (Tuning → Controller)

**Before (bidirectional):**
```cpp
struct tuning_params {
    float time_to_max_speed;
    float jump_height;

    void apply_to(controller& c) const;   // params → controller
    void read_from(const controller& c);  // controller → params (BAD)
};

// GUI modifies both directly
gui::slider(&character.max_speed);        // Modifies controller
gui::slider(&params.time_to_max_speed);   // Modifies params
// Which is source of truth? Unclear!
```

**After (unidirectional):**
```cpp
struct tuning_params {
    float time_to_max_speed;
    float jump_height;
    float max_speed;        // Moved from controller
    float gravity;          // Moved from controller

    void apply_to(controller& c) const;   // params → controller ONLY
    // No read_from - unidirectional
};

// GUI modifies params only (single source)
gui::slider(&params.max_speed);
gui::slider(&params.time_to_max_speed);

// After changes, flow to controller
if (changed) {
    params.apply_to(controller);
}
```

**Result:** `tuning_params` = source of truth, `controller` = derived runtime state

---

### World Geometry (Collision → Scene)

**Before (dual storage):**
```cpp
world.scene.add_collision_box(box);          // Storage 1
world.world_geometry.boxes.push_back(box);   // Storage 2
// Manual sync required, two sources of truth
```

**After (single source):**
```cpp
world.world_geometry.boxes.push_back(box);   // Single source
// Scene reads from world_geometry when rendering
render(world.world_geometry.boxes);
```

**Result:** `world_geometry` owns data, `scene` reads via reference

---

### Buffer Creation (Persistent → Stream)

**Before (create/destroy cycle):**
```cpp
// Every frame:
sg_buffer vbuf = sg_make_buffer(&desc);      // Create
// ... use ...
sg_destroy_buffer(vbuf);                     // Destroy
// Bidirectional: CPU → GPU → CPU (deletion)
```

**After (unidirectional streaming):**
```cpp
// Init once:
sg_buffer dynamic_vbuf = sg_make_buffer(&desc);

// Every frame:
sg_append_buffer(dynamic_vbuf, &data);       // Stream only
// Unidirectional: CPU → GPU
```

**Result:** Persistent buffer is source, GPU receives stream

---

## Detection

**Red flags:**
```cpp
// Dual transformation functions
void apply_to(other& o);
void read_from(const other& o);  // RED FLAG

// Dual storage
a.storage.push_back(data);
b.storage.push_back(data);       // RED FLAG

// Unclear source of truth
if (a.dirty) sync_to_b();
if (b.dirty) sync_to_a();        // RED FLAG

// Create/destroy cycles
for (...) {
    auto handle = create();
    use(handle);
    destroy(handle);             // RED FLAG (consider persistent)
}
```

---

## Decision Tree

**When adding data transformation:**

1. **Which struct represents intent?** (What designers/users specify)
2. **Which struct represents implementation?** (What runtime needs)
3. **Is the relationship a transformation?** (Intent → implementation)
   - Yes → One function, one direction: `intent.apply_to(impl)`
   - No → Question if both structs should exist

**If you think you need bidirectional sync:**
- Stop
- Ask: "Which one is the source of truth?"
- Pick one
- Delete the reverse transformation
- Make the other side derived-only

---

## The Question

**When you find yourself writing both `apply_to()` and `read_from()`:**

Stop and ask: **"Why do I need both?"**

The answer is usually: **"I don't—I need to pick a source of truth."**

---

## Prevention Checklist

**Before adding storage:**
- [ ] Does similar data exist elsewhere?
- [ ] Which should be the authoritative source?
- [ ] Can one derive from the other?

**Before adding transformation:**
- [ ] Which direction is the natural flow?
- [ ] Is the reverse transformation actually needed?
- [ ] Am I creating a sync cycle?

**Before keeping both:**
- [ ] Can I delete one storage location entirely?
- [ ] Can I make one a reference/pointer to the other?
- [ ] Have I clearly documented which is source of truth?

---

## Core Rules

**One source. One direction. No cycles.**

- If data exists in two places, one is wrong
- If flow goes both ways, pick one way
- If you need sync, you have two sources (bad)

**Unidirectional flow is not just cleaner—it's simpler.**

One function. One direction. One truth.
