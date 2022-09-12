message(STATUS "FetchContent: Catch2 @ https://github.com/catchorg/Catch2.git")

set(CATCH_INSTALL_DOCS OFF)
set(CATCH_DEVELOPMENT_BUILD OFF)

FetchContent_Declare(
    _catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    # release v3.1.0
    GIT_TAG 97c48e0c343d26d50764fafdc90b1e630fbd10ce)
FetchContent_MakeAvailable(_catch2)

include(${_catch2_SOURCE_DIR}/extras/Catch.cmake)
