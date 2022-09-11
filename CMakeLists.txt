cmake_minimum_required(VERSION 3.24.1)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Use project_options
include(./third_party/cmake/project_options.cmake)

# Define build options
include(./cmake/options.cmake)

project(
    sc2-cpp-api
    VERSION 0.0.1
    DESCRIPTION ""
    HOMEPAGE_URL "https://github.com/sc2-cpp-tools/sc2-cpp-api"
    LANGUAGES CXX C)

set(ENABLE_CLANG_TIDY OFF)
set(ENABLE_CPPCHECK OFF)
set(ENABLE_SANITIZER_ADDRESS OFF)
set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR OFF)
set(ENABLE_COVERAGE OFF)

# Enable sanitizers and clang-tidy if running the tests
if (BUILD_TESTS)
    set(ENABLE_CLANG_TIDY "ENABLE_CLANG_TIDY")
    set(ENABLE_CPPCHECK "ENABLE_CPPCHECK")
    set(ENABLE_COVERAGE "ENABLE_COVERAGE")
    enable_testing()

    if (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        set(ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
        set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
            "ENABLE_SANITIZER_UNDEFINED_BEHAVIOR")
    else ()
        # MSVC has vcvarsall
        string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)
        if (MSVC AND "${index_of_vs_install_dir}" STREQUAL "-1")
            set(ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
        endif ()
    endif ()
endif ()

if (BUILD_DOCS)
    set(ENABLE_DOXYGEN "ENABLE_DOXYGEN")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE doc/doxygen/mainpage.md)
else ()
    set(ENABLE_DOXYGEN OFF)
endif ()

if (DEVEL_MODE)
    set(WARNINGS_AS_ERRORS ON)
else ()
    set(WARNINGS_AS_ERRORS OFF)
endif ()

# See README of project_options Initialize project_options variable related to
# this project. This overwrites `project_options` and sets `project_warnings`.
# Uncomment to enable the options, some accept one or more inputs:
project_options(
    ENABLE_CACHE
    ${ENABLE_CPPCHECK}
    ${ENABLE_CLANG_TIDY}
    ${ENABLE_DOXYGEN}
    ${ENABLE_COVERAGE}
    ${ENABLE_SANITIZER_ADDRESS}
    ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
    WARNINGS_AS_ERRORS
    ENABLE_INCLUDE_WHAT_YOU_USE)

add_subdirectory(./src)
add_subdirectory(./test)

package_project(
    TARGETS
    temp_main
    project_options
    project_warnings
    INTERFACE_DEPENDENCIES_CONFIGURED
    INTERFACE_INCLUDES
    PUBLIC_INCLUDES)

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you track a
# build to a specific GIT SHA
# cmake-format: off
set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)
# cmake-format: on

include(CPack)
