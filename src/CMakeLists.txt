add_subdirectory(./temp)
add_subdirectory(./util)

add_library(
    sc2api
    INTERFACE)
add_library(
    sc2::api
    ALIAS
    sc2api)

target_link_libraries(sc2api INTERFACE sc2::util)
