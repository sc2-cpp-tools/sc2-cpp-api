# add_executable(test_process test_process.cpp)
add_executable(test_options test_options.cpp)
catch_discover_tests(test_options)

target_link_libraries(
    test_options
    PRIVATE sc2::util
            project_options
            project_warnings
            Catch2::Catch2WithMain)
