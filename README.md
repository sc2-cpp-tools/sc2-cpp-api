# Overview

A modern C++17/20-based re-implementation of the [`Blizzard/s2client-api`](https://github.com/Blizzard/s2client-api).

## Building

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
  ENABLE_DEVELOPER_MODE="ON"
...

# Build from the generated preset
$ cmake --build build/gcc-devel -j8

# Alternatively, create a buildPreset in your local CMakeUserPresets.json for
# consistent IDE execution:
{
    "version": 3,
    "cmakeMinimumRequired": {
        "major": 3,
        "minor": 24,
        "patch": 1
    },
    "buildPresets": [
        {
            "name": "build-common",
            "description": "Build CMake settings that apply to all configurations",
            "hidden": true,
            "jobs": 25  <--- System dependent, adjust as necessary
        },
        {
            "name": "build-linux-gcc-debug",
            "displayName": "DEBUG",
            "description": "Build preset for gcc-devel-debug",
            "configurePreset": "gcc-devel-debug",
            "inherits": "build-common"
        },
        {
            "name": "build-linux-clang-debug",
            "displayName": "DEBUG",
            "description": "Build preset for clang-devel-debug",
            "configurePreset": "clang-devel-debug",
            "inherits": "build-common"
        }
    ]
}
```

With presets, multiple build configurations with isolated binary and library outputs can exist in the build directory simultaneously.[^1]

[^1]: There is a known issue with doxygen and mismatched md5 hashes with more than one build configuration present.

## Requirements

The project is currently under development, and incorporates [`project_options`](https://github.com/aminya/project_options) as the primary interface for managing CMake configuration. Development dependencies (such as `clang-tidy`) can be disabled from the `project_options` interface in the root `CMakeLists.txt`.

### Installing Dependencies

As an alternative to individually installing each dependency to the recommended version, the project includes a configurable `Dockerfile` to build an image with all dependencies installed. The `.ubuntu/bootstrap.sh` script can also be used to bootstrap a Ubuntu image (tested with 20.04).

Irrespective of the dependency versions installed, the project's CMake build configuration is expecting the base executable name (e.g. `clang` instead of `clang-14`). Both the `Dockerfile` and bootstrap script create the necessary symbolic links, which must otherwise be completed manually for custom installs.

### Build Dependencies

The following dependencies are required to build the project, and in some cases are pre-requisite for additional development tools:

- An installation of `clang` and/or `gcc` with C++17/20 support
  - Recommended: [`clang-14`](https://releases.llvm.org/14.0.0/tools/clang/docs/ReleaseNotes.html) and/or [`gcc 11`](https://gcc.gnu.org/gcc-11/changes.html)
  - `clang` is required for `include-what-you-use`
- [`CMake`](https://cmake.org/)
  - Recommended: [`v3.24.1`](https://github.com/Kitware/CMake/releases/tag/v3.24.1) and newer
  - Warning: while [`v3.21.0`](https://github.com/Kitware/CMake/releases/tag/v3.21.0) should be sufficient to support the current CMake presets version schema, the project will not support backwards compatibility for releases prior to the recommended
- [`Boost`](https://www.boost.org/users/history/version_1_80_0.html)
  - Version 1.80.0 is **required** for `Boost::process::v2`
  - **Boost is not currently part of the install script or Dockerfile**
- [`doxygen`](https://www.doxygen.nl/)
  - Required to build project documentation

### Development Dependencies

While these dependencies are not required to build the project, they are used to enhance development and enforce coding standards:


- `clang-tidy`
- `clang-format`
- [`cmakelang`](https://github.com/cheshirekow/cmake_format) for `cmake-format` and `cmake-lint`
- [`ccache`](https://ccache.dev/)
- [`cppcheck`](https://cppcheck.sourceforge.io/)
- [`include-what-you-use`](https://github.com/include-what-you-use/include-what-you-use)
  - Used sparingly to identify extraneous headers which can be removed
  - Will not be fully integrated into the project until mappings for Boost 1.80.0 are released (required for process v2)

### Integrated Dependencies

Some third party requirements are incorporated as part of the CMake configuration, and will be downloaded at configuration time:

- [`project_options`](https://github.com/aminya/project_options)
  - A useful CMake project configuration library
- [`Catch2 v3`](https://github.com/catchorg/Catch2)
  - C++ test framework
  - Also provides CMake targets and functions for test running and registration
