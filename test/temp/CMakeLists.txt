add_executable(test_temp test_temp.cpp)
catch_discover_tests(test_temp)

target_link_libraries(
    test_temp
        PRIVATE
            temp
            project_options
            project_warnings
            Catch2::Catch2WithMain)
