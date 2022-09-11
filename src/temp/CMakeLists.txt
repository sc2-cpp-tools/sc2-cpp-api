add_library(temp temp.cpp)

target_include_directories(temp PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(temp_main temp_main.cpp)

target_link_libraries(temp_main PRIVATE temp)
