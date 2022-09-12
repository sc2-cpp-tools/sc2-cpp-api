# Overview

A modern C++17/20-based re-implementation of the [`Blizzard/s2client-api`](https://github.com/Blizzard/s2client-api).

## Building
---

`CMakePresets.json` defines CMake build configurations available for the project. Presets available for your operating system can be viewed with the `--list-presets` command:

```bash
$ cmake --list-presets

Available configure presets:

    "gcc-devel-debug"   - gcc development mode
    "clang-devel-debug" - clang development mode
    ...

$ cmake --preset gcc-devel

Preset CMake variables:

    BUILD_TESTS="ON"
    CMAKE_BUILD_TYPE="Debug"
    CMAKE_CXX_COMPILER="g++"
    CMAKE_C_COMPILER="gcc"
    DEVEL_MODE="ON"
...

# Build from the generated preset
$ cmake --build build/gcc-devel -j
```

With presets, multiple build configurations with isolated binary and library outputs can exist in the build directory simultaneously.

## Requirements
---

The project is currently under development, and incorporates [`project_options`](https://github.com/aminya/project_options) as the primary interface for managing CMake configuration. Development dependencies (such as `clang-tidy`) can be disabled from the `project_options` interface in the root `CMakeLists.txt`.

### Installing Dependencies
---

As an alternative to indivdiually installing each dependency to the recommended version, the project includes a configurable `Dockerfile` to build an image with all dependencies installed. The `.ubuntu/bootstrap.sh` script can also be used to bootstrap a Ubuntu image (tested with 20.04).

Irrespective of the dependency versions installed, the project's CMake build configuration is expecting the base executable name (e.g. `clang` instead of `clang-14`). Both the `Dockerfile` and bootstrap script create the necessary symbolic links, which must otherwise be completed manually for custom installs.

### Build Dependencies
---

The following dependencies are required to build the project, and in some cases are pre-requisite for additional development tools:

- An installation of `clang` and/or `gcc` with C++17/20 support
  - Recommended: [`clang-14`](https://releases.llvm.org/14.0.0/tools/clang/docs/ReleaseNotes.html) and/or [`gcc 11`](https://gcc.gnu.org/gcc-11/changes.html)
  - `clang` is required for `include-what-you-use`
- [`CMake`](https://cmake.org/)
  - Recommended: [`v3.24.1`](https://github.com/Kitware/CMake/releases/tag/v3.24.1) and newer
  - Warning: while [`v3.21.0`](https://github.com/Kitware/CMake/releases/tag/v3.21.0) should be sufficient to support the current CMake presets version schema, the project will not support backwards compatability for releases prior to the recommended
- [`doxygen`](https://www.doxygen.nl/)
  - Required to build project documentation

### Development Dependencies
---

While these dependencies are not required to build the project, they are used to enhance development and enforce coding standards:


- `clang-tidy`
- `clang-format`
- [`cmakelang`](https://github.com/cheshirekow/cmake_format) for `cmake-format` and `cmake-lint`
- [`ccache`](https://ccache.dev/)
- [`cppcheck`](https://cppcheck.sourceforge.io/)
- [`include-what-you-use`](https://github.com/include-what-you-use/include-what-you-use)
  - Used sparingly to identify extraneous headers which can be removed
  - Will not be fully integrated into the project until mappings for Boost 1.80.0 are released (required for process v2)
