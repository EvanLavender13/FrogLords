# Merge Feature Branch to Main

### 1. Verify Current Branch

1.  Run `git status` to ensure the current working directory is clean.
2.  Confirm you are on the correct feature branch (`iteration/<feature_name>`). All work should be committed.

### 2. Switch to Main Branch

1.  Run `git checkout main`.

### 3. Merge Feature Branch

1.  **Get Feature Name:** Extract the feature name from the previous branch name (`iteration/<feature_name>`).
2.  **Execute Merge:** Run `git merge --ff-only iteration/<feature_name>`.
    -   This command will only succeed if the merge can be resolved as a fast-forward. It prevents the creation of a merge commit.

### Tone & Constraints

-   Use precise git commands.
-   Ensure no merge commits are created by strictly using the `--ff-only` flag.
-   All operations are local; this task does not involve pushing to a remote.

---

### Example Merge Workflow

**Feature Branch:** `iteration/add_dash_ability`

**Commands:**

1.  `git status` (confirm on `iteration/add_dash_ability` and clean)
2.  `git checkout main`
3.  `git merge --ff-only iteration/add_dash_ability`
