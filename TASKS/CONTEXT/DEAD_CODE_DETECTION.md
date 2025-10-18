# Dead Code Detection

**Pattern:** Storage, functions, or logic that exists but is never actually used.

---

## Detection

**Before adding storage:**
```bash
# Check if data already exists elsewhere
grep -r "struct.*<data_name>" src/
grep -r "<field_name>" src/

# After adding, verify it's actually read
grep -r "<getter_name>\|<field_access>" src/
```

**Before keeping a function:**
```bash
# Find all call sites
grep -r "<function_name>" src/ --include="*.cpp" --include="*.h"

# Exclude definition/declaration
grep -r "<function_name>" src/ | grep -v "^[^:]*\.h:" | grep -v "definition_file.cpp"
```

**Red flags:**
- Storage added but never accessed
- Getter method defined but never called
- Dual `add()` calls: `a.add(x); b.add(x);` - one is likely unused
- Functions with zero call sites outside their definition

---

## Examples from Refinements

**World Geometry (scene.boxes):**
- Added: `std::vector<aabb> boxes` to scene class
- Methods: `add_collision_box()`, `collision_boxes()`
- Reality: Never called by renderer, never accessed by collision system
- Only `world_geometry.boxes` was actually used
- Fix: Delete entire scene storage

**Character State (read_from):**
- Added: `read_from(const controller&)` for bidirectional sync
- Reality: Never called after unidirectional flow established
- Fix: Delete function entirely

**Embedded Tests:**
- Added: Test execution in runtime initialization
- Reality: Tests should be in /tests, not runtime
- Fix: Delete from runtime.cpp

---

## Prevention Checklist

**Before adding storage:**
- [ ] Does this data already exist elsewhere?
- [ ] Who will read this data? (Name the specific code)
- [ ] Is there already a source of truth for this?

**Before keeping code:**
- [ ] Grep for usages - are there actual call sites?
- [ ] If zero usages, why keep it?
- [ ] "Might need later" is not a reason - delete it

**Pattern to avoid:**
```cpp
// Adding storage "just in case"
world.scene.add(data);           // Storage 1
world.physics.add(data);         // Storage 2
// Later: Only one is ever accessed
```

**Correct pattern:**
```cpp
// Single source of truth
world.physics.add(data);         // Physics owns it
// Scene reads via reference when needed
render(world.physics.get_data());
```

---

## Quick Audit

```bash
# Find large structs with many fields
grep -A 20 "^struct\|^class" src/**/*.h | grep "public:\|private:"

# For each field, verify usage:
grep -r "<field_name>" src/ --include="*.cpp"

# If zero results (excluding definition): dead field
```

---

## Core Rule

**Never add storage without a single clear reader.**

If you can't name the specific code that will read this data, don't add it.

Dead code is invisible until you look for it. Look before adding, grep before keeping.
