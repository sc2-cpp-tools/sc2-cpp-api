message(STATUS "FetchContent: CLI11 @ https://github.com/CLIUtils/CLI11.git")

FetchContent_Declare(
    _cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    # release v2.2
    GIT_TAG b9be5b9444772324459989177108a6a65b8b2769)
FetchContent_MakeAvailable(_cli11)
