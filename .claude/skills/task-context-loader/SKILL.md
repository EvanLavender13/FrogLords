---
name: FrogLords Task Context Loader
description: Load relevant TASKS/CONTEXT/ cards when user presents development tasks (movement, camera, physics, animation, refinement, validation)
---

When user presents a FrogLords development task:

1. Read `TASKS/CONTEXT/INDEX.md`
2. Identify task type from user description
3. Match task type to selection hints and tags
4. Read relevant context cards (1-3 maximum)
5. State: "Loading context: [names] (reason: [type] → [tags])"
6. Proceed with task
