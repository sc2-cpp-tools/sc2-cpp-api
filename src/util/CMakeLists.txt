add_library(
    sc2api_util
    options.cpp)
add_library(
    sc2::util
    ALIAS
    sc2api_util)

target_include_directories(
    sc2api_util
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>)

target_link_libraries(
    sc2api_util
    PUBLIC project_options
           project_warnings)

target_link_system_libraries(
    sc2api_util
    PUBLIC
    Boost::boost
    CLI11::CLI11
    Threads::Threads)

target_disable_static_analysis(
    Boost::boost
    CLI11::CLI11
    Threads::Threads)
