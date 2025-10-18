# Refinement: Per-Draw Buffer Creation

Eliminate per-frame buffer creation/destruction for clean dynamic buffer pattern.

---

## Selected

**Date:** 2025-10-18
**Complexity:** Standard (4 points)
**Path:** B (standard)

---

## Violation

**Location:** `src/rendering/renderer.cpp:61-107`
**Principle:** Radical Simplicity | Score: 6/10
**Severity:** Medium
**Type:** Complexity, Performance, Inefficient pattern

**Current state:**
```cpp
void mesh_renderer::draw_mesh(const foundation::mesh& mesh, ...) {
    // ... every frame ...

    // Convert edges to indices
    std::vector<uint16_t> indices;
    indices.reserve(mesh.edges.size() * 2);
    for (const foundation::edge& e : mesh.edges) {
        indices.push_back(static_cast<uint16_t>(e.v0));
        indices.push_back(static_cast<uint16_t>(e.v1));
    }

    // Create NEW buffers for THIS draw call
    sg_buffer_desc vertex_buffer_desc = {};
    vertex_buffer_desc.data = {mesh.vertices.data(), ...};
    sg_buffer vbuf = sg_make_buffer(&vertex_buffer_desc);

    sg_buffer_desc index_buffer_desc = {};
    index_buffer_desc.data = {indices.data(), ...};
    sg_buffer ibuf = sg_make_buffer(&index_buffer_desc);

    // ... bind and draw ...

    // Destroy buffers immediately
    sg_destroy_buffer(ibuf);
    sg_destroy_buffer(vbuf);
}
```

**Why violation:**
- Creates and destroys buffers for EVERY mesh drawn EVERY frame
- Even though sokol defers destruction, still inefficient API pattern
- Unnecessary complexity - dynamic buffers exist for this exact use case
- CPU overhead building vertex/index data every frame
- Violates "simplicity through proper tool use"

**Impact:**
- Blocks: Clean rendering architecture
- Cascades to: Future rendering features, performance optimization
- Foundation: 96% (rendering layer)

---

## Fix

**Approach:** Simplify

**SIMPLIFY:**

**From:** Per-draw immutable buffer creation/destruction
**To:** Single persistent dynamic buffer reused every frame

**Pattern:**
1. Create persistent dynamic buffer at initialization (once)
2. Each frame: append all vertex data to dynamic buffer
3. Issue draw calls with base vertex/index offsets
4. Clear/reset buffer for next frame

**Implementation:**
- Add persistent `sg_buffer dynamic_vertex_buffer` to `mesh_renderer` state
- Add persistent `sg_buffer dynamic_index_buffer` to `mesh_renderer` state
- Initialize in `init()` with `SG_USAGE_STREAM` (dynamic)
- In `draw_mesh()`: use `sg_append_buffer()` or `sg_update_buffer()`
- Track current offset for base vertex/index in draw calls

**Benefits:**
- Eliminates per-draw allocation/deallocation
- Matches idiomatic sokol pattern for dynamic geometry
- Reduces API calls (one buffer creation vs hundreds)
- Clearer intent: "dynamic buffer for dynamic data"
- Simpler mental model

---

## Success

- [ ] Violation resolved
- [ ] Principle score improved
- [ ] Tests passing
- [ ] No visual regressions

**Metrics:**
- Before: LOC ~50, Principle 6/10, API calls ~2N per frame (N=meshes)
- After: LOC ~60 (+10 setup), Principle 9/10 (+3), API calls ~2 per frame

**Visual validation:**
- All meshes still render correctly
- No performance regression
- No flickering/artifacts

---

## Risk

**Could go wrong:**
- Buffer overflow if too many meshes exceed dynamic buffer size
- Incorrect offset calculations causing garbled rendering
- Synchronization issues with buffer updates

**Mitigation:**
- Start with generous buffer size (64KB vertices, 64KB indices)
- Assert on buffer capacity before append
- Log warning if approaching capacity
- Visual testing of multi-mesh scenes

**Rollback:**
```bash
git checkout main -- src/rendering/renderer.{h,cpp}
```

---

## Implementation Notes

**Sokol dynamic buffer pattern:**
```cpp
// Initialization (once)
sg_buffer_desc desc = {};
desc.size = 65536; // generous initial size
desc.usage = SG_USAGE_STREAM; // dynamic updates
desc.vertex_buffer = true;
dynamic_vbuf = sg_make_buffer(&desc);

// Per-frame usage
sg_range data = {vertices.data(), vertices.size() * sizeof(vertex)};
sg_append_buffer(dynamic_vbuf, &data);
// OR for complete replacement:
sg_update_buffer(dynamic_vbuf, &data);
```

**Alternative considered:** Persistent buffers per mesh
- Rejected: Adds complexity for static geometry (we don't have much)
- Current approach simpler: all geometry treated uniformly as dynamic

---

**Delete creation. Use persistent buffers. Simplify rendering.**
