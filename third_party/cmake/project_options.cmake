# cmake-format: off
message(
    STATUS
        "FetchContent: project_options @ https://github.com/aminya/project_options.git"
)
# cmake-format: on

# project_options CMake version does not support DOWNLOAD_EXTRACT_TIMESTAMP for
# FetchContent
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)

FetchContent_Declare(
    _project_options
    GIT_REPOSITORY https://github.com/aminya/project_options.git
    # release v0.24.1
    GIT_TAG 5e077f5f29a1be1dc09e5a937d56b1db39025cd4)
FetchContent_MakeAvailable(_project_options)

include(${_project_options_SOURCE_DIR}/Index.cmake)
include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)

# Developer mode defaults
set(ENABLE_COVERAGE_DEVELOPER_DEFAULT ON)
set(ENABLE_DOXYGEN_DEVELOPER_DEFAULT ON)
