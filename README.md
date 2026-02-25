# StringUtilities

`StringUtilities` is a header-only C++23 utility library for string handling and stream-friendly formatting of values and STL containers.

## What this repository provides

- String helpers in `include/stringutil.h`:
  - `toLower`, `toUpper`
  - `trim`, `trimLeft`, `trimRight`, `strip`
  - `replaceChar`, `replaceCharLeft`, `replaceCharRight`
  - `splitIntoVector`, `splitIntoSet`
  - `scanBoolString`, `classifyNumberString`
- Case-insensitive string and traits in `include/ci_string.h` (`util::ci_string` and variants).
- Generic stream decoration/formatting in `include/decorator.h` and conversion helpers in `include/to_string.h` (`toString`, `toWString`).
- Bracket presets for formatted output in `include/brackets.h`.

## Build and test

The project uses CMake and builds tests with GoogleTest.

### 1. Configure

```bash
cmake -S . -B build
```

### 2. Build

```bash
cmake --build build --parallel "$(nproc)"
```

### 3. Run unit tests

```bash
ctest --test-dir build --output-on-failure
```

If your environment does not provide a packaged GoogleTest installation, install/build GoogleTest first and make it discoverable to CMake.

## Install headers

```bash
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build --parallel "$(nproc)"
sudo cmake --install build
```

Headers are installed under `${CMAKE_INSTALL_PREFIX}/include/dkyb`.

## Recent changes

- Added `cmake-common` as a submodule and migrated root build settings to shared `dkyb_apply_common_settings()`.
- Root `CMakeLists.txt` now includes `CTest` and gates test directory inclusion with `BUILD_TESTING`.
- Expanded unit-test coverage with additional tests for:
  - boolean parsing
  - inclusive substring bounds
  - split edge-cases (empty segments, repeated separators)
  - directional replacement helpers
  - numeric string classification
