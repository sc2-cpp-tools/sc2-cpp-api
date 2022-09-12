add_library(temp temp.cpp)

target_include_directories(temp
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(temp PRIVATE project_options project_warnings)
