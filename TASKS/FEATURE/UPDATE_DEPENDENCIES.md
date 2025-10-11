# Update Dependency Stack

### 1. Review Current State

1.  Open `PLANS/DEPENDENCY_STACK.md`
2.  Locate `← YOU ARE HERE` marker in the tree visualization
3.  Review certainty scores across all layers (Foundation, Core Gameplay, Completed Features)

### 2. Update System Certainty

1.  **After Iteration:** If system iterated and tested, update certainty percentage in the tree. If stable and proven (100%), mark with `✅` and move to "Completed Features" section
2.  **On System Validation:** Update corresponding entry in Layer Details section with key learnings and pattern discoveries
3.  **On System Failure:** If playtesting revealed fundamental flaw, decrease certainty score in tree to reflect new risk

### 3. Modify Tree Structure

**Adding New Feature:**
1.  When beginning work on new feature from `DESIGN_BACKLOG.md`, add box to tree above its dependencies
2.  Connect with arrows flowing bottom → top (dependencies point upward)
3.  Assign low, realistic initial certainty score (e.g., `40-50%`)
4.  Add corresponding entry to "Layer Details" section with implementation notes

**Moving Completed Feature:**
1.  When feature reaches 100% certainty, move its box into "Completed Features" section of tree
2.  Update tree connections—other features may now depend on this stable system
3.  Add detailed entry in Layer Details with learnings, patterns proven, implementation links

**Refactoring Dependencies:**
1.  If system relationships changed, update arrows in tree to reflect new dependency graph
2.  Consider if parallel branches can now merge or if new branches split off
3.  Document rationale in Layer Details section

### 4. Update Development Focus

1.  Move `← YOU ARE HERE` marker to indicate current frontier:
    - Place marker between "Completed Features" and "Design Backlog"
    - Or place at specific layer if actively working in that area
2.  Update "Development Strategy" section:
    - Add completed work to "Completed Work Order" list
    - Adjust Planning Horizon based on new certainty levels
    - Update Risk Management if new patterns discovered

### 5. Verify Tree Readability

Check that the updated tree clearly shows:
- Bottom-up dependency flow (arrows go upward)
- Parallel branches for independent development paths
- Clear separation between layers (Foundation, Core Gameplay, Completed, Backlog)
- Certainty percentages visible on all active systems

### Tone & Constraints

-   Mechanical and concise language
-   Reflect only tested knowledge; avoid speculation
-   Prefer systemic additions over content
-   Avoid introducing content restrictions
-   Maintain visual clarity of tree structure

---

### Example: Solidifying a System

**Tree Update:**
```
Move from:
    ┌────────────────────────┐
    │ Pose Blending (95%)    │
    │ (quat slerp)           │
    └────────────────────────┘

To "Completed Features" section:
    ┌────────────────────────┐
    │ Pose Blending          │ ✅ 100%
    │ (quat slerp)           │
    └────────────────────────┘
```

**Layer Details Update:**
Add detailed entry under "Completed Features" section:
- Implementation summary and key learnings
- Links to implementation/iteration docs
- Patterns proven and techniques validated

**Marker Update:**
Move `← YOU ARE HERE` to next frontier feature or between Completed and Backlog if ready to pull new work.