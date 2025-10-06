# Archive Iteration Artifacts

### 1. Identify Iteration Artifacts

1.  **Get Feature Name:** Extract the feature name from the current git branch name (format: `iteration/<feature_name>`).
2.  **Locate Artifacts:** Find all related markdown files in the `PLANS/` directory. These typically include:
    -   `iteration_<feature_name>.md`
    -   `implementation_<feature_name>.md`
    -   `review_<feature_name>.md`
    -   `code_review_<feature_name>.md`

### 2. Generate Timestamp

Create a standardized, sortable timestamp string using the `YYYYMMDD_HHMMSS` format. This will be used to prefix the archived files.

### 3. Rename and Move Artifacts

1.  **Prefix Filenames:** For each identified artifact, prepend the generated timestamp to its filename.
    -   Example: `iteration_jump.md` becomes `20251027_143000_iteration_jump.md`.
2.  **Move to Archive:** Move all the renamed files from the `PLANS/` directory to the `PLANS/ARCHIVE/` directory.

### 4. Update Internal References

This is a critical step to maintain the integrity of the archived documentation.

1.  **Scan Content:** Read the content of each newly archived file.
2.  **Find and Replace:** Search for any references to the original filenames of the other artifacts.
3.  **Update Links:** Replace each found reference with the new timestamped filename.
    -   Example: A link `[Implementation Plan](implementation_jump.md)` in the `iteration_jump.md` file should be updated to `[Implementation Plan](20251027_143000_implementation_jump.md)`.

### Tone & Constraints

-   Use precise, script-like commands for file operations.
-   Ensure all internal links are updated; broken links are unacceptable.
-   Verify file contents post-modification to confirm changes.

---

### Example Archiving Workflow

**Feature:** `jump`
**Timestamp:** `20251027_143000`

**Files to archive:**
-   `PLANS/iteration_jump.md`
-   `PLANS/implementation_jump.md`

**Actions:**

1.  **Rename and Move:**
    -   `mv PLANS/iteration_jump.md PLANS/ARCHIVE/20251027_143000_iteration_jump.md`
    -   `mv PLANS/implementation_jump.md PLANS/ARCHIVE/20251027_143000_implementation_jump.md`

2.  **Update Content:**
    -   **In `PLANS/ARCHIVE/20251027_143000_iteration_jump.md`:**
        -   Find link: `[Implementation](implementation_jump.md)`
        -   Replace with: `[Implementation](20251027_143000_implementation_jump.md)`
