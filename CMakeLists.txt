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

# Custom cppcheck options to suppress analysis of third party libs
dynamic_project_options(
    CPPCHECK_OPTIONS
    "--inline-suppr"
    "--enable=style,performance,warning,portability"
    "--suppress=internalAstError"
    "--suppress=unmatchedSuppression"
    "--suppress=*:${_catch2_SOURCE_DIR}/*"
    "--suppress=*:${Boost_INCLUDE_DIRS}/*"
    "--inconclusive")

add_subdirectory(./src)

if (BUILD_TESTS)
    message(STATUS "Building Tests.")

    include(CTest)
    enable_testing()
    add_subdirectory(./test)
endif ()

package_project(
    TARGETS
    temp
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
