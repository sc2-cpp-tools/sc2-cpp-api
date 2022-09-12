add_library(temp temp.cpp)

target_include_directories(temp PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(temp PRIVATE project_options project_warnings)
