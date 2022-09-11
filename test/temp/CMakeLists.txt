add_executable(test_temp test_temp.cpp)

target_link_libraries(test_temp temp)

add_test(NAME test_temp COMMAND test_temp)
