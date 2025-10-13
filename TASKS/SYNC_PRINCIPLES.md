# Sync Project Principles

**Purpose:** Keep `AGENTS.md` and principle guidance aligned with the live architecture and curated NOTES references. Ensure principles remain actionable (not academic), links stay current, and cross-doc triggers are in place.

**Scope:** Update references and guidance. Do not commit external textbook content; keep NOTES as references only.

---

## When to Run This Task

Run SYNC_PRINCIPLES when:
- Core architecture changes land (e.g., in `src/foundation/*`, `src/character/*`, `src/rendering/*`, `src/app/*`).
- Major NOTES are added/updated (new deep dives or philosophy changes) — reference only.
- Planning triggers change (e.g., updates to `PLANS/*_BACKLOG.md` or `PLANS/DEPENDENCY_STACK.md`).
- Before milestone snapshots or after several rapid iterations.

---

## Inputs
- `AGENTS.md` (project principles, quick start, layering, references)
- `CONVENTIONS.md` (coordinate system, rotations, camera, animation conventions)
- `PLANS/DEPENDENCY_STACK.md` and `PLANS/DESIGN_BACKLOG.md` (verify "Triggers to Update" sections)
- Directory map of `src/` (ownership/layering confirmation)
- NOTES index (filenames only; keep as references)

---

## Process

### 1. Review Development Principles

Read `AGENTS.md` for:
- Quick Start bullets reflect current priorities (clarity, simplicity, iteration, graybox).
- Layered Architecture matches code layering (Core -> Reactive -> Polish).
- Procedural Animation rules align with current controller/pose/IK implementation.
- Dependencies/Planning guidance matches your iteration cadence.
- References list points to currently relevant NOTES (references only).

### 2. Gather Context (Architecture + Notes)

Collect current context:
- Scan `src/` directories for new subsystems or ownership shifts.
- Skim `CONVENTIONS.md` for mismatches (e.g., axis definitions, interpolation patterns).
- List recent/important NOTES docs (filenames only) that should be referenced.

### 3. Update References and Cross-Links

In `AGENTS.md`:
- Update "References (deep dives)" to include relevant NOTES paths (do not embed content).
- Ensure "Code and Repo" points to `CONVENTIONS.md` instead of duplicating details.
- Keep language operational and concise; remove duplication already covered in `CONVENTIONS.md`.

In PLANS:
- Confirm "Triggers to Update" exist in `PLANS/DEPENDENCY_STACK.md` and `PLANS/DESIGN_BACKLOG.md`.
- Add missing triggers if needed (dependencies shifts, certainty changes, backlog pulls, improvements that change data representations).

In CONVENTIONS:
- If the implementation changed (e.g., yaw conventions, slerp edge cases), update `CONVENTIONS.md` accordingly.

### 4. Encoding & Line Endings (Optional but Recommended)

Normalize encodings to prevent mojibake in principles and plans:

```powershell
# Preview (no writes), LF endings, exclude NOTES (default)
powershell -ExecutionPolicy Bypass -File scripts/normalize_encoding.ps1 -LineEndings LF -DryRun

# Apply normalization (UTF-8 no BOM + LF)
powershell -ExecutionPolicy Bypass -File scripts/normalize_encoding.ps1 -LineEndings LF

# Include NOTES (only if desired)
powershell -ExecutionPolicy Bypass -File scripts/normalize_encoding.ps1 -LineEndings LF -IncludeNotes
```

### 5. Verify Links and Ownership

Checklist:
- [ ] `AGENTS.md` references only existing files; links resolve.
- [ ] `AGENTS.md` does not embed external textbook content (references only).
- [ ] `CONVENTIONS.md` matches code behavior (UP axis, yaw-to-forward, interpolation).
- [ ] PLANS trigger sections present and relevant.
- [ ] Layering guidance matches code ownership (Foundation -> Character -> Rendering -> App).

### 6. Document the Sync

Append a short summary to your planning docs (choose one):

```markdown
## Principles Sync Summary

**Date:** [YYYY-MM-DD]
**Scope:** AGENTS.md, CONVENTIONS.md, PLANS triggers

### Changes
- Updated references: [paths]
- Adjusted guidance: [bullets]
- Verified triggers: [yes/no]

### Rationale
- [One sentence tying to recent architecture/notes changes]
```

---

## Success Criteria
- `AGENTS.md` remains a concise, actionable guide (no duplication of `CONVENTIONS.md`).
- References list points to the right NOTES documents (as references only).
- PLANS trigger sections exist and reflect current practice.
- Encoding issues minimized (UTF-8 no BOM, consistent line endings).
- Layering and responsibilities in principles match the actual code.

---

## Tone & Constraints
- Concise and direct; principle-first; operational over exhaustive.
- Keep NOTES as references only; do not embed textbook material.
- Prefer links to `CONVENTIONS.md` for low-level details.
- Avoid broad rewrites unless architecture materially changed.
